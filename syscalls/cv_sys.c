#include <cv_sys.h>

int cs_lock(int mutex_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = mutex_id;

    int result;
    while((result = _syscall(CV, 1, &msg)) == -1){
        if(errno == EINTR)
            continue;
        else
            return -1;
    }
    return result;
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

    int result;
    if((result = _syscall(CV, 3, &msg)) == -1 && errno == EINTR){
        int l = cs_lock(mutex_id);
        if(l != -1)
            return 0;
        return l;
    }
    return result;
}

int cs_broadcast(int cond_var_id){
	int CV;
    int status = minix_rs_lookup("cv", &CV);
    message msg;
    msg.m1_i1 = cond_var_id;

    return _syscall(CV, 4, &msg);
}