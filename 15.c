#include <stdio.h>

extern char **environ;

int main(){
	//char ** env = environ;
	while(*environ){
		printf("%s\n",*environ);
		environ++;
	}
}
