#include "header.h"
#include <string.h>
#include <dirent.h>

char* concat(char* s1, char* s2) {
	char* result = malloc(strlen(s1)+strlen(s2)+1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

void serves_client(int nsfd, char* directory) {
	char* data = "Hola";
	char* data_sent;
	char filename[PATH_MAX + NAME_MAX + 1];
	int answer = HELLO, number_sent;
	long length = strlen(data), length_sent, data_read;
	write(nsfd, &answer, sizeof(answer));
	write(nsfd, &length, sizeof(length));
	write(nsfd, data, length * sizeof(char));
	srand(getpid());
	do {
			data = "";
			read(nsfd, &number_sent, sizeof(number_sent));
			// printf("EL SERVIDOR ESTA RECIBIENDO: %i\n", number_sent);
			read(nsfd, &length_sent, sizeof(length_sent));
			// printf("EL SERVIDOR ESTA RECIBIENDO LONGITUD: %li\n", length_sent);
			data_sent = (char*) malloc(length_sent*sizeof(char));
			data_read = read(nsfd, data_sent, length_sent * sizeof(char));
			// printf("EL SERVIDOR ESTA RECIBIENDO DATOS: %s\n", data_sent);
			struct stat info;

			if(number_sent == SEND) {
				if(data_sent[0] != '/') {
					answer = ERROR;
					data = "Error interno";
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					printf("%s\n", filename);
					lstat(filename, &info);
					if(S_ISDIR(info.st_mode)) {
						answer = DIRECTORY;
						data = "La ruta es un directorio";
					} else {
						if(access(filename, F_OK) != -1) {
							if(access(filename, R_OK) == 0) {
								// printf("I CAN READ THE FILE!\n");
								char buffer;
								char* target_filename = data_sent + 1;
								int fd_in, fd_out;
								fd_in = open(filename, O_RDONLY);
								fd_out = open(target_filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
								ssize_t nbytes;
								while((nbytes = read(fd_in, &buffer, sizeof(buffer))) != 0) {
									write(fd_out, &buffer, nbytes);
								}
								answer = SENDFILE;
								data = "Enviando archivo";
								close(fd_in);
								close(fd_out);
							} else {
								answer = DENIED;
								data = "Permiso denegado";
							}
						} else {
							answer = NOTFOUND;
							data = "Archivo no encontrado";
							// printf("FILE DOESNT EXIST\n");
						}
					}
				}
			} else if (number_sent == LS) {
				if(data_sent[0] != '/') {
					answer = ERROR;
					data = "Error interno";
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					if(lstat(filename, &info) != 0) {
						answer = DIRNOTFOUND;
						data = "Directory not found";
					} else {
						// printf("IT EXISTS\n");
						DIR* dir;
						if((dir = opendir(filename)) == NULL) {
							answer = NOTDIR;
							data = "La ruta no es un directorio";
						} else {
							answer = SENDDIR;
							struct dirent* direntry;
							while((direntry = readdir(dir)) != NULL) {
								if(strcmp(direntry->d_name, ".") != 0 &&
								strcmp(direntry->d_name, "..") != 0) {
									// printf("%s\n", direntry->d_name);
									data = concat(data, direntry->d_name);
									data = concat(data, "\n");
									// strcpy(data, "\n");
								}
							}
						}
						closedir(dir);
					}
				}
			} else {
				answer = UNKNOWN;
				data = "Comando no conocido";
			}
			length = strlen(data);
			write(nsfd, &answer, sizeof(answer));
			write(nsfd, &length, sizeof(length));
			write(nsfd, data, length * sizeof(char));
			free(data_sent);
	} while (number_sent != END);
	close(nsfd);
}

void server(char* ip, int port, char* directory, char* program) {
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}

	listen(sfd, 1);
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}

		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			serves_client(nsfd, directory);
			exit(0);
		} else {
			close(nsfd);
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc != 4) {
		printf("Usage: %s dir_ip port root_dir \n", argv[0]);
		return -1;
	}
	int port = atoi(argv[2]);
	if(port < 500) {
		printf("%s The port must be grater than 5000\n", argv[0]);
		return -1;
	}
	char* directory = argv[3];
	if(directory[0] != '/') {
		printf("%s The path must be an absolute path\n", argv[0]);
		return -1;
	}


	DIR* dir = opendir(directory);
	if (!dir) {
		perror(argv[0]);
		return -1;
	}


	server(argv[1], port, directory, argv[0]);

	return 0;
}
