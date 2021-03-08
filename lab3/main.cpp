#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>



const int n = 4;
int matrix[n][n] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
bool rowPhase = true;

void read_from_file() {
    
    FILE *file;
    file = fopen("input.txt", "r");


    if (file == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d,", &matrix[i][j] );
        }
    }

    fclose(file);
}

void print_integers_to_stdout(){
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}


void swap(int *xp, int *yp) { 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void bubbleSort(int arr[], int n, int (*cmp)(int a, int b)) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (cmp(arr[j],arr[j+1])); 
              swap(&arr[j], &arr[j+1]); 
} 

int cmp_min(int a, int b) { return a < b ? 1 : 0; }
int cmp_max(int a, int b) { return a > b ? 1 : 0; }

void* sort_right(int arr[], int size){}
void* sort_left(int arr[], int size){}
void* sort_down(int arr[], int size){}

void* task(void* arg){
    int thread_number = (int) arg;
    printf("running task %d \n", thread_number);

    int vals[n];
    for (int i = 0; i < n; i++) {
        vals[i] = rowPhase ? matrix[thread_number][i] : matrix[i][thread_number];
    }
    bubbleSort(vals, n, rowPhase % 2 == 0 ? cmp_min : cmp_max);
    for (int i = 0 ; i < n; i++) {
        if (rowPhase) {
            matrix[thread_number][i] = vals[i];
        } else {
            matrix[i][thread_number] = vals[i];
        }
    }
}


int main(void){
    
// 1. Read the integers into a 2-dimensional nxn array in global memory from 
// the file "input.txt". This can be done by the main thread before creating the
// n sorting threads.
read_from_file();

// 2. Print the integers in the order entered to stdout.
print_integers_to_stdout();

// 3. Create and initialize the semaphores or condition variable necessary for 
// the algorithm.
// 4. Create the n threads to sort the array using Shearsort.
// 5. Wait for the threads to finish a phase.
// 6. Print the array of sorted integers to stdout and go to the next phase.

/*
 *  1. Create a semaphore
 *  2. Give each worker its portion of the matrix 
 *  3. Wait for each worker to finish its routine
 *  4. 
 */
pthread_t pthreads[n];

// sem_t mutex;
// sem_t count;
// sem_init(&count, 0, 1);
// sem_init(&mutex, 0, 1);

// int completions;

for (int p = 0; p < 8; p++) {
    printf("Phase %d\n", p+1);
    // Create threads
    for (int i = 0; i < n; i++) {
        printf("Thread %d\n", i);
        pthread_create(&pthreads[i], NULL, task, i);
        // sleep(1);
    }
    // Close threads
    for (int i = 0; i < n; i++) {
        pthread_join(pthreads[i], NULL);
        // sleep(1);
    }
    printf("===============\n");

    // next phase
    rowPhase = !rowPhase;

    // print array
    print_integers_to_stdout();
}




// 1. By performing the appropriate number of wait operations on sempahores (or 
// by a proper wait on a condition variable), block until the prior phase (if 
// any) is finished.
// 2. Sort the row/column in the appropriate order using Bubble Sort.
// 3. Perform appropriate signal operations to signal the other threads to begin
// the next phase.

}



