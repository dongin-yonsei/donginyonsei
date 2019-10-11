#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define LINE_SIZE 100

char* strerror(int errnum);
int find(char src[], char tar[]);

int main(int argc, char *argv[]) {
  
  FILE* fp;             // file pointer for opening, reading and closing files
  char buf[LINE_SIZE];  // buffer for reading lines from files
  int line_counter;		// line counter which is used when printing out matching lines
 
  // variables for method getline
  size_t len = 0;
  ssize_t read;
  char *line = NULL;

  // when only my-grep is typed in command line without any term or file
  if (argc == 1) {

  	printf("my-grep: searchterm [file...]\n");
  	exit(1);
  }

  // when there is a term to find specified but not file
  if (argc == 2) {
  	fgets(buf, LINE_SIZE, stdin);
  	if (find(argv[1], buf)) printf("%s",buf);
  }

  if (argc > 2) {
  	// matching no line when search term is an empty string
  	// print nothing and just exit
  	if (!*argv[1]) {
  		exit(0);
  	}

  	for (int i = 2; i < argc; i++) {
  		fp = fopen(argv[i], "r");

  		// print error message and exit if file is not opened
  		if (fp == NULL) {
  			printf("my-grep: cannot open file: %s\n", strerror(errno));
      		exit(1);
  		}

  		// read each line until reaching the last line of the file
  		while ((read = getline(&line, &len, fp)) != -1) {
  			line_counter++;
  			if(find(argv[1],line)) {
  				printf("%d:%s",line_counter, line);
  			}
  		}
  		// close the file and go to next file if exists
  		fclose(fp);
  	}
  }

exit(0);
}


int find(char src[], char tar[]) {

	// allocating some pointers to be used in comparison
	char *str1, *str2;
	char *comp_point = (char *)tar;

	// starting from the first char of the target string
	while (*comp_point) {
		// str1 is a pointer which points a character of target string
		str1 = comp_point;
		// str2 is a pointer which points a character of search term string
		str2 = (char *)src;

		// if both characters of each string are identical
		while (*str1 && *str2 && (*str1 == *str2)) {
			// increment each pointer and compare next characters
			str1++;
			str2++;
		}

		// when there is a identical string of search term in target string
		// return 1 which means true
		if (!*str2) return 1;

		// otherwise, compare from the next character of target string
		comp_point++;
	}
	// if nothing identical to search term is found in target string 
	// return 0 which means false
	return 0;
}
