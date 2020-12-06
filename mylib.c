#include "mylib.h"

int insertData(RingBuffer* c, unsigned char data){

    int next = c->head + 1;

    if(next >= c->maxlen) next = 0;

    if(next == c->tail) return -1;

    c->buffer[c->head] = data;
    c->head = next;
    return 0; 
}

int removeData(RingBuffer * c, unsigned char * data){
    int next = c->tail+1;

    if (c->head == c->tail) return -1;

    if (next >= c->maxlen) next = 0;

    *data = c->buffer[c->tail];
    c->tail = next;
    return 0;
}

int isEmpty(RingBuffer * c){
    if (c->head == c->tail) return 1;
    return 0;
}

void bit_set(unsigned char* num, int bt){
    *num |= (1 << bt);
}

void bit_clear(unsigned char* num, int bt){
    *num  &= ~(1 << bt);
}

    
    
