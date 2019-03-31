//15.03.2019
//Saurabh Singh Gautam
//Sauram
/*

Following is the solution of Dining Philosopher Problem

*/
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> 
  
#define N 5 
#define Thinking_of_solution 2 
#define Feeling_hungry 1 
#define Eating_Food 0 
#define left_side_fork (phnum + 4) % N 
#define right_side_fork (phnum + 1) % N 
  
int state[N]; 
int philospher_baithne_wale[N] = { 0, 1, 2, 3, 4 }; 
  
sem_t mutual_wala_exclusion; 
sem_t S[N]; 
  
void test(int phnum) { 
    if (state[phnum] == Feeling_hungry && state[left_side_fork] != Eating_Food && state[right_side_fork] != Eating_Food) { 
        state[phnum] = Eating_Food; 
  
        sleep(2); 
  
        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, left_side_fork + 1, phnum + 1); 
  
        printf("Philosopher %d is Eating\n", phnum + 1); 
        sem_post(&S[phnum]); 
    } 
} 
  
// take up chopsticks 
void take_fork(int phnum) { 
  
    sem_wait(&mutual_wala_exclusion); 
  
    // state that hungry 
    state[phnum] = Feeling_hungry; 
  
    printf("Philosopher %d is Hungry\n", phnum + 1); 
  
    // eat if neighbours are not eating 
    test(phnum); 
  
    sem_post(&mutual_wala_exclusion); 
  
    // if unable to eat wait to be signalled 
    sem_wait(&S[phnum]); 
  
    sleep(1); 
} 
  
// put down chopsticks 
void put_fork(int phnum) { 
  
    sem_wait(&mutual_wala_exclusion); 
  
    // state that thinking 
    state[phnum] = Thinking_of_solution; 
  
    printf("Philosopher %d putting fork %d and %d down\n", 
           phnum + 1, left_side_fork + 1, phnum + 1); 
    printf("Philosopher %d is thinking\n", phnum + 1); 
  
    test(left_side_fork); 
    test(right_side_fork); 
  
    sem_post(&mutual_wala_exclusion); 
} 
  
void* philospher(void* num) { 
  
    while (1) { 
  
        int* i = num; 
  
        sleep(1); 
  
        take_fork(*i); 
  
        sleep(0); 
  
        put_fork(*i); 
    } 
} 
  
int main() { 
  
    int i; 
    pthread_t thread_id[N]; 
  
    // initialize the semaphores 
    sem_init(&mutual_wala_exclusion, 0, 1); 
  
    for (i = 0; i < N; i++) 
  
        sem_init(&S[i], 0, 0); 
  
    for (i = 0; i < N; i++) { 
  
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL, 
                       philospher, &philospher_baithne_wale[i]); 
  
        printf("Philosopher %d is thinking\n", i + 1); 
    } 
  
    for (i = 0; i < N; i++) 
  
        pthread_join(thread_id[i], NULL); 
} 
