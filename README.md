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

Added support for BSP for Decawave on RIOT for Nordic NRF52840 and STM32F401RE Nucleo
platform.

Added support for DecaRange RTLS ARM Application

## Dependent tools to used for Nordic board

* To flash the binary JLink is use, please install the tool manually from
https://www.segger.com/downloads/jlink.
* Expand J-Link Software and Documentation Pack and select the suitable
package and click download.
* Double click on the downloaded package to install.


## GETTING STARTED

## Installing Tool chain and Building DecaRange RTLS ARM Application for Nordic NRF52840 Platform
*  To initialize the Environment Variables, installing Dependent Libraries (First time), setup(clone the RIOT repo and sym link creation for dw1000) and building the application
    * Inside the cloned repo,run the script – `source ./build_setup.sh INITIAL`
*  To initialize the Environment Variables, setup(clone the RIOT repo and sym link creation for dw1000) and building the application(Next Time on Wards)
    * Inside the cloned repo,run the script – `source ./build_setup.sh UPDATE`
*  To build the DecaRange RTLS ARM application
    * Inside the cloned repo run the script – `source ./build_setup.sh`
## Hardware setup
The Connection Details of the Hardware setup is explained in [Hardware_setup.md][HS].

[HS]: ./DW1000/doc/Hardware_setup.md


### Running the DecaRange RTLS ARM Application on Nordic NRF52840 Platform
* To flash
     ```bash
     $ sudo make -C $RIOT_ROOT/examples/dw1000_rtls_arm/ BOARD=$DW_HW_PLATFORM flash
    ```
### Running the DecaRange RTLS ARM Application on STM32 Nucleo F401RE Platform
   * To Build
      - Modify the setenv.sh as `$DW_HW_PLATFORM=nucleo-f401`
      - To build the DecaRange RTLS ARM application run the script – `source ./build_setup.sh`
   * To Flash
       ```bash
       sudo make -C $RIOT_ROOT/examples/dw1000_rtls_arm/ BOARD=$DW_HW_PLATFORM flash”
       ```

## To Test DecaRange RTLS ARM Application Usecases

1) Tracking Usecase:
     * The Tracking use-case is explained in [Tracking.md][Tracking].

       [Tracking]: ./DW1000/doc/Tracking.md
2) Navigation Usecase:
     * The Navigation use-case is explained in [Navigation.md][Navigation].

       [Navigation]: ./DW1000/doc/Navigation.md
3) Geo-Fencing Usecase:
     * The Geo-fencing use-case is explained in [Geo-Fencing.md][Geo-Fencing].

       [Geo-Fencing]: ./DW1000/doc/Geo-Fencing.md


## KNOWN ISSUES

* Tracking Use case
Observed Anchor to anchor bias corrected range(ma), Tag to anchor bias
corrected range(mc) and Tag to anchor raw range(mr) range deviation is between 25-40 cm instead of 15cm for all modes randomly.
* Navigation Use case
Observed that T0 to A0, A1, A2 Range info is detected as 0 randomly
* Geo-Fencing Use case
Log Pattern is not proper as expected: A0:T0, A0:T1, A0:T0 & A0:T2 instead of
A0:T0, A0:T1, A0:T2 for all modes. In case of Mode 3, this is observed occasionally


## Known Limitations

  Due to EVB 1000 Hardware constraints with External Microcontroller

  * Range Distance Estimation is displayed on PyTerm Terminal
  * Selection of Mode(Mode-1/2/3/4), UNIT(Tag/Anchor) and UNIT ID (0 to 2) is
    configured in DecaRange RTLS ARM Application based on User Input


## DOCUMENTATION

* Userguide is available in DW1000/doc/PP_DecaWave_BSP_ReleaseNotes.pdf
* ReleaseNote is available in DW1000/doc/PP_DecaWave_BSP_UserGuide.pdf



