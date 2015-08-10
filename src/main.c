#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "server_manager.h"
#include "user.h"

void handle_login(char* buff, int cli_fd);
void handle_userlist(int cli_fd);

user_list* list;

int main(int argc, const char *argv[])
{
	list = construct(10);
    int i = 0;
    ssize_t msg_len = 0;

    int srv_fd = -1;
    int cli_fd = -1;
    int epoll_fd = -1;

    int ile;

    struct sockaddr_in srv_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    struct epoll_event e, es[2];

    memset(&srv_addr, 0, sizeof(srv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));
    memset(&e, 0, sizeof(e));

    srv_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (srv_fd < 0) {
        printf("Cannot create socket\n");
        return 1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(5556);
    if (bind(srv_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0) {
        printf("Cannot bind socket\n");
        close(srv_fd);
        return 1;
    }

    if (listen(srv_fd, 1) < 0) {
        printf("Cannot listen\n");
        close(srv_fd);
        return 1;
    }

    epoll_fd = epoll_create(2);
    if (epoll_fd < 0) {
        printf("Cannot create epoll\n");
        close(srv_fd);
        return 1;
    }

    e.events = EPOLLIN;
    e.data.fd = srv_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv_fd, &e) < 0) {
        printf("Cannot add server socket to epoll\n");
        close(epoll_fd);
        close(srv_fd);
        return 1;
    }

    for(;;) {
        i = epoll_wait(epoll_fd, es, 2, -1);
        if (i < 0) {
            printf("Cannot wait for events\n");
            close(epoll_fd);
            close(srv_fd);
            return 1;
        }

        for (--i; i > -1; --i) {
            if (es[i].data.fd == srv_fd) {
                cli_fd = accept(srv_fd, (struct sockaddr*) &cli_addr, &cli_addr_len);
                if (cli_fd < 0) {
                    printf("Cannot accept client\n");
                    close(epoll_fd);
                    close(srv_fd);
                    return 1;
                }

                e.data.fd = cli_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_fd, &e) < 0) {
                    printf("Cannot accept client\n");
                    close(epoll_fd);
                    close(srv_fd);
                    return 1;
                }
            } else {
                if (es[i].events & EPOLLIN) {
                    //msg_len = read(cli_fd, buff, BUFF_SIZE);
                    //ze chuj sprawdza czy 2.gunwo
                    //jezeli 2.gunwo dodaje gunwo przez add_user
                	char *buff = 0;
                	size_t length = 0;
                	read(cli_fd, &length, sizeof(size_t));
                	//TODO error handling (call clean_client_fd)
                	buff = malloc((length + 1) * sizeof(char));
                	buff[length] = '\0';
                	ile = read(cli_fd, buff, length);
                	//TODO error handling (call clean_client_fd)

                	switch (buff[0]) {
                	case '2':
                		//TODO handle_login(...);
                		handle_login(buff, cli_fd);
                		break;
                	case '6':
                		//TODO hndle_userlist(...);
                		handle_userlist(cli_fd);
                		break;
                	default:
                		//Unknown message handling
                		break;
                	}

                	free(buff);
                }
            }
        }
    }

	return 0;
}

void handle_login(char* buff, int cli_fd)
{
	char* msg;
	strcpy(msg, buff +2);
	user* user = create_user(cli_fd, msg);
	list->add_user(list, user);
}

void handle_userlist(int cli_fd)
{
	char** names;
	size_t size = list->get_user_list_size(list);
	size_t* lengths;

	names=list->get_users(list, cli_fd, size);

	size_t i = 0;

	for(; i < size; ++i)
	{
		char* msg;
		strcpy(msg, names[i][0]);
		lengths[i] = strlen(msg);
	}

	//user* user = list->ctx->users[0];
	//name = user->nick;
	//ssize_t length = strlen(name);
	//write(cli_fd, name, length);
}

void clean_client_fd(int cli_fd, int epoll_fd)
{
	//TODO: remove user from the list
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cli_fd, 0);
	close(cli_fd);
}
