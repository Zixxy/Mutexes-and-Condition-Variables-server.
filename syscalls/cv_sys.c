#include <cv_sys.h>

int cs_lock(int mutex_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = mutex_id;

    return _syscall(CV, 1, &msg);
}

int cs_unlock(int mutex_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = mutex_id;

    return _syscall(CV, 2, &msg);
}

int cs_wait(int cond_var_id, int mutex_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = mutex_id;
    msg.m1_i2 = cond_var_id;

    return _syscall(CV, 3, &msg);
}

int cs_broadcast(int cond_var_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = cond_var_id;

    return _syscall(CV, 4, &msg);
}