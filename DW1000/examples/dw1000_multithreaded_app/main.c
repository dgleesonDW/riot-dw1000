/**
 * Copyright 2017 Decawave Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! ----------------------------------------------------------------------------
 *  @file    main.c
 *  @brief   Multi Threaded aplication in which solver_thread collects the
 *           range data from timer_thread and ranging_thread and then prints
 *           on the console
 */

/* Includes */
#include "dw1000_compiler.h"
#include "dw1000_port.h"
#include "dw1000_board.h"
#include "dw1000_instance.h"
#include "dw1000_types.h"
#include "xtimer.h"
#include "thread.h"
#include "mutex.h"
#include "multithreaded_app.h"

/* Thread IDs for ranging thread, timer thread and solver thread*/
kernel_pid_t ranging_pid, timer_pid, solver_pid;

/* Stack for the threads which holds the thread related data in their stacks*/
char ranging_thread_stack[THREAD_STACKSIZE_MAIN];
char timer_thread_stack[THREAD_STACKSIZE_MAIN];
char solver_thread_stack[THREAD_STACKSIZE_MAIN];

/* instance data structure */
instance_data_t instance_data[NUM_INST] ;

/* Range Data is a Shared Resource between Ranging thread and solver thread */
typedef struct {
    uint16 valid;
    uint16 instancegetidist_mm_0;
    uint16 instancegetidist_mm_1;
    uint16 instancegetidist_mm_2;
    uint16 instancegetidist_mm_3;
    uint16 l;
    uint16 r;
    uint16 range_time;
    uint16 taddr;
    uint16 aaddr;
    uint8 instance_mode;
}ranging_data_t;

ranging_data_t range_data;

/* Timer Data is a Shared Resource between Timer thread and solver thread */
uint16 timer_data;

/* Mutex variable for timer data protection */
mutex_t timer_mutex;

/* Mutex variable for range data protection */
mutex_t range_mutex;

/* Semaphore variable to notify the timer thread */
sem_t sem;

#define SWS1_SHF_MODE 0x02  //short frame mode (6.81M)
#define SWS1_CH5_MODE 0x04  //channel 5 mode
#define SWS1_ANC_MODE 0x08  //anchor mode
#define SWS1_A1A_MODE 0x10  //anchor/tag address A1
#define SWS1_A2A_MODE 0x20  //anchor/tag address A2
#define SWS1_A3A_MODE 0x40  //anchor/tag address A3
#define SWS1_USB2SPI_MODE 0x78  //USB to SPI mode
#define SWS1_TXSPECT_MODE 0x38  //Continuous TX spectrum mode
                                //"1234567812345678"
#define SOFTWARE_VER_STRING    "Ver.  2.10  TREK" //16 bytes!

uint8 s1switch = 0;
int instance_anchaddr = 0;
int dr_mode = 0;
int chan, tagaddr, ancaddr;
int instance_mode = ANCHOR;

uint32_t pauseTWRReports  = 0;
uint32_t printLCDTWRReports  = 0;
uint8_t sendTWRRawReports = 1;

typedef struct
{
    uint8 channel ;
    uint8 prf ;
    uint8 datarate ;
    uint8 preambleCode ;
    uint8 preambleLength ;
    uint8 pacSize ;
    uint8 nsSFD ;
    uint16 sfdTO ;
} chConfig_t ;

/* configuration for DecaRangeRTLS TREK Modes (4 default use cases selected by
 *  the switch S1 [2,3] on EVB1000, indexed 0 to 3 )
 */
chConfig_t chConfig[4] ={
                    //mode 1 - S1: 2 off, 3 off
                    {
                        2,               // channel
                        DWT_PRF_16M,     // prf
                        DWT_BR_110K,     // datarate
                        4,               // preambleCode
                        DWT_PLEN_1024,   // preambleLength
                        DWT_PAC32,       // pacSize
                        1,               // non-standard SFD
                        (1025 + 64 - 32) //SFD timeout
                    },
                    //mode 2 - S1: 2 on, 3 off
                    {
                        2,              // channel
                        DWT_PRF_16M,    // prf
                        DWT_BR_6M8,     // datarate
                        4,              // preambleCode
                        DWT_PLEN_128,   // preambleLength
                        DWT_PAC8,       // pacSize
                        0,              // non-standard SFD
                        (129 + 8 - 8)   //SFD timeout
                    },
                    //mode 3 - S1: 2 off, 3 on
                    {
                        5,               // channel
                        DWT_PRF_16M,     // prf
                        DWT_BR_110K,     // datarate
                        3,               // preambleCode
                        DWT_PLEN_1024,   // preambleLength
                        DWT_PAC32,       // pacSize
                        1,               // non-standard SFD
                        (1025 + 64 - 32) //SFD timeout
                    },
                    //mode 4 - S1: 2 on, 3 on
                    {
                        5,              // channel
                        DWT_PRF_16M,    // prf
                        DWT_BR_6M8,     // datarate
                        3,              // preambleCode
                        DWT_PLEN_128,   // preambleLength
                        DWT_PAC8,       // pacSize
                        0,              // non-standard SFD
                        (129 + 8 - 8)   //SFD timeout
                    }
};

//Slot and Superframe Configuration for DecaRangeRTLS TREK Modes (4 default use cases selected by the switch S1 [2,3] on EVB1000, indexed 0 to 3 )
sfConfig_t sfConfig[4] ={
                    //mode 1 - S1: 2 off, 3 off
                    {
                        (28), //ms -
                        (10),   //thus 10 slots - thus 280ms superframe means 3.57 Hz location rate (10 slots are needed as AtoA ranging takes 30+ ms)
                        (10*28), //superframe period
                        (10*28), //poll sleep delay
                        (20000)
                    },
                    //mode 2 - S1: 2 on, 3 off
                    {
                        (10),   // slot period ms
                        (10),   // number of slots (only 10 are used) - thus 100 ms superframe means 10 Hz location rate
                        (10*10), // superframe period (100 ms - gives 10 Hz)
                        (10*10), // poll sleep delay (tag sleep time, usually = superframe period)
#ifdef NORDIC_NRF52840_BOARD
                        (4500)
#else
                        (2500)
#endif
                    },
                    //mode 3 - S1: 2 off, 3 on
                    {
                        (28),    // slot period ms
                        (10),     // thus 10 slots - thus 280ms superframe means 3.57 Hz location rate
                        (10*28),  // superframe period
                        (10*28),  // poll sleep delay
                        (20000)
                    },
                    //mode 4 - S1: 2 on, 3 on
                    {
                        (10),   // slot period ms
                        (10),   // number of slots (only 10 are used) - thus 100 ms superframe means 10 Hz location rate
                        (10*10), // superframe period (100 ms - gives 10 Hz)
                        (10*10), // poll sleep (tag sleep time, usually = superframe period)
#ifdef NORDIC_NRF52840_BOARD
                        (4500)
#else
                        (2500) // this is the Poll to Final delay - 2ms (NOTE: if using 6.81 so only 1 frame per ms allowed LDC)
#endif
                    }
};

// ======================================================
//
//  Configure instance tag/anchor/etc... addresses
//
void addressconfigure(uint8 s1switch, uint8 mode)
{
    uint16 instAddress ;

    instance_anchaddr = (((s1switch & SWS1_A1A_MODE) << 2) + (s1switch & SWS1_A2A_MODE) + ((s1switch & SWS1_A3A_MODE) >> 2)) >> 4;

    if(mode == ANCHOR)
    {
        if(instance_anchaddr > 3)
        {
            instAddress = GATEWAY_ANCHOR_ADDR | 0x4 ; //listener
        }
        else
        {
            instAddress = GATEWAY_ANCHOR_ADDR | instance_anchaddr;
        }
    }
    else
    {
        instAddress = instance_anchaddr;
    }

    instancesetaddresses(instAddress);
}

uint32 inittestapplication(uint8 s1switch);


//returns the use case / operational mode
int decarangingmode(uint8 s1switch)
{
    int mode = 0;

    if(s1switch & SWS1_SHF_MODE)
    {
        mode = 1;
    }

    if(s1switch & SWS1_CH5_MODE)
    {
        mode = mode + 2;
    }

    return mode;
}

uint32 inittestapplication(uint8 s1switch)
{
    uint32 devID ;
    instanceConfig_t instConfig;
    int result;

    SPI_ConfigFastRate(DW1000_SPI_LOW);  //max SPI before PLLs configured is ~4M

    //this is called here to wake up the device (i.e. if it was in sleep mode before the restart)
    devID = instancereaddeviceid() ;
    printf("DW1000 Device Id:%x\n",(unsigned int)devID);

    if(DWT_DEVICE_ID != devID) //if the read of device ID fails, the DW1000 could be asleep
    {
        port_SPIx_clear_chip_select();  //CS low
        //200 us to wake up then waits 5ms for DW1000 XTAL to stabilise
        xtimer_usleep(1000);
        port_SPIx_set_chip_select();  //CS high
        //200 us to wake up then waits 5ms for DW1000 XTAL to stabilise
        xtimer_usleep(7000);
        devID = instancereaddeviceid() ;
        // SPI not working or Unsupported Device ID
        if(DWT_DEVICE_ID != devID)
            return(-1) ;
    }

    result = instance_init() ;
    if (0 > result) return(-1) ; // Some failure has occurred

    SPI_ConfigFastRate(DW1000_SPI_HIGH); //increase SPI to max
    devID = instancereaddeviceid() ;

    if (DWT_DEVICE_ID != devID)   // Means it is NOT DW1000 device
    {
        // SPI not working or Unsupported Device ID
        return(-1) ;
    }

    if((s1switch & SWS1_ANC_MODE) == 0)
    {
        instance_mode = TAG;
    }
    else
    {
        instance_mode = ANCHOR;
    }

    addressconfigure(s1switch, instance_mode) ;                            // set up initial payload configuration

    if((instance_mode == ANCHOR) && (instance_anchaddr > 0x3))
    {
        instance_mode = LISTENER;
    }

    instancesetrole(instance_mode) ;     // Set this instance role

    // get mode selection (index) this has 4 values see chConfig struct initialiser for details.
    dr_mode = decarangingmode(s1switch);

    chan = instConfig.channelNumber = chConfig[dr_mode].channel ;
    instConfig.preambleCode = chConfig[dr_mode].preambleCode ;
    instConfig.pulseRepFreq = chConfig[dr_mode].prf ;
    instConfig.pacSize = chConfig[dr_mode].pacSize ;
    instConfig.nsSFD = chConfig[dr_mode].nsSFD ;
    instConfig.sfdTO = chConfig[dr_mode].sfdTO ;
    instConfig.dataRate = chConfig[dr_mode].datarate ;
    instConfig.preambleLen = chConfig[dr_mode].preambleLength ;

    instance_config(&instConfig, &sfConfig[dr_mode]) ;                  // Set operating channel etc

    return devID;
}
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
void process_dwRSTn_irq(void)
{
    instance_notify_DW1000_inIDLE(1);
}

void instance_process_irq(int x)
{
      dwt_isr();
}

void process_deca_irq(void)
{
#ifndef RIOT_TREK_DW1000_APP
    while(port_CheckEXT_IRQ() == 1); //while IRS line active (ARM can only do edge sensitive interrupts)
    {
        instance_process_irq(0);
    }
#else // TBD : Nucleo fix with while(port_CheckEXT_IRQ() == 1);
    do{

        instance_process_irq(0);
    }
    while(port_CheckEXT_IRQ() == 1); //while IRS line active (ARM can only do edge sensitive interrupts)
#endif
}

void configure_continuous_txspectrum_mode(uint8 s1switch)
{
    //configure DW1000 into Continuous TX mode
    instance_starttxtest(0x1000);
    //measure the power
    //Spectrum Analyser set:
    //FREQ to be channel default e.g. 3.9936 GHz for channel 2
    //SPAN to 1GHz
    //SWEEP TIME 1s
    //RBW and VBW 1MHz
    //measure channel power

    //user has to reset the board to exit mode
    while(1)
    {
        xtimer_usleep(2000);
    }

}

extern uint32 starttime[];
extern int time_idx;

static void *ranging_thread(void *arg)
{
    int rx = 0;
    int toggle = 0;

    xtimer_sleep(1);

    port_DisableEXT_IRQ(); //disable ScenSor IRQ until we configure the device

    //run DecaRangeRTLS application for TREK
    {
        if(inittestapplication(s1switch) == (uint32)-1)
        {
            printf(" inittestapllication  failed\n");
            return 0; //error
        }

        // Is continuous spectrum test mode selected?
        if((s1switch & SWS1_TXSPECT_MODE) == SWS1_TXSPECT_MODE)
        {
            //this function does not return!
            configure_continuous_txspectrum_mode(s1switch);
        }
    }

    port_EnableEXT_IRQ(); //enable ScenSor IRQ before starting

    // main loop
    while(1)
    {
        int monitor_local = instance_data[0].monitor ;
        int txdiff = (portGetTickCnt() - instance_data[0].timeofTx);

        instance_run();
        instance_mode = instancegetrole();

        //if delayed TX scheduled but did not happen after expected time then it has failed... (has to be < slot period)
        //if anchor just go into RX and wait for next message from tags/anchors
        //if tag handle as a timeout
        if((monitor_local == 1) && ( txdiff > instance_data[0].slotPeriod))
        {
            uint32 tdly ;
            uint32 reg1, reg2;

            reg1 = dwt_read32bitoffsetreg(0x0f, 0x1);
            reg2 = dwt_read32bitoffsetreg(0x019, 0x1);
            tdly = dwt_read32bitoffsetreg(0x0a, 0x1);

            printf("T%08x %08x time %08x %08x \n", (unsigned int) reg2, (unsigned int) reg1,
                    (unsigned int) dwt_read32bitoffsetreg(0x06, 0x1), (unsigned int) tdly); /* TBD */

            instance_data[0].wait4ack = 0;

            if(instance_mode == TAG)
            {
                inst_processrxtimeout(&instance_data[0]);
            }
            else //if(instance_mode == ANCHOR)
            {
                dwt_forcetrxoff();  //this will clear all events
                //dwt_rxreset();
                //enable the RX
                instance_data[0].testAppState = TA_RXE_WAIT ;
            }
            instance_data[0].monitor = 0;
        }

        rx = instancenewrange();

        //if there is a new ranging report received or a new range has been calculated, then prepare data
        //to output over USB - Virtual COM port, and update the LCD
        if(rx != TOF_REPORT_NUL)
        {
            int l = 0, r= 0, aaddr, taddr;
            int rangeTime, valid;
            //int correction ;
            uint16 txa, rxa;

            //send the new range information to LCD and/or USB
            aaddr = instancenewrangeancadd() & 0xf;
            taddr = instancenewrangetagadd() & 0xf;
            rangeTime = instancenewrangetim() & 0xffffffff;
#if (LCD_UPDATE_ON == 1)
            //if((dr_mode & 0x1) == 0) //only print for 110k
            if(printLCDTWRReports + 2000 <= portGetTickCnt())
            {
                //anchors will print a range to each tag in sequence with 1 second pause
                //they will show the last rage to that tag
                if(instance_mode == ANCHOR)
                {
                    int b = 0;
                    double rangetotag = getTagDist(toggle) ;

                    while(((int) (rangetotag*1000)) == 0) //if 0 then go to next tag
                    {
                        if(b > MAX_TAG_LIST_SIZE)
                            break;

                        toggle++;
                        if(toggle >= MAX_TAG_LIST_SIZE)
                            toggle = 0;

                        rangetotag = getTagDist(toggle) ;
                        b++;
                    }

                    ancaddr = instance_anchaddr;
                    printf("ANCHOR: A%d T%d: %3.2f\n", ancaddr, toggle, rangetotag);
                    toggle++;

                    if(toggle >= MAX_TAG_LIST_SIZE)
                        toggle = 0;
                }
                else if(instance_mode == TAG)
                {
                    tagaddr = instance_anchaddr;
                    printf("\n TAG: T%d  A%d: %3.2f\n", tagaddr, toggle, instancegetidist(toggle));
                    toggle++;

                    if(toggle >= MAX_ANCHOR_LIST_SIZE)
                        toggle = 0;
                }

                //update the print time
                printLCDTWRReports = portGetTickCnt();
            }
#endif
            l = instancegetlcount() & 0xFFFF;
            if(instance_mode == TAG)
            {
                r = instancegetrnum();
            }
            else
            {
                r = instancegetrnuma(taddr);
            }
            txa =  instancetxantdly();
            rxa =  instancerxantdly();
            valid = instancevalidranges();

            if(rx == TOF_REPORT_T2A)
            {
                //correction = instance_data[0].tagSleepCorrection2;
                // anchorID tagID range rangeraw countofranges rangenum rangetime txantdly rxantdly address

                /* Write RTLS data into shared memory */
                mutex_lock(&range_mutex);

                range_data.valid = valid;
                range_data.instancegetidist_mm_0 = instancegetidist_mm(0);
                range_data.instancegetidist_mm_1 = instancegetidist_mm(1);
                range_data.instancegetidist_mm_2 = instancegetidist_mm(2);
                range_data.instancegetidist_mm_3 = instancegetidist_mm(3);
                range_data.l = l;
                range_data.r = r;
                range_data.range_time = rangeTime;
                range_data.taddr = taddr;
                range_data.aaddr = aaddr;
                range_data.instance_mode = ((instance_mode == TAG)?'t':'a');

                mutex_unlock(&range_mutex);

                printf("mc %02x %08x %08x %08x %08x %04x %02x %08x %c%d:%d \n",
                       valid, instancegetidist_mm(0), instancegetidist_mm(1),
                       instancegetidist_mm(2), instancegetidist_mm(3), l, r,
                       rangeTime, (instance_mode == TAG)?'t':'a', taddr, aaddr);


                if(sendTWRRawReports == 1)
                {
                    printf("mr %02x %08x %08x %08x %08x %04x %02x %04x%04x %c%d:%d\r\n",
                            valid, instancegetidistraw_mm(0), instancegetidistraw_mm(1),
                            instancegetidistraw_mm(2), instancegetidistraw_mm(3),
                            l, r, txa, rxa, (instance_mode == TAG)?'t':'a', taddr, aaddr);

                }
            }
            else //anchor to anchor ranging
            {
                printf( "ma %02x %08x %08x %08x %08x %04x %02x %08x a0:%d\r\n",
                        valid, instancegetidist_mm(0), instancegetidist_mm(1),
                        instancegetidist_mm(2), instancegetidist_mm(3),
                        l, instancegetrnumanc(0), rangeTime, aaddr);
            }
            instancecleardisttableall();
        } //if new range present
    }
    return NULL;
}

static void* timer_thread(void *arg)
{
    xtimer_sleep(1);

    while(1){
        /* waiting for sem_post: timer isr gives the sem_post */
        sem_wait(&sem);

        /* Lock mutex before changing the data*/
        mutex_lock(&timer_mutex);
        /* Change the timer data */
        timer_data++;
        /* unlock mutex sfter changing the data*/
        mutex_unlock(&timer_mutex);
    }

    return NULL;
}

static void* solver_thread(void *arg)
{
    /* Local data structure fo hold global data*/
    ranging_data_t  data_local;
    uint16 timerData = 0;

    xtimer_sleep(1);

    while (1){
        /* If sensor data changed then only send the data */
        if (timer_data != timerData) {
            /* Lock mutex before changing the data*/
            mutex_lock(&timer_mutex);
            /* Copy global data to local variable*/
            timerData = timer_data;
            /* unlock mutex sfter changing the data*/
            mutex_unlock(&timer_mutex);

            mutex_lock(&range_mutex);
            memcpy(&data_local, &range_data, sizeof( ranging_data_t));

            /* Print(Send) the data on console: Accumulated MC Ranging data and Timer Data */
            printf("\n MC %02x %08x %08x %08x %08x %04x %02x %08x %c%d:%d\n",
                    data_local.valid,
                    data_local.instancegetidist_mm_0,
                    data_local.instancegetidist_mm_1,
                    data_local.instancegetidist_mm_2,
                    data_local.instancegetidist_mm_3,
                    data_local.l,
                    data_local.r,
                    data_local.range_time,
                    data_local.instance_mode,
                    data_local.taddr,
                    data_local.aaddr);
            printf("Timer Data %d\n\n", timerData);
            mutex_unlock(&range_mutex);
        }
        else {
            printf("Solver: Running continuously\n");
        }
    }

    return NULL;
}

#pragma GCC optimize ("O3")
int main(void)
{
    int Unit = 0, Unitid = 0, Mode = 0;
    while (1)
    {
        printf("Select the DW1000 Device Configuration\n");
        printf("Select Mode Configuration:  0:Mode-0 1:Mode-1 2:Mode-2 3:Mode-3\n");
        scanf("%d", &Mode);
        printf("Select UNIT: 0:TAG 1:ANCHOR \n");
        scanf(" %d", &Unit);
        printf("Enter UNIT ID: 0 t0 2\n");
        scanf("%d", &Unitid);

        if( (Unit > 1) || (Unit < 0) || (Mode > 3) || (Mode < 0) || (Unitid > 2) || (Unitid < 0) )
        {
            printf(" User Input is invalid !!!\n");
            continue;
        }
        else
        {
            if(Unitid == 1)
                Unitid = 4;

            s1switch = 0x01 | ( Unit << 3 ) | ( Mode << 1) | ( Unitid  << 4 );
            break;
        }
    }

    /* dw1000 initialization */
    dw1000_board_init();

    /* Initialize semaphore */
    sem_init(&sem, 0, 0);

    /* Initialize mutex */
    mutex_init(&range_mutex);
    mutex_init(&timer_mutex);

    /* Ranging thread creation with highest priority*/
    ranging_pid = thread_create(ranging_thread_stack, sizeof(ranging_thread_stack),
            THREAD_PRIORITY_MAIN-3, THREAD_CREATE_STACKTEST,
            ranging_thread, NULL, "ranging_th");

    /* Sensor thread creation with medium priority*/
    timer_pid = thread_create(timer_thread_stack, sizeof(timer_thread_stack),
            THREAD_PRIORITY_MAIN-2, THREAD_CREATE_STACKTEST,
            timer_thread, NULL, "timer_th");

    /* Solver thread creation with low priority*/
    solver_pid = thread_create(solver_thread_stack, sizeof(solver_thread_stack),
            THREAD_PRIORITY_MAIN-1, THREAD_CREATE_STACKTEST,
            solver_thread, NULL, "solver_th");

    while(1);   /* Keep main() alive */

    sem_destroy(&sem);

    return 0;
}
