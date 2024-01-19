#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>


void s_wait();
void s_signal();
struct sembuf sop;

int main() {
    
    int P1P3,P2P1;
	pid_t pid1, pid2, pid3;
    key_t key_P1P3, key_P2P1;
    

	
	if ((key_P1P3 = ftok("Att1.c", 'K')) == -1){ 
		perror("ftok");	
	}
	
	if ((key_P2P1 = ftok("Att1.c", 'Q')) == -1){ 
		perror("ftok");	
	}
	
	P1P3=semget(key_P1P3,1,IPC_CREAT|0666);   
	P2P1=semget(key_P2P1,1,IPC_CREAT|0666); 
	
	semctl(P1P3,0,SETVAL,0);	
	semctl(P2P1,0,SETVAL,0);
	
	FILE *file = fopen("output.txt", "a");
	
	pid1 = fork();

	
	
    if (pid1 == 0) {
        while (1) {
        	s_wait(P2P1);
        	fprintf(file, "P1:Hello from p1\n");
            fprintf(stdout,"P1:Hello from p1\n");
            fflush(file); 
            s_signal(P1P3);
            sleep(1); 
        }
    }
    

    pid2 = fork();
	
	
    if (pid2 == 0) {
        while (1) {
        	fprintf(file, "P2:This is p2\n");
            fprintf(stdout,"P2:This is p2\n");
            fflush(file); 
            s_signal(P2P1);
            sleep(1); 
        }
    }
    
    pid3 = fork();
    
    
    if (pid3 == 0) {
        while (1) {
        	s_wait(P1P3);
        	fprintf(file, "P3:I am p3\n");
            fprintf(stdout, "P3:I am p3\n");
            fflush(file); 
            sleep(1); 
        }
    }

    return 0;
}

void s_wait(int semid){
	sop.sem_num=0;
	sop.sem_op=-1;
	sop.sem_flg=0;
	if(semop(semid, &sop, 1) != 0){
		perror("Locking:");
	}
}
void s_signal(int semid){
	sop.sem_num=0;
	sop.sem_op=1;
	sop.sem_flg=0;
	if(semop(semid, &sop, 1) != 0){
		perror("Unlocking:");
	}
} 

