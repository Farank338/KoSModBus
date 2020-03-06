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
#include <arpa/inet.h>
void ust_sviz(void);
void recvset(void);
int startc(void);
int predstart(void);
void rabcikl(void);
int clientSocketFD;
struct serv{
    int kol_c;
};
struct cli{
    char adress[16];
    int port;
    int slaveFD;
};

struct serv server;
struct cli client[256];
struct sockaddr_in slave;

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

      return EXIT_FAILURE;
    }

    struct sockaddr_in* in_addr = (struct sockaddr_in *)resultHints->ai_addr;
    memcpy(&stSockAddr.sin_addr.s_addr, &in_addr->sin_addr.s_addr, sizeof(in_addr_t));
    freeaddrinfo(resultHints);
    sleep(1);
    if (-1 == connect(clientSocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
      perror("connect failed");

      return EXIT_FAILURE;
    }

    char buf=4;
    write(clientSocketFD,&buf,sizeof(buf));

}
void recvset(void){
    int temp=0;
    read(clientSocketFD,&server.kol_c,sizeof(server.kol_c));
    fprintf(stdout,"CLIENT: kol_c:%i \n",server.kol_c);
    write(clientSocketFD,&temp,sizeof(temp));
    for(int i=0;i<server.kol_c;i++){
        read(clientSocketFD,&client[i].adress,sizeof(client[i].adress));
        fprintf(stdout,"CLIENT: adress:%16s ",client[i].adress);
        write(clientSocketFD,&temp,sizeof(temp));
        read(clientSocketFD,&client[i].port,sizeof(client[i].port));
        fprintf(stdout,"CLIENT: port:%i \n",client[i].port);
        write(clientSocketFD,&temp,sizeof(temp));
    }
}

int startc(void){
    for(int i=0;i<server.kol_c;i++){
        struct sockaddr_in stSockAddr,slave;
        client[i].slaveFD=socket(AF_INET, SOCK_STREAM,0);
        if (client[i].slaveFD < 0) {
            fprintf(stdout,"\n CLIENT: Connetction failed\n");
            perror("ошибка при создании сокета");
            return 2;
        }
        memset(&slave, 0, sizeof (slave));
        slave.sin_family = AF_INET;
        slave.sin_port = htons(client[i].port);
        inet_pton(AF_INET, client[i].adress, &slave.sin_addr);
        if (connect(client[i].slaveFD, (struct sockaddr*) &slave, sizeof (slave)) < 0) {
            fprintf(stdout,"\n CLIENT: Connetction failed\n");
            perror("Ошибка: связывания");

            close(client[i].slaveFD);
            return 2;
        }
        fprintf(stdout,"\n CLIENT: Connetction successfull\n");
    }
    return 1;
}
int predstart(void){
    int sc=0;
    sc=startc();
    write(clientSocketFD,&sc,sizeof(sc));
    return sc;
}

struct data{
int k;
int f;
unsigned char soob[1500];
};
void rabcikl(void){
    struct data buf;
    struct data mesage;
    struct pollfd fds[1+server.kol_c];
	int timeout_msecs=10;
	int ret,b=1;
	fds[0].fd=clientSocketFD;
	fds[0].events=POLLIN;
	for(int i=1;i<=server.kol_c;i++){
        fds[i].fd=client[i-1].slaveFD;
        fds[i].events=POLLIN;
	}
	while(b){
        buf.k=0;
        buf.f=0;
        memset(&buf.soob,0,sizeof(buf.soob));
		ret=poll(fds,1+server.kol_c,timeout_msecs);
		if(ret>0){
            //ot manag
            if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nCLIENT: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    for(int i=1;i<=server.kol_c;i++){
                        close(fds[i].fd);
                    }
                    return;
                    break;
            }
			if(fds[0].revents & POLLIN){
                int k=read(clientSocketFD,&buf,sizeof(buf));

				k=write(client[buf.f].slaveFD,&buf.soob,buf.k);

			}
            for(int i=1;i<=server.kol_c;i++){
                if((fds[i].revents & POLLHUP)||(fds[i].revents & POLLERR)||(fds[i].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nCLIENT: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    close(fds[i].fd);
                    return;
                    break;
                }
                if(fds[i].revents & POLLIN){
                    buf.k=read(fds[i].fd,&buf.soob,sizeof(buf.soob));

                    buf.f=-2;
                    fprintf(stdout,"\nPaket prishol ot clienta");
                    fprintf(stdout,"Paket lenght:%i ",buf.k-1);
                    int k=write(clientSocketFD,&buf,sizeof(buf));

                }

            }

            /*if(fds[1].revents & POLLIN){
                fprintf(stdout,"\n\nsoobwenie polycheno oy clienta \n\n");
                mesage.k=recv(slaveFD,mesage.soob,sizeof(mesage.soob),0);

                for(int i=0;i<mesage.k;i++){
                    fprintf(stdout,"%i ",mesage.soob[i]);
                }
                fprintf(stdout,"\n\n%i\n\n",mesage.k);
                mesage.types=2;
                mesage.sdest=1;
                write(clientSocketFD,&mesage,sizeof(mesage));
                b=0;
            }*/

		}
		if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nCLIENT: Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
                    for(int i=1;i<=server.kol_c;i++){
                        close(fds[i].fd);
                    }
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
    fprintf(stdout,"\n\n CLIENT: Terminated\n\n");
    return 0;
    }

   rabcikl();
   fprintf(stdout,"\n\n CLIENT: Terminated\n\n");
    return 0;
}
