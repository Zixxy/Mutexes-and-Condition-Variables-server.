#ifndef _MUT_H
#define _MUT_H

#include <minix/type.h> // <--definition of endpoint inside
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

int lock_mutex(int, endpoint_t);
int unlock_mutex(int, endpoint_t);
void create_mutexes();

void remove_process(endpoint_t);
int remove_signalled(endpoint_t);

#endif