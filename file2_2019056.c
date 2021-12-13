// Name : Kushagra Gupta
// Roll No. : 2019056

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 10


struct my_semaphore {//Semaphore Structure
	int count;// count of resources available. for eg. for 10 philosopher seat required is 10
	pthread_mutex_t mutex; // for mutual exclusion among threads.
};	

// declaring semaphores
struct my_semaphore seat;
struct my_semaphore bowl[2];
struct my_semaphore chopstick[n];

void initialise(struct my_semaphore *, int );// prototype for intialising function.
int wait(struct my_semaphore *); // wait function prototype which returns error message of -1 on inavailability of resource.
void signal(struct my_semaphore *);// signal function prototype.
void _signal(struct my_semaphore *); // function for debugging

void * philosopher(int index)
{	
	while(1){

		int wait_mesg = 1;
		// allocating resources to the philosopher and checking if the error message is -1,
		// then the while loop is called until it is 1 again. i.e. resource is available again or not.
		wait_mesg = wait(&seat);
	
		while(wait_mesg == -1) {
			wait_mesg = wait(&seat);
		}
	
		wait_mesg = wait(&bowl[0]);
		while(wait_mesg == -1) {
			wait_mesg = wait(&bowl[0]);
		}
	
		wait_mesg = wait(&bowl[1]);
		while(wait_mesg == -1) {
			wait_mesg = wait(&bowl[1]);
		}
	
		printf("Philosopher %d received both Bowls \n", index);
	
		wait_mesg = wait(&chopstick[index]);
		while(wait_mesg == -1) {
			wait_mesg = wait(&chopstick[index]);
		}
	
		wait_mesg = wait(&chopstick[(index+1)%n]);
		while(wait_mesg == -1) {
			wait_mesg = wait(&chopstick[(index+1)%n]);
		}
	
		printf("Philosopher %d eats using forks %d and %d\n", index, index, (index+1)%n);
		sleep(2); // eating process
		
		// Returning resources from the philosopher.
		signal(&bowl[0]);
		signal(&bowl[1]);
	
		signal(&chopstick[(index+1)%n]);
		signal(&chopstick[index]);
	
		signal(&seat);
	}
}

int main(){

	// Initialising semaphores with the value of their available resource
	pthread_t phil_id[n];
	initialise(&seat,n-1);
	initialise(&bowl[0],1);
	initialise(&bowl[1],1);

	for(int i=0;i<n;i++)
		initialise(&chopstick[i],1);

	// Starting threads.
	for(int i=0;i<n;i++)
		pthread_create(&phil_id[i],NULL,philosopher,i);

	// joining threads.
	for(int i=0;i<n;i++)
		pthread_join(phil_id[i],NULL);
}

void initialise(struct my_semaphore *s, int value) { // initialising attributes of semaphore to an appropriate value.
	s->count = value;
	pthread_mutex_init(&s->mutex, NULL);
}

int wait(struct my_semaphore *s) {
	pthread_mutex_lock(&s->mutex); // mutex locked critical section. only one thread to execute at a time.
	s->count = s->count - 1; // to reduce the amonut of resource available.
	if(s->count < 0) { // If resource goes below zero it means it is not available hence increasing it back
		(s->count)++;
		pthread_mutex_unlock(&s->mutex);
	      	return -1; // returning -1 as a message
	}
	pthread_mutex_unlock(&s->mutex); // mutex unlocked
	return 1;
}

void signal(struct my_semaphore *s) { // signalling function to signal other processes that resource is available.
	pthread_mutex_lock(&s->mutex);
	(s->count)++; // increasing available resource.
 	pthread_mutex_unlock(&s->mutex);
}

void _signal(struct my_semaphore *s) {
	printf("%d\n", s->count);
}

