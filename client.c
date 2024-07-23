#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

static int sock;
char Sname[20] = "Anh";
char Cname[20] = "Em";

void* send_mess()
{
	char buffer[1024];
	while(1) {
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		char* temp = strstr(buffer, ":name");	
		// set nickname
		if(temp) {
			strcpy(Sname, buffer + 6);
			Sname[strlen(Sname) - 1] = '\0';
			printf("You set boy name is: %s\n", Sname);
			continue;
		}
		// quit() function
		if(strstr(buffer, ":quit")) {
			send(sock, buffer, strlen(buffer), 0);
			printf("Chatting is finished!\n");
			exit(0);
		}	
		// clear() function
		if(strstr(buffer, ":clear")) {
			system("clear");
			continue;
		}
		// help() function
		if(strstr(buffer, ":help")) {
			printf("---\n");
			printf(":name newName is to set name\n");
			printf(":quit is to exit chat\n");
			printf(":clear is to clear screen\n");
			printf(":help is to show basic functions\n");
			printf("---\n");
			continue;
		}
		//
		char* mess = buffer;
		while(*mess == ' ') ++mess;
		if(strlen(mess) == 1) {
			printf("\x1b[1F");
			printf("\x1b[2K");
			continue;
		}
		printf("\x1b[1F");
		printf("\x1b[2K");
		printf("%s: ", Cname);
		printf("%s", mess);
		send(sock, mess, strlen(buffer),0);
	}
}

void* recv_mess()
{
	char buffer[1024];
	while(1) {
		bzero(buffer, 1024);
		recv(sock, buffer, sizeof(buffer), 0);
		if(strstr(buffer, ":quit")) {
			printf("%s left chat.\n", Sname);
			printf("Please enter Y and Enter to exit!\n");
			exit(0);
		}
		printf("%s: %s", Sname, buffer);
	}
}

int main() {
	char *ip = "127.0.0.1";
	int port = 5566;
	pthread_t thread1, thread2;

	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("[-]Socket error");
		exit(1);
	}
	printf("[+]TCP server socket created.\n");

	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);

	int sts = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(sts < 0) {
		perror("[-]Connect fail\n");
		exit(1);
	}
	printf("Em da tham gia chat.\n");
	system("clear");
	
	pthread_create(&thread1, NULL, send_mess, NULL);
	pthread_create(&thread2, NULL, recv_mess, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
