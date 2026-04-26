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


#include <NucleoImp/SerialCom/COBS.h>
#include <NucleoImp/SerialCom/Ringbuffer.h>
#include "NucleoImp/SerialCom/UartSnakeManager.h"
#include "main.h"
#include <cstring>

using namespace ELE3312;

UartSnakeManager uartManager;

const uint8_t COBS_DELIMITER = 0x00;

// ==========================================
// MÉTHODES DE LA CLASSE
// ==========================================
UartSnakeManager::UartSnakeManager(){}

void UartSnakeManager::setup(UART_HandleTypeDef *huart){
this->huart = huart;
}


uint8_t UartSnakeManager::negotiatePlayerId() {
    // 1. Lire les 32 premiers bits de l'UID unique de cette puce
    uint32_t my_uid = *(uint32_t*)STM32F4_UID_BASE;
    uint32_t opponent_uid = 0;
    uint8_t assigned_id = 1; // Joueur 1 par défaut

    // Pour éviter que les deux cartes parlent exactement en même temps avant d'écouter,
    // on peut utiliser une boucle de tentative de connexion.
    bool connected = false;

    while (!connected) {
        // 2. Envoyer notre UID à l'autre carte
        HAL_UART_Transmit(huart, (uint8_t*)&my_uid, sizeof(my_uid), 500);

        // 3. Écouter pour recevoir l'UID adverse (avec un timeout de 500ms)
        if (HAL_UART_Receive(huart, (uint8_t*)&opponent_uid, sizeof(opponent_uid), 500) == HAL_OK) {
            connected = true; // On a reçu une réponse !
        }
    }

    // 4. Déterminer qui est qui
    if (my_uid > opponent_uid) {
        assigned_id = 1;
    } else if (my_uid < opponent_uid) {
        assigned_id = 2;
    }

    // 5. La négociation est terminée, on lance l'interruption asynchrone pour le jeu
    HAL_UART_Receive_IT(huart, &rx_byte, 1);

    return assigned_id;
}

void UartSnakeManager::sendData(const SnakePayload& mySnake) {
    uint8_t raw_buffer[sizeof(SnakePayload)];
    uint8_t encoded_buffer[sizeof(SnakePayload) + 2];

    memcpy(raw_buffer, &mySnake, sizeof(SnakePayload));
    cobsEncode(raw_buffer, sizeof(SnakePayload), encoded_buffer);
    size_t encoded_len = sizeof(SnakePayload);

    encoded_buffer[encoded_len] = COBS_DELIMITER;
    encoded_len++;

    HAL_UART_Transmit_IT(huart, encoded_buffer, encoded_len);
}

bool UartSnakeManager::receiveData(SnakePayload& opponentSnake) {
    static uint8_t packet_buffer[64];
    static uint8_t packet_index = 0; //erreur ici : revoir la logique
    uint8_t byte_read;

    while (rx_ring_buffer.read(byte_read)) {
        if (byte_read == COBS_DELIMITER) {
            if (packet_index > 0) {
                uint8_t decoded_buffer[64];
                memcpy(&opponentSnake, decoded_buffer, sizeof(SnakePayload));
                cobsDecode(packet_buffer, 64, decoded_buffer);
                packet_index = 0;
                return true;
            }
        } else {
            if (packet_index < sizeof(packet_buffer)) {
                packet_buffer[packet_index++] = byte_read;
            } else {
                packet_index = 0;
            }
        }
    }
    return false;
}

// ==========================================
// LE PONT AVEC L'INTERRUPTION HAL (C)
// ==========================================

extern "C" {
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
        if (huart->Instance == USART2) {
        	uartManager.rx_ring_buffer.write(uartManager.rx_byte);
            HAL_UART_Receive_IT(huart, &uartManager.rx_byte, 1);
        }
    }
}



/**
  * @}
  */ // End of documentation group ELE3312
