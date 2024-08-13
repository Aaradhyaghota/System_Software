#include <stdio.h>
#include <fcntl.h>

int main(){
	 // int open(const char *pathname, int flags, mode_t mode);

	 //1.->>The  open() system call opens the file specified by pathname.  If the specified file
         //does not exist, it may optionally (if O_CREAT is specified in flags) be  created  by
         //open().
	 //

	 //  The argument flags must  include  one  of  the  following  access  modes:  O_RDONLY,
         // O_WRONLY,  or  O_RDWR.   These  request  opening  the file read-only, write-only, or
         // read/write, respectively.
	 //
	 //O_CREAT
             //-->> If pathname does not exist, create it as a regular file.
	//
	//O_EXCL
	//-->>> O_EXCL ensure forcefully creation of file if it already exist it doesnot open file and return error.
	//--->>> Ensure that this call creates the file: if this flag is specified in conjunc‐
        //      tion  with  O_CREAT,  and pathname already exists, then open() fails with the
        //      error EEXIST.
	 
	 //2.-->>>open return file discripter value.
	 //
	 //3/mode -->>>The mode argument specifies the permissions to use in case a new file is created (when O_CREAT is used).
	 //This parameter is only used if the file is being created. The mode is typically specified using octal constants, such as 0644, which corresponds to:
	 //Owner can read and write (rw-).
	 //Group can read (r--).
	 //Others can read (r--).

	 //The argument flags must  include  one  of  the  following  access  modes:  O_RDONLY,
         // O_WRONLY,  or  O_RDWR.   These  request  opening  the file read-only, write-only, or
         //read/write, respectively.
	 //
	 //

	int fd  = open("nwfile",O_CREAT | O_EXCL|O_RDWR, 0777);   //O_CREAT | O_EXCL -> opens function opens file only when is its not exist  
	if(fd == -1) printf("file not opended");
	else printf("File Opened");
return 0;
}

