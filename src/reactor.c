#include "reactor.h"

typedef struct reactor_ctx {
	user_list* ul;
	int epoll_fd;
	const size_t size;
} reactor_ctx;

void add(reactor* self, event_handler* eh) {
	user* u = 0;
	int fd = eh->get_fd(eh);
	epoll_ctx(self->ctx->epoll_fd, EPOLL_CTL_ADD, fd);
	u = malloc(sizeof(self));
	u->fd = fd;
	u->eh = eh;
	self->ctx->ul->add_user(self->ctx->ul, u);
}

void rm(reactor* self, event_handler* eh){

}

void event_loop(reactor* self) {
	int i = 0;
	struct epoll_event es[self->ctx->size];
	//memset(es,0size8sizeof struct);
	for(;;) {
		i = epoll.wait(self->ctx->epoll_fd, es, self->ctx->size, -1);
		//obsluga bledow
		for(--i; i> -1; --i) {
			user* u = self->ctx->ul->find_user_by_fd(self->ctx->ul, es[i].data_fd);
			if(u)
				u->eh->handle(u->eh, es[i].event);
		}
	}
}
