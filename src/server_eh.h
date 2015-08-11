typedef struct reactor reactor;
typedef struct event_handler event_handler;

event_handler* alloc_server_eh (reactor* r);
void free_server_eh (event_handler* eh);
