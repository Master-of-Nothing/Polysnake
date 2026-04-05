
#include "NucleoImp/Sound/SnakeAudio.h"

#include <cmath>
#include "main.h" // Nécessaire pour inclure les types HAL (comme DAC_HandleTypeDef)

// Variable globale HAL générée par CubeMX (à adapter selon votre timer/dac)

SnakeAudio* g_SnakeAudio = nullptr;

// Étape 6 : Mélodie rétro de 20 notes (Thème classique de type Korobeiniki/Tetris)
const Note melody[20] = {
    {659.25, 150}, {493.88, 75}, {523.25, 75}, {587.33, 150},
    {523.25, 75}, {493.88, 75}, {440.00, 150}, {440.00, 75},
    {523.25, 75}, {659.25, 150}, {587.33, 75}, {523.25, 75},
    {493.88, 150}, {493.88, 75}, {523.25, 75}, {587.33, 150},
    {659.25, 150}, {523.25, 150}, {440.00, 150}, {440.00, 300}
};
constexpr uint32_t MELODY_LENGTH = 20;

SnakeAudio::SnakeAudio() : currentPhase(0.0f), currentFrequency(0.0f), noteTimer(0), currentNoteIndex(0), isPlaying(false) {
	g_SnakeAudio = this; // Assigne l'instance en cours au pointeur global
}

void SnakeAudio::Init(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, Waveform wave) {

	this->hdac = hdac;
	this->htim = htim;
	HAL_TIM_Base_Start(htim);
	switch(wave) {
        case SINE: InitSineTable(); break;
        case SQUARE: InitSquareTable(); break;   // Parfait pour un rendu "chiptune" 8-bit !
        case TRIANGLE: InitTriangleTable(); break;
    }
}

void SnakeAudio::InitSineTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Formule pour le DAC 12 bits : valeurs comprises entre 0 et 4095
        waveTable[i] = (uint16_t)(2047.0 * sin(2.0 * pi * i / TABLE_SIZE) + 2048.0);
    }
}

void SnakeAudio::InitSquareTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        waveTable[i] = (i < TABLE_SIZE / 2) ? 4095 : 0;
    }
}

void SnakeAudio::InitTriangleTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (i < TABLE_SIZE / 2) {
            waveTable[i] = (i * 4095) / (TABLE_SIZE / 2);
        } else {
            waveTable[i] = 4095 - (((i - TABLE_SIZE / 2) * 4095) / (TABLE_SIZE / 2));
        }
    }
}

void SnakeAudio::Start() {
    currentNoteIndex = 0;
    noteTimer = melody[0].duration_ms;
    currentFrequency = melody[0].frequency;
    isPlaying = true;

    // Lancement du transfert DMA sur la totalité du tableau (les 2 buffers en même temps)
    // Le HAL gérera automatiquement les moitiés avec le mode "Circular".
    HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, (uint32_t*)dmaBuffer, BUFFER_SIZE * 2, DAC_ALIGN_12B_R);
}

// Fonction centrale (Étape 4) : remplit la zone mémoire spécifiée
void SnakeAudio::FillBuffer(uint16_t* buffer) {
    if (!isPlaying) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
        	buffer[i] = 2048; // Silence (milieu du DAC)
        }
        return;
    }

    // Le ratio qui détermine la vitesse de lecture de la table
    float phaseIncrement = (currentFrequency * TABLE_SIZE) / SAMPLE_RATE;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = waveTable[(uint16_t)currentPhase];

        currentPhase += phaseIncrement;
        if (currentPhase >= TABLE_SIZE) {
            currentPhase -= TABLE_SIZE; // Assure la continuité de phase !
        }
    }
}

void SnakeAudio::ProcessHalfBuffer() {
    FillBuffer(dmaBuffer); // Remplit la première moitié
}

void SnakeAudio::ProcessFullBuffer() {
    FillBuffer(&dmaBuffer[BUFFER_SIZE]); // Remplit la seconde moitié
}

// Étape 5 : La machine à états
void SnakeAudio::UpdateState() {
    if (!isPlaying) return;

    if (noteTimer > 0) {
        noteTimer--;
    } else {
        currentNoteIndex++;
        if (currentNoteIndex >= MELODY_LENGTH) {
            currentNoteIndex = 0; // Rejoue en boucle
        }

        // Change la fréquence pour la prochaine exécution de FillBuffer
        currentFrequency = melody[currentNoteIndex].frequency;
        noteTimer = melody[currentNoteIndex].duration_ms;
    }
}

// =========================================================================
// PONT C -> C++ POUR LES CALLBACKS HAL
// =========================================================================
extern "C" {
    // Appelé quand le DMA a fini de lire la PREMIÈRE moitié du buffer
    void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
        if (g_SnakeAudio) g_SnakeAudio->ProcessHalfBuffer();
    }

    // Appelé quand le DMA a fini de lire la DEUXIÈME moitié du buffer
    void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
        if (g_SnakeAudio) g_SnakeAudio->ProcessFullBuffer();
    }
    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    	if(htim->Instance == TIM6){
    		if (g_SnakeAudio) g_SnakeAudio->UpdateState();
    	}
    }
}
