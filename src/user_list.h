#include <stdlib.h>

typedef struct event_handler event_handler;
typedef struct user_list user_list;

#define MAX_USERS 1000
#define MAX_USER_NAME 50

struct user {
	int fd;
	char name[MAX_USER_NAME];
	event_handler *eh;
};

struct user_list_ctx;

struct user_list {
	void (*add_user)(struct user_list_ctx* ctx, struct user* u);
	void (*rm_user_by_fd)(struct user_list_ctx* ctx, int fd);
	const struct user* (*get_user_by_fd)(struct user_list_ctx* ctx, int fd);
	size_t (*current_size)(struct user_list_ctx* ctx);
	const char** (*get_user_names)(struct user_list_ctx* ctx, size_t* len);

	struct user_list_ctx* ctx;
};

struct user_list* create_user_list();
void delete_user_list(struct user_list* ul);
