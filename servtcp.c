#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define PORT 2024
extern int errno;

int n, i;

int main(int argc, char** argv)
{
	struct sockaddr_in server;	// structura folosita de server
	struct sockaddr_in from;
	char msg[10000];		//mesajul primit de la client 
	int sd;			//descriptorul de socket 
	int optval = 1;

	char users[1000][1000];
	char category[1000];
	char user[1000];
	char showCategory[1000];
	char writeCategory[1000];
	char createAccount[1000];

	FILE* accountCreation;
	FILE* passwordsAuthentification;
	FILE* categories;
	FILE* categoriesAppend;
	FILE* fileCreation;
	FILE* categoryShow;
	FILE* writting;
	
	//creare socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("[server]Eroare la socket().\n");
		return errno;
	}

	//setam pentru socket optiunea SO_REUSEADDR
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	//pregatim structurile de date
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	//umplem structura folosita de server
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	//atasam socketul
	if (bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
		perror("[server]Eroare la bind().\n");
		return errno;
	}

	// punem serverul sa asculte daca vin clienti sa se conecteze 
	if (listen(sd, 5) == -1) {
		perror("[server]Eroare la listen().\n");
		return errno;
	}

	while (1)
	{
		int client;
		int length = sizeof(from);

		printf("[server]Asteptam la portul %d...\n", PORT);
		fflush(stdout);

		// acceptam un client (stare blocanta pina la realizarea conexiunii) 
		client = accept(sd, (struct sockaddr*)&from, &length);

		// eroare la acceptarea conexiunii de la un client 
		if (client < 0) {
			perror("[server]Eroare la accept().\n");
			continue;
		}

		int pid = fork();

		if (pid == -1) {
			perror("[server]Eroare la fork()).\n");
			return errno;
		}

		printf("[server]S-a conectat un user...\n");
		fflush(stdout);
		if (pid == 0) {
			while (1) { 
				bzero(msg, 10000);
				fflush(stdout);
				int bitsMsg = read(client, msg, 10000);
				/* citirea mesajului */
				if (bitsMsg <= 0) {
					perror("[server]Eroare la read() de la client.\n");
					close(client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
				}
				printf("[server]Comanda introdusa de client a fost receptionata %s.\n", msg);
				fflush(stdout);

				char* p1 = strtok(msg, " ");
				p1[strlen(p1)] = '\0';

				if (strcmp(p1, "login") == 0) {
					p1 = strtok(NULL, " ");
					strcpy(user, p1);
					user[strlen(user) - 1] = '\0';
					passwordsAuthentification = fopen("authentificationPasswords.txt", "r"); //deschidem fisierul text unde se afla conturile 

					if (passwordsAuthentification == NULL) {
						perror("[Server]Eroare la fopen.\n");
						exit(1);
					}
					//parcurgem matricea de caractere din file-ul nostru 
					i = 0;
					while (fgets(users[i], sizeof(users[i]), passwordsAuthentification) != NULL) {
						users[i][strlen(users[i]) - 1] = '\0';
						i++; //tinem evidenta numarului de useri care se afla in fisierul nostru
					}
					fclose(passwordsAuthentification);
					n = i;
					//parcurgem userii cu un for si in cazul in care cel introdus de la tastatura se afla acolo va iesi din instructiune returnand ok=1
					int ok = 0;
					for (i = 0; i <= n; i++) {
						if (strcmp(user, users[i]) == 0) {
							ok = 1;
							break;
						}
					}
					if (ok == 1) {
						write(client, "1", sizeof("1"));
					}
					else {
						write(client, "0", sizeof("0"));
					}
					if (ok == 1) {
						break;
					}
				}
				else if (strcmp(msg, "quit\n") == 0) {
					printf("[Server]User-ul s-a deconectat de la server.\n");
					fflush(stdout);
					close(client);
					exit(0);
				}

				else {
					write(client, "Comanda invalida\n", sizeof("Comanda invalida\n"));
				}
			}

			printf("[server]S-a conectat un user la contul sau cu parola %s.\n", user);
			fflush(stdout);

			while (1) {
				/* s-a realizat conexiunea, se astepta mesajul */
				bzero(msg, 10000);
				fflush(stdout);
				int bitsMsg = read(client, msg, 10000);
				/* citirea mesajului */

				if (bitsMsg <= 0) {
					perror("[server]Eroare la read() de la client.\n");
					close(client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
				}

				printf("[server]Comanda introdusa de client a fost receptionata...%s\n", msg);
				fflush(stdout);

				char* p2 = strtok(msg, " "); 
				p2[strlen(p2)] = '\0';


				if (strcmp(p2, "show_categories\n") == 0) {
					char c; char total[10000]; int len = 0;

					categories = fopen("categories.txt", "r");
					if (categories == NULL) {
						perror("[Server]Eroare la fopen.\n");
						exit(1);
					}

					c = fgetc(categories);
					while (c != EOF)
					{	
						total[len++] = c;
						c = fgetc(categories);
					}
					total[len++] = '\n';
					total[len++] = '\0';
					write(client, total, sizeof(total));
					printf("Categoriile i-au fost aratate user-ului cu parola %s.\n",user);
					fflush(stdout);
					fclose(categories);
				}

				else if (strcmp(p2, "create_category") == 0) {
					char fileCat[10000];
					p2 = strtok(NULL, " ");
					strcpy(category, p2);
					category[strlen(category) - 1] = '\0';
				
				   	categoriesAppend = fopen("categories.txt", "a+");
					
				   	if(categoriesAppend == NULL){
					  perror("[Server]Eroare la fopen.\n");
					  exit(1);
				   	}
					strcpy(fileCat,category);
				   	strcat(category,"\r\n");
					fputs(category,categoriesAppend);
					printf("User-ul cu parola %s a adaugat categoria cu numele de: %s.\n",user,category);
					fflush(stdout);
					write(client,"Ai reusit sa adaugi categoria\n",sizeof("Ai reusit sa adaugi categoria\n"));
					fclose(categoriesAppend);
					strcat(fileCat,".txt");
					fileCreation = fopen(fileCat,"w");
					fclose(fileCreation);
				}

				else if (strcmp(p2, "show_the_category") == 0) {
					char total[10000]; char c; int len = 0;
					p2 = strtok(NULL, " ");
					strcpy(showCategory, p2);
					showCategory[strlen(showCategory) - 1] = '\0';
					strcat(showCategory,".txt");

					categoryShow = fopen(showCategory,"r");
					if (categoryShow == NULL) {
						perror("[Server]Eroare la fopen.\n");
						exit(1);
					}

					c = fgetc(categoryShow);
					while (c != EOF)
					{	
						total[len++] = c;
						c = fgetc(categoryShow);
					}
					total[len++] = '\n';
					total[len++] = '\0';
					write(client, total, sizeof(total));
					printf("[Server]Categoria i-a fost aratata user-ului cu parola %s.\n",user);
					fflush(stdout);
					fclose(categoryShow);
				}

				else if (strcmp(p2, "write_in_category") == 0) {
					char buffer[10000];
					p2 = strtok(NULL, " ");
					strcpy(writeCategory, p2);
					writeCategory[strlen(writeCategory)] = '\0';
					//printf("%s",writeCategory);
					strcat(writeCategory,".txt");

					writting = fopen(writeCategory,"a+");
					if (writting== NULL) {
						perror("[Server]Eroare la fopen.\n");
						exit(1);
					}
				 	p2 = strtok(NULL," ");
					strcpy(buffer, p2);
					buffer[strlen(buffer)] = '\0';
					fputs(buffer,writting);
					printf("[Server]User-ul cu parola %s a reusit sa adauge informatiile.\n",user);
					fflush(stdout);
					write(client,"S-au adaugat informatiile.\n",sizeof("S-au adaugat informatiile.\n"));
					fclose(writting);
				}

				else if (strcmp(p2, "quit\n") == 0) {
					printf("[Server]User-ul cu parola %s, s-a deconectat.\n",user);
					fflush(stdout);
					close(client);
					exit(0);
				}

				else {
					write(client, "Comanda invalida\n", sizeof("Comanda invalida\n"));
				}
			}
		} //incheiere proces copil
		close(client);
	}
}
