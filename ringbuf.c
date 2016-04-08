#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define SIZE 10

typedef struct message {
    int value;
    /* Value to be passed to consumer */
    int consumer_sleep;
    /* Time (in ms) for consumer to sleep */
    int line;
    /* Line number in input file */
    int print_code;
    /* Output code; see below */
    int quit; /* NZ if consumer should exit */
} message;

typedef struct ringbuffer {
    message messages[SIZE];
    int head;
    int tail;
    int number_of_elements;
} ringbuffer;

void printRingBufferMessage(message *element) {
    printf("Value is %d, consumer sleep is %d, line num is %d, print code is %d, and quit number is %d\n",
           element->value, element->consumer_sleep, element->line, element->print_code, element-> quit);
}

void printBuffer(struct ringbuffer *ringbuffer) {
    for (int i = 0; i < SIZE; i++) {
        if (i == ringbuffer->head) {
            printf("Head: ");
        }
        if (i == ringbuffer->tail) {
            printf("Tail: ");
        }
        printf("%dth element in the buffer is ", i);
        printRingBufferMessage(&ringbuffer->messages[i]);
    }
    printf("\n");
}

void insertElement(struct ringbuffer *ringbuffer, message *element) {
    if (ringbuffer->number_of_elements < SIZE) {
        ringbuffer->messages[ringbuffer->head] = *element;
        ringbuffer->number_of_elements++;
        if (ringbuffer->head == SIZE - 1) {
            ringbuffer->head = 0;
        } else {
            ringbuffer->head++;
        }
    } else {
        // Wait for consumer to extract element
    }
    return;
}

message extractElement(struct ringbuffer *ringbuffer) {
    if (ringbuffer->number_of_elements > 0) {
        message element = ringbuffer->messages[ringbuffer->tail];
        ringbuffer->number_of_elements--;
        if (ringbuffer->tail == SIZE - 1) {
            ringbuffer->tail = 0;
        } else {
            ringbuffer->tail++;
        }
        return element;
    } else {
        // Wait for producer to insert element
        message empty = {0,0,0,0,0};
        return empty;
    }
}

void * consumerThread(void * sum) {
  *((int*)sum) = 42;
  //while (message != QUIT) {
    // Wait
  //}
  pthread_exit(0);
}

// From http://stackoverflow.com/a/33412960
int nsleep(long miliseconds)
{
   struct timespec req, rem;

   if(miliseconds > 999) {
        req.tv_sec = (int)(miliseconds / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   }
   else {
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }
   return nanosleep(&req , &rem);
}

//static struct ringbuffer ringbufferimpl;
int main(int argc, char *argv[]) {
  pthread_t consumer;	// this is our thread identifier
  int sum = 0;
  pthread_create(&consumer, NULL, consumerThread, &sum);

  char * filename = "testinput0.txt";
  FILE *trace_file_pointer = fopen(filename, "r");
  if (!trace_file_pointer) {
      fprintf(stderr, "%s: %s\n", filename, strerror(errno));
      exit(1);
  }

  char line[100];
  while (fgets(line, 100, trace_file_pointer) != NULL)  {
    char value[5], consumerSleep[5], producerSleep[5], printCode[5];
    sscanf(line, "%s %s %s %s", value, consumerSleep, producerSleep, printCode);
    printf("%s", value);
    nsleep(1000);
    fflush(stdout);
  }
  fclose(trace_file_pointer);
  pthread_join(consumer, NULL);
  printf("%d", sum);
  return 0;
}
