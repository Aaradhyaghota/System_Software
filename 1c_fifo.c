#include <sys/stat.h>
#include <stdio.h>

int main(){
	int flag = mknod("1c_fifo",744,0);
	if(flag == 0)printf("Succesful\n");
	else printf("Unsuccesful\n");

return 0;
}
