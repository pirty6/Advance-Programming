#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void move(char* program, char *oldfilename, char *newfilename) {
	int fd_in, fd_out;

	if ( (fd_in = open(oldfilename, O_RDONLY)) < 0 ) {
		perror(program);
		exit(-1);
	}

	if ( (fd_out = open(newfilename, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

	off_t length;
	length = lseek(fd_in, 0, SEEK_END);
	lseek(fd_in, 0, SEEK_SET);

	char *buffer;
	buffer = (char*) malloc(length * sizeof(char));

	ssize_t nbytes;
	while ( (nbytes = read(fd_in, buffer, length)) != 0 ) {
		write(fd_out, buffer, nbytes);
	}

	free(buffer);

	close(fd_in);
	close(fd_out);

	int c;

	if ((c=unlink(oldfilename)) != 0) {
		perror(program);
		exit(-1);
	}
}

void copy(char* program, char *oldpath, char *newpath, long min, long max) {
	DIR* olddir;
	DIR* newdir;

	if ( (olddir = opendir(oldpath)) == NULL )
	{
		perror(program);
		exit(-1);
	}

	if ( (newdir = opendir(newpath)) == NULL )
	{
		perror(program);
		exit(-1);
	}

	struct dirent* direntry;
	struct stat info;
	char oldfilename[PATH_MAX + NAME_MAX + 1], newfilename[PATH_MAX + NAME_MAX + 1];

	while ((direntry = readdir(olddir)) != NULL)
	{
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0)
		{
			sprintf(oldfilename, "%s/%s", oldpath, direntry->d_name);
			lstat(oldfilename, &info);

			if (S_ISDIR(info.st_mode))
			{
				copy(program, oldfilename, newpath, min, max);
			}
			else if (min <= info.st_size && info.st_size <= max)
			{
				sprintf(newfilename, "%s/%s", newpath, direntry->d_name);
				move(program, oldfilename, newfilename);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 5)
	{
		printf("usage: %s old_path new_path min_size max_size\n", argv[0]);
		return -1;
	}

	char *oldpath, *newpath, *p, *q;
	long min, max;

	oldpath = argv[1];
	newpath = argv[2];
	min = strtol(argv[3], &p, 10);
	max = strtol(argv[4], &q, 10);

	copy(argv[0], oldpath, newpath, min, max);
	return 0;
}
