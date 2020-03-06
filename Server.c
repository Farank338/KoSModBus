#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/select.h>
#include <arpa/inet.h>
void ust_sviz(void);
void recvset(void);
void closes(void);
int starts(void);
int predstart(void);
void rabcikl(void);

struct serv{
    char adress[16];
    int port;

};
struct serv server;
int clientSocketFD;
int serverFD;
int ConnectFD;

void ust_sviz(void){
    clientSocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == clientSocketFD)
    {
      perror("cannot create socket");
      return EXIT_FAILURE;
    }
    struct sockaddr_in stSockAddr;
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(43210);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *resultHints = NULL;
    int res = getaddrinfo("localhost", NULL, &hints, &resultHints);
    if (res != 0)
    {
      perror("cannot get address info");
      close(clientSocketFD);
      return EXIT_FAILURE;
    }

    struct sockaddr_in* in_addr = (struct sockaddr_in *)resultHints->ai_addr;
    memcpy(&stSockAddr.sin_addr.s_addr, &in_addr->sin_addr.s_addr, sizeof(in_addr_t));
    freeaddrinfo(resultHints);
    sleep(1);
    if (-1 == connect(clientSocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
      perror("connect failed");
      close(clientSocketFD);
      return EXIT_FAILURE;
    }

    char buf=1;
    write(clientSocketFD,&buf,sizeof(buf));
}
void recvset(void){
    int temp=0;
    read(clientSocketFD,&server.adress,sizeof(server.adress));
    write(clientSocketFD,&temp,sizeof(temp));
    read(clientSocketFD,&server.port,sizeof(server.port));
    write(clientSocketFD,&temp,sizeof(temp));
    fprintf(stdout,"SERVER: Adress:%16s \n",server.adress);
    fprintf(stdout,"SERVER: Port:%i \n",server.port);
}
void closes(void){
    shutdown(clientSocketFD, SHUT_RDWR);
    close(clientSocketFD);
    shutdown(serverFD, SHUT_RDWR);
    close(serverFD);
    shutdown(ConnectFD, SHUT_RDWR);
    close(ConnectFD);
}
int starts(void){
    serverFD = socket(AF_INET, SOCK_STREAM,0);
    if (-1 == serverFD){
      return 2;
    }
    int optval = 1;
    if (-1 == setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
      return 2;
    }
    struct sockaddr_in stSockAddr;
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(server.port);
    inet_pton(AF_INET, server.adress, &stSockAddr.sin_addr);
    if (-1 == bind(serverFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr))){
      return 2;
    }
    if(-1 == listen(serverFD, 1)){
      return 2;
    }

    int sel;
    struct timeval tv;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(serverFD, &rfds);
    tv.tv_sec = 600;
    tv.tv_usec = 0;
    sel = select(serverFD+1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
    if(sel > 0){
        ConnectFD = accept(serverFD, NULL, 0);
        return 1;
    }
    else{
         return 2;
    }
}
int predstart(void){
    int ss=0;
    ss=starts();
    write(clientSocketFD,&ss,sizeof(ss));
    return ss;
}
struct data{
int k;
int f;
unsigned char soob[1500];
};
void rabcikl(void){
    struct data buf;
    struct pollfd fds[2];
	int timeout_msecs=10;
	int ret,b=1;
	fds[0].fd=clientSocketFD;
	fds[0].events=POLLIN;
	fds[1].fd=ConnectFD;
	fds[1].events=POLLIN;
	while(b){
        buf.k=0;
        buf.f=0;
        memset(&buf.soob,0,sizeof(buf.soob));
		ret=poll(fds,2,timeout_msecs);
		if(ret>0){
            if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nServer: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    close(fds[1].fd);
                    return;
                    break;
                }
            if((fds[1].revents & POLLHUP)||(fds[1].revents & POLLERR)||(fds[1].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nServer: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    close(fds[1].fd);
                    return;
                    break;
                }
            if(fds[0].revents & POLLIN){
                int k=read(clientSocketFD,&buf,sizeof(buf));
                k=send(ConnectFD,&buf.soob,buf.k,0);
            }
            if(fds[1].revents & POLLIN){
                buf.k=read(ConnectFD,&buf.soob,sizeof(buf.soob));
                int k=send(clientSocketFD,&buf,sizeof(buf),0);
            }
        }
         if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nServer: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    close(fds[1].fd);
                    return;
                    break;
                }
                 if((fds[1].revents & POLLHUP)||(fds[1].revents & POLLERR)||(fds[1].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nServer: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    close(fds[1].fd);
                    return;
                    break;
                }
        if(ret<0){
            fprintf(stdout,"\n\nobriv svizi s odnim iz komponentov\n\n");
            return;
        }
    }
}


int main(int argc, const char *argv[])
{
    ust_sviz();
    recvset();
    if(predstart()==2){
    fprintf(stdout,"\n\n SERVER: Terminated\n\n");
    return 0;
    }


    rabcikl();
    fprintf(stdout,"\n\n SERVER: Terminated\n\n");
return 0;
}
