In this assignment we have to create our own semaphore structure and
implementations of wait() and signal() functions for using the semaphores to
achieve mutual exclusion in modified dining philosopher problem using pthread_mutex
structure. With n >= 2 philosophers.
The structure my defined semaphore contains 4 attributes.
1) Count: available of the resources.
2) wait_queue: to check currently active thread which are need waiting of a
particular resource.
3) mutex : to ensure mutual exclusion.
4) cond : to pass blocking wait signal to all the threads.

##In the Blocking Implementation:
I have created a counting semaphore structure as described above. Then i have
defined 3 different semaphores using structures. That are seats, chopsticks, bowl.
2 bowls because we have 2 bowls which are simultaneously required for eating as
described in the question. In the "philosopher" function i have used resources like
seat, chopsticks and bowl for the philosopher. And the count of available resources
is decreased by 1. and once the resources are free i have used signal function to
again increase the count by 1 of the resource. Also if another threads tries to
access the already under use resource it goes in a block/ wait state. Hence the
deadlock is avoided. The philosophers are in a infinite loop as mentioned in the
question.

##In Non Blocking Implementation:
I have created a counting semaphore without wait_queue and conditon attribute.
In this the rest of the implementation is same as blocking but in this I have
returned a value from the wait function of the semaphore. Since it is a non
blocking implementation threads are not blocked or wait until the resource is
available instead i have checked the returned value of the wait() function. if it
returns a wait message of -1 it means that the resource is not available and it has
to wait so i have used the while loop to ensure that thread wait until it is
available and avoid deadlock. The return value of -1 acts as an error which is done
in Non blocking implementation.
I have outputed the The process of philosophers picking bowl and chopsticks to eat.
