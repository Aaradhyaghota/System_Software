#include <stdio.h>
#include <unistd.h>

int main(){
    char *path = "/bin/ls";
  	// path    //name  // argument till null  
    execl(path,"ls","-Rl",NULL); 
}
