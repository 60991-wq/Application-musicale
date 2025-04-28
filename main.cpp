#include "MainWindow.h"
#include "AudioGenerator.h"
#include "AudioParam.h"
#include <iostream>

int main() {
    // Crée un POD sécurisé
    LockedPOD sharedParams;

    // Crée le générateur audio
    AudioGenerator generator(sharedParams);
    generator.init();

    // Crée et lance la fenêtre principale
    MainWindow window(sharedParams);
    window.init();
    window.run();  // Cette fonction contient la boucle principale et ne retourne que lorsque l'utilisateur ferme la fenêtre

    std::cout << "Application terminée." << std::endl;
    return 0;
}