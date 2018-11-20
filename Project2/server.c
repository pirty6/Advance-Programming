#include "header.h"
#include <string.h>
#include <dirent.h>

void writeFile(char* filename, char* text) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *fp;
	if(access(filename, F_OK ) != -1 ) {
		fp = fopen(filename, "a");
	} else {
		fp = fopen(filename, "w+");
	}
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%d-%d-%d %d:%d:%d IP: %s\n",tm.tm_year + 1900, tm.tm_mon + 1,
	tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text);
	fflush(fp);
	fclose(fp);
}

char* concat(char* s1, char* s2) {
	char* result = malloc(strlen(s1)+strlen(s2)+1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

void serves_client(int nsfd, char* directory, char* ip, char* program) {
	int semid;
	key_t key;

	if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(program);
    exit(-1);
  }

  if ( (semid = semget(key, 1, 0666))  < 0 ) {
    perror(program);
    exit(-1);
  }

	char* data_log= "";
	char log_filename[PATH_MAX + NAME_MAX + 1];
	sprintf(log_filename, "%s/log.txt", directory);
	data_log = concat(ip, " Ha iniciado una conexion");
	mutex_wait(semid, MUTEX);
	writeFile(log_filename, data_log);
	mutex_signal(semid, MUTEX);
	char* data;
	char* data_sent;
	char filename[PATH_MAX + NAME_MAX + 1];
	int answer = HELLO, number_sent;
	long length = strlen(HOLA), length_sent, data_read;
	write(nsfd, &answer, sizeof(answer));
	write(nsfd, &length, sizeof(length));
	write(nsfd, HOLA, length * sizeof(char));
	int flag;
	long size;
	srand(getpid());
	do {
			flag = 0;
			size = 0;
			data_log = "";
			read(nsfd, &number_sent, sizeof(number_sent));
			read(nsfd, &length_sent, sizeof(length_sent));
			data_sent = (char*) malloc(length_sent*sizeof(char));
			data_read = read(nsfd, data_sent, length_sent * sizeof(char));
			data_sent[length_sent] = '\0';
			struct stat info;

			if(number_sent == SEND) {
				if(data_sent[0] != '/') {
					mutex_wait(semid, MUTEX);
					data_log = concat(ip, " Comando: 101 Parametro: ");
					data_log = concat(data_log, data_sent);
					writeFile(log_filename, data_log);
					data_log = "";
					data_log = concat(ip, " Respuesta: 203 Parametro: Error interno");
					writeFile(log_filename, data_log);
					mutex_signal(semid, MUTEX);
					answer = ERROR;
					data = ERRORINTERNO;
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					//printf("%s\n", filename);
					int s = lstat(filename, &info);
					if( s < 0) {
						mutex_wait(semid, MUTEX);
						data_log = concat(ip, " Comando: 101 Parametro: ");
						data_log = concat(data_log, data_sent);
						writeFile(log_filename, data_log);
						data_log = "";
						data_log = concat(ip, " Respuesta: 202 Parametro: Archivo no encontrado");
						writeFile(log_filename, data_log);
						mutex_signal(semid, MUTEX);
						answer = NOTFOUND;
						data = SNOTFOUND;
					} else {
						if(S_ISDIR(info.st_mode)) {
							mutex_wait(semid, MUTEX);
							data_log = concat(ip, " Comando: 101 Parametro: ");
							data_log = concat(data_log, data_sent);
							writeFile(log_filename, data_log);
							data_log = "";
							data_log = concat(ip, " Respuesta: 205 Parametro: La ruta es un directorio");
							writeFile(log_filename, data_log);
							mutex_signal(semid, MUTEX);
							answer = DIRECTORY;
							data = RDIRECTORY;
						} else {
							//printf("%s\n", filename);
							if(access(filename, R_OK) == 0) {
								flag = 1;
								FILE *fd;
								fd = fopen(filename, "r");
								size = info.st_size;
								answer = SENDFILE;
								mutex_wait(semid, MUTEX);
								write(nsfd, &answer, sizeof(answer));
								write(nsfd, &size, sizeof(size));
								char send_buffer[size];
								fread(send_buffer, 1, sizeof(send_buffer), fd);
								write(nsfd, send_buffer, sizeof(send_buffer));
								memset(send_buffer, 0, size);
								data_log = concat(ip, " Comando: 101 Parametro: ");
								data_log = concat(data_log, data_sent);
								writeFile(log_filename, data_log);
								data_log = "";
								data_log = concat(ip, " Respuesta: 301 Parametro: Enviando archivo");
								writeFile(log_filename, data_log);
								mutex_signal(semid, MUTEX);
								fclose(fd);
							} else {
								mutex_wait(semid, MUTEX);
								data_log = concat(ip, " Comando: 101 Parametro: ");
								data_log = concat(data_log, data_sent);
								writeFile(log_filename, data_log);
								data_log = "";
								data_log = concat(ip, " Respuesta: 201 Parametro: Permiso denegado");
								writeFile(log_filename, data_log);
								mutex_signal(semid, MUTEX);
								answer = DENIED;
								data = PDENIED;
							}
						}
					}
				}
			} else if (number_sent == LS) {
				if(data_sent[0] != '/') {
					mutex_wait(semid, MUTEX);
					data_log = concat(ip, " Comando: 102 Parametro: ");
					data_log = concat(data_log, data_sent);
					writeFile(log_filename, data_log);
					data_log = "";
					data_log = concat(ip, " Respuesta: 203 Parametro: Error interno");
					writeFile(log_filename, data_log);
					mutex_signal(semid, MUTEX);
					answer = ERROR;
					data = ERRORINTERNO;
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					if(lstat(filename, &info) != 0) {
						mutex_wait(semid, MUTEX);
						data_log = concat(ip, " Comando: 102 Parametro: ");
						data_log = concat(data_log, data_sent);
						writeFile(log_filename, data_log);
						data_log = "";
						data_log = concat(ip, " Respuesta: 206 Parametro: Directorio no encontrado");
						writeFile(log_filename, data_log);
						mutex_signal(semid, MUTEX);
						answer = DIRNOTFOUND;
						data = DNOTFOUND;
					} else {
						DIR* dir;
						if((dir = opendir(filename)) == NULL) {
							mutex_wait(semid, MUTEX);
							data_log = concat(ip, " Comando: 102 Parametro: ");
							data_log = concat(data_log, data_sent);
							writeFile(log_filename, data_log);
							data_log = "";
							data_log = concat(ip, " Respuesta: 207 Parametro: La ruta no es un directorio");
							writeFile(log_filename, data_log);
							mutex_signal(semid, MUTEX);
							answer = NOTDIR;
							data = RNOTDIRECTORY;
						} else {
							answer = SENDDIR;
							struct dirent* direntry;
							data = "";
							while((direntry = readdir(dir)) != NULL) {
								if(strcmp(direntry->d_name, ".") != 0 &&
								strcmp(direntry->d_name, "..") != 0) {
									data = concat(data, direntry->d_name);
									data = concat(data, "\n");
								}
							}
							mutex_wait(semid, MUTEX);
							data_log = concat(ip, " Comando: 102 Parametro: ");
							data_log = concat(data_log, data_sent);
							writeFile(log_filename, data_log);
							data_log = "";
							data_log = concat(ip, " Respuesta: 302 Parametro: Enviando el contenido de un directorio: \n");
							data_log = concat(data_log, data);
							writeFile(log_filename, data_log);
							mutex_signal(semid, MUTEX);
						}
						closedir(dir);
					}
				}
			} else {
				if(number_sent == END){
					mutex_wait(semid, MUTEX);
					data_log = concat(ip, " Ha terminado la conexion");
					writeFile(log_filename, data_log);
					mutex_signal(semid, MUTEX);
				} else {
					mutex_wait(semid, MUTEX);
					data_log = concat(ip, " Comando: 102 Parametro: ");
					data_log = concat(data_log, data_sent);
					writeFile(log_filename, data_log);
					data_log = "";
					data_log = concat(ip, " Respuesta: 204 Parametro: Comando no conocido");
					writeFile(log_filename, data_log);
					mutex_signal(semid, MUTEX);
					answer = UNKNOWN;
					data = UNKNOWNCOMMAND;
				}
			}
			length = strlen(data);
			if(flag == 0) {
				write(nsfd, &answer, sizeof(answer));
				write(nsfd, &length, sizeof(length));
				write(nsfd, data, length * sizeof(char));
			}
			free(data_sent);
			memset(filename, 0, PATH_MAX + NAME_MAX + 1);
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
			serves_client(nsfd, directory, ip, program);
			exit(0);
		} else {
			close(nsfd);
		}
	}
}

int main(int argc, char* argv[]) {
	int semid;
	unsigned short final_values[1];
	key_t key;

	if(argc != 4) {
		printf("Usage: %s dir_ip port root_dir \n", argv[0]);
		return -1;
	}
	int port = atoi(argv[2]);
	if(port < 5000) {
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

	if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    perror(argv[0]);
    return -1;
  }

  if( (semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    perror(argv[0]);
    return -1;
  }

	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, 0, GETALL, final_values);

	server(argv[1], port, directory, argv[0]);

	return 0;
}
