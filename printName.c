#include <stdio.h>
int main(int argc, char *argv[]) {
    	if (argc !=2) {
		printf("Error - Invalid Output");
		return 0;
	}
      
	printf("Hello  %s\n", argv[1]);
     
}
