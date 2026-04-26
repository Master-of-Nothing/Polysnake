#ifndef SNAKE_AUDIO_H
#define SNAKE_AUDIO_H

#include <cstdint>
#include "main.h"

// Configuration du système audio
constexpr uint32_t SAMPLE_RATE = 16000; // Fréquence d'échantillonnage (16 kHz) arbitraire
constexpr uint16_t TABLE_SIZE = 512;    // Taille de la période d'une onde
constexpr uint16_t BUFFER_SIZE = 256;   // Taille d'UN seul buffer (la moitié du tableau DMA)


// Structure représentant quel musique est joué en fonction de la machine à état
enum Track {
    TRACK_MENU,
    TRACK_GAME,
	TRACK_GAME_OVER
};

// Structure représentant une note de musique
struct Note {
    float frequency;     // Fréquence en Hertz
    uint32_t duration_ms;// Durée en millisecondes
};

class SnakeAudio {
public:
    enum Waveform { SINE, SQUARE, TRIANGLE };

    SnakeAudio();

    // Initialise la table d'onde avec la forme désirée
    void Init(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim,Waveform wave);

    // Démarre le DMA et la musique
    void Start();

    // Machine à états : à appeler chaque milliseconde dans une interruption Timer6
    void UpdateState();

    // Méthodes appelées par les interruptions DMA
    void ProcessHalfBuffer();
    void ProcessFullBuffer();

    // Méthode pour changer la musique en cours
    void setTrack(Track trackName);

    void UpdateVolumeFromADC(ADC_HandleTypeDef *hadc);

    //Fixe le volume directement (sans ADC). vol Valeur entre 0.0f (silence) et 1.0f (plein volume).
    void SetVolume(float vol);


    //Retourne le volume actuel (utile pour l'afficher à l'écran).
    float GetVolume() const;

private:
    DAC_HandleTypeDef *hdac = nullptr;
    TIM_HandleTypeDef *htim = nullptr;

    constexpr static float pi{3.1415926535f};

    uint16_t waveTable[TABLE_SIZE];          // Échantillons d'une période
    uint16_t dmaBuffer[BUFFER_SIZE * 2];     // Le double buffer continu (étape 1)

    float currentPhase;                      // Phase courante pour la continuité
    volatile float currentFrequency;         // Fréquence modifiée par la machine à états
    volatile float volume;

    uint32_t noteTimer;
    uint32_t currentNoteIndex;

    volatile bool isPlaying;
    const Note* currentMelody;

    // Fonctions internes d'initialisation des formes d'ondes
    void InitSineTable();
    void InitSquareTable();
    void InitTriangleTable();

    // Fonction centrale de remplissage mathématique (étape 4)
    void FillBuffer(uint16_t* buffer);
};

// Pointeur global pour faire le lien avec les callbacks C du HAL
extern SnakeAudio* g_SnakeAudio;

#endif // SNAKE_AUDIO_H
