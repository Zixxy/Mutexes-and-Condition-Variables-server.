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
		if((r = sef_receive(ANY, &m)) != OK)	
			printf("receive failed %d.\n", r);
		resolve_message(m);
	}
}

static void resolve_message(message m){
	
	endpoint_t who_e;
	int call_type = m.m_type;
	who_e = m.m_source;

	int result;
	int res1, res2;
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
		res1 = cs_remove(m.PM_PROC);
		res2 = remove_signalled(m.PM_PROC);
		if(res1 == SUCCESS || res2 == SUCCESS){
			who_e = m.PM_PROC;
			result = (-1)*EINTR;
		}
		else
			result = EDONTREPLY;
		break;

		case PROC_TERMINATED:
		cs_remove(m.PM_PROC);
		remove_process(m.PM_PROC);
		result = EDONTREPLY;
		break;

		default:
        result = (-1)*EINVAL;
	}
	if (result != EDONTREPLY && (result * (-1) != EDONTREPLY)) {
		result *= (-1);
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
 
static void sef_cb_signal_handler(int signo){}