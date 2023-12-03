#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){

  if (argc != 2){
    fprintf(stderr, "Usage: %s <infile>\n", argv[0]);
    exit(303);
  }

  int fd[2];
  if (pipe(fd) == -1){
    perror("Pipe failed!");
    exit(10);
  }

  int pid = fork();
  if (pid == -1){
    perror("Fork failed!");
    exit(20);
  }

  if (pid == 0){ //child process
    close(fd[0]);
       
    dup2(fd[1], STDOUT_FILENO);

    execlp("sort", "sort", "-n", argv[1],  (char *)0);

  } else { //parent process
		close(fd[1]); // close writing end of pipe
		
		char ch;
		printf("Data received through pipe: ");
		
		// read from the pipe and print received data line by line
		while (read(fd[0], &ch, 1) > 0) {
		    if (ch == '\n') {
		        printf("\nData received through pipe: ");
		    } else {
		        putchar(ch);
		    }
		}
		
		printf("\n"); // ensure a newline after the last line
		wait(NULL); // wait for child process to finish
    close(fd[0]); // close reading end of pipe


  }

  return 0;
}
