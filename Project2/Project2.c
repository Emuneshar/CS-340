#include <stdio.h>


void *threadOne(){
    printf("Thread One\n");
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