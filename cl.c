#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char* argv[])
{
	int sd;			// descriptorul de socket
	struct sockaddr_in server;	// structura folosita pentru conectare 
	char msg[10000];		// mesajul trimis

	/* exista toate argumentele in linia de comanda? */
	if (argc != 3)
	{
		printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
		return -1;
	}

	/* stabilim portul */
	port = atoi(argv[2]);

	/* cream socketul */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}

	/* umplem structura folosita pentru realizarea conexiunii cu serverul */
	/* familia socket-ului */
	server.sin_family = AF_INET;
	/* adresa IP a serverului */
	server.sin_addr.s_addr = inet_addr(argv[1]);
	/* portul de conectare */
	server.sin_port = htons(port);

	/* ne conectam la server */
	if (connect(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("[client]Eroare la connect().\n");
		return errno;
	}
	printf("Welcome to password Manager! You got the next commands:\n");
	printf("login [insert password]\n");
	printf("quit\n");
	fflush(stdout);

	while (1) {
		bzero(msg, 10000);
		printf("[client]Introduceti o comanda: ");
		fflush(stdout);
		read(0, msg, 10000);

		/* trimiterea mesajului la server */
		if (write(sd, msg, sizeof(msg)) <= 0)
		{
			perror("[client]Eroare la write() spre server.\n");
			return errno;
		}

		if (strcmp(msg, "quit\n") == 0) {
			close(sd);
			exit(0);
		}
		/* citirea raspunsului dat de server
		   (apel blocant pina cind serverul raspunde) */
		if (read(sd, msg, sizeof(msg)) < 0)
		{
			perror("[client]Eroare la read() de la server.\n");
			return errno;
		}
		if (strcmp(msg, "1") == 0) {
			break;
		}
		else if (strcmp(msg, "0") == 0) {
			printf("Nu exista acest cont\n");
			fflush(stdout);
		}
		else printf("%s", msg);
		fflush(stdout);
	}

	sleep(2);
	system("clear");
	printf("Te-ai conectat cu succes!\n");
	printf("Ai urmatoarele comenzi ca user logat...\n");
	printf("show_categories\n");
	printf("show_the_category [insert category]\n");
	printf("write_in_category [insert category] [insert password,title,username,url,notes]\n");
	printf("create_category[insert category]\n");
	printf("quit\n");
	printf("\n");
	fflush(stdout);

	while (1) {
		bzero(msg, 10000);
		printf("[client]Introduceti o comanda din cele disponibile: ");
		fflush(stdout);
		read(0, msg, 10000);

		/* trimiterea mesajului la server */
		if (write(sd, msg, sizeof(msg)) <= 0)
		{
			perror("[client]Eroare la write() spre server.\n");
			return errno;
		}

		if (strcmp(msg, "quit\n") == 0) {
			break;
			return 0;
		}

		/* citirea raspunsului dat de server
		   (apel blocant pina cind serverul raspunde) */
		if (read(sd, msg, sizeof(msg)) < 0)
		{
			perror("[client]Eroare la read() de la server.\n");
			return errno;
		}

		/* afisam mesajul primit */
		//printf ("[client]Mesajul primit este: %s\n", msg);
		write(1, msg, strlen(msg));
		fflush(stdout);
	}
	close(sd);
	return 0;
}