#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "devices.h"

#undef errno
extern int errno;

void _exit(int status) {
  while(1);
}

int _open(const char *pathname, int flags, mode_t mode) {
    errno=ENOSYS;
    return -1;
}

int _close(int file) {
    errno=ENOSYS;
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    errno = ENOSYS;
    return -1;
}

int _fork() {
    errno = ENOSYS;
    return -1;
}

int _kill(int pid, int sig) {
  errno = ENOSYS;
  return (-1);
}

int _link(char *old, char *new) {
  errno = ENOSYS;
  return -1;
}

int _lseek(int file, int ptr, int dir) {
  errno=ENOSYS;
  return -1;
}

int _fstat(int file, struct stat *st) {
  errno=ENOSYS;
  return -1;
}

int _stat(const char *filepath, struct stat *st) {
    errno=ENOSYS;
    return -1;
}

clock_t _times(struct tms *buf) {
   errno=ENOSYS;
   return -1;
}

int _unlink(char *name) {
   errno = ENOSYS;
   return -1;
}

int _wait(int status) {
    errno = ENOSYS;
    return -1;
}

/*
 getpid
 Return current process ID (only one process).
*/

int _getpid() {
  return 1;
}

/*
 isatty
 Query whether output stream is a terminal. For consistency with the other
 minimal implementations,
*/

int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        errno = EBADF;
        return 0;
    }
}

#ifndef STDOUT_USART
#define STDOUT_USART USART2
#endif

#ifndef STDERR_USART
#define STDERR_USART USART2
#endif

#ifndef STDIN_USART
#define STDIN_USART USART2
#endif

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */

int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
	  while ((STDIN_USART.SR & (1<<5)) == 0) {}
            *ptr++ = STDIN_USART.DR & 0xff;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */

int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /* stdout */
        for (n = 0; n < len; n++) {

    	  while ((STDOUT_USART.SR & (1<<7)) == 0) {}
          STDOUT_USART.DR = *ptr++;
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {

    	  while ((STDERR_USART.SR & (1<<7)) == 0) {}
          STDERR_USART.DR = *ptr++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

/*
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */

caddr_t _sbrk(int incr) {

    extern char __heap_bottom; /* Defined by the linker */
    static char *heap_top=&__heap_bottom;

    char *prev_heap_top=heap_top;

    register char * stack_bottom ;

    asm volatile ("MRS %0, msp\n" : "=r" (stack_bottom) );

     if (heap_top + incr + 0x100 > stack_bottom)
     {
       _write (STDERR_FILENO, "Out of heap space !!!\r\n", 23);
       errno = ENOMEM;
       return  (caddr_t) -1;
       exit(255);
     } else {
       heap_top += incr;
       return (caddr_t) prev_heap_top;
     }
}

/*
 environ
 A pointer to a list of environment variables and their values.
 For a minimal environment, this empty list is adequate:
 */

char *__env[1] = { 0 };
char **environ = __env;
