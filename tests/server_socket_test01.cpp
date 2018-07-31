/*
 * needs socket_test01
 */

#include <stdio.h>
#include <string.h>

#include "../socket/socket_exception.h"
#include "../socket/server_socket.h"
#include "../socket/socket.h"

#define PORT 0xCCCC

void handleConnection(ServerSocket &server, int number) {
    Socket sock = server.acceptConnection();
    printf("client %s has connected\n", sock.getRemoteIPString().c_str());

    char text[255];
    snprintf(text, sizeof(text), "Hello Client, I am your Server. This is the %d connection!\n", number);
    sock.sendAll(text, strlen(text) + 1);
    try {
        while(true) {
            char c;
            sock.recvAll(&c, 1);
            printf("%c", c);
        }
    } catch(SocketClosedException &e) {
        printf("\nconnection has been closed by client \n");
    }
}

int main() {
    ServerSocket server(PORT);
    for(int i = 0; i < 3; i++) {
        handleConnection(server, i);
    }
    return 0;
}