	#include <minix/type.h> // <--definition of endpoint inside
#include <sys/errno.h>

#include "includes.h"
#include "constants.h"
#include "mutexes.h"
#include "condition_variables.h"

typedef struct condition_variable{
	int id;
	int size;
	endpoint_t processes[POSSIBLE_PROCESSES];
	int mutexes[POSSIBLE_PROCESSES];
} condition_variable;

struct condition_variable* cvs;

void initialize_cv(){
	cvs = malloc(POSSIBLE_CVS * sizeof(condition_variable));
	for(int i = 0; i < POSSIBLE_CVS; ++i){
		cvs[i].size = 0;
	}
}

int cs_remove(endpoint_t who){
	int flag = false;
	for(int i = 0; i < POSSIBLE_CVS; ++i){
		for(int u = 0; u < cvs[i].size; ++u){
			if(flag){
				cvs[i].processes[u - 1] = cvs[i].processes[u];
				cvs[i].mutexes[u - 1] = cvs[i].mutexes[u];
			}
			if(cvs[i].processes[u] == who){
				flag = true;
			}
		}
		if(flag){
			--cvs[i].size;
			return SUCCESS;
		}
	}
	return FAILURE;
}

int cs_wait(int cond_var_id, int mutex_id, endpoint_t who){
	int unlock_result = unlock_mutex(mutex_id, who);
	if(unlock_result == EPERM)
		return EINVAL;
	int first_not_used = -1;

	for(int i = 0; i < POSSIBLE_CVS; ++i){
		if(cvs[i].id == cond_var_id){
			cvs[i].processes[cvs[i].size] = who;
			cvs[i].mutexes[cvs[i].size] = mutex_id;
			cvs[i].size++;
			return EDONTREPLY;
		}
		else
			if(first_not_used == -1)
				if(cvs[i].size == 0)
					first_not_used = i;
	}

	cvs[first_not_used].id = cond_var_id;
	cvs[first_not_used].size++;
	cvs[first_not_used].mutexes[0] = mutex_id;
	cvs[first_not_used].processes[0] = who;
	return EDONTREPLY;
}

int cs_broadcast(int cond_var_id){
	for(int i = 0; i < POSSIBLE_CVS; ++i){
		if(cvs[i].id == cond_var_id){
			for(int u = 0; u < cvs[i].size; ++u){
				int result = lock_mutex(cvs[i].mutexes[u], cvs[i].processes[u]);
				if(result == SUCCESS){
					message m;
					m.m_type = SUCCESS;
					send(cvs[i].processes[u], &m);
				}
			}
			cvs[i].size = 0;
			return SUCCESS;
			break;
		}
	}
	return EINVAL;
}