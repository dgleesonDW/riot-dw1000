                          ZZZZZZ
                        ZZZZZZZZZZZZ
                      ZZZZZZZZZZZZZZZZ
                     ZZZZZZZ     ZZZZZZ
                    ZZZZZZ        ZZZZZ
                    ZZZZZ          ZZZZ
                    ZZZZ           ZZZZZ
                    ZZZZ           ZZZZ
                    ZZZZ          ZZZZZ
                    ZZZZ        ZZZZZZ
                    ZZZZ     ZZZZZZZZ       777        7777       7777777777
              ZZ    ZZZZ   ZZZZZZZZ         777      77777777    77777777777
          ZZZZZZZ   ZZZZ  ZZZZZZZ           777     7777  7777       777
        ZZZZZZZZZ   ZZZZ    Z               777     777    777       777
       ZZZZZZ       ZZZZ                    777     777    777       777
      ZZZZZ         ZZZZ                    777     777    777       777
     ZZZZZ          ZZZZZ    ZZZZ           777     777    777       777
     ZZZZ           ZZZZZ    ZZZZZ          777     777    777       777
     ZZZZ           ZZZZZ     ZZZZZ         777     777    777       777
     ZZZZ           ZZZZ       ZZZZZ        777     777    777       777
     ZZZZZ         ZZZZZ        ZZZZZ       777     777    777       777
      ZZZZZZ     ZZZZZZ          ZZZZZ      777     7777777777       777
       ZZZZZZZZZZZZZZZ            ZZZZ      777      77777777        777
         ZZZZZZZZZZZ               Z
            ZZZZZ

The friendly Operating System for IoT!

RIOT is a real-time multi-threading operating system that supports a range of
devices that are typically found in the Internet of Things (IoT): 
8-bit, 16-bit and 32-bit microcontrollers.

RIOT is based on the following design principles: energy-efficiency, real-time
capabilities, small memory footprint, modularity, and uniform API access,
independent of the underlying hardware (this API offers partial POSIX
compliance).

RIOT is developed by an international open source community which is
independent of specific vendors (e.g. similarly to the Linux community).
RIOT is licensed with LGPLv2.1, a copyleft license which fosters
indirect business models around the free open-source software platform
provided by RIOT, e.g. it is possible to link closed-source code with the
LGPL code.


## FEATURES

This repository contains folders to build and work with the BSP for Decawave 
on RIOT for Nordic NRF52840 and STM32F401RE Nucleo platforms. The BSP package 
for Decawave on RIOT serves to provide a software platform for development, 
deployment and execution of DecaRange RTLS ARM application.


## Dependent tools to used for Nordic board

* To flash the binary JLink is use, please install the tool manually from
https://www.segger.com/downloads/jlink.
* Expand J-Link Software and Documentation Pack and select the suitable 
package and click download.
* Double click on the downloaded package to install.


## GETTING STARTED

There is a shellscript in `riot-dw1000` called `setup.sh` which
you can use to install all the required packages.
* If you are using this repository for the first time, then dependent packages
  or modules needs to be installed. 
  Use the following command `./setup.sh INITIAL`
* If you do not wish to install the dependent packages or modules and still want
  to get latest code.
  Use the following command `./setup.sh`


### USING THE RTLS ARM Application

* Command to build
  make -C RIOT/examples/dw1000_rtls_arm BOARD=HW_PLATFORM
  For Nordic - HW_PLATFORM = nrf52dk, 
  For NucleoF401RE - HW_PLATFORM = nucleo-f401
            For Example: make -C RIOT/examples/dw1000_rtls_arm BOARD=nrf52dk    
* Command to build and flash
  sudo make -C RIOT/examples/dw1000_rtls_arm BOARD=HW_PLATFORM flash
  For Nordic - HW_PLATFORM = nrf52dk, 
  For NucleoF401RE - HW_PLATFORM = nucleo-f401
            For Example: sudo make -C RIOT/examples/dw1000_rtls_arm BOARD=nrf52dk flash    


#### KNOWN ISSUES

* Tracking Use case
Observed Anchor to anchor bias corrected range(ma), Tag to anchor bias corrected
range(mc) and Tag to anchor raw range(mr) range deviation is between 25-40 cm
instead of 15cm for all modes randomly.
* Navigation Use case
Observed that T0 to A0, A1, A2 Range info is detected as 0 randomly
* Geo-Fencing Use case
Log Pattern is not proper as expected: A0:T0, A0:T1, A0:T0 & A0:T2 instead of
A0:T0, A0:T1, A0:T2 for all modes. In case of Mode 3, this is observed occasionally


## DOCUMENTATION

* Userguide is available in DW1000/doc/PP_DecaWave_BSP_ReleaseNotes.pdf
* ReleaseNote is available in DW1000/doc/PP_DecaWave_BSP_UserGuide.pdf
* TestReport is available in DW1000/doc/PP_DecaWave_BSP_TestReport.xls

