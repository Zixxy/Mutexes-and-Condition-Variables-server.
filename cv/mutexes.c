#include "mutexes.h"
#include "includes.h"

#define NOBODY_HAS -1 // endpoint is int. Processess have pids greater than 0(am i sure?? - check it). We can use it as flag.

typedef struct Pender{
	struct Pender* next_pending;
	endpoint_t who;
} Pender;

typedef struct Reservation{
	endpoint_t who_has;
	struct Pender* next_pending;
	struct Pender* last_pending;
	int number;
} Reservation;

struct Reservation* reservations; // MEMORY LEAKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void create_mutexes(){
	reservations = malloc(POSSIBLE_MUTEXES * sizeof(Reservation));
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		reservations[i].who_has = NOBODY_HAS;
		reservations[i].next_pending = NULL;
		reservations[i].last_pending = NULL;
	}
}

void remove_process(endpoint_t who){
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(reservations[i].who_has != NOBODY_HAS){
			if(reservations[i].who_has == who){
				if(lose_mutex(who) == SUCCESS)
				continue;
			}

			if(reservations[i].next_pending == NULL)
				continue;

			if(reservations[i].next_pending -> who == who){
				reservations[i].next_pending = reservations[i].next_pending -> next_pending;
				continue;
			}

			if(reservations[i].next_pending -> next_pending == NULL)
				continue;

			Pender* current = reservations[i].next_pending -> next_pending;
			Pender* prev = reservations[i].next_pending;
			do{
				if(current -> who == who){
					prev -> next_pending = current -> next_pending;
					break;
				}
				prev = current;
				current = current -> next_pending;
			}while(current != NULL);
		}
	}
}

int remove_signalled(endpoint_t who){
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(reservations[i].who_has != NOBODY_HAS){
			if(reservations[i].next_pending == NULL)
				continue;

			if(reservations[i].next_pending -> who == who){
				reservations[i].next_pending = reservations[i].next_pending -> next_pending;
				return SUCCESS;
			}

			if(reservations[i].next_pending -> next_pending == NULL)
				continue;

			Pender* current = reservations[i].next_pending -> next_pending;
			Pender* prev = reservations[i].next_pending;
			do{
				if(current -> who == who){
					prev -> next_pending = current -> next_pending;
					return SUCCESS;
				}
				prev = current;
				current = current -> next_pending;
			}while(current != NULL);
		}
	}
	return FAILURE;
}

int try_reservate(int which, endpoint_t who, int number){
	printf("%d reservates %d with number %d\n", who, which, number);
	if(reservations[which].who_has == who){ // he already has it. Cannot take it twice.
		return FAILURE;
	}
	if(reservations[which].who_has == NOBODY_HAS){
		reservations[which].number = number;
		reservations[which].who_has = who;
		return SUCCESS;
	}
	else{
		Pender* new_pender = malloc(sizeof(Pender));
		new_pender -> next_pending = NULL;
		new_pender -> who = who;

		if(reservations[which].last_pending == NULL){
			reservations[which].next_pending = new_pender;
			reservations[which].last_pending = new_pender;
			return EDONTREPLY;
		}
		else{
			reservations[which].last_pending -> next_pending = new_pender;
			reservations[which].last_pending = new_pender;
			return EDONTREPLY;
		}

	}
}

int lock_mutex(int number, endpoint_t who){
	int first_not_used = -1;
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(reservations[i].number == number)
			return try_reservate(i, who, number);
		else
			if(first_not_used == -1)
				if(reservations[i].who_has == NOBODY_HAS)
					first_not_used = i;
	}
	return try_reservate(first_not_used, who, number);
}

int unlock_mutex(int number, endpoint_t who){
	int which = -1;
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(reservations[i].number == number){
			which = i;
			break;
		}
	}
	printf("which %d who %d\n", which, who);
	if(which == -1)
		return EPERM;

	if(reservations[which].who_has != who){
		return EPERM;
	}
	else{
		if(reservations[which].next_pending == NULL){
			reservations[which].who_has = NOBODY_HAS;
			return SUCCESS;
		}
		else{
			reservations[which].who_has = reservations[which].next_pending -> who;
			//temporrary solution
			message m;
			m.m_type = SUCCESS;
			send(reservations[which].who_has, &m);
			//----
			if(reservations[which].next_pending == reservations[which].last_pending){
				reservations[which].next_pending = NULL;
				reservations[which].last_pending = NULL;
				return SUCCESS;
			}
			else{
				reservations[which].next_pending = reservations[which].next_pending -> next_pending;
				return SUCCESS;
			}
		}
	}
}

int lose_mutex(endpoint_t who){
	int which = -1;
	for(int i = 0; i < POSSIBLE_MUTEXES; ++i){
		if(reservations[i].who_has == who){
			which = i;
			break;
		}
	}
	if(which == -1)
		return EPERM;

	if(reservations[which].who_has != who){
		return EPERM;
	}
	else{
		if(reservations[which].next_pending == NULL){
			reservations[which].who_has = NOBODY_HAS;
			return SUCCESS;
		}
		else{
			reservations[which].who_has = reservations[which].next_pending -> who;
			//temporrary solution
			message m;
			m.m_type = SUCCESS;
			send(reservations[which].who_has, &m);
			//----
			if(reservations[which].next_pending == reservations[which].last_pending){
				reservations[which].next_pending = NULL;
				reservations[which].last_pending = NULL;
				return SUCCESS;
			}
			else{
				reservations[which].next_pending = reservations[which].next_pending -> next_pending;
				return SUCCESS;
			}
		}
	}
}
