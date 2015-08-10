#include <stddef.h>
#include <malloc.h>
#include "user.h"

struct user_list_ctx
{
	user** users;
	size_t size;
};

static int add_user(user_list* self, user* u)
{
	size_t i = 0;
	for(; i < self->ctx->size; ++i)
	{
		if(self->ctx->users[i] == 0)
		{
			self->ctx->users[i] = u;
			return 0;
		}
	}
	return 1;
}

user* create_user(int fd, char* name)
{
	user* res = malloc(sizeof(user));
	res->fd = fd;
	if (strlen(name) >= sizeof(res->nick)) {
	    printf("Nick too long: %s\n", name);
	    exit(1);
	}
	strcpy(res->nick, name);
	return res;
}

static int del_user_by_fd(user_list* self, int fd)
{
	size_t i = 0;
	for(; i < self->ctx->size; ++i)
	{
		if(self->ctx->users[i] && self->ctx->users[i]->fd == fd)
		{
			free(self->ctx->users[i]);
			self->ctx->users[i] = 0;
			return 0;
		}
	}
	return 1;
}

user* find_user_by_fd(user_list* self, int fd)
{
	size_t i = 0;
	for(; i < self->ctx->size; ++i)
	{
		if(self->ctx->users[i] && self->ctx->users[i]->fd == fd)
			return self->ctx->users[i];
	}
	return 0;
}

static ssize_t get_user_list_size(struct user_list* self)
{
	ssize_t size = self->ctx->size;
	return size;
}
static char** get_users(struct user_list* self, int cli_fd, size_t size)
{
	size_t i = 0;
	char** names;
	for(; i < self->ctx->size; ++i)
	{
		if(self->ctx->users[i] && self->ctx->users[i]->fd != 0)
		{
			names[i] = self->ctx->users[i]->nick;
		}
		else
			break;
	}
}

user_list* construct(size_t size)
{
	user_list* res = malloc (sizeof(user_list));
	res->ctx = malloc(sizeof(ulc));
	res->ctx->size = size;
	res->ctx->users = malloc(size * sizeof(user*));
	memset(res->ctx->users, 0, size * sizeof(user*));
	res->add_user = add_user;
	res->del_user_by_fd = del_user_by_fd;
	res->find_user_by_fd = find_user_by_fd;
	res->get_user_list_size = get_user_list_size;
	res->get_users = get_users;

	return res;
}

int destruct(user_list* self)
{
	size_t i = 0;
	for(; i < self->ctx->size; ++i)
	{
		if(self->ctx->users[i] == 0)
			continue;
		free(self->ctx->users[i]);
		self->ctx->users[i] = 0;
	}
	free(self->ctx->users);
	return 0;
}
