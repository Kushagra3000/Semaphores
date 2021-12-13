//Name : Kushagra Gupta
//Roll No. : 2019056

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 10

//int n = 10;

struct my_semaphore { //Semaphore Structure
	int count; // count of resources available. for eg. for 10 philosopher seat is 10
	int wait_queue; // wait_queue to check the number of active threads that require a resource
	pthread_mutex_t mutex;  // for mutual exclusion among threads.
	pthread_cond_t cond;  // for blocking.
};	

// declaring semaphores
struct my_semaphore seat;
struct my_semaphore bowl[2];
struct my_semaphore chopstick[n];

void initialise(struct my_semaphore *, int ); // prototype for intialising function.
void wait(struct my_semaphore *);  // wait function prototype
void signal(struct my_semaphore *); // signal function prototype.
void _signal(struct my_semaphore *); // function for debugging


void * philosopher(int index){

	while(1){ // infinite while loop
	
		// allocating resources to the philosopher
		wait(&seat);
		wait(&bowl[0]);
		wait(&bowl[1]);
		printf("Philosopher %d received both Bowls \n", index);
		//printf("%d", bowl[0]->val);
		wait(&chopstick[index]);
		wait(&chopstick[(index+1)%n]);
		printf("Philosopher %d eats using forks %d and %d\n", index, index, (index+1)%n);
		sleep(2); // eating process
		
		//signalling threads that resources are being made available as the thread ends.
		signal(&bowl[0]);
		signal(&bowl[1]);

		signal(&chopstick[(index+1)%n]);
		signal(&chopstick[index]);

		signal(&seat);
	}
}

int main()
{
	//int m = 10;
	//initialise(&seat,n-1);
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
	pthread_cond_init(&s->cond, NULL);
	s->wait_queue = 0;
	return;
}

void wait(struct my_semaphore *s) {
	pthread_mutex_lock(&s->mutex); // mutex locked critical section. only one thread to execute at a time.
	s->count = s->count - 1; // to reduce the amonut of resource available.
	if(s->count < 0) { // To check if resource is below zero if it is below zero, the process waits.
		do{
			//printf("executing");
			pthread_cond_wait(&s->cond, &s->mutex);
		}while(s->wait_queue < 1);	
		s->wait_queue--; // Removing from wait queue as the wait ends as resource is available.
	}
	pthread_mutex_unlock(&s->mutex); // mutex unlocked
}

void signal(struct my_semaphore *s) { // signalling function to signal other processes that resource is available.
	pthread_mutex_lock(&s->mutex);
	(s->count)++; // increasing available resource.
	if (s->count <= 0) {
 		(s->wait_queue)++; // if less than equal to zero it means another process needs the resource hence wait_queue is increased.
 		//printf("%d",s->wait_queue);
		pthread_cond_signal(&s->cond);
 	}
 	pthread_mutex_unlock(&s->mutex);
}

void _signal(struct my_semaphore *s){
	printf("%d\n", s->count);
}

