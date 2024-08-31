#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	struct Record{
		int recNo;
		int recVal;	
	};
	
	struct Record arr[3];
	for(int i =0;i<3;i++){
		arr[i].recNo = i+1;
		arr[i].recVal = 0;
	}
	
	int fd = open ("18db.txt",O_CREAT|O_RDWR, 0744);
        write(fd, &arr, sizeof(arr));
	close(fd);

	fd = open("18db.txt", O_RDONLY);
	read(fd, &arr,sizeof(arr));
	for(int i =0;i<3;i++){

		printf("Record No. :%d \nRecord Value : %d\n",arr[i].recNo, arr[i].recVal);
	}
	close(fd);
}
