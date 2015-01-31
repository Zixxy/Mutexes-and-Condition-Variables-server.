#include <minix/type.h> // <--definition of endpoint inside
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "mutexes.h"

#define POSSIBLE_RESERVATIONS 1000
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

struct Reservation* reservations;

void create_mutexes(){
	reservations = malloc(POSSIBLE_RESERVATIONS * sizeof(Reservation));
	for(int i = 0; i < POSSIBLE_RESERVATIONS; ++i){
		reservations[i].who_has = NOBODY_HAS;
		reservations[i].next_pending = NULL;
		reservations[i].last_pending = NULL;
	}
}

int try_reservate(int which, endpoint_t who, int number){
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
	for(int i = 0; i < POSSIBLE_RESERVATIONS; ++i){
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
	for(int i = 0; i < POSSIBLE_RESERVATIONS; ++i){
		if(reservations[i].number == number){
			which = i;
			break;
		}
	}
	if(which == -1)
		return IT_IS_NOT_YOURS;

	if(reservations[which].who_has != who){
		return IT_IS_NOT_YOURS;
	}
	else{
		if(reservations[which].next_pending == NULL){
			reservations[which].who_has = NOBODY_HAS;
			return SUCCESS;
		}
		else{
			reservations[which].who_has = reservations[which].next_pending -> who;
			//notify who_has!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
