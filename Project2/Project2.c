#include <stdio.h>


void *threadOne(){
    char buf[SIZ]; 
    int n;
    while ((n = read(f1, buf, SIZ)) > 0)
			if (write(fd[1], buf, n) != n) {
				write(STDOUT_FILENO, "Can't write to screen", sizeof(msg) - 1);
				close(f1);
				close(f2);
				return 4;
			}
    return NULL;
}
void *threadTwo(){
    printf("Thread Two\n");
    return NULL;
}
void *threadThree(){
    printf("Thread 3\n");
    return NULL;
}


int main(void) {
    printf("Before Thread Create\n");
    pthread_t threadOneID;
    pthread_t threadTwoID;
    pthread_t threadThreeID;
    
    pthread_create(&threadOneID, NULL, threadOne, NULL);
    pthread_create(&threadTwoID, NULL, threadTwo, NULL);
    pthread_create(&threadThreeID, NULL, threadThree, NULL);

    pthread_join(threadOneID, NULL);
    pthread_join(threadTwoID, NULL);
    pthread_join(threadThreeID, NULL);
    return 0;
}