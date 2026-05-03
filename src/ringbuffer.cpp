#include<stdint.h>
#include "config.h"



struct ringBuffer{
    int writeIndex;
    int readIndex;
    bool isAvailable;
    int16_t buffer[BufferSize];
    
};

void initilize_rb(ringBuffer* rb){
    
    rb->writeIndex = 0;
    rb->readIndex = 0;
    rb->isAvailable = false;

    for(int i = 0;i<BufferSize;i++){
        rb->buffer[i] = (int16_t)0;
    }
}

void write_rb(ringBuffer* rb,int16_t data,int datalen = 1){
    for(int i = 0;i < datalen;i++)
    {
        rb->buffer[rb->writeIndex] = data;        
        rb->writeIndex = (rb->writeIndex + 1) % BufferSize;
        
    }  
    if(rb->writeIndex > processLatency)
        rb->isAvailable = true;
    
}

int16_t read_rb(ringBuffer* rb){
    int16_t data = 0;
    if(rb->isAvailable){
        data = rb->buffer[rb->readIndex];
        rb->readIndex = (rb->readIndex + 1) %BufferSize;       
    }
    else 
        return 0;
    return data;
    
} 

