/**
  * @file Ringbuffer.cpp
  * @date June 2025
  * @brief This file implements a circular buffer that is used to store
  * incoming bytes from the UART interface.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "NucleoImp/SerialCom/Ringbuffer.h"




/** @brief Default constructor for a Ringbuffer object.
  */
Ringbuffer::Ringbuffer() { 
}


void Ringbuffer::setup() {
	buffer = nullptr;
	head = 0;
	tail = 0;
	size = 0;
}

/** @brief Destructor for the Ringbuffer class. 
  */
Ringbuffer::~Ringbuffer() {
}


void Ringbuffer::write(uint8_t data) {
    uint16_t next_head = (head + 1) % RING_BUFFER_SIZE;
    if (next_head != tail) { // Si le buffer n'est pas plein
        buffer[head] = data;
        head = next_head;
    }
}


bool Ringbuffer::read(uint8_t *data) {
    if (head == tail) {
        return false; // Buffer vide
    }
    *data = buffer[tail];
    tail = (tail + 1) % RING_BUFFER_SIZE;
    return true;
}





/**
  * @}
  */ // End of the documentation group ELE3312

