#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	
	int fd[2];
	pid_t pid;
	char line[64];
	ssize_t bytesRead;
	
	//check number of arguments
	if (argc != 2){
		printf("Usage:helper filename\n"); //error handling
		exit(0);
	}
	
	//check pipe error
	if (pipe(fd) < 0){
		printf("Pipe Error:");
		exit(1);
	}
	
	//create child proccess 	
	pid=fork();
	
	//check fork error
	if (pid < 0){
		printf("Fork Error:");
		exit(1);
	}
	
	//redirect standard output to write end of pipe
	//will return the sorted data of the file to the write end
	if (pid == 0){
		//child process
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("sort", "sort","-n", argv[1], (char *)NULL);
		perror("Error during execlp command");
		exit(13);
		
	}
	else{
	  //parent process
	  close(fd[1]);
	  //will read data from the pipe
	  while ((bytesRead = read(fd[0], line, sizeof(line))) > 0) {	
	  		printf("Data received through pipe:");
	  		//will print the required message everytime a newline is encountered
	  		//otherwise prints the buffer's data 
	  		for (ssize_t i = 0; i < bytesRead-1; ++i) {
				if (line[i] == '\n') {
                    printf("Data received through pipe:");
                }
                else{
	                printf("%c", line[i]);
	                if (line[i+1]=='\n'){
	                	printf("\n");
					}
	   			}
    		}
		}
	}
}

