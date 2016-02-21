#include <sys/types.h>
#include <unistd.h>

void new_chat_window (void) ;
void * server_f (void * arg);
void print_msg(char* , pid_t);
int new_user (int );
int clear_msg_window (void);
void delete_user (pid_t );
void clear_term (void);
