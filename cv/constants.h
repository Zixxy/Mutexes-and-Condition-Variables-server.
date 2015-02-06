#ifndef _CONST_H
#define _CONST_H

//call_types
#define LOCK_MUTEX 1
#define UNLOCK_MUTEX 2
#define CS_WAIT 3
#define CS_BROADCAST 4	
#define PROC_SIGNALLED 5
#define PROC_TERMINATED 6
//----

#define SUCCESS 0
#define FAILURE -1

#define POSSIBLE_MUTEXES 1024
#define POSSIBLE_CVS 256
#define POSSIBLE_PROCESSES 256

//cannot live without it :D
#define true 1
#define false 0
//----

#endif