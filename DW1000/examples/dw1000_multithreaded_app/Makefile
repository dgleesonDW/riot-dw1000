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
#### Makefile for building Multi-threaded application with the RIOT OS
####
#### The example file system layout is:
#### ./application Makefile
#### ../../RIOT
####

# Set the name of your application:
APPLICATION = dw1000_multithreaded_app

# Board to be build for:
BOARD ?= nrf52dk

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Uncomment this to enable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

CFLAGS += -DRIOT_TREK_DW1000_APP

CFLAGS += -DRIOT_MULTI_THREADED_APP

CFLAGS += -DNORDIC_NRF52840_BOARD

# Change this to 0 to show compiler invocation lines by default:
QUIET ?= 1

DRIVER += dw1000

# Modules to include:
USEMODULE += xtimer
USEMODULE += dw1000_common
USEMODULE += $(DRIVER)
USEMODULE += printf_float
USEMODULE += posix_semaphore

include $(RIOTBASE)/Makefile.include
