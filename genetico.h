#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "vitima.h"

using namespace std;

enum EstrategiaCruzamento {
    //Cruza dois individuos a partir de um pivô
    Padrao = 1,
    //Cruza indivíduos com 50% de chance de trocar cada gene
    Aleatorio = 2
};

class Genetico {
    private:
        vector<Vitima> vitimas;
        int tempo; 

    public:
        int iteracoes; 
        int num_individuos;
        float crossover; 
        float mutacao; 
        EstrategiaCruzamento estrategiaCruz;
        double melhor_fitness;

    Genetico(const vector<Vitima> vit, int tempo, 
    int iteracoes, int num_ind, float crossover, float mutacao);

    vector<Vitima> algoritmo_genetico();

    private:
        vector<vector<bool>> preenche_individuos_iniciais(int qtd_individuos);

        pair<double, vector<bool>> selecionar_individuo_probabilidade(list<pair<double, vector<bool>>> &ind, double &soma_fit);

        vector<double> fitness_individuos(vector<vector<bool>> individuos);

        double calcula_fitness(vector<bool> individuo);

        vector<pair<vector<bool>, vector<bool>>> seleciona_individuos(vector<vector<bool>> indivuduos, 
        vector<double> fitness);

        vector<vector<bool>> cruza_individuos(vector<pair<vector<bool>, vector<bool>>> cruzamento);

        pair<vector<bool>, vector<bool>> cruzar_padrao(vector<bool> ind1, vector<bool> ind2);
        pair<vector<bool>, vector<bool>> cruzar_aleatorio(vector<bool> ind1, vector<bool> ind2);

        void mutar_individuo(pair<vector<bool>, vector<bool>> *filhos);

        pair<double, vector<bool>> melhor_individuo(vector<double> fitness, vector<vector<bool>> individuos);

        vector<Vitima> to_vitima(vector<bool> ind);
};