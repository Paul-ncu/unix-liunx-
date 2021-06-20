#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define oops(m, x) {perror(m);exit(x);}

int make_dgram_client_socket();
int make_internet_address(char *, int, struct sockaddr_in *);
void recv_from_server(int, char *, int, struct sockaddr *, socklen_t *);

int main(int ac, char *av[]) {
    int sock;
    char *msg;
    char buf[BUFSIZ];
    size_t msglen;
    struct sockaddr_in saddr;
    socklen_t saddrlen;

    if (ac != 4) {
        fprintf(stderr, "usage: dgsend host port 'message'\n");
        exit(1);
    }

    msg = av[3];
    if ((sock = make_dgram_client_socket()) == -1)
        oops("cannot make socket", 2);
    
    make_internet_address(av[1], atoi(av[2]), &saddr);

    if (sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
        oops("sendto failed", 3);

    saddrlen = sizeof(saddr);

    if ((msglen = recvfrom(sock, buf, BUFSIZ, 0, (struct sockaddr *)&saddr, &saddrlen)) > 0) {
        buf[msglen] = '\0';
        printf("dgsend: got a reply: %s\n", buf);
    }
    return 0;
}
    
