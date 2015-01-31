#define SUCCESS 0
#define IT_IS_NOT_YOURS -1

int lock_mutex(int number, endpoint_t who);
int unlock_mutex(int number, endpoint_t who);
void create_mutexes();
