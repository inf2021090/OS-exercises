#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Function declarations
void* P1(void* arg);
void* P2(void* arg);
void* P3(void* arg);

sem_t sem_P1P3, sem_P2P1;

int main() {
    pthread_t thread_P1, thread_P2, thread_P3;

    sem_init(&sem_P1P3, 0, 1);
    sem_init(&sem_P2P1, 0, 0);

    pthread_create(&thread_P1, NULL, P1, NULL);
    pthread_create(&thread_P2, NULL, P2, NULL);
    pthread_create(&thread_P3, NULL, P3, NULL);

    pthread_join(thread_P1, NULL);
    pthread_join(thread_P2, NULL);
    pthread_join(thread_P3, NULL);

    sem_destroy(&sem_P1P3);
    sem_destroy(&sem_P2P1);

    return 0;
}

// Function definitions
void* P1(void* arg) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(&sem_P1P3);
        fprintf(file, "P1:Hello from p1\n");
        fprintf(stdout, "P1:Hello from p1\n"); 
        fflush(file);  // Flush the file buffer
        sem_post(&sem_P2P1);
        sleep(1); 
    }
    fclose(file);
}

void* P2(void* arg) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(&sem_P2P1);
        fprintf(file, "P2:This is p2\n");
        fprintf(stdout, "P2:This is p2\n");  
        fflush(file);  // Flush the file buffer
        sem_post(&sem_P1P3);
        sleep(1); 
    }
    fclose(file);
}

void* P3(void* arg) {
    FILE *file = fopen("output.txt", "a");
    while (1) {
        sem_wait(&sem_P1P3);
        fprintf(file, "P3:Im p3\n");
        fprintf(stdout, "P3:Im p3\n");  
        fflush(file);  // Flush the file buffer
        sem_post(&sem_P2P1);
        sleep(1); 
    }
    fclose(file);
}

