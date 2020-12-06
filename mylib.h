
#ifndef MY_LIB_H
#define	MY_LIB_H

#include <stdint.h>
#define MAX_LEN 16
#define SIZE    32


typedef struct{

    unsigned char * buffer;
    int head;
    int tail;
    int maxlen;

} RingBuffer;

// TODO Insert C++ class definitions if appropriate

void bit_set(unsigned char* num, int bt);
void bit_clear(unsigned char* num, int bt);
int insertData(RingBuffer* c, unsigned char data);
int removeData(RingBuffer * c, unsigned char * data);
int isEmpty(RingBuffer * c);

#endif	

