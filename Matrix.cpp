#include <fstream>
#include <algorithm>
#include <iomanip>
#include <random>
#include "Matrix.h"

void Matrix::initializeDistanceMatrix(int newDimension) {
    // Uaktualnienie rozmiaru macierzy odległości
    dimension = newDimension;

    // Inicjalizacja macierzy o odpowiednich wymiarach
    distanceMatrix = vector<vector<int>>(dimension, vector<int>(dimension));
}

// Funkcja wyświetlająca zawartość macierzy odległości
void Matrix::printDistanceMatrix() {
    // Ustaw szerokość wyświetlanego pola na
    int fieldWidth = 4;

    for (const auto& row: distanceMatrix) {
        for (const auto& distance: row) {
            cout << setw(fieldWidth) << distance;
        }
        cout << endl;
    }
}

// Funkcja generująca liczby losowe z zadanego przedziału
int Matrix::generateRandomNumber(int min, int max) {
    random_device rd;

    // Inicjalizacja generatora liczb losowych
    mt19937 generator(rd());

    // Definicja rozkładu prawdopodobieństwa
    uniform_int_distribution<> distribution(min,max);

    return distribution(generator);
}

// Funkcja generująca liczby losowe między 0 - 1
double Matrix::generateRandomDoubleZeroOne(){
    random_device rd;

    // Inicjalizacja generatora liczb losowych
    mt19937 generator(rd());

    // Definicja rozkładu prawdopodobieństwa
    uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

// Funkcja generująca liczby losowe w przedziale
double Matrix::generateRandomDoubleInRange(double min, double max){
    random_device rd;

    // Inicjalizacja generatora liczb losowych
    mt19937 generator(rd());

    // Definicja rozkładu prawdopodobieństwa
    uniform_real_distribution<double> distribution(min, max);

    return distribution(generator);
}

void Matrix::loadATSPFile(const string& fileName) {
    // Otwórz plik do odczytu
    string filename = "../" + fileName;
    //string filename = fileName;
    ifstream file;
    file.open(filename, ifstream::in);
    bool fileLoaded = false;

    if (file.is_open()) {
        string line;

        // Sprawdź czy plik jest typu ATSP
        bool isATSP = false;
        while (getline(file, line) && !fileLoaded) {
            if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
                isATSP = true;
                break;
            }
        }

        file.clear();
        file.seekg(0);

        if (isATSP) {
            // Plik typu ATSP
            while (getline(file, line)) {
                // Sprawdź czy w obecnej linii znajduje się info o wymiarach
                if (line.find("DIMENSION: ") != std::string::npos) {
                    fileLoaded = true;
                    size_t pos = line.find(": ");
                    initializeDistanceMatrix(stoi(line.substr(pos + 1)));
                } else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
                    // Wczytywanie odległości do macierzy
                    for (int i = 0; i < dimension; i++) {
                        for (int j = 0; j < dimension; j++) {
                            file >> distanceMatrix[i][j];
                            if (i == j) {
                                // Ustawianie -1 na przekątnej
                                distanceMatrix[i][j] = -1;
                            }
                        }
                    }
                }
            }
        } else {
            // Standardowy plik
            file >> dimension;
            initializeDistanceMatrix(dimension);  // Inicjalizacja macierzy odległości

            for (int i = 0; i < dimension; i++) {
                for (int j = 0; j < dimension; j++) {
                    file >> distanceMatrix[i][j];
                    if (i == j) {
                        distanceMatrix[i][j] = -1;
                    }
                }
            }
            fileLoaded = true;
        }

        if (!fileLoaded) {
            cerr << "BŁĄD: niewspierany format pliku." << endl;
        }
    } else {
        cerr << "BŁĄD podczas otwierania pliku." << endl;
    }

    file.close();
}

int Matrix::calculateLength(vector<int> chromosome) {
    int value = 0;
    for (int i = 0; i < dimension - 1; i++) {
        value += distanceMatrix[chromosome[i]][chromosome[i + 1]];
    }
    // Koszt powrotu do miasta początkowego
    value += distanceMatrix[chromosome[dimension - 1]][chromosome[0]];
    return value;
}

vector<int> Matrix::mutationSwapCity(vector<int> chromosome){
    vector<int> newChromosome = chromosome;
    int indexToMove1 = generateRandomNumber(1, chromosome.size()-2);
    int indexToMove2 = generateRandomNumber(1, chromosome.size()-2);
    while(indexToMove2 == indexToMove1){
        indexToMove2 = generateRandomNumber(1, chromosome.size()-2);
    }
    int valueToMove = newChromosome[indexToMove1];

    newChromosome.erase(newChromosome.begin() + indexToMove1);
    newChromosome.insert(newChromosome.begin() + indexToMove2, valueToMove);

    return newChromosome;
}

vector<int> Matrix::mutationInvertCities(vector<int> chromosome) {
    vector<int> newChromosome = chromosome;

    // Wygeneruj dwa różne losowe indeksy
    int index1 = generateRandomNumber(1, chromosome.size() - 2);
    int index2 = generateRandomNumber(1, chromosome.size() - 2);
    while (index2 == index1) {
        index2 = generateRandomNumber(1, chromosome.size() - 2);
    }

    // Zamień kolejność miast między dwoma wybranymi indeksami
    if (index1 > index2) {
        swap(index1, index2);
    }
    reverse(newChromosome.begin() + index1, newChromosome.begin() + index2 + 1);

    return newChromosome;
}

vector<int> Matrix::orderCrossover(vector<int> p1, vector<int> p2){
    // Utworzenie wektoru dla dziecka
    vector<int> child(p1.size(), -1);

    // Określenie zakresu kopiowanej sekcji
    int rand1 = generateRandomNumber(0, child.size() - 1);
    int rand2 = generateRandomNumber(0, child.size() - 1);
    while((abs(rand1-rand2) == p1.size() - 1) or (rand1 == rand2)){
        rand2 = generateRandomNumber(0, child.size() - 1);
    }
    int start = min(rand1, rand2);
    int end = max(rand1, rand2);
    // cout << "\nstart=" << start << " end=" << end << endl;

    // Kopiowanie sekcji między start a end
    for (int i = start; i < end; ++i) {
        child[i] = p1[i];
    }

    int offset = 0;

    // Iterowanie po nieskopiowanej z p1 reszcie dziecka
    for (int i = end; i < end + p1.size() - (end - start); ++i) {
        int index = (i % p1.size());

        // Dopóki element jest już w skopiowanym fragmencie to przesuwaj się w p2 dalej
        while(find(child.begin()+start, child.begin()+end, p2[(index + offset) % p1.size()]) != child.begin()+end) {
            offset += 1;
        }

        // Dodawanie elementu z p2 do dziecka
        child[index] = p2[(index + offset) % p1.size()];
    }

    return child;
}

void Matrix::geneticAlgorithm(int stopSeconds, int populationSize, int mutationMethod, double mutationProbability, double crossoverProbability) {
    auto startTime = chrono::high_resolution_clock::now();
    auto stopTime = startTime + chrono::seconds(stopSeconds);

    // Wypisanie obecnej konfiguracji algorytmu
    cout << "Czas trwania: " << stopSeconds << endl;
    cout << "Rozmiar populacji: " << populationSize << endl;
    if(mutationMethod == 0){
        cout << "Metoda mutacji: swap" << endl;
    } else {
        cout << "Metoda mutacji: invert" << endl;
    }
    cout << "Wsp. mutacji: " << mutationProbability << endl;
    cout << "Wsp. krzyżowania: " << crossoverProbability << endl;


    // Losowanie początkowej populacji
    vector<tuple<vector<int>, int, double, double>> chromosomes;
    for (int i = 0; i < populationSize; i++) {
        vector<int> citiesOrder(this->dimension);
        for (int j = 0; j < this->dimension; j++) {
            citiesOrder[j] = j;
        }
        for (int j = this->dimension - 1; j > 0; j--) {
            int randomIndex = generateRandomNumber(0, j);
            swap(citiesOrder[j], citiesOrder[randomIndex]);
        }
        int chromosomeLength = calculateLength(citiesOrder);
        double currFit = 1.0 / chromosomeLength;
        chromosomes.push_back(make_tuple(citiesOrder, chromosomeLength, currFit, 0.0));
    }

    //for (int generation = 0; generation < numGenerations; generation++) {
    while(chrono::high_resolution_clock::now() < stopTime){
        // Ocena przystosowania
        double prevFit = 0.0;
        double fitSum = 0.0;
        double maxFit = 0.0;
        for (auto& chromosome : chromosomes) {
            double currFit = 1.0 / get<1>(chromosome);
            get<2>(chromosome) = currFit;
            get<3>(chromosome) = fitSum + prevFit;
            fitSum += prevFit;
            maxFit += currFit;
            prevFit = 1.0 / get<1>(chromosome);
        }

        // Selekcja przy użyciu metody ruletki
        vector<tuple<vector<int>, int, double, double>> newChromosomes;
        for (int i = 0; i < populationSize * 1; i++) {
            double rand = generateRandomDoubleInRange(0.0, maxFit);

            int chosenIndex = 0;
            int counter = 0;
            for (auto& chromosome : chromosomes) {
                if (rand > get<3>(chromosome)) {
                    chosenIndex = counter;
                }
                counter++;
            }
            newChromosomes.push_back(chromosomes[chosenIndex]);
        }

        // Krzyżówki i mutacje wśród nowowybranych osobników
        vector<vector<int>> crossedChromosomes;
        for (int i = 0; i < newChromosomes.size() - 1; i++) {
            vector<int> parent1 = get<0>(newChromosomes[i]);
            vector<int> parent2 = get<0>(newChromosomes[i + 1]);
            vector<int> child;

            // Krzyżówka
            if (generateRandomDoubleZeroOne() < crossoverProbability) {
                child = orderCrossover(parent1, parent2);
            } else {
                child = parent1;
            }

            // Mutacja
            if (generateRandomDoubleZeroOne() < mutationProbability) {
                // 0 - swap
                // 1 - invert
                if(mutationMethod == 0){
                    child = mutationSwapCity(child);
                } else if (mutationMethod == 1) {
                    child = mutationInvertCities(child);
                }
            }

            crossedChromosomes.push_back(child);
        }

        // Utworzenie nowej populacji
        vector<tuple<vector<int>, int, double, double>> newPopulation = chromosomes;
        for (auto& chromosome : crossedChromosomes) {
            int length = calculateLength(chromosome);
            double fitness = 1.0 / length;
            double fitSum2 = (newPopulation.empty() ? 0.0 : get<3>(newPopulation.back())) + fitness;
            newPopulation.emplace_back(chromosome, length, fitness, fitSum2);
        }

        // Sortowanie populacji według długości chromosomów
        sort(newPopulation.begin(), newPopulation.end(),
             [](const auto& a, const auto& b) {
                 return get<1>(a) < get<1>(b);
             });

        // Odrzucenie najsłabszych osobników
        newPopulation.resize(populationSize);

        // Wypisywanie najlepszego osobnika w bieżącej populacji
        auto bestChromosome = newPopulation.front();
        // cout << get<1>(bestChromosome) << endl;

        // Aktualizacja populacji
        chromosomes = newPopulation;
    }
    cout << "\n" << get<1>(chromosomes.front()) << " - ";
    for (auto city : get<0>(chromosomes.front())) {
        cout << city << " ";
    }
}





