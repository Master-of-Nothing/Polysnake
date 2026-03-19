/**
  * @file UART.cpp
  * @date June 2025
  * @brief This file contains the implementation of the UART class that 
  * implements the Communication interface.
  * @details The UART class implements an interface for the USART interface
  * as present on the STM32 Cortex M4 micro controller used for the STM32
  * NUCLEO 64 development board.
  * 
  * @defgroup ELE3312
  * @{
  */



#include "NucleoImp/SerialCom/UART.h"
#include "stm32f4xx_hal_def.h"
#include "NucleoImp/SerialCom/Ringbuffer.h"

using namespace ELE3312;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART1) { // Vérifie que c'est le bon UART

    	//Refaire pour integrer le ringbuffer
        Ringbuffer_Write(rx_byte); // 1. On sauvegarde l'octet

        HAL_UART_Receive_IT(&huart, &rx_byte, 1); // 2. On relance l'écoute
    }
}

// Mettre à jour par rapoort au stabndarts Cpp
void UART::Process_Data() {
    static uint8_t packet_buffer[128]; // Stocke le message en cours de lecture
    static uint8_t packet_index = 0;
    uint8_t byte_read;

    // Tant qu'il y a des données dans le Ring Buffer
    while (RingBuffer_Read(&byte_read)) {

        if (byte_read == COBS_DELIMITER) {
            // Fin de trame COBS détectée !
            if (packet_index > 0) {
                uint8_t decoded_payload[128];

                // 1. Appeler ta fonction de décodage COBS sur 'packet_buffer'
                size_t decoded_len = cobs_decode(packet_buffer, packet_index, decoded_payload);

                // 2. Mettre à jour le snake de l'adversaire
                // Update_Opponent_Snake(decoded_payload, decoded_len);
            }
            // Réinitialiser pour le prochain message
            packet_index = 0;
        }
        else {
            // Ajouter l'octet au message en cours
            if (packet_index < sizeof(packet_buffer)) {
                packet_buffer[packet_index++] = byte_read;
            } else {
                // Erreur : message trop long, on rejette tout
                packet_index = 0;
            }
        }
    }
}



/**
  * @}
  */ // End of documentation group ELE3312
