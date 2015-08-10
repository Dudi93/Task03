typedef struct user
{
	int fd;
	char nick[50];
} user;

typedef struct user_list_ctx ulc;
typedef struct user_list
{
	ulc* ctx;
	int(*add_user)(struct user_list* self, user* u);
	int(*del_user_by_fd)(struct user_list* self, int fd);
	user*(*find_user_by_fd)(struct user_list* self, int fd);
	ssize_t(*get_user_list_size)(struct user_list* self);
	void (*get_users)(struct user_list* self, int cli_fd, char* strings[], size_t size);
} user_list;

user_list* construct(size_t size);
user* create_user(int fd, char* name);
int destruct(user_list* self);
