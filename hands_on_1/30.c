/*
========================================================================================================

Question no.: 30
Author: Aaradhya GHota
Description: Write a program to run a script at a specific time using a Daemon process.
Date: 30th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include <sys/stat.h>

int main( int argc, char** argv ){
	if( argc != 4 ){
		printf("enter time in HH MM SS format\n");
		return 0;
	}

	time_t start, end;
	struct tm* spTime;

	time( &start );
	spTime = localtime( &start );

	spTime->tm_hour = atoi( argv[1] );
	spTime->tm_min = atoi( argv[2] );
	spTime->tm_sec = atoi( argv[3] );

	end = mktime( spTime );
	int p = fork();

	if( p == 0 ){
		setsid();
		chdir("/");
		umask(0);
		printf("child pid : %d\n",getpid());
		do{
			time( &start );
		} while( difftime( end, start ) > 0 );

		system("echo Specific time reached.\n");
	}
	else{
		printf("Parent process terminated\n");
		exit(0);
	} 

	return 0;
}

// to check for demon process use ps -el | grep <pid of child> 


/*
========================================================================================================
Output:

./a.out 22 41 40
Parent process terminated
child pid : 22730

---
Specific time reached.



========================================================================================================
*/
