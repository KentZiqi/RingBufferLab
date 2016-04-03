#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

static struct ringbuffer ringbufferimpl;
int main(int argc, char *argv[]) {
    message firstMessage = {1, 1, 1, 1, 1};
    message secondMessage = {2, 2, 2, 2, 2};
    message thirdMessage = {3, 3, 3, 3, 3};
    message fourthMessage = {4, 4, 4, 4, 4};
    message fifthMessage = {5, 5, 5, 5, 5};

    insertElement(&ringbufferimpl, &firstMessage);
    printBuffer(&ringbufferimpl);

    insertElement(&ringbufferimpl, &secondMessage);
    printBuffer(&ringbufferimpl);

    message removeFirst = extractElement(&ringbufferimpl);
    printRingBufferMessage(&removeFirst);

    insertElement(&ringbufferimpl, &thirdMessage);
    printBuffer(&ringbufferimpl);

    insertElement(&ringbufferimpl, &fourthMessage);
    printBuffer(&ringbufferimpl);

    insertElement(&ringbufferimpl, &fifthMessage);
    printBuffer(&ringbufferimpl);

    message removeSecond = extractElement(&ringbufferimpl);
    printRingBufferMessage(&removeSecond);

    printf("%s", "Hello ZQ");
}
