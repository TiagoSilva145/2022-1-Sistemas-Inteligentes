#include "vitima.h"
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <iostream>

using namespace std;

class Tempera {
public:
    vector<Vitima> vitimas;
    int iteracoes;
    int tempo;
    int temp_inicial;
    int temp_final;

    Tempera(vector<Vitima> vit, int tempo, int iter);
    vector<Vitima> executar();

private:
    int schedule(int tempo);
    double calcula_fitness(vector<bool> individuo);
    vector<bool> escolhe_acao(vector<bool> inicial);
    vector<Vitima> to_vitima(vector<bool> ind);
};