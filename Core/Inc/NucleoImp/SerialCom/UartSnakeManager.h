#ifndef UARTSNAKEMANAGER_H
#define UARTSNAKEMANAGER_H


#include <NucleoImp/SerialCom/COBS.h>
#include <NucleoImp/SerialCom/Ringbuffer.h>
#include <stdint.h>
#include <cstddef>
#include "main.h"

namespace ELE3312 {

// L'adresse de base de l'Unique ID pour la famille STM32F4
#define STM32F4_UID_BASE 0x1FFF7A10

extern UART_HandleTypeDef huart2;

RingBuffer rx_ring_buffer;
uint8_t rx_byte;
const uint8_t COBS_DELIMITER = 0x00;
// Structure des données échangées (doit être identique sur les deux cartes)

struct SnakePayload {
    uint8_t player_id;
    uint16_t head_x;
    uint16_t head_y;
    uint8_t direction;
};

class UartSnakeManager {
public:
	UartSnakeManager();
	void setup(UART_HandleTypeDef *huart);
    // Négocie les rôles via l'UID et lance l'écoute. Retourne 1 (Joueur 1) ou 2 (Joueur 2)
    uint8_t negotiatePlayerId();

    // Envoie la position de ton serpent
    void sendData(const SnakePayload& mySnake);

    // Récupère la position de l'adversaire (retourne true si nouveau message complet)
    bool receiveData(SnakePayload& opponentSnake);
private :
    UART_HandleTypeDef *huart;
    RingBuffer rx_ring_buffer;
    uint8_t rx_byte;
    const uint8_t COBS_DELIMITER = 0x00;
};

}

// Instance globale accessible partout
//extern UartSnakeManager uartManager;

#endif // UARTSNAKEMANAGER_H
