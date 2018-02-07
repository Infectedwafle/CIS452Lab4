#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 255

void *find_file1(void *arg);
void stop_server(int);

// global (shared and specific) data
int requestsRecieved = 0;
int requestsServiced = 0;
int threadCount = 0;

int quit = 0;
int main()
{
    signal(SIGINT, stop_server);
    pthread_t thread1;
    int status;
 
    while(!quit) {
	char fileName[MAXLINE];
	fflush(stdout);
	printf("Enter a File Name: ");
	fflush(stdout);
	fflush(stdin);
	if (0 < read(fileno(stdin), &fileName, MAXLINE))
	
	fileName[strcspn(fileName, "\n")] = 0;
    
	if ((status = pthread_create(&thread1, NULL, find_file1, &fileName)) != 0) {
	    fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
	    exit(1);
	}
    }
    
    printf("Waiting for Requests to finish");	
    while(threadCount > 0) {
	
    }
    	// wait for threads to finish if any
   
    printf("\nFile Requests Recieved: %d\n", requestsRecieved);
    printf("File Requests Serviced: %d\n", requestsServiced);
    
    printf("Shutting Down Server\n");    
    fflush(stdout); 
    return 0;
}

void *find_file1(void *arg)
{	
    threadCount++;
    requestsRecieved++;
    //note the cast of the void pointer to the desired data type
    char fileName[strlen(arg)];
    memcpy(fileName, arg, strlen(arg));
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

    threadCount--;
    return NULL;
}

void stop_server(int sigNum) {
    quit = 1;
}
