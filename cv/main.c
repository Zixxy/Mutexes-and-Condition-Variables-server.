#include "lib.h"
#include "stdio.h"

#define true 1

void sef_setcb_init_fresh(int type, sef_init_info_t *info){}
void sef_setcb_init_restart(){}

int main(int argc, char *argv[]){
	message m;

	sef_local_startup();

	while(true){
		int r;
		int status;
		if((r = sef_receive(ANY, &m, &status)) != 0) //tu ma byc OK
			printf("receive failed %d.\n", r);
		else
			printf("success!\n");

	}
}



static void sef_local_startup()
{
  /* Register init callbacks. */
  sef_setcb_init_fresh(int type, sef_init_info_t *info); // this is in ipc main CHECK IT OUT
  sef_setcb_init_restart(sef_cb_init_fresh);

  /* No live update support for now. */

  /* Register signal callbacks. */
  sef_setcb_signal_handler(sef_cb_signal_handler);

  /* Let SEF perform startup. */
  sef_startup();
}
