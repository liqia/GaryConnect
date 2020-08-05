#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <errno.h>

int parse_argv(char*str_dip,char*str_dport,int argc,char* const argv[]){
	int opt;
	while((opt=getopt(argc,argv,"d:p:"))!=-1){
		switch (opt){
			case 'p':
				strcpy(str_dport,optarg);
				printf("str_dport:%s\n", str_dport);
				break;
			case 'd':
				strcpy(str_dip,optarg);
				printf("str_dip:%s\n", str_dip);
				break;
		}
	}
	if(!(strlen(str_dip)&&strlen(str_dport)))
		return 0;
	return 1;
}

int main (int argc,char *const argv[]){
	char str_dip[20],str_dport[10];
	char send_text[1024];
	int int_dip;
	struct sockaddr_in sock_daddr;

	if(!parse_argv(str_dip,str_dport,argc,argv)){
		printf("parse argv error!\n");
		return 0;
	}

	int socketfd,confd;
	if((socketfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		printf("socket() failed\n");
		return 0;
	}

	memset(&sock_daddr,0,sizeof(sock_daddr));
	inet_pton(AF_INET,str_dip,&int_dip);
	sock_daddr.sin_family=AF_INET;
	sock_daddr.sin_addr.s_addr=htonl(int_dip);
	sock_daddr.sin_port=htons(strtoul(str_dport,NULL,10));

	if(connect(socketfd,&sock_daddr,sizeof(sock_daddr))<0){
		printf("connect() failed: %s\n",strerror(errno));
		return 0;
	}
	while(scanf("%s",send_text)!=EOF){
		int n=send(socketfd,send_text,strlen(send_text),0);
		if(n<0){
			printf("send msg failed\n");
			break;
		}
	}
	close(socketfd);
	return 0;
}