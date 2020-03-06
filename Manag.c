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
void vivod(void);
void load(void);
void start(void);
void ust_sviz();
void sendset(void);
int start_raboti(void);
void rabcikl(void);

struct serv{
    char adress[16];
    int port,kol_c,nb;
    char filter[10000];
};
struct cli{
    char adress[16];
    int port,zb;
};
struct serv server;
struct cli client[256];
int vib;
int servd=0,filtd=0,clid=0,logd=0,ss=0,sc=0;


void vivod(void){
    fprintf(stdout,"%s ",&server.adress);
    fprintf(stdout,"%i ",server.port);
    fprintf(stdout,"%i ",server.kol_c);
    fprintf(stdout,"%i ",server.nb);
    for(int i=0;i<server.kol_c;i++){
        fprintf(stdout,"\n%s ",&client[i].adress);
        fprintf(stdout,"%i ",client[i].port);
        fprintf(stdout,"%i",client[i].zb);
    }
    fprintf(stdout,"%s ",&server.filter);
    fprintf(stdout,"\n");
}

/*void news(void){
    fprintf(stdout,"Vvedite address mastera\n");
    memset(&server.adress,0,sizeof(server.adress));
    scanf("%s",&server.adress);
    fprintf(stdout,"Vvedite port mastera\n");
    scanf("%i",&server.port);
    fprintf(stdout,"Vvedite kolichestvo clientov\n");
    scanf("%i",&server.kol_c);
    fprintf(stdout,"Vvedite nomer konrolinogo baita\n");
    scanf("%i",&server.nb);
    for(int i=0;i<server.kol_c;i++){
        fprintf(stdout,"Vvedite address clienta\n");
        memset(&client[i].adress,0,sizeof(server.adress));
        scanf("%s",&client[i].adress);
        fprintf(stdout,"Vvedite port clienta\n");
        scanf("%i",&client[i].port);
        fprintf(stdout,"Vvedite znachenie konrolnogo baita\n");
        scanf("%i",&client[i].zb);
    }
    fprintf(stdout,"Vvedite pravilo dlya filtra\n");
    scanf("%s",&server.filter);
    vivod();
    fprintf(stdout,"Hotite sohranit nastroiki v fail? \n [0]-NET [1]-DA\n");
    scanf("%i",&vib);
    if(vib==1){
        save();
    }


}*/

void load(void){
    char name[256]="set1";
    //fprintf(stdout,"Vvedite imi faila\n");
    //memset(name,0,sizeof(name));
    //scanf("%s",&name);
    FILE *fail;
    if((fail=fopen(name,"r"))==NULL){
        fprintf(stdout,"Error on opening file");
        exit(0);
    }
    fscanf(fail,"%s",&server.adress);
    fscanf(fail,"%i",&server.port);
    fscanf(fail,"%i",&server.kol_c);
    fscanf(fail,"%i",&server.nb);
    for(int i=0;i<server.kol_c;i++){
        fscanf(fail,"%s",&client[i].adress);
        fscanf(fail,"%i",&client[i].port);
        fscanf(fail,"%i",&client[i].zb);
    }
    fscanf(fail,"%s",&server.filter);
    fclose(fail);
    vivod();
}
/*void save(void){
    char name[256];
    fprintf(stdout,"Vvedite imi faila\n");
    memset(name,0,sizeof(name));
    scanf("%s",&name);
    FILE *fail;
    if((fail=fopen(name,"w"))==NULL){
        fprintf(stdout,"Error on opening file");
        exit(0);
    }
    fprintf(fail,"%s ",&server.adress);
    fprintf(fail,"%i ",server.port);
    fprintf(fail,"%i ",server.kol_c);
    fprintf(fail,"%i ",server.nb);
    for(int i=0;i<server.kol_c;i++){
        fprintf(fail,"\n%s ",&client[i].adress);
        fprintf(fail,"%i ",client[i].port);
        fprintf(fail,"%i",client[i].zb);
    }
    fprintf(fail,"\n%s",&server.filter);
    fclose(fail);
}*/
void start(void){
    /*fprintf(stdout,"Zagruzka iz faila ili sozdat new\n [0]-FAIL [1]-NEW\n");
    scanf("%i",&vib);
    if(vib==1){
        news();
    }
    else{*/
        load();
    //}
}
void ust_sviz(){

    struct sockaddr_in stSockAddr;
    /* Create socket accepting incoming connection requests. */
    int serverSocketFD = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
    if(-1 == serverSocketFD)
    {
      perror("can not create socket");
      return EXIT_FAILURE;
    }
    int optval = 1;
    if (-1 == setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
      perror("error setsockopt failed");
      close(serverSocketFD);
      return EXIT_FAILURE;
    }
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(43210);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(-1 == bind(serverSocketFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
      perror("error bind failed");
      close(serverSocketFD);
      return EXIT_FAILURE;
    }
    if(-1 == listen(serverSocketFD, 4))
    {
      perror("error listen failed");
      close(serverSocketFD);
      return EXIT_FAILURE;
    }
	int ConnectFD1 = accept(serverSocketFD, NULL, 0);
    int ConnectFD2 = accept(serverSocketFD, NULL, 0);
    int ConnectFD3 = accept(serverSocketFD, NULL, 0);
    int ConnectFD4 = accept(serverSocketFD, NULL, 0);
	if (0 > ConnectFD1){
		perror("error accept failed");
		close(serverSocketFD);
		return EXIT_FAILURE;
	}
	if (0 > ConnectFD2){
		perror("error accept failed");
		close(serverSocketFD);
		return EXIT_FAILURE;
	}
	if (0 > ConnectFD3){
		perror("error accept failed");
		close(serverSocketFD);
		return EXIT_FAILURE;
	}
	if (0 > ConnectFD4){
		perror("error accept failed");
		close(serverSocketFD);
		return EXIT_FAILURE;
	}

	char buf=0;
	read(ConnectFD1,&buf,sizeof(buf));
    switch (buf){
    case 1:
        servd=ConnectFD1;
        break;
    case 2:
        filtd=ConnectFD1;
        break;
    case 3:
        logd=ConnectFD1;
        break;
    case 4:
        clid=ConnectFD1;
        break;
    }

    read(ConnectFD2,&buf,sizeof(buf));
    switch (buf){
    case 1:
        servd=ConnectFD2;
        break;
    case 2:
        filtd=ConnectFD2;
        break;
    case 3:
        logd=ConnectFD2;
        break;
    case 4:
        clid=ConnectFD2;
        break;
    }

    read(ConnectFD3,&buf,sizeof(buf));
    switch (buf){
    case 1:
        servd=ConnectFD3;
        break;
    case 2:
        filtd=ConnectFD3;
        break;
    case 3:
        logd=ConnectFD3;
        break;
    case 4:
        clid=ConnectFD3;
        break;
    }

    read(ConnectFD4,&buf,sizeof(buf));
    switch (buf){
    case 1:
        servd=ConnectFD4;
        break;
    case 2:
        filtd=ConnectFD4;
        break;
    case 3:
        logd=ConnectFD4;
        break;
    case 4:
        clid=ConnectFD4;
        break;
    }
};
void sendset(void){
    //toserver
    int temp=0;
    write(servd,&server.adress,sizeof(server.adress));
    read(servd,&temp,sizeof(temp));
    write(servd,&server.port,sizeof(server.port));
    read(servd,&temp,sizeof(temp));
    //tofilter
    write(filtd,&server.kol_c,sizeof(server.kol_c));
    read(filtd,&temp,sizeof(temp));
    write(filtd,&server.nb,sizeof(server.nb));
    read(filtd,&temp,sizeof(temp));
    for(int i=0;i<server.kol_c;i++){
        write(filtd,&client[i].zb,sizeof(client[i].zb));
        read(filtd,&temp,sizeof(temp));
    }
    write(filtd,&server.filter,sizeof(server.filter));
    read(filtd,&temp,sizeof(temp));
    //toclient
    write(clid,&server.kol_c,sizeof(server.kol_c));
    read(clid,&temp,sizeof(temp));
    for(int i=0;i<server.kol_c;i++){
        write(clid,&client[i].adress,sizeof(client[i].adress));
        read(clid,&temp,sizeof(temp));
        write(clid,&client[i].port,sizeof(client[i].port));
        read(clid,&temp,sizeof(temp));
    }
    fprintf(stdout,"\n");
}



int start_raboti(void){
    fprintf(stdout,"\n Zapusk podklushenia ckientov\n");
    read(clid,&vib,sizeof(ss));
    if(vib==1){
        fprintf(stdout,"\n Podklushenia ckientov uspesni\n");
    }else{
        fprintf(stdout,"\n Pri podklushenia ckientov voznikli owibki\n");
        return 0;
    }
    fprintf(stdout,"\n Zapusk proslushki servera\n");
    read(servd,&vib,sizeof(ss));
    if(vib==1){
        fprintf(stdout,"\n Proslushki servera uspeshna\n");
    }else{
        fprintf(stdout,"\n Pri proslushki servera voznikli owibki\n");
        return 0;
    }
    return 1;

}
struct data{
    int k;
    int f;
    unsigned char soob[1500];
};
void rabcikl(void){
    struct data buf;
    struct pollfd fds[4];
	int timeout_msecs=10;
	int ret,b=1;
	fds[0].fd=servd;
	fds[0].events=POLLIN;
	fds[1].fd=filtd;
	fds[1].events=POLLIN;
	fds[2].fd=clid;
	fds[2].events=POLLIN;
	fds[3].fd=logd;
	fds[3].events=POLLIN;
	while(b){
        buf.k=0;
        buf.f=0;
        memset(&buf.soob,0,sizeof(buf.soob));
		ret=poll(fds,4,timeout_msecs);
		if(ret>0){
            for(int i=0;i<4;i++)
                if((fds[i].revents & POLLHUP)||(fds[i].revents & POLLERR)||(fds[i].revents & POLLNVAL)){
                    b=0;
                    fprintf(stdout,"\n\obriv svizi s odnim iz komponentov\n\n");
                    close(fds[i].fd);
                    return;
                }
            if(fds[0].revents & POLLIN){
                int k=read(servd,&buf,sizeof(buf));
                k=write(filtd,&buf,sizeof(buf));
            }
            if(fds[1].revents & POLLIN){
                int k=read(filtd,&buf,sizeof(buf));
                if(buf.f!=-1){
                    k=write(clid,&buf,sizeof(buf));
                }

                k=write(logd,&buf,sizeof(buf));
            }
            if(fds[2].revents & POLLIN){
                int k=read(clid,&buf,sizeof(buf));
                fprintf(stdout,"\n\n Paket ot clienta k serveru\n Lenght: %i \n\n",buf.k);
                k=write(servd,&buf,sizeof(buf));
                k=write(logd,&buf,sizeof(buf));
            }


        }
        if(ret<0){
            fprintf(stdout,"\n\nobriv svizi s odnim iz komponentov\n\n");
            return;
        }
        for(int i=0;i<4;i++)
            if((fds[i].revents & POLLHUP)||(fds[i].revents & POLLERR)||(fds[i].revents & POLLNVAL)){
                b=0;
                fprintf(stdout,"\n\nobriv svizi s odnim iz komponentov\n\n");
                close(fds[i].fd);
                return;
            }
    }
}

int main()
{
    ust_sviz();
    start();
    sendset();
    if(start_raboti()==0){
        fprintf(stdout,"\n\n MANAG: Terminated\n\n");
        return 0;
    }


    rabcikl();
    fprintf(stdout,"\n\n MANAG: Terminated\n\n");
    return 0;
}
