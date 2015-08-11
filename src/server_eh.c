#include "server_eh.h"
#include "client_eh.h"
#include "reactor.h"
#include <sys/epoll.h>
#include <arpa/inet.h>

typedef struct event_handler_ctx {
	int fd;
	reactor *r;
} event_handler_ctx;

int get_fd(event_handler *self){
	return self->ctx->fd;
}

void handle_event(event_handler *self, uint32_t e){
	event_handler *eh=0;
	int cli_fd=-1;
	struct sockaddr_in cli_addr;
	socklen_t cli_addr_len = sizeof(cli_addr);
	cli_fd = accept (self->ctx->fd, &cli_addr, &cli_addr_len);
	eh = alloc_client_eh(cli_fd, self->ctx->r);
	self->ctx->r->add(self->ctx->r,eh);
}

event_handler *alloc_server_eh(reactor *r) {
	event_handler *seh=0;
	event_handler_ctx *ctx=0;

	int srv_fd = -1;
	struct sockaddr_in srv_addr;

	memset(&srv_addr, 0, sizeof(struct sockaddr_in));

	srv_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (srv_fd < 0) {
		printf("Cannot create socket\n");
		return 0;
	}

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(5557);
	if (bind(srv_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0) {
		printf("Cannot bind socket\n");
		close(srv_fd);
		return 0;
	}

	if (listen(srv_fd, 1) < 0) {
		printf("Cannot listen\n");
		close(srv_fd);
		return 0;
	}

	seh=malloc(sizeof(event_handler));
	ctx=malloc(sizeof(event_handler_ctx));
	ctx->fd=srv_fd;
	ctx->r=r;
	seh->ctx=ctx;
	seh->get_fd=get_fd;
	seh->handle=handle_event;

	return seh;

}
