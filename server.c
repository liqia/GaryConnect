#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<pthread.h>
#include <errno.h>

int parse_argv(char *listen_addr,char *port,int argc,char* const argv[]){
	int opt;
	while((opt=getopt(argc,argv,"l:p:h::"))!=-1){
		switch (opt){
			case 'l':
				strcpy(listen_addr,optarg);
				printf("listen_addr:%s\n", listen_addr);
				break;
			case 'p':
				strcpy(port,optarg);
				printf("port:%s\n", port);
				break;
			case '?':
				printf("Unknown option:%c\n",optopt);

		}
	}
	if(strlen(listen_addr)==0){
		printf("Please input listening address\n");
		return 0;
	}
	if(strlen(port)==0){
		printf("Please input listening port\n");
		return 0;
	}
	return 1;
}

void process_with_client(void* confd){
	char buff[1024];
	memset(buff,0,1024);
	while(1){
		int n=recv(*(int*)confd,buff,1024,0);
		if(n){
			printf("receive msg from client:%s\n",buff);
		}else if(n<0){
			printf("thread exit\n");
			close(*(int*)confd);
			break;
		}
	}
	printf("exit thread\n");
}


int main (int argc,char* const argv[]){
	char listen_addr[20],port_str[20];
	char *temp;
	char buff[4096];
	if(!(parse_argv(listen_addr,port_str,argc,argv))){
		return 0;
	}
	int port=strtoul(port_str,NULL,10);
	int sockfd,confd;

	if((sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		printf("socket() failed\n");
		return 0;
	}
	unsigned int addr_int;
	inet_pton(AF_INET,listen_addr,&addr_int);
	struct in_addr addr={
		.s_addr=addr_int
	};
	struct sockaddr_in addr_sock;
	memset(&addr_sock,0,sizeof(struct sockaddr));
	addr_sock.sin_family=AF_INET;
	addr_sock.sin_port=htons(port);
	addr_sock.sin_addr=addr;

	if(bind(sockfd,(struct sockaddr*)&addr_sock,sizeof(addr_sock))==-1){
		printf("bind() failed :%s\n",strerror(errno));
		return 0;
	}
	if(listen(sockfd,10)==-1){
		printf("listen() failed: %s\n",strerror(errno));
	}
	pthread_t thread_arr[10];
	int i=0;
	while(i<10){
		if((confd=accept(sockfd,(struct sockaddr*)NULL,NULL))==-1){
			printf("accept() failed : %s\n",strerror(errno));
		}
		if(pthread_create(thread_arr+i,NULL,process_with_client,&confd)){
			printf("create thread error!!");
			continue;
		}
		i++;
	}

	return 0;
}