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
