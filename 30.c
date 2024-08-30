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

		system("echo Specific time reached!!!\n");
	}
	else{
		printf("Parent process terminated\n");
		exit(0);
	} 

	return 0;
}

// to check for demon process use ps -el | grep <pid of child> 
