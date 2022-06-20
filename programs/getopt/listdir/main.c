#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void listfile(char *name)
{
	struct stat sb;		/* the stat buffer */
	char *modtime;

	/* array of filetypes, indexed by the top four bits of st_mode */

	char *filetype[] = { "?", "p", "c", "?", "d", "?", "b", "?", "-", "?", "l", "?", "s"};
	
	if (stat(name, &sb) < 0) {
		perror(name);
		exit(2);
	}

	/* File type (using the same indicator characters as ls) */
	printf("%s", filetype[(sb.st_mode >> 12) & 017]);

	/* permissions same as ls */
	printf("%c%c%c%c%c%c%c%c%c",
			(sb.st_mode & S_IRUSR) ? 'r' : '-',
			(sb.st_mode & S_IWUSR) ? 'w' : '-',
			(sb.st_mode & S_IXUSR) ? 'x' : '-',
			(sb.st_mode & S_IRGRP) ? 'r' : '-',
			(sb.st_mode & S_IWGRP) ? 'w' : '-',
			(sb.st_mode & S_IXGRP) ? 'x' : '-',
			(sb.st_mode & S_IROTH) ? 'r' : '-',
			(sb.st_mode & S_IWOTH) ? 'w' : '-',
			(sb.st_mode & S_IXOTH) ? 'x' : '-');

	printf("%8ld", sb.st_size);

	modtime = ctime(&sb.st_mtime);
	/* ctime() string has a newline char at the end */
	modtime[strlen(modtime) - 1] = '\0';
	printf("  %s  ", modtime);
	printf("%s\n", name);
}

int main(int argc, char *argv[])
{
	DIR *d;
	struct dirent *info;
	int c, allflag = 0;

	opterr = 0; /* do not get getopts error messages */

	/*  getopt works by taking argc, argv and a sample structure of your 
	 *  arguments as a string.  a boolean argument is specified as just
	 *  the character it represents i.e 'a' or 'a' or 'ab'
	 *  - if the character is followed by ':' then it means it will takes
	 *  a value. so getopt will parse them for you. i.e 'ab:' this means 
	 *  'a' is a boolean argument but 'b' takes a value.*/

	while((c = getopt(argc, argv, "a")) != EOF) {
		switch(c) {
			case 'a':
				allflag = 1;
				break;
			case '?':
				fprintf(stderr, "invalid option: -%c\n", optopt);
		}
	}

	argv += optind;
	argc -= optind; /* adjust to move past the options */

	/* now argc is the number of non-optional arguments and argv[0] is the
	 * first one
	 */

	if (argc != 1) {
		fprintf(stderr, "usage: listdir2 [-a] dirname\n");
		exit(1);
	}
	chdir(argv[0]);
	d = opendir(".");

	while ((info = readdir(d)) != NULL) {
		if (info->d_name[0] != '.' || allflag)
			listfile(info->d_name);
	}
}
