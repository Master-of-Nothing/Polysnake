
#include "NucleoImp/Sound/SnakeAudio.h"

#include <cmath>
#include "main.h"


SnakeAudio* g_SnakeAudio = nullptr;

// Mélodie rétro de 20 notes (Thème classique de Tetris)
const Note melody[20] = {
    {659.25, 150}, {493.88, 75}, {523.25, 75}, {587.33, 150},
    {523.25, 75}, {493.88, 75}, {440.00, 150}, {440.00, 75},
    {523.25, 75}, {659.25, 150}, {587.33, 75}, {523.25, 75},
    {493.88, 150}, {493.88, 75}, {523.25, 75}, {587.33, 150},
    {659.25, 150}, {523.25, 150}, {440.00, 150}, {440.00, 300}
};

// Mélodie pour le Menu (20 notes) - Ambiance rétro-futuriste
const Note melodyMenu[20] = {
    {440.00, 125}, {0.0, 50},      // A4 (Staccato)
    {440.00, 125}, {523.25, 125},  // A4, C5
    {587.33, 125}, {0.0, 50},      // D5
    {587.33, 125}, {659.25, 125},  // D5, E5
    {783.99, 250}, {659.25, 250},  // G5, E5
    {587.33, 125}, {523.25, 125},  // D5, C5
    {440.00, 250}, {0.0, 125},     // A4, pause
    {392.00, 125}, {440.00, 125},  // G4, A4
    {523.25, 125}, {587.33, 125},  // C5, D5
    {659.25, 250}, {440.00, 500}   // E5, A4 (longue)
};

// Mélodie Game Over (20 notes) - La chute du serpent
const Note melodyGameOver[20] = {
    {523.25, 200}, {493.88, 200},  // C5, B4
    {466.16, 200}, {440.00, 400},  // Bb4, A4
    {392.00, 150}, {349.23, 150},  // G4, F4
    {329.63, 600}, {0.0, 100},     // E4 (très longue pause)
    {329.63, 100}, {311.13, 100},  // E4, Eb4 (rapide)
    {293.66, 100}, {277.18, 100},  // D4, Db4
    {261.63, 100}, {246.94, 100},  // C4, B3
    {233.08, 100}, {220.00, 100},  // Bb3, A3
    {207.65, 150}, {196.00, 150},  // Ab3, G3
    {174.61, 300}, {164.81, 800}   // F3, E3 (Note finale très basse)
};

constexpr uint32_t MELODY_LENGTH = 20;

SnakeAudio::SnakeAudio() : currentPhase(0.0f), currentFrequency(0.0f), volume(1.0f), noteTimer(0), currentNoteIndex(0), isPlaying(false), currentMelody(melodyMenu){
	g_SnakeAudio = this; // Assigne l'instance en cours au pointeur global, test : fonctionne bien
}

void SnakeAudio::Init(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, Waveform wave) {

	this->hdac = hdac;
	this->htim = htim;
	HAL_TIM_Base_Start(htim);
	switch(wave) {
        case SINE: InitSineTable(); break;
        case SQUARE: InitSquareTable(); break;
        case TRIANGLE: InitTriangleTable(); break;
    }
}

void SnakeAudio::InitSineTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Formule pour le DAC 12 bits : valeurs comprises entre 0 et 4095, 2048 pour la moitié de 4096
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
    noteTimer = currentMelody[0].duration_ms;
    currentFrequency = currentMelody[0].frequency;
    isPlaying = true;

    // Lancement du transfert DMA sur la totalité du tableau (les 2 buffers en même temps), dma est mit en option circulaire
    HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, (uint32_t*)dmaBuffer, BUFFER_SIZE * 2, DAC_ALIGN_12B_R);
}

// fonction permettant de remplir la zone mémoire spécifiée, utiliser pour la double buffer
void SnakeAudio::FillBuffer(uint16_t* buffer) {
    /*if (!isPlaying) {
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
            currentPhase -= TABLE_SIZE; // Assure la continuité de phase
        }
    }*/
	if (!isPlaying) {
		for (int i = 0; i < BUFFER_SIZE; i++) {
			buffer[i] = 2048; // Silence
		}
		return;
	}

	float phaseIncrement = (currentFrequency * TABLE_SIZE) / SAMPLE_RATE;

	for (int i = 0; i < BUFFER_SIZE; i++) {
		// Échantillon brut depuis la table d'onde
		uint16_t rawSample = waveTable[(uint16_t)currentPhase];

		// === APPLICATION DU VOLUME ===
		// 1. Centre l'échantillon autour de zéro
		int32_t centered = (int32_t)rawSample - 2048;
		// 2. Multiplie par le coefficient de volume
		centered = (int32_t)((float)centered * volume);
		// 3. Recentre autour du point médian du DAC
		buffer[i] = (uint16_t)(centered + 2048);

		// Avance la phase
		currentPhase += phaseIncrement;
		if (currentPhase >= (float)TABLE_SIZE) {
			currentPhase -= (float)TABLE_SIZE;
		}
	}
}

void SnakeAudio::SetVolume(float vol) {
	// Clamp entre 0.0 et 1.0 pour éviter la saturation
	if (vol < 0.0f) vol = 0.0f;
	if (vol > 1.0f) vol = 1.0f;
	volume = vol;
}

float SnakeAudio::GetVolume() const {
	return volume;
}

void SnakeAudio::UpdateVolumeFromADC(ADC_HandleTypeDef *hadc) {
	// Lance une conversion unique (mode polling, non bloquant longtemps)
	HAL_ADC_Start(hadc);

	// Attend la fin de conversion avec un timeout de 10ms
	// En pratique, une conversion ADC à 16MHz d'horloge prend ~1µs
	if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
		uint32_t rawValue = HAL_ADC_GetValue(hadc);

		// rawValue est entre 0 et 4095 (12 bits)
		// On mappe vers [0.0, 1.0]
		float newVolume = (float)rawValue / 4095.0f;

		// === FILTRAGE OPTIONNEL ===
		// Un léger lissage évite les clics audio si le potentiomètre
		// est bruité. Coefficient faible = changement lent et doux.
		const float VOL_ALPHA = 1.0f;
		volume = VOL_ALPHA * newVolume + (1.0f - VOL_ALPHA) * volume;
	}

	HAL_ADC_Stop(hadc);
}

void SnakeAudio::ProcessHalfBuffer() {
    FillBuffer(dmaBuffer); // Remplit la première moitié
}

void SnakeAudio::ProcessFullBuffer() {
    FillBuffer(&dmaBuffer[BUFFER_SIZE]); // Remplit la seconde moitié
}

// fonction gérant la machine à états de la mélodie
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
        currentFrequency = currentMelody[currentNoteIndex].frequency;
        noteTimer = currentMelody[currentNoteIndex].duration_ms;
    }

}

void SnakeAudio::setTrack(Track trackName)
{
/*
	// A modifier car
	if (trackName == TRACK_MENU) {
	        son = melodyMenu; // Remplace par ta variable de tableau actif
	        //currentMelodyLength = sizeof(melodyMenu) / sizeof(float);
	    }
	    else if (trackName == TRACK_GAME) {
	        son = melody;
	        //currentMelodyLength = sizeof(melodyGame) / sizeof(float);
	    }
	    else if (trackName == TRACK_GAME_OVER) {
	    	son = melodyGameOver;
	    	//currentMelodyLength = sizeof(melodyGame) / sizeof(float);
	    }
	    currentNoteIndex = 0; // Recommence au début de la piste
*/
	// Change le pointeur vers la mélodie appropriée
	switch(trackName) {
	case TRACK_MENU:
		currentMelody = melodyMenu;
		break;
	case TRACK_GAME:
		currentMelody = melody;  // La mélodie Tetris
		break;
	case TRACK_GAME_OVER:
		currentMelody = melodyGameOver;
		break;
	default:
		currentMelody = melody;  // Fallback sécurisé
		break;
	}

	// Recommence au début de la nouvelle piste
	currentNoteIndex = 0;
	noteTimer = currentMelody[0].duration_ms;
	currentFrequency = currentMelody[0].frequency;
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
    // Appelé quand le timer6 lance une interruption toute les 1 millisecondes, mets à jour la machine à état
    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    	if(htim->Instance == TIM6){
    		if (g_SnakeAudio) g_SnakeAudio->UpdateState();
    	}
    }
}
