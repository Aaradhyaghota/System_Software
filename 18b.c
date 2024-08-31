/*
========================================================================================================

Question No.: 18b
Author: Aaradhya Ghota
Description: Write a program to perform Record locking.
	b. Implement read lock
Date: 30th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
        int fd = open("18db.txt",O_RDWR, 0644);
        if(fd == -1) printf("file not opened");
	
        struct Record{
	      int recNo;
	      int recVal;
        };
        
        int rn;
	printf("Enter record Number from 1,2,3\n");
	scanf("%d",&rn);
	
        
        int p1 = lseek(fd, (rn-1) * sizeof(struct Record), SEEK_SET);
        
        
        struct flock lk;
        lk.l_type = F_RDLCK;
        lk.l_whence = SEEK_CUR;
        //lk.l_start = 0;
        lk.l_len = sizeof(struct Record);
        //lk.l_pid = getpid();
        
        struct Record rec;
        //locking
        printf("Acquiring read lock\n");
        fcntl(fd, F_SETLKW, &lk);
        
        int flag = read(fd, &rec , sizeof(struct Record));
        if(flag == -1){
            printf("Error in reading1\n");
        }
        
        printf("Record No. :%d \nRecord Value : %d\n", rec.recNo, rec.recVal);
        
        printf("Enter to release lock");
        getchar();
        getchar();
        
        lk.l_type = F_UNLCK;
        fcntl(fd,F_SETLK, &lk);
        printf("Read Lock released\n");
        
        close(fd);
        return 0; 
}

/*
========================================================================================================
Output:

terminal 1:
./18reader 
Enter record Number from 1,2,3
2
Acquiring read lock
Record No. :2 
Record Value : 4
Enter to release lock

Terminal 2:
./18reader 
Enter record Number from 1,2,3
3
Acquiring read lock
Record No. :3 
Record Value : 6
Enter to release lock

Terminal 3:
./18writer
Enter record Number from 1,2,3 you want to write
3
Acquiring write lock




========================================================================================================
*/
