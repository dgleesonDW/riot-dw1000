#
#  Copyright 2017 Decawave Ltd.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

####
#### Makefile for building Decarange RTLS ARM application with the RIOT OS
####
#### The example file system layout is:
#### ./application Makefile
#### ../../RIOT
####

# Set the name of your application:
APPLICATION = dw1000_rtls_arm

# Board to be build for:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_spi

ifeq ($(BOARD),nrf52dk)
FEATURES_REQUIRED += periph_rtt
endif

# Uncomment this to enable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

CFLAGS += -DRIOT_TREK_DW1000_APP

ifeq ($(BOARD),nrf52dk)
CFLAGS += -DNORDIC_NRF52840_BOARD
endif

# Change this to 0 to show compiler invocation lines by default:
QUIET ?= 1

DRIVER += dw1000

# Modules to include:
USEMODULE += xtimer
USEMODULE += dw1000_common
USEMODULE += $(DRIVER)
USEMODULE += printf_float

include $(RIOTBASE)/Makefile.include
