
/*
========================================================================================================
Name : 18
Author : Aaradhya Ghota
Description : Write a program to find out total number of directories on the pwd. Execute ls -l | grep ^d | wc. Use only dup2.
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main(){
      int pipe1[2], pipe2[2];
    
      if (pipe(pipe1) == -1) {
          printf("Pipe1 failed");
          exit(1);
      }

      
      if(pipe(pipe2) == -1){
          printf("Pipe2 failed");
          exit(1);
      }

      if(!fork())
      {   //child 1
          close(1);//closing std out
          
          dup2(pipe1[1], 1); // pipe1[1] --> STDOUT
          close(pipe1[0]);
          close(pipe2[0]);
          close(pipe2[1]);
          execl("/bin/ls", "/bin/ls", "-l", NULL);
      }
      else{
      
          if (!fork()){
              //child 2
              
              close(0);
              
              dup2(pipe1[0], 0); // pipe1[0] --> STDIN
              
              close(1);
              
              dup2(pipe2[1], 1); // pipe2[1] --> STDOUT
              close(pipe1[1]);
              close(pipe2[0]);
              execl("/bin/grep", "/bin/grep", "^d", NULL);
          }
          else{
              //parent
              
              close(0);
              
              dup2(pipe2[0], 0); // pipe2[0] --> STDIN
              close(pipe2[1]);
              close(pipe1[0]);
              close(pipe1[1]);
              execl("/bin/wc", "/bin/wc", "-l", NULL);
          }
      }

      return 0;
}

/*
========================================================================================================
Output:

./a.out
0
there are zero output
========================================================================================================
*/

