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
    printf("Error code: %d\n", 3003);
    exit(EXIT_FAILURE);
  }

  int fd[2];
  if (pipe(fd) == -1){
    perror("Pipe failed!");
    printf("Error code: %d\n", 10);
    exit(0);
  }

  int pid = fork();
  if (pid == -1){
    perror("Fork failed!");
    printf("Error code: %d\n", errno);
    exit(20);
  }

  if (pid == 0){ //child process
    close(fd[0]);
       
    dup2(fd[1], STDOUT_FILENO);

    execlp("sort", "sort", "-n", argv[1],  (char *)0);

  } else { //parent process
		close(fd[1]); // close writing end of pipe

		char ch;
		int newLine = 1; // Flag to indicate start of a new line
		
		// read from the pipe and print received data line by line
		while (read(fd[0], &ch, 1) > 0) {
		    if (newLine) {
		        printf("Data received through pipe: ");
		        newLine = 0;
		    }
		
		    if (ch == '\n') {
		        putchar('\n');
		        newLine = 1;
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
