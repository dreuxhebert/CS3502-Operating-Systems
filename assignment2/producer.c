/*
 * Name: Dreux Hebert
 * CS 3502 - Assignment 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "buffer.h"

#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <id> <num_items>\n", argv[0]);
        return 1;
    }

    int id = atoi(argv[1]);
    int num_items = atoi(argv[2]);

    int shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), IPC_CREAT | IPC_EXCL | 0666);
    int created = 1;

    if (shm_id == -1) {
        created = 0;
        shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), 0666);
        if (shm_id == -1) {
            perror("shmget");
            return 1;
        }
    }

    shared_buffer_t *buffer = (shared_buffer_t *)shmat(shm_id, NULL, 0);
    if (buffer == (void *)-1) {
        perror("shmat");
        return 1;
    }

    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0644, BUFFER_SIZE);
    sem_t *full = sem_open(SEM_FULL, O_CREAT, 0644, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, 1);

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
        perror("sem_open");
        shmdt(buffer);
        return 1;
    }

    if (created) {
        sem_wait(mutex);
        buffer->head = 0;
        buffer->tail = 0;
        buffer->count = 0;
        sem_post(mutex);
    }

    for (int i = 0; i < num_items; i++) {
        item_t item;
        item.value = id * 1000 + i;
        item.producer_id = id;

        sem_wait(empty);
        sem_wait(mutex);

        buffer->buffer[buffer->head] = item;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        buffer->count++;

        printf("Producer %d: Produced value %d\n", id, item.value);
        fflush(stdout);

        sem_post(mutex);
        sem_post(full);

        usleep(100000);
    }

    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    shmdt(buffer);

    return 0;
}