#include <stdio.h>
#include <stdbool.h>

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
    message* messages[SIZE];
    message* head;
    message* tail;
    int number_of_elements;
} ringbuffer;

void insertElement(ringbuffer* ringbuffer, message* element) {
    if (buffer->number_of_elements < SIZE) {
        // Insert element
        //ringbuffer->head = *element

    } else {
        // Wait for consumer to extract element
    }
    return;
}

static struct ringbuffer;

int main(int argc, char *argv[]) {
    printf("%s", "Hello ZQ");
}
