#include "AudioGenerator.h"
#include "AudioParam.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Crée un POD sécurisé
    LockedPOD sharedParams;

    // Crée le générateur audio
    AudioGenerator generator(sharedParams);
    generator.init();

    // --- Simulation d'une note appuyée ---
    {
        POD pod = sharedParams.getCopy();
        pod.activeNote = true;   // Appuie sur une note
        pod.noteIndex = 0;       // C4
        sharedParams.setCopy(pod);
    }

    std::cout << "Appui sur une note... (attendre 5 secondes)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5)); // On laisse jouer 5s

    // --- Simulation d'une note relâchée ---
    {
        POD pod = sharedParams.getCopy();
        pod.activeNote = false;  // Relâchement
        sharedParams.setCopy(pod);
    }

    std::cout << "Note relâchée." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Attendre encore 2s

    std::cout << "Fin du test." << std::endl;

    return 0;
}
