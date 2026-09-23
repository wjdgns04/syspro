#include <stdio.h>
#include "copy.h"

char line[MAXLINE]; 
char longest[MAXLINE];
char shortest[MAXLINE];

main(){
	int len;
	int max;
	int min = MAXLINE;
	max = 0;

	while(gets(line) != NULL) {
		len = strlen(line);

		if(len > max) {
			max = len;
			copy(line,longest);
		}
		if(len < min &&len > 0){
			min = len;
			copy(line,shortest);
		}
	}
	if(max > 0){
		printf("longest:%s\n",longest);
	}
	if (min < MAXLINE){
		printf("shortest:%s\n",shortest);
	}
	return 0;
}
