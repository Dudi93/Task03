#include <stdlib.h>
#include <stdint.h>

typedef struct user_list user_list;
typedef struct event_handler_ctx ehc;

typedef struct event_handler {
	ehc* ctx;
	void (*handle)(struct event_handler *self, uint32_t events);
	int (*get_fd)(struct event_handler *self);
} event_handler;

typedef struct reactor_ctx rc;
typedef struct reactor {
	rc* ctx;
	void (*add)(struct reactor *self, event_handler *eh);
	void (*rm)(struct reactor *self, event_handler *eh);
	void (*event_loop)(struct reactor *self);
} reactor;

reactor* alloc_reactor(size_t size, user_list* ul);
void free_reactor(reactor* r);
