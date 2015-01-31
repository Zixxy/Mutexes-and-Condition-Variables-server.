#include "includes.h"
#include "utils.h"
#include "mutexes.h"

static void sef_local_startup(void);
static int sef_cb_init_fresh(int type, sef_init_info_t *info);
static void sef_cb_signal_handler(int signo);
static void send_response(int value);
int main(int argc, char *argv[]){
	message m;

	env_setargs(argc, argv);
    sef_local_startup();

	storeValue = 0;

	while(true){
		int r;
		if((r = sef_receive(ANY, &m)) != OK) //tu ma byc OK zapytac
			printf("receive failed %d.\n", r);
		else{
			printf("success!\n");
		}
		resolve_message(m);
		printf("CV received %d %d from %d\n", r, callnr, who_e);
	}
}

static void resolve_message(message m){
	int callnr;
	endpoint_t who_e;
	int call_type = m.m_type;
	who_e = m.m_source;
	int result;
	switch(call_type){
		case LOCK_MUTEX:
		result = lock_mutex(m1_i1, who_e);
		break;
		case UNLOCK_MUTEX:
		result = unlock_mutex(m1_i1, who_e);
		break;
		default:
        printf("CV warning: got illegal request from %d\n", who_e); //debug
        m.m_type = -EINVAL;
        result = EINVAL;
	}
	if (result != EDONTREPLY) {
		m.m_type = result;
		send_response(who_e, m);
	}
}

static void send_response(endpoint_t who, message &m){
	int result = send(who_e, &m);
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