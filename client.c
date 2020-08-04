#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int parse_argv(char *str_sip,char *str_sport,char*str_dip,char*str_dport,int argc,char* const argv[]){
	while((opt=getopt(argc,argv,"a:b:c:d:"))!=-1){
		switch (opt){
			case 'a':
				strcpy(str_sip,optarg);
				printf("listen_addr:%s\n", listen_addr);
				break;
			case 'b':
				strcpy(str_sport,optarg);
				printf("port:%s\n", port);
				break;
			case 'c':
				strcpy(str_dip,optarg);
				printf("port:%s\n", port);
				break;
			case 'd':
				strcpy(str_dport,optarg);
				printf("port:%s\n", port);
				break;
		}
	}
	return 1
}

int main (int argc,char *const argv[]){

	return 0;
}