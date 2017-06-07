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
 * @file    syscalls.c
 * @brief   system call functions
 *
 */

#include "dw1000_compiler.h"
#include "dw1000_port.h"
#include <errno.h>
#undef errno
extern int errno;

#ifdef __caddr_t_required_
typedef char * caddr_t;
#endif

#ifdef __stat_struct_required_
struct stat
{
    unsigned long int st_mode;
};
#endif

#ifndef ENOMEM
#define ENOMEM 12 /* Not enough core */
#endif

#ifndef EBADF
#define EBADF 9 /* Bad file number */
#endif

#ifndef EINVAL
#define EINVAL 22 /* Invalid argument */
#endif

#ifndef _IFCHR
#define _IFCHR 0020000 /* character special */
#endif

#ifndef S_IFCHR
#define S_IFCHR _IFCHR
#endif


/* ALL non reentrant function declaration start here
 *
 */
#ifndef RIOT_TREK_DW1000_APP   /* TBD */
int _write(int fd, char *ptr, size_t len)
{
    size_t counter = len;

    if (fd == STDOUT_FILENO || fd == STDERR_FILENO){}
    else return -1;

    while(counter-- > 0) { // Send the character from the buffer to UART
        while (port_USARTx_busy_sending());
        port_USARTx_send_data(*ptr);

        ptr++;
    }

    return len;
}

caddr_t _sbrk(int incr)
{
    extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;
    char *stack;

    if (heap_end == 0) heap_end = &_ebss;

    prev_heap_end = heap_end;

    stack = (char*) port_GET_stack_pointer();

    if (heap_end + incr >  stack)
    {
        _write (STDERR_FILENO, "Heap and stack collision\n", 25);
        errno = ENOMEM;
        return  (caddr_t) -1;
        //abort ();
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}

void _exit(int status) {
    _write (STDERR_FILENO, "exit", 4);
    // visual blinking indicator when application exits and terminated
    while (1)
    {
        Sleep(50);
    }
}


/* ALL reentrant function declaration start here
 *
 */

/* Return a clock that ticks at CLOCKS_PER_SEC Hz.  */
clock_t _times_r(void *reent, struct tms * tp)
{
  clock_t timeval = portGetTickCount(); //time32_incr;

  if (tp)
    {
      tp->tms_utime  = timeval; /* user time */
      tp->tms_stime  = 0;   /* system time */
      tp->tms_cutime = 0;   /* user time, children */
      tp->tms_cstime = 0;   /* system time, children */
    }

  return timeval;
};

int _read_r(int fd, char *ptr, size_t len)
{
    int i;
    size_t counter = 0;

    if(fd == STDIN_FILENO){}
    else return -1;

    for (i = 0; i < len; i++) { // Get characters from the UART
        while (port_USARTx_no_data());
        *ptr++ = (char)port_USARTx_receive_data();
        counter++;
    }

    return counter;
}

int _close_r(int file)
{
    return -1;
}

int _fstat_r(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;

    return 0;
}

int _isatty_r(int file)
{
    switch (file)
    {
        case STDOUT_FILENO:
        case STDERR_FILENO:
        case STDIN_FILENO:
            return 1;
        default:
            //errno = ENOTTY;
            errno = EBADF;
            return 0;
    }
}

#define __MY_PID    1
int _getpid_r(void)
{
    return  __MY_PID; /* return any number as PID */
}

int _lseek_r(int file, int ptr, int dir)
{
    return 0;
}

int _kill_r(int pid, int sig)
{
    errno = EINVAL;

    return -1; /* Always fails */
}

#endif /* TBD */
