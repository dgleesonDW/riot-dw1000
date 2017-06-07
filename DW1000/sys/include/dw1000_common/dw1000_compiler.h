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
 * @file	dw1000_compiler.h
 * @brief
 *
 */

#ifndef DW1000_COMPILER_H_
#define DW1000_COMPILER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#ifdef __GNUC__
#include <unistd.h>
#include <stdbool.h>
#ifndef __cplusplus
#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif
#endif
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#define Sleep(x)            usleep((x*1000))
#define __align4            __attribute__((aligned (4)))
#define __weak              __attribute__((weak))
#ifndef __always_inline
#define __always_inline     __attribute__((always_inline))
#endif
#endif

#ifdef __ICCARM__
#include <yfuns.h>
#define __caddr_t_required_
#define __useconds_t_required_
#define __tms_struct_required_
#define __stat_struct_required_
#define STDIN_FILENO        _LLIO_STDIN
#define STDOUT_FILENO       _LLIO_STDOUT
#define STDERR_FILENO       _LLIO_STDERR
#define __align4            /* to be defined? */
#define __weak              /* to be defined? */
#define _exit               __exit
#define Sleep(x)            usleep((x*1000))
#include "dw1000_sleep.h"
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <share.h>
#define __align4            /* no implementation */
#define __weak              /* no implementation */
#define __always_inline
#else
#define sprintf_s(x,y,...)      0   // temporary workaround
#endif

#ifndef __IO
#define __IO        volatile
#endif

/* Redefine CLOCKS_PER_SEC to make it work with Sleep(1) */
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC      1000


#ifdef __cplusplus
}
#endif

#endif /* DW1000_COMPILER_H_ */
