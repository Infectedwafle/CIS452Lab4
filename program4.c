#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 255

void *find_file1(void *arg);
void print_stats(int);

// global (shared and specific) data
int requestsRecieved = 0;
int requestsServiced = 0;

int quit = 0;
int main()
{
    signal(SIGINT, print_stats);
    pthread_t thread1;
    int status;
    
    while(!quit) {
	fflush(stdout);
	char fileName[MAXLINE];
	printf("Enter a File Name: ");
	fgets(fileName, MAXLINE, stdin);
	
	fileName[strcspn(fileName, "\n")] = 0;
    
	if ((status = pthread_create(&thread1, NULL, find_file1, &fileName)) != 0) {
	    fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
	    exit(1);
	}
    }
    
    if ((status = pthread_join(thread1, NULL)) != 0) {
	    fprintf(stderr, "join error %d:%s\n", status, strerror(status));
	    exit(1);
    }
    
    return 0;
}

void *find_file1(void *arg)
{	
    printf("%p\n", arg);
    requestsRecieved++;
    //note the cast of the void pointer to the desired data type
    char *fileName = (char *)arg;
    
    int rand1 = rand() % 10 + 1;
    
    if(rand1 < 9) {
      sleep(1);
      requestsServiced++;
      printf("%s Found\n", fileName);
    } else {
      int rand2 = rand() % 10 + 7;
      sleep(rand2);
      printf("%s Not Found\n", fileName);
    }
    return NULL;
}

void print_stats(int sigNum) {
    printf("\nFile Requests Recieved: %d\n", requestsRecieved);
    printf("File Requests Serviced: %d\n", requestsServiced);
    quit = 1;
}