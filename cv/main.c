#include "lib.h"
#include "stdio.h"

#define true 1

int callnr;
endpoint_t who_e;

static void sef_local_startup(void);
static int sef_cb_init_fresh(int type, sef_init_info_t *info);
static void sef_cb_signal_handler(int signo);

int main(int argc, char *argv[]){
	message m;

	env_setargs(argc, argv);
    sef_local_startup();
	
	while(true){
		int r;
		int status;
		if((r = sef_receive(ANY, &m, &status)) != OK) //tu ma byc OK zapytac
			printf("receive failed %d.\n", r);
		else{
			printf("success!\n");
			break;
		}
		printf("CV received %d %d from %d\n", r, callnr, who_e);
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