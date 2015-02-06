#include "includes.h"

static void sef_local_startup(void);
static int sef_cb_init_fresh(int type, sef_init_info_t *info);
static void sef_cb_signal_handler(int signo);
static void resolve_message(message m);

void send_response(endpoint_t who, int content){
	message m;
	m.m_type = content;
	send(who, &m);
}

int main(int argc, char *argv[]){
	message m;

	env_setargs(argc, argv);
    sef_local_startup();

    create_mutexes();
    initialize_cv();
    
	while(true){
		int r;
		if((r = sef_receive(ANY, &m)) != OK) //tu ma byc OK zapytac 	
			printf("receive failed %d.\n", r);
		resolve_message(m);
	//	printf("CV received %d %d from %d\n", r, callnr, who_e);
	}
}

static void resolve_message(message m){
	
	endpoint_t who_e;
	int call_type = m.m_type;
	who_e = m.m_source;

	printf("received type: %d with number %d \n", call_type, m.m1_i1);

	int result;
	switch(call_type){
		case LOCK_MUTEX:
		result = lock_mutex(m.m1_i1, who_e);
		break;

		case UNLOCK_MUTEX:
		result = unlock_mutex(m.m1_i1, who_e);
		break;

		case CS_WAIT:
		result = cs_wait(m.m1_i2, m.m1_i1, who_e);
		break; 

		case CS_BROADCAST:
		result = cs_broadcast(m.m1_i1);
		break;

		case PROC_SIGNALLED:
		int r1 = cs_remove(m.m1_i1);
		int r2 = remove_signalled(m.m1_i1);
		if(r1 == SUCCESS || r2 == SUCCESS)
			result = EINTR;
		else
			result = EDONTREPLY;
		break;

		case PROC_TERMINATED:
		cs_remove(m.m1_i1);
		remove_process(m.m1_i1);
		result = EDONTREPLY;
		break;

		default:
        printf("CV warning: got illegal request from %d\n", who_e); //debug
        m.m_type = -EINVAL;
        result = EINVAL;
	}
	if (result != EDONTREPLY && (result * (-1) != EDONTREPLY)) {
		printf("result = %d EDONTREPLY = %d\n", result, EDONTREPLY);
		send_response(who_e, result);
	}
}

static void sef_local_startup()
{
    /* Register init callbacks. */
    sef_setcb_init_fresh(sef_cb_init_fresh);
    sef_setcb_init_restart(sef_cb_init_fresh);
 
    /* Register signal callbacks. */
    sef_setcb_signal_handler(sef_cb_signal_handler);
    /* Let SEF perform startup. */
    sef_startup();
}
 
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
    return OK;
}
 
static void sef_cb_signal_handler(int signo)
{
    if (signo != SIGTERM) return;
}