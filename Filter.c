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
void recvset(void);
void kolchisel(void);
void black(void);
void rabcikl(void);
int clientSocketFD;
struct serv{
    int kol_c,nb;
    char filter[10000];
};
struct cli{
    int zb;
};

struct serv server;
struct cli client[256];
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
    char buf=2;
    write(clientSocketFD,&buf,sizeof(buf));
}

void recvset(void){
    int temp=0;
    read(clientSocketFD,&server.kol_c,sizeof(server.kol_c));
    write(clientSocketFD,&temp,sizeof(temp));
    read(clientSocketFD,&server.nb,sizeof(server.nb));
    write(clientSocketFD,&temp,sizeof(temp));
    for(int i=0;i<server.kol_c;i++){
        read(clientSocketFD,&client->zb,sizeof(client->zb));
        write(clientSocketFD,&temp,sizeof(temp));
    }
    fprintf(stdout,"FILTER: Number of client:%i ",server.kol_c);
    fprintf(stdout,"FILTER: Number of check byte:%i ",server.nb);
    for(int i=0;i<server.kol_c;i++){
        fprintf(stdout,"FILTER: Value of check byte:%i ",client[i].zb);
    }
    read(clientSocketFD,&server.filter,sizeof(server.filter));
    write(clientSocketFD,&temp,sizeof(temp));
    fprintf(stdout,"\nFILTER: Filter rule:%s \n",server.filter);
    fprintf(stdout,"\n");

}

struct tier1 * fpra;
int kfpra;
typedef struct tier1{
    int * nslave;
    int kslave;
    int * nbait;
    int knbait;
    int * zbait;
    int kzbait;
};

void kolchisel(void){
    for(int i=0;i<strlen(server.filter);i++){
        if(server.filter[i]=='\n'||server.filter[i]==';'){
            kfpra++;
        }
    }
    fpra=malloc(kfpra*sizeof(struct tier1));
    for(int i=0;i<kfpra;i++){
        fpra[i].kslave=0;
        fpra[i].knbait=0;
        fpra[i].kzbait=0;
    }
    int k=0,t=0,i=0;
    while(k!=kfpra){
        if(server.filter[i]==','||server.filter[i]=='-'||server.filter[i]==':'||server.filter[i]==';'){
            if(t==2){
                fpra[k].kzbait++;
                if(server.filter[i]==':')
                    t++;
            }
            if(t==1){
                fpra[k].knbait++;
                if(server.filter[i]==':')
                    t++;
            }
            if(t==0){
                fpra[k].kslave++;
                if(server.filter[i]==':')
                    t++;
            }

            if(server.filter[i]==';'){
                k++;
                t=0;
            }
        }
        i++;
    }
    for(int j=0;j<kfpra;j++){
        fpra[j].nslave=malloc((int)fpra[j].kslave*sizeof(int));
        fpra[j].nbait=malloc((int)fpra[j].knbait*sizeof(int));
        fpra[j].zbait=malloc((int)fpra[j].kzbait*sizeof(int));
    }
    k=0;
    t=0;
    i=0;
    int z=1,g=0,j=1;
    int sum=0;
    while(k!=kfpra){
        if(server.filter[i]!=','&&server.filter[i]!='-'&&server.filter[i]!=':'&&server.filter[i]!=';'){
                sum=10*sum+server.filter[i]-48;
        }
        if(server.filter[i]==','||server.filter[i]=='-'||server.filter[i]==':'||server.filter[i]==';'){
            if(t==2){
                fpra[k].zbait[g]=sum*z;
                z=1;
                sum=0;
                j=1;
                g++;
                if(server.filter[i]==':'){
                    t++;
                    g=0;
                }
            }
            if(t==1){
                fpra[k].nbait[g]=sum*z;
                z=1;
                sum=0;
                j=1;
                g++;
                if(server.filter[i]==':'){
                    t++;
                    g=0;
                }
            }
            if(t==0){
                fpra[k].nslave[g]=sum*z;
                z=1;
                sum=0;
                j=1;
                g++;
                if(server.filter[i]==':'){
                    t++;
                    g=0;
                }
            }
            if(server.filter[i]==';'){
                k++;
                g=0;
                j=1;
                t=0;
                sum=0;
            }
        }
        if(server.filter[i]=='-'){
            z=-1;
        }
        i++;
    }
    struct tier1 * temp_fpra;
    int temp_kfpra=kfpra;
    temp_fpra=malloc(temp_kfpra*sizeof(struct tier1));

    for(int a1=0;a1<kfpra;a1++){
        temp_fpra[a1].knbait=fpra[a1].knbait;
        temp_fpra[a1].kslave=fpra[a1].kslave;
        temp_fpra[a1].kzbait=fpra[a1].kzbait;
        for(int a2=0;a2<fpra[a1].kslave;a2++){
            if(fpra[a1].nslave[a2]<0){
                temp_fpra[a1].kslave=temp_fpra[a1].kslave+abs(fpra[a1].nslave[a2])-fpra[a1].nslave[a2-1]-1;
            }
        }

        for(int a2=0;a2<fpra[a1].knbait;a2++){
            if(fpra[a1].nbait[a2]<0){
                temp_fpra[a1].knbait=temp_fpra[a1].knbait+abs(fpra[a1].nbait[a2])-fpra[a1].nbait[a2-1]-1;
            }
        }

        for(int a2=0;a2<fpra[a1].kzbait;a2++){
            if(fpra[a1].zbait[a2]<0){
                temp_fpra[a1].kzbait=temp_fpra[a1].kzbait+abs(fpra[a1].zbait[a2])-fpra[a1].zbait[a2-1]-1;
            }
        }
    }
    for(int j=0;j<kfpra;j++){
        temp_fpra[j].nslave=malloc((int)temp_fpra[j].kslave*sizeof(int));
        temp_fpra[j].nbait=malloc((int)temp_fpra[j].knbait*sizeof(int));
        temp_fpra[j].zbait=malloc((int)temp_fpra[j].kzbait*sizeof(int));
    }
    for(int a1=0;a1<kfpra;a1++){
        int temp_knbait=0;
        int temp_kslave=0;
        int temp_kzbait=0;
        for(int a2=0;a2<fpra[a1].kslave;a2++){
            if(fpra[a1].nslave[a2]<0){
                for(int a3=fpra[a1].nslave[a2-1]+1;a3<=abs(fpra[a1].nslave[a2]);a3++){
                        temp_fpra[a1].nslave[temp_kslave]=a3;
                        temp_kslave++;
                }
            }else{
            temp_fpra[a1].nslave[temp_kslave]=fpra[a1].nslave[a2];
                        temp_kslave++;
            }
        }

        for(int a2=0;a2<fpra[a1].knbait;a2++){
            if(fpra[a1].nbait[a2]<0){
                    for(int a3=fpra[a1].nbait[a2-1]+1;a3<=abs(fpra[a1].nbait[a2]);a3++){
                        temp_fpra[a1].nbait[temp_knbait]=a3;
                        temp_knbait++;
                    }
            }else{
            temp_fpra[a1].nbait[temp_knbait]=fpra[a1].nbait[a2];
                        temp_knbait++;
            }
        }

        for(int a2=0;a2<fpra[a1].kzbait;a2++){
            if(fpra[a1].zbait[a2]<0){
                    for(int a3=fpra[a1].zbait[a2-1]+1;a3<=abs(fpra[a1].zbait[a2]);a3++){
                        temp_fpra[a1].zbait[temp_kzbait]=a3;
                        temp_kzbait++;
                    }
            }else{
            temp_fpra[a1].zbait[temp_kzbait]=fpra[a1].zbait[a2];
                        temp_kzbait++;
            }
        }
    }
    /*for(int a1=0;a1<kfpra;a1++){
        for(int a2=0;a2<temp_fpra[a1].kslave;a2++){
            fprintf(stdout,"\n nslave %i\n",temp_fpra[a1].nslave[a2]);
        }
        for(int a2=0;a2<temp_fpra[a1].knbait;a2++){
            fprintf(stdout,"\n nbait %i\n",temp_fpra[a1].nbait[a2]);
        }
        for(int a2=0;a2<temp_fpra[a1].kzbait;a2++){
            fprintf(stdout,"\n zbait %i\n",temp_fpra[a1].zbait[a2]);
        }
    }*/
    for(int j=0;j<kfpra;j++){
        free(fpra[j].nslave);
        free(fpra[j].nbait);
        free(fpra[j].zbait);
    }
    free(fpra);
    fpra=temp_fpra;
    /*for(int a1=0;a1<kfpra;a1++){
        for(int a2=0;a2<fpra[a1].kslave;a2++){
            fprintf(stdout,"\n nslave %i\n",fpra[a1].nslave[a2]);
        }
        for(int a2=0;a2<fpra[a1].knbait;a2++){
            fprintf(stdout,"\n nbait %i\n",fpra[a1].nbait[a2]);
        }
        for(int a2=0;a2<fpra[a1].kzbait;a2++){
            fprintf(stdout,"\n zbait %i\n",fpra[a1].zbait[a2]);
        }
    }*/

}



struct data{
int k;
int f;
unsigned char soob[1500];
};
struct data buf;

void black(void){
    int flag,id=-1;
    for(int i=0;i<server.kol_c;i++){
        if(buf.soob[server.nb-1]==client[i].zb){
            id=i;
        }
    }
    if(id==-1){
        buf.f=-1;
        return;
    }
    for(int a1=0;a1<kfpra;a1++){
        flag=-1;
        for(int a2=0;a2<fpra[a1].kslave;a2++){
            if(fpra[a1].nslave[a2]==id+1){
                flag=1;
                break;
            }else{
                flag=-1;
            }
        }
        if(flag==1){
            for(int a2=0;a2<fpra[a1].knbait;a2++){
                for(int a3=0;a3<fpra[a1].kzbait;a3++){
                    if(buf.soob[fpra[a1].nbait[a2]-1]==fpra[a1].zbait[a3]){
                        buf.f=-1;
                        if(buf.f==-1)
                            fprintf(stdout,"\nPaket ne proshol filtaciu ");
                        return;
                    }
                }
            }
        }
    }
    buf.f=id;
    if(buf.f>=0)
        fprintf(stdout,"\nPaket proshel filtr i bil otpravlen slavu s poridkovim nomerom:%i ",buf.f+1);
    fprintf(stdout,"Paket lenght:%i ",buf.k-1);
    return;
}

void rabcikl(void){
    struct pollfd fds[1];
	int timeout_msecs=10;
	int ret,b=1;
	fds[0].fd=clientSocketFD;
	fds[0].events=POLLIN;
	while(b){
        buf.k=0;
        buf.f=0;
        memset(&buf.soob,0,sizeof(buf.soob));
		ret=poll(fds,1,timeout_msecs);
		if(ret>0){
            if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                fprintf(stdout,"\n \nFILTER: : Terminated\n \n");
                b=0;
                close(fds[0].fd);
                return;

            }
            if(fds[0].revents & POLLIN){
                int k=read(clientSocketFD,&buf,sizeof(buf));
                black();
                k=write(clientSocketFD,&buf,sizeof(buf));
            }

        }
        if((fds[0].revents & POLLHUP)||(fds[0].revents & POLLERR)||(fds[0].revents & POLLNVAL)){
                    fprintf(stdout,"\n\nFILTER: : Terminated\n\n");
                    b=0;
                    close(fds[0].fd);
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
   kolchisel();
   rabcikl();
   fprintf(stdout,"\n\n FILTER: : Terminated\n\n");
   return 0;
}
