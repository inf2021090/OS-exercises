#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

// Function declarations
void P1(sem_t *sem_P1P3, sem_t *sem_P2P1);
void P2(sem_t *sem_P1P3, sem_t *sem_P2P1);
void P3(sem_t *sem_P1P3, sem_t *sem_P2P1);

int main() {
    sem_t *sem_P1P3, *sem_P2P1;

    sem_P1P3 = sem_open("/sem_P1P3", O_CREAT, 0666, 1);
    sem_P2P1 = sem_open("/sem_P2P1", O_CREAT, 0666, 0);

    if (sem_P1P3 == SEM_FAILED || sem_P2P1 == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid_P1, pid_P2, pid_P3;

    pid_P1 = fork();
    if (pid_P1 == -1) {
        perror("Fork P1 failed");
        exit(EXIT_FAILURE);
    } else if (pid_P1 == 0) {
        P1(sem_P1P3, sem_P2P1);
        exit(EXIT_SUCCESS);
    }

    pid_P2 = fork();
    if (pid_P2 == -1) {
        perror("Fork P2 failed");
        exit(EXIT_FAILURE);
    } else if (pid_P2 == 0) {
        P2(sem_P1P3, sem_P2P1);
        exit(EXIT_SUCCESS);
    }

    pid_P3 = fork();
    if (pid_P3 == -1) {
        perror("Fork P3 failed");
        exit(EXIT_FAILURE);
    } else if (pid_P3 == 0) {
        P3(sem_P1P3, sem_P2P1);
        exit(EXIT_SUCCESS);
    }

    waitpid(pid_P1, NULL, 0);
    waitpid(pid_P2, NULL, 0);
    waitpid(pid_P3, NULL, 0);

    sem_unlink("/sem_P1P3");
    sem_unlink("/sem_P2P1");

    sem_close(sem_P1P3);
    sem_close(sem_P2P1);

    return 0;
}

// Function definitions
void P1(sem_t *sem_P1P3, sem_t *sem_P2P1) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(sem_P1P3);
        fprintf(file, "P1:Hello from P1\n");
        fprintf(stdout, "P1:Hello from P1\n");
        fflush(file);
        sem_post(sem_P2P1);
        sleep(1);
    }
    fclose(file);
}

void P2(sem_t *sem_P1P3, sem_t *sem_P2P1) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(sem_P2P1);
        fprintf(file, "P2:This is P2\n");
        fprintf(stdout, "P2:This is P2\n");
        fflush(file);
        sem_post(sem_P1P3);
        sleep(1);
    }
    fclose(file);
}

void P3(sem_t *sem_P1P3, sem_t *sem_P2P1) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(sem_P1P3);
        fprintf(file, "P3:I'm P3\n");
        fprintf(stdout, "P3:I'm P3\n");
        fflush(file);
        sem_post(sem_P2P1);
        sleep(1);
    }
    fclose(file);
}
