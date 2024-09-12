/*
========================================================================================================

Question No.: 18a
Author: Aaradhya Ghota
Description: Write a program to perform Record locking.
	a. Implement write lock
	b. Implement read lock
Date: 30th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


int main (){
	int fd = open("18db.txt", O_RDWR, 0644);
        if(fd == -1) printf("file not opened");
        
        struct Record{
	      int recNo;
	      int recVal;
        };
      
	int rn;
	printf("Enter record Number from 1,2,3 you want to write\n");
	scanf("%d",&rn);
	
        int p1 = lseek(fd, (rn-1) * sizeof(struct Record), SEEK_SET);
        
        
	struct flock lk;
        lk.l_type = F_WRLCK;
        lk.l_whence = SEEK_CUR;
        //lk.l_start = 0;
        lk.l_len = sizeof(struct Record);
        //lk.l_pid = getpid();
        
	struct Record rec;
        //locking
        printf("Acquiring write lock\n");
        fcntl(fd, F_SETLKW, &lk);
        
        int flag = read(fd, &rec , sizeof(struct Record));
        if(flag == -1){
            printf("Error in reading1\n");
        }
        
        printf("Record No. :%d \nRecord Value : %d\n",rec.recNo, rec.recVal);
        
        int p2 = lseek(fd, (rn-1) * sizeof(struct Record), SEEK_SET);
        
        //updating
        printf("Record Updating......\n");
        sleep(1);
        rec.recVal++;
        write( fd, &rec, sizeof(struct Record) );
        printf("Record No. :%d \nNew Record Value : %d\n",rec.recNo, rec.recVal);
        
        printf("Enter to release lock");
        getchar();
        getchar();
        
        lk.l_type = F_UNLCK;
        fcntl(fd,F_SETLK, &lk);
        printf("Write Lock released\n");
        
        close(fd);
        return 0; 

}

/*
========================================================================================================
Output:

----------------acquiring same record------------------------------

Terminnal 1:
./18writer
Enter record Number from 1,2,3 you want to write
3
Acquiring write lock
Record No. :3 
Record Value : 2
Record Updating......
Record No. :3 
New Record Value : 3
Enter to release lock


Terminal 2:

./18reader 
Enter record Number from 1,2,3
3
Acquiring read lock

terminal 3;
./18writer
Enter record Number from 1,2,3 you want to write
3
Acquiring write lock

----------------acquiring different record------------------------------
Terminnal 1:
./18writer
Enter record Number from 1,2,3 you want to write
2
Acquiring write lock
Record No. :2 
Record Value : 3
Record Updating......
Record No. :2 
New Record Value : 4
Enter to release lock

terminal 2:
./18reader
Enter record Number from 1,2,3
1
Acquiring read lock
Record No. :1 
Record Value : 6
Enter to release lock

Terminal 3:
./18writer
Enter record Number from 1,2,3 you want to write
3
Acquiring write lock
Record No. :3 
Record Value : 5
Record Updating......
Record No. :3 
New Record Value : 6
Enter to release lock


========================================================================================================
*/
