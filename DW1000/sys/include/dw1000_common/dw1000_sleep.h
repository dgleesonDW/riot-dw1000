/*! ----------------------------------------------------------------------------
 * @file    dw1000_sleep.h
 * @brief   sleep implementation instead of library sleep
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
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
