/*
 * needs server_socket_test01
 */

#include <stdio.h>

#include "../socket/socket.h"

#define PORT 0xCCCC
#define IP "192.168.4.1" //"127.0.0.1"

int main() {
    Socket sock(Socket::stringToInt(IP), PORT);
    char c = 0xFF;
    while(c != '\0') {
        sock.recvAll(&c, 1);
        printf("%c", c);
    }
    char text[] = "Hello Server. Bye Server.";
    sock.sendAll(text, sizeof(text));
    return 0;
}