#ifndef PROJEKT_3___ATSP_MATRIX_H
#define PROJEKT_3___ATSP_MATRIX_H


#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Matrix {
public:
    void initializeDistanceMatrix(int newDimension);

    void printDistanceMatrix();

    void loadATSPFile(const string& fileName);

    static int generateRandomNumber(int min, int max);

    static double generateRandomDoubleZeroOne();

    static double generateRandomDoubleInRange(double min, double max);

    vector<int> mutationSwapCity(vector<int> chromosome);
    vector<int> mutationInvertCities(vector<int> chromosome);

    vector<int> orderCrossover(vector<int> p1, vector<int> p2);

    void geneticAlgorithm(int stopSeconds, int populationSize, int mutationMethod, double mutationProbability, double crossoverProbability);

    int calculateLength(vector<int> chromosome);

private:
    const int INF = numeric_limits<int>::max();

    int dimension;

    vector<vector<int>> distanceMatrix;
};


#endif //PROJEKT_3___ATSP_MATRIX_H