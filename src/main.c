#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_sockets.h"

#define PORT_NUMBER 8081

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char **argv)
{

    SocketObject server;
    SocketObject accepted_client;

    // char buffer_send[4096];
    char buffer_recv[4096] = {};

    int keep_alive = 1;

    uint16_t len = 0;

    socket_common_init(&server);
    socket_common_init(&accepted_client);

    socket_common_config(&server, socket_type_server, AF_INET, INADDR_ANY, htons(PORT_NUMBER));

    if (socket_common_fd(&server, SOCK_STREAM) < 0) error("failed to get socket fd");

    if (socket_server_bind(&server, 1) < 0) error("failed to bind socket");

    printf("socket bound\r\n");

    if (socket_server_accept(&server, &accepted_client) < 0) error("couldn't accept connection");

    printf("client accepted\r\n");

    while(keep_alive > 0)
    {

        if(socket_client_write(&accepted_client, "\r\n", 3) < 0) // check if still socket still connected
        {
             keep_alive = 0;
        }
        else
        {
            socket_client_read(&accepted_client, buffer_recv, 4096, &len);
        }

        if(!strncmp(buffer_recv, "exit", 4))
        {
            keep_alive = 0;
        }
        else
        {
            printf("%s", buffer_recv);
        }

    }

    printf("Closing connection\r\n");

    socket_common_close(&accepted_client);
    socket_common_close(&server);

    return 0;

}