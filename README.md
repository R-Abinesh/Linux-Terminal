This project is all about trying to create a Bash Terminal using System Programming.

## Functions added
	-cat command
	-change directory
	-list command
	-make new directory
	-print current working directory
	-remove directory

## Implementation

### cat command
The names of the file(s) to be read is got as input and passed to **void cat()** function.
The file is opened using **"r"** mode and the contents of the file are stored in a buffer **s**. The size of the buffer is chosen to be 1024, because **1024 is physical block size**. Buffer size with a multiple of block size gives **huge performance improvement** than any other value, as most of the internal operations takes place interms of block size.
###### Arguments:
1. File name(s)
###### System Call used:`
1. fopen(const char *file_name, mode) \
_fopen() opens the  filename pointed in the respective mode (here in read mode)_
***

### Change directory
Every directory has default **.** and **.** **.** folder.
The **.** points to current directory and **.** **.** folder points to the previous parent directory. This helps us in moving to the parent directory.
###### Arguments:
1. const char* path_
###### System Call used:` 
1. chdir(const char *path)
\
_Call to chdir() function changes the current working directory to the pathname specified by path_

****
### list command
This command is used to **list the directories, files** (hidden) present inside the current directory.
###### Arguments:
1. string param\
_to check if -a is used with ls command or not._\
_If -a is present we print also need to **hidden entries** (files and directories), else print only normal entries._
###### System Call used:`
1. DIR *dStream = opendir(const char* dir);\
_opendir() creates a **directory stream** representing the directory given. Directory stream is similar to a file descriptor, but represents a open directory, along with some meta data._
2. readdir(DIR *dStream)\
_readdir() returns details of every file and folder(called entries) in the directory one by one. The return value is stored in a **dirent structure.**_
		
        struct dirent {
            ino_t d_ino; 			 /* inode number */
			off_t d_off; 			 /* offset to the next dirent */
			unsigned short d_reclen; /* length of this record */
			unsigned char d_type; 	 /* type of file */
			char d_name[256]; 		 /* filename */
	    };

### run command
The applications can be opened from the terminal using the fork() and execv() system call. The fork() system call creates a new child process, followed by execv() system call which puts the process image/binary executable file into the process stack of child process and it gets running.
