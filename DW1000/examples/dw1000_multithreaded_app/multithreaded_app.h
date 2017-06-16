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

/**
 * @brief       Multi Threaded aplication in which solver_thread collects the
 *              range data from timer_thread and ranging_thread and then prints
 *              range data on the console
 * @{
 *
 * @file        multithreaded_app.h
 *
 * @author      Sunil Sharma P <sunil.sharma@pathpartnertech.com>
 */

/**
 * @brief   The ranging thread is a high priority thread which collects the
 *          time stamp and calculates the ranging between tags and anchors
 *          and then the ranging data is stored in shared buffer "range_data".
 *
 *          If the ranging thread is configured for Tag devices then this thread
 *          goes to sleep state and only wakes up for the next defined time slot
 *          This allows CPU to execute other waiting threads.
 *
 *          If the ranging thread is configured for Anchor devices then this
 *          thread never goes to sleep state, then this thread doesnt allow
 *          CPU to execute other waiting threads.
 *
 * @param   arg     pointer to arg function parameter
 *
 * @return  void
 */
static void *ranging_thread(void *arg);

/**
 * @brief   The Timer thread is a medium priority thread which increments
 *          "timer_data" buffer for every time its gets the time to execute
 *
 *          This thread will be waiting on semaphore and sem_post will be
 *          done in isr. But this thread will not run immediatly when sem_post
 *          executes at isr, instead it will run when ranging thread goes to
 *          sleep
 *
 * @param   arg      pointer to arg function parameter
 *
 * @return  void
 */
static void* timer_thread(void *arg);

/**
 * @brief   The solver thread is a low priority thread. It reads both
 *          rang_data and timer_data buffers and prints it on console
 *          only if the timer_data has been modified.
 *
 *          As this is lower priority thread, this will run only when both
 *          ranging thread and timer thread are sleeping.
 *
 * @param   arg     pointer to arg function parameter
 *
 * @return  void
 */
static void* solver_thread(void *arg);
