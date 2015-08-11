#include "client_eh.h"
#include "reactor.h"

typedef struct event_handler_ctx {
	int fd;
	reactor *r;
} event_handler_ctx;

static int get_handle_cli(event_handler* self)
{
	//TODO
}

static void handle_cli(event_handler* self, uint32_t events)
{
	//TODO
}

event_handler *alloc_client_eh(int fd, reactor *r)
{
	event_handler *eh = malloc(sizeof(event_handler));
	eh->ctx = malloc(sizeof(event_handler_ctx));
	eh->ctx->fd = fd;
	eh->ctx->r = r;

	//TODO methods

	return eh;
}
void free_client_eh (event_handler *eh){
	//TODO free
}
