#define SUCCESS 0

/*in mutexes.c*/
int lock_mutex(int number, endpoint_t who);
int unlock_mutex(int number, endpoint_t who);
void create_mutexes();


/*in main.c */
void send_response(endpoint_t who, int content);
