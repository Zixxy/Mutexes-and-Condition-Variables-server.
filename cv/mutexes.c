#include <mutexes.h>
#include <minix/type.h> // <--definition of endpoint

#define POSSIBLE_RESERVATIONS 1000
#define NOBODY_HAS -1 // endpoint is int. Processess have pids greater than 0(am i sure?? - check it). We can use it as flag.

struct Pender {
	Pender* next_pending;
	endpoint_t who;
};

struct Reservation {
	endpoint_t who_has;
	Pender* next_pending;
	Pender* last_pending;
	int number;
};

Reservation* reservations;

int create_mutexes(){
	reservations = new Reservation()[POSSIBLE_RESERVATIONS];
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
		Pender* new_pender = new Pender();
		new_pender.next_pending = NULL;
		new_pender.who = who;

		if(reservations[which].last_pending == NULL){
			reservations[which].next_pending = new_pender;
			reservations[which].last_pending = new_pender;
			return WAIT;
		}
		else{
			reservations[which].last_pending -> next_pending = new_pender;
			reservations[which].last_pending = new_pender;
			return WAIT;
		}

	}
}

int lock_mutex(int number, endpoint_t who){
	int first_not_used = -1;
	for(int i = 0; i < POSSIBLE_RESERVATIONS; ++i){
		if(reservations[i].number == number)
			return result = try_reservate(i, who, number);
		else
			if(first_not_used == -1)
				if(reservations[i].who_has == NOBODY_HAS)
					first_not_used = i;
	}
	return result = try_reservate(first_not_used, who, number);
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
			reservations[which].who_has = reservations[which].next_pending.who;
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
