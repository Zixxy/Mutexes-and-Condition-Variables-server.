int cs_wait(int cond_var_id, int mutex_id, endpoint_t who);
int cs_broadcast(int cond_var_id);
void initialize_cv();
int cs_remove(endpoint_t who);