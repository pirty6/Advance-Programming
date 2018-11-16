#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<pthread.h>
#include <string.h>

#define MUTEX 0

int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void *connection_handler(void *);

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

struct va {
    char* name;
    int number;
    char* ip;
} variables;

int main(int argc , char *argv[]) {
  int semid;
  unsigned short final_values[1];
  key_t key;

  if(argc != 4) {
    printf("Usage: %s dir_ip port log_name\n", argv[0]);
    return -1;
  }
  int port = atoi(argv[2]);
  if(port < 5000) {
		printf("%s The port must be grater than 5000\n", argv[0]);
		return -1;
	}
  variables.name = argv[3];
  variables.ip = argv[1];
  //printf("%s\n", variables.name);
  variables.number = 0;

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

    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons( port );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	  pthread_t thread_id;

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc) {
  int semid;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) -1) {
    exit(-1);
  }

  if ( (semid = semget(key, 1, 0666))  < 0 ) {
    exit(-1);
  }

  FILE *fp;
  char log_filename[PATH_MAX + NAME_MAX + 1];
  sprintf(log_filename, "%s.txt", variables.name);
  char* data_log = "";
  mutex_wait(semid, MUTEX);
  data_log = concat(variables.ip, "Ha iniciado una conexion");
  writeFile(log_filename, data_log);
  mutex_signal(semid, MUTEX);
    int sock = *((int*)socket_desc);
    int number_sent, response, value;
    char num[10], temp[10];
    do {
      read(sock, &number_sent, sizeof(number_sent));
      printf("NUMBER: %i\n", number_sent);
      //printf("VALOR: %i\n", value);
      if(number_sent == 101) {
        //printf("ENTRO\n");
        mutex_wait(semid, MUTEX);
        sprintf(num, "%d", variables.number);
        data_log = "";
        data_log = concat(variables.ip, " Valor anterior: ");
        data_log = concat(data_log, num);
        data_log = concat(data_log, " Valor actual: ");
        data_log = concat(data_log, num);
        writeFile(log_filename, data_log);
        mutex_signal(semid, MUTEX);
        response = variables.number;
      } else if(number_sent == 301) {
        data_log = concat(variables.ip, " Ha terminado la conexion");
        mutex_wait(semid, MUTEX);
        writeFile(log_filename, data_log);
        mutex_signal(semid, MUTEX);
      } else if(number_sent == 201) {
        //AQUI
        read(sock, &value, sizeof(value));
        mutex_wait(semid, MUTEX);
        sprintf(temp, "%d", variables.number);
        variables.number = value;
        sprintf(num, "%d", variables.number);
        data_log = "";
        data_log = concat(variables.ip, " Valor anterior: ");
        data_log = concat(data_log, temp);
        data_log = concat(data_log, " Valor actual: ");
        data_log = concat(data_log, num);
        writeFile(log_filename, data_log);
        mutex_signal(semid, MUTEX);
        response = 1;
      }
      //printf("AQUI QUEDA\n");
      write(sock, &response, sizeof(response));
    } while(number_sent != 301);
    close(sock);
    return 0;
}
