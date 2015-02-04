#ifndef _CV_H
#define _CV_H

#include <lib.h>
#include <sys/cdefs.h>
#include <minix/rs.h>
#include <minix/endpoint.h>
#include <stdio.h>

int cs_lock(int);
int cs_unlock(int);
int cs_wait(int, int);
int cs_broadcast(int);

#endif