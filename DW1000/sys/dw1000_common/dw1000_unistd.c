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
 * @file    dw1000_unistd.c
 * @brief   sleep implementation instead of library sleep
 *
 */


#include "dw1000_compiler.h"
#include "dw1000_sleep.h"

#ifndef RIOT_TREK_DW1000_APP

#define _clock(x) clock(x)

unsigned __weak sleep(unsigned seconds)
{
    clock_t t0 = _clock();
    clock_t dt = seconds * CLOCKS_PER_SEC;

    while (_clock() - t0  < dt);
    return 0;
}

int __weak usleep(useconds_t useconds)
{
    clock_t t0 = _clock();
    clock_t dt = useconds / (1000000/CLOCKS_PER_SEC);

    while (_clock() - t0  < dt);
    return 0;
}
#endif

