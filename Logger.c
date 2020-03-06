#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
void ust_sviz(void);
void rabcikl(void);
int clientSocketFD;

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

    char buf=3;
    write(clientSocketFD,&buf,sizeof(buf));
}
struct data{
int k;
int f;
unsigned char soob[1500];
};
void rabcikl(void){
    struct data buf;

    FILE *fail;
    if((fail=fopen("logging","w"))==NULL){
        fprintf(stdout,"Error on opening file");
        exit(0);
    }
    int b=1;
	while(b){
        buf.k=0;
        buf.f=0;
        memset(&buf.soob,0,sizeof(buf.soob));
        int k=read(clientSocketFD,&buf,sizeof(buf));

        if(buf.f==-2)
            fprintf(fail,"\n Soobwenie ot clienta k serveru ");
        if(buf.f==-1)
            fprintf(fail,"\n Paket ne proshol filtaciu ");
        if(buf.f>=0)
            fprintf(fail,"\nPaket proshel filtr i bil otpravlen slavu s poridkovim nomerom:%i ",buf.f+1);
        fprintf(fail,"Paket lenght:%i \n Soderwanie paketa:",buf.k);

        for(int i=0;i<buf.k;i++){
            int t=0;
            t=buf.soob[i];
            fprintf(fail,"%i ",t);
        }

    }
}

int main(int argc, const char *argv[])
{
   ust_sviz();
   rabcikl();
   fprintf(stdout,"\n\n LOG: Terminated\n\n");
    return 0;

}
