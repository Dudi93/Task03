#include "server_eh.h"
#include "client_eh.h"

typedef struct event_handler_ctx {
	int fd;
	reactor* r;
} event_handler_ctx;

static int get_fd(event_handler* self) {
	return self->ctx->fd;
}

static void handle_event(event_handler* self, uint32_t e) {
	event_handler* eh = 0;
	int cli_fd = -1;
	struct sockaddr_in cli_addr;
	socklen_t cli_addr_len = sizeof(struct sockaddr_in);
	cli_fd = accept(self->ctx->fd, &cli_addr, &cli_addr_len);
	eh = alloc_client_eh(cli_fd);
	self->ctx->r->add(self->ctx->r, eh);
}

event_handler* alloc_server_eh(reactor* r) {
	event_handler* seh = 0;
	event_handler_ctx* ctx = 0;
	int fd = socket();
	//bind(fd,.....);
	//listen(fd,.....);
	seh = malloc(sizeof(event_handler));
	ctx=malloc(sizeof(event_handler_ctx));
	ctx->fd = fd;
	ctx->r = r;
	seh->ctx = ctx;
	seh->get_fd = get_fd;
	seh->handle = handle_event;
	return seh;

}
