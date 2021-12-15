// Eshwar Muneshar

// gcc EMuneshar_prj3_sect37_src.c -o EMuneshar_prj3_sect37_src.exe
// ./EMuneshar_prj3_sect37_src.exe output.txt


#include <sys/types.h>
#include <stdbool.h>
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// GLOBAL SECTION 


// declare stack
int stack[20];

//popOrder array
int popOrder[120];

// variable for stack
int top = 0;

// variables to print
int pushSum = 0;
int popSum = 0;
int orderKeeper = 0;

// declare  global mutex
pthread_mutex_t myLock;

void push(int item) {
	
	if (top < 20) {
		pthread_mutex_lock(&myLock);
		stack[top] = item;
		top++;
		pthread_mutex_unlock(&myLock);
	}
	else{
		sched_yield();
	}
}


int pop() {
	
	if (top != 0) {
		pthread_mutex_lock(&myLock);
			top--;
		pthread_mutex_unlock(&myLock);
		return stack[top];
	}
	else {
		pthread_mutex_unlock(&myLock);
		sched_yield();
	}

}


void* threadOne() {
	int toStack = 1;
	
	// loop will go from 1 to 120 while push counter will keep 
	// track of how many times
	// we've pushed. If push counter's last digit is a 1, that means we've done 10
	// consecutive pushes, meaning we need to yield. 
	while (toStack < 121) {
		int pushCounter = 0;
		if ((pushCounter % 10) == 1) {
			sched_yield();
		}
	// if the stack is full, meaning top = 20, yield because the stack needs to be emptied
		if (top == 20) {
			sched_yield();
		}

		// push toStack to the stack and increment toStack and pushCounter
		else {
			push(toStack);
			pushSum = pushSum + toStack;
			toStack++;
			pushCounter++;
		}
	}
	
}



 void* threadTwo() {
	 /*Here we have 3 if statements, 1 for each condition where we would need to pop 
	 from the stack.*/


	 /*If the stack is full, pop all the numbers off of the stack. Order keeper is a 
	 global variable initialized to 0, this is what we will use to pass the popped values to 
	 the popOrder array. Once the stack is empty, sched yield*/
	if (top >= 20) {
		while (top != 0) {
			int popped = pop();
			popOrder[orderKeeper] = popped;
			orderKeeper++;
			popSum = popSum + popped;
		}
		sched_yield();
	}

	/*If the stack is not full, but has more than 5 numbers on it, we will want to pop 5 numbers off of the stack.
	Once 5 numbers have been popped we add them to popSum and then yield */
	else if (top > 5) {
		int counter = 5;
		while (counter >= 0) {
			int popped = pop();
			popOrder[orderKeeper] = popped;
			orderKeeper++;
			popSum = popSum + popped;
			counter--;
		}
		sched_yield();
	}

	/*If there are 5 or less numbers on the stack, pop all of them off and then yield. Add the popped values too popSum*/
	 else if (top <= 5) {
		while (top != 0) {
			int popped = pop();
			popOrder[orderKeeper] = popped;
			orderKeeper++;
			popSum = popSum + popped;
		}
		sched_yield();
	}
	
}


int main(int argc, char* argv[]){
	
	// declares file 
	FILE* myFile;

	//// names the file and operations of file
	myFile = fopen(argv[1], "w+");


	// error checking for file creation
	if (myFile == NULL) {
		printf("error when creating file");
		exit(1);
	}
		

	// initialize mutex lock with error checking
	if (pthread_mutex_init(&myLock, NULL) != 0) {
		printf("Mutex Failed \n");
	}
	else {
		printf("Mutex Passed \n");
	}

	
	// declare pthreads
	pthread_t threadOneID;
	pthread_t threadTwoID;

	// create pthreads
	pthread_create(&threadOneID, NULL, threadOne, NULL);
	pthread_create(&threadTwoID, NULL, threadTwo, NULL);

	// join pthreads
	pthread_join(threadOneID, NULL);
	pthread_join(threadTwoID, NULL);


	// prints out the value of popOrder to the output.txt file
	for (int i = 0; i < 121; i++) {
		fprintf(myFile, "%s", "The Value of element ");
		fprintf(myFile, "%d", i);
		fprintf(myFile, "%s", " of popOrder array is: ");
		fprintf(myFile, "%d\n", popOrder[i]);
	}

	// prints out the value of pushSum and popSum to the output.txt file
	fprintf(myFile, "%s", "Value of Pthread 1 pushSum is: ");
	fprintf(myFile, "%d\n", pushSum);
	fprintf(myFile, "%s", "Value of Pthread 2 popSum is: ");
	fprintf(myFile, "%d\n", popSum);


	// Closes the file
	fclose(myFile);

	// destroys mutex lock
	int pthread_mutex_destroy(pthread_mutex_t * myLock);
	return 0;

	

}