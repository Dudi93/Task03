#include <stdint.h>
typedef struct reactor reactor;
typedef struct event_handler event_handler;

static int get_handle_cli(event_handler* self);
static void handle_cli(event_handler* self, uint32_t events);
event_handler *alloc_client_eh(int fd, reactor *r);
void free_client_eh (event_handler *eh);
