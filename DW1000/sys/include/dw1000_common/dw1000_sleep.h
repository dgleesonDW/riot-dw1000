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
 * @file    dw1000_sleep.h
 * @brief   sleep implementation instead of library sleep
 *
 */

#ifndef DW1000_SLEEP_H_
#define DW1000_SLEEP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RIOT_TREK_DW1000_APP

#ifdef __useconds_t_required_
typedef unsigned long useconds_t;
#endif

#ifdef __tms_struct_required_
struct tms {
    clock_t tms_utime;      /* user time */
    clock_t tms_stime;      /* system time */
    clock_t tms_cutime;     /* user time, children */
    clock_t tms_cstime;     /* system time, children */
};
#endif

extern unsigned __weak sleep(unsigned seconds);
extern int __weak usleep(useconds_t useconds);

#endif

#ifdef __cplusplus
}
#endif

#endif /* DW1000_SLEEP_H_ */
