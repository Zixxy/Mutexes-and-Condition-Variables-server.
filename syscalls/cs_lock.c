#include <sys/cdefs.h>

#define m1_i1  m_u.m_m1.m1i1

int cs_lock(int mutex_id){
	int CV;
    endpoint_t endpoint = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m_type = 1;
    msg.m1_i1 = mutex_id;

    return _syscall(val, endpoint, &msg);
}