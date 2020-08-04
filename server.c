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

int parse_argv(char *listen_addr,char *port,int argc,char* const argv[]){
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
	return 1
}

void process_with_client(int confd){
	printf("confd addr: %d\n", &confd);
	char buff[4096];
	while(1){
		int n=recv(confd,buff,4096,0);
		if(n){
			buff[n]='\0';
			printf("receive msg from client: %s\n", buff);
		}else if(ECONNREST){
			printf("thread exit\n");
			close(confd);
			break;
		}
	}
}


int main (int argc,char* const argv[]){
	int opt;
	char listen_addr[20],port_str[20];
	char *temp;
	char buff[4096];
	if(!(parse_argv(listen_addr,port_str,argc,argv))){
		return 0
	}
	int port=strtoul(port,temp,10);

	int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP),confd;

	unsigned int addr_int;
	inet_pton(AF_INET,listen_addr,&addr_int);
	struct in_addr addr={
		.s_addr=htonl(addr_int)
	};
	struct sockaddr_in addr_sock={
		.sin_family=AF_INET,
		.sin_port=htonl(port),
		.sin_addr=addr
	};

	if(bind(sockfd,addr_sock,sizeof(sockaddr_in))==-1){
		printf("bind() failed\n");
		return 0;
	}

	if(listen(sockfd,3)==-1){
		printf("listen() failed\n");
	}
	pthread_t thread_arr[10];
	int i=0;
	while(i<10){
		if((confd=accept(sockfd,(struct sockaddr*)NULL,NULL))==-1){
			printf("accept() failed\n");
		}
		if(pthread_create(thread_arr+i,NULL,process_with_client,&confd)){
			printf("create thread error!!");
			continue;
		}
		i++;
	}

	return 0;
}