#include "reactor.h"
#include "user_list.h"
#include <sys/epoll.h>

typedef struct reactor_ctx {
	user_list *ul;
	int epoll_fd;
	size_t size;
} reactor_ctx;

static void add(reactor *self, event_handler *eh){
	user* u = 0;
	int fd = eh->get_fd(eh);
	struct epoll_event e;
	memset(&e, 0, sizeof(struct epoll_event));
	e.data.fd=fd;
	e.events = EPOLLIN;
	epoll_ctl(self->ctx->epoll_fd, EPOLL_CTL_ADD, fd, &e);
	//err
	u = malloc(sizeof(user));
	u->fd=fd;
	u->eh=eh;
	self->ctx->ul->add_user(self->ctx->ul,u);
	//err
}

static void rm (reactor *self, event_handler *eh){
	int fd = eh->get_fd(eh);
	self->ctx->ul->rm_user_by_fd(self->ctx, fd);
	//err
	epoll_ctl(self->ctx->epoll_fd, EPOLL_CTL_DEL, fd, 0);
	//err
	close(fd);
}

static void event_loop (reactor *self){
	int i=0;
	struct epoll_event es[self->ctx->size];
	memset(es,0, self->ctx->size);
	for(;;){
		i=epoll_wait(self->ctx->epoll_fd,es,self->ctx->size,-1);
		//err

		for(--i;i>-1;--i){
			user *u = self->ctx->ul->get_user_by_fd(self->ctx->ul,es[i].data.fd);
			if(u)
				u->eh->handle(u->eh,es[i].events);
		}
	}
}

reactor* alloc_reactor(size_t size, user_list* ul){
	reactor* r = 0;
	int epoll_fd = epoll_create(size);
	if (epoll_fd < 0) {
		printf("Cannot create epoll\n");
		return 0;
	}

	r = malloc(sizeof(reactor));
	r->ctx = malloc(sizeof(rc));
	r->ctx->size = size;
	r->ctx->ul = ul;

	r->add = add;
	r->rm = rm;
	r->event_loop = event_loop;

	return r;
}

void free_reactor(reactor* r)
{

}
