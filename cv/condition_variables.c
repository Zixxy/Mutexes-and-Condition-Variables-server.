#include <minix/type.h> // <--definition of endpoint inside
#include <sys/errno.h>

#include "constants.h"
#include "mutex.h"

#define POSSIBLE_MUTEXES 1024

typedef struct condition_variable{
	int id;
	int size;
	endpoint_t processes[POSSIBLE_PROCESSES];
	int mutexes[POSSIBLE_PROCESSES];
} condition_variable;

struct condition_variable* cvs;

void initialize(){
	cvs = malloc(POSSIBLE_CVS * sizeof(Reservation));
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		cvs[i].size = 0;
	}
}

int cs_wait(int cond_var_id, int mutex_id, endpoint_t who){
	int unlock_result = unlock_mutex(mutex_id, who);
	if(unlock_result == EPERM)
		return EINVAL;

	int first_not_used = -1;

	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(cvs[i].id == cond_var_id){
			cvs[i].processes[cvs.size] = who;
			cvs[i].mutexes[cvs.size] = mutex_id;
			cvs[i].size++;
			return EDONTREPLY;
		}
		else
			if(first_not_used == -1)
				if(cvs[i].size == 0)
					first_not_used = i;
	}
	// if(first_not_used == -1) should never happen. TODO

	cvs[first_not_used].id = cond_var_id;
	cvs[first_not_used].size++;
	cvs[first_not_used].mutexes[0] = mutex_id;
	cvs[first_not_used].processes[0] = who;
	return EDONTREPLY;
}

int cs_broadcast(int cond_var_id){
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(cvs[i].id == cond_var_id){
			for(int u = 0; u < cvs[i].size; ++u){
				int result = lock_mutex(cvs[i].mutexes[u], cvs[i].processes[u]);
				if(result == SUCCESS){
					send_response(cvs[i].processes[u], SUCCESS);
				}
			}
			return SUCCESS;
			break;
		}
	}
	return EINVAL; // not sure.
}