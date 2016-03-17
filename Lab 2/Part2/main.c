#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_RUNTIME 10 //Max runtime of a request
#define BUFFER_SIZE 10 //Max size of the buffer

/* The mutex lock */
pthread_mutex_t mutex;

/* The semaphores */
sem_t full, empty;

/* Buffer counter, also represents the current position */
int counter;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes
int id = 1; //ID for the requests, first request gets ID 1
int consumerID = 0; //ID for the consumers, first consumer gets ID 1

/* A struct representing the request */
typedef struct _request{
  int tid; //Request ID
  int secs; //Request duration
} request;

/* The buffer */
request buffer[BUFFER_SIZE];

void *producer(void *param); /* The producer thread */
void *consumer(void *param); /* The consumer thread */
int insertRequest(request* r); /* Method for inserting a request */
int removeRequest(request* r); /* Method for removing a request */


/* Producer Thread */
void *producer(void *param) {

   while(1) {

      time_t rawtime;
      struct tm * timeinfo;

      /* Create a request */
      request* item = (request *)malloc(sizeof(request));
      srand(time(NULL));
      int r = rand() % MAX_RUNTIME;

      //The request fields are set
      item->tid = id;
      item->secs = r;
      id++;

      /* Acquire the empty lock */
      sem_wait(&empty);
      /* Acquire the mutex lock */
      pthread_mutex_lock(&mutex);

      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      
      /* Insert the request */
      if(insertRequest(item)) 
         fprintf(stderr, "Producer reports problem inserting request.\n");
      
      else 
         printf("Producer: produced request ID %d, length %d seconds at time %s\n",item->tid,item->secs,asctime (timeinfo));

      /* Release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* Signal full */
      sem_post(&full);

      /* Sleep for a random period of time */
      int s = rand() % MAX_RUNTIME;
      printf("Producer: sleeping for %d seconds.\n",s);
      sleep(s);
   }
}

/* Consumer Thread */
void *consumer(void *param) {
   consumerID++;
   request* item;

   while(1) {

      /* Aquire the full lock */
      sem_wait(&full);
      /* Aquire the mutex lock */
      pthread_mutex_lock(&mutex);
      /* Remove the request and process it */
      item = removeRequest(&item);
      int id = buffer[counter].tid;
      int secs = buffer[counter].secs;
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );

      if(item == -1) 
         fprintf(stderr, "Consumer reports error removing request.\n");

      else {
         /* Report the completion of the request */
         time_t t;
         struct tm * ti;
         time ( &t );
         ti = localtime ( &t );
         printf("Consumer: %d  assigned request ID %d, processing request for the next %d seconds, current time is %s\n", consumerID, id, secs, asctime (timeinfo));
         sleep(secs);
         printf("Consumer: %d  completed request ID %d at time %s\n", consumerID, id, asctime(ti));
      }
      
      /* Release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* Signal empty */
      sem_post(&empty);

   }
}

/* Add a request to the buffer */
int insertRequest(request* r) {
   /* When the buffer is not full add the request
      and increment the counter*/
   if(counter < BUFFER_SIZE) {
      buffer[counter] = *r;
      counter++;
      return 0;
   }
   else { /* Error the buffer is full */
      return -1;
   }
}

/* Remove a request from the buffer */
int removeRequest(request* r) {
   /* When the buffer is not empty remove the request
      and decrement the counter */
   if(counter > 0) {
      *r = buffer[(counter-1)];
      counter--;
      return 0;
   }
   /* Buffer is empty */
   else { 
      return -1;
   }
}

int main(int argc, char *argv[]) {

   /* Number of consumers */
   int numCons = 10;

   /* Durration of the program */
   int duration = 20;
  
   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Create the full semaphore and initialize to 0 */
   sem_init(&full, 0, 0);

   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&empty, 0, BUFFER_SIZE);

   /* Get the default attributes */
   pthread_attr_init(&attr);

   /* Initialize buffer */
   counter = 0;

   /* Create the consumer threads */
   int i;
   for(i = 0; i < numCons; i++) 
      pthread_create(&tid,&attr,consumer,NULL);
   

   /* Create the producer thread */
   pthread_create(&tid,&attr,producer,NULL);

   sleep(duration);

   /* Exit the program */
   printf("Program finished.\n");
   exit(0);
}