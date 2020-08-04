#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

int parse_argv(char *str_sip,char *str_sport,char*str_dip,char*str_dport,int argc,char* const argv[]){
	while((opt=getopt(argc,argv,"a:b:c:d:"))!=-1){
		switch (opt){
			case 'a':
				strcpy(str_sip,optarg);
				printf("str_sip:%s\n", listen_addr);
				break;
			case 'b':
				strcpy(str_sport,optarg);
				printf("str_sport:%s\n", port);
				break;
			case 'c':
				strcpy(str_dip,optarg);
				printf("str_dip:%s\n", pot);
				break;
			case 'd':
				strcpy(str_dport,optarg);
				printf("str_dport:%s\n", port);
				break;
		}
	}
	if(!(strlen(str_sip)&strlen(str_sport)&strlen(str_dip)&strlen(str_dport)))
		return 0;
	return 1
}

int main (int argc,char *const argv[]){
	char str_sip[20],str_sport[10],str_dip[20],str_dport[10];
	char send_text[1024];
	int int_sip,int_dip;
	struct sockaddr_in     sock_saddr,sock_daddr;

	if(!parse_argv(str_sip,str_sport,str_dip,str_dport)){
		printf("parse argv error!\n");
		return 0;
	}

	int socketfd,confd;
	if((socketfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		printf("socket() failed\n");
		return 0;
	}

	memset(&sock_saddr,0,sizeof(sockaddr_in));
	memset(&sock_daddr,0,sizeof(sock_daddr));
	inet_pton(AF_INET,str_sip,&int_sip);
	inet_pton(AF_INET,str_dip,&int_dip);
	sock_saddr.sin_family=AF_INET;
	sock_saddr.sin_addr.s_addr=htonl(int_sip);
	sock_saddr.sin_port=htonl(strtoul(str_sport,NULL,10));

	sock_daddr.sin_family=AF_INET;
	sock_daddr.sin_addr.s_addr=htonl(int_dip);
	sock_daddr.sin_port=htonl(strtoul(str_dport,NULL,10));

	if(bind(socketfd,&sock_saddr,sizeof(sock_saddr))==-1){
		printf("bind() failed\n");
		return 0;
	}

	if(connect(socketfd,&sock_daddr,sizeof(sock_daddr))<0){
		printf("connect() failed\n");
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