#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

int main (){
	int cnt = 1;
	while(1){
	    bool flag = true;
	    if(flag){
		    creat("nwfile_1",0774);
		    creat("nwfile_2",0774);
		    creat("nwfile_3",0774);
		    creat("nwfile_4",0774);
		    creat("nwfile_5",0774);
		    flag = false;
	    }
	}
return 0;
}
