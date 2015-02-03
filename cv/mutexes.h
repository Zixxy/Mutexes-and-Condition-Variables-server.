#ifndef _MUT_H
#define _MUT_H

#include <minix/type.h> // <--definition of endpoint inside
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

/*in mutexes.c*/
int lock_mutex(int number, endpoint_t who);
int unlock_mutex(int number, endpoint_t who);
void create_mutexes();


/*in main.c */
void send_response(endpoint_t who, int content);

#endif