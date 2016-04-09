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

ringbuffer buffer;

void printRingBufferMessage(message *element) {
    printf("v: %d, cs: %d, ln: %d, pc: %d, and q: %d\n",
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

void * consumerThread(void * sum) {
  while (true) {
    message msg = extractElement(&buffer);
    if (msg.quit == 1){
      printf("Final sum is %d\n", *((int*)sum));
      pthread_exit(0);
    }
    nsleep(msg.consumer_sleep);
    *((int*)sum) += msg.value;
    if (msg.print_code == 2 || msg.print_code ==3)
      printf("Consumed %d from input line %d; sum = %d\n", msg.value, msg.line, *((int*)sum));
  }
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
  int line_number = 0;
  int value, consumerSleep, producerSleep, printCode;
  while (fgets(line, 100, trace_file_pointer) != NULL)  {
    line_number ++;
    sscanf(line, "%d %d %d %d", &value, &consumerSleep, &producerSleep, &printCode);
    nsleep(500);
    message msg = {value, consumerSleep, line_number, printCode, 0};
    insertElement(&buffer, &msg);
    if (printCode == 1 || printCode == 3)
      printf("Produced %d from input line %d \n", value, line_number);
  }
  message msg = {0, 0, 0, 0, 1};
  insertElement(&buffer, &msg);
  fclose(trace_file_pointer);
  pthread_join(consumer, NULL);
  printf("%d", sum);
  return 0;
}
