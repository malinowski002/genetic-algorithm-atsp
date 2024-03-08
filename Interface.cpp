#include "Interface.h"
#include "Matrix.h"
#include <future>
#include <chrono>

void Interface::menu() {

    Matrix matrix;

    char option;
    string filename;
    int stopSeconds = 120;
    int populationSize = 1000;
    double mutationFactor = 0.01;
    double crossoverFactor = 0.8;
    int mutationMethod = 0;

    do {
        cout << endl << "-----MENU GLOWNE-----\n";
        cout << "[1] Wczytanie danych z pliku\n";
        cout << "[2] Wyświetlenie wczytanych danych\n";
        cout << "[3] Wprowadzenie kryterium stopu\n";
        cout << "[4] Ustawienie wielkości populacji początkowej\n";
        cout << "[5] Ustawienie współczynnika mutacji\n";
        cout << "[6] Ustawienia współczynnika krzyżowania\n";
        cout << "[7] Wybór metody mutacji\n";
        cout << "[8] Uruchomienie algorytmu genetycznego\n";
        cout << "[0] Wyjście\n";

        cout << "Podaj opcje: ";
        cin >> option;

        switch (option) {
            case '0': {
                cout << "\nWyjście\n";
                break;
            }
            case '1': {
                cout << "\nOpcja 1\n";
                cout << "Podaj nazwe pliku z rozszerzeniem (np. .atsp): ";
                cin >> filename;
                matrix.loadATSPFile(filename);
                break;
            }
            case '2': {
                cout << "\nOpcja 2\n";
                matrix.printDistanceMatrix();
                break;
            }
            case '3': {
                cout << "\nOpcja 3\n";
                // wprowadzenie kryterium stopu
                cout << "Podaj kryterium stopu (w sekundach): ";
                cin >> stopSeconds;
                break;
            }
            case '4': {
                cout << "\nOpcja 4\n";
                // ustawienie wielkości populacji początkowej
                cout << "Podaj rozmiar populacji początkowej: ";
                cin >> populationSize;
                break;
            }
            case '5': {
                cout << "\nOpcja 5\n";
                // ustawienie współczynnika mutacji
                cout << "Podaj współczynnik mutacji: ";
                cin >> mutationFactor;
                break;
            }
            case '6': {
                cout << "\nOpcja 6\n";
                // ustawienia współczynnika krzyżowania
                cout << "Podaj współczynnik krzyżowania: ";
                cin >> crossoverFactor;
                break;
            }
            case '7': {
                cout << "\nOpcja 7\n";
                // wybór metody mutacji
                cout << "Wybierz metodę mutacji (0-swap, 1-invert): ";
                cin >> mutationMethod;
                break;
            }
            case '8': {
                cout << "\nOpcja 8\n";
                // uruchomienie algorytmu genetycznego
                matrix.geneticAlgorithm(stopSeconds, populationSize, mutationMethod, mutationFactor, crossoverFactor);
                break;
            }
            default: {
                cout << "\nPodana opcja nie istnieje!\n";
                break;
            }
        }
    } while (option != '0');
}
