#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

static int client_sock;
char Sname[20] = "Anh";
char Cname[20] = "Em";

void* recv_mess()
{
	char buffer[1024];
	while(1) {
		bzero(buffer, 1024); 
		recv(client_sock, buffer, sizeof(buffer), 0);
		if(strstr(buffer, ":quit")) {
			printf("%s left chat.\n", Cname);
			printf("Please enter Y and Enter to exit!\n");
			exit(0);
		}
		printf("%s: %s", Cname, buffer);
	}	
}

void* send_mess()
{
	char buffer[1024];
	char mess[1024];
	while(1) {
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		char* temp = strstr(buffer, ":name");
		// set name
		if(temp) {
			strcpy(Cname, buffer + 6);
			Cname[strlen(Cname) - 1] = '\0';
			printf("You set girl name is %s\n", Cname);
		       	continue;	
		}
		// quit() function
		if(strstr(buffer, ":quit")) {
			send(client_sock, buffer, strlen(buffer), 0); 
			printf("Chatting is finished!\n");
			close(client_sock);
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
		printf("%s: ", Sname);
		printf("%s", mess);
		send(client_sock, mess, strlen(buffer), 0);
	}
}

int main() {
	char *ip = "127.0.0.1";
	int port = 5566;
	pthread_t thread1, thread2;

	int server_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		perror("[-]Socket error");
		exit(1);
	}
	printf("[+]TCP server socket created.\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0) {
		perror("[-]Bind error");
		exit(1);
	}
	printf("[+]Bind to the port number: %d\n", port);

	if(listen(server_sock, 5) < 0) {
		perror("[-]Listen fail!\n");
		exit(1);
	}
	printf("Dang cho e...\n");

	addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
	if(client_sock < 0) {
		perror("[-]Accept fail!\n");
		exit(1);
	}
	printf("[+]Em da tham gia chat.\n");
	system("clear");

	pthread_create(&thread1, NULL, send_mess, NULL);
	pthread_create(&thread2, NULL, recv_mess, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
