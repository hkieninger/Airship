/*
 * counterpart is DataOutputStreamTest01.java
 */

#include <stdio.h>
#include <string.h>

#include "../socket/socket_exception.h"
#include "../socket/server_socket.h"
#include "../socket/socket.h"

#define PORT 0xCCCC

int main() {
    ServerSocket server(PORT);
    Socket sock = server.acceptConnection();
    printf("client %s has connected\n", sock.getRemoteIPString().c_str());

    try {
        int c = 0;
        sock.recvAll(&c, 1);
        printf("%x\n", c);
        sock.recvAll(&c, 1);
        printf("%x\n", c);
    } catch(SocketClosedException &e) {
        printf("\nconnection has been closed by client\n");
    }
    return 0;
}