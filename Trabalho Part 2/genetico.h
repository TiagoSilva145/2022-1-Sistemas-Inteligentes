#pragma once
#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "vitima.h"

#include "individuo.h"

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
    int iteracoes, int num_ind, float crossover, float mutacao, EstrategiaCruzamento e);

    vector<Vitima> algoritmo_genetico();

    private:
        vector<Individuo> preenche_individuos_iniciais(int qtd_individuos);
        
        double calcula_fitness(Individuo individuo);

        void cruzamento(vector<Individuo>& geracao);

        int seleciona_individuo(vector<Individuo> geracao, int tam);

        void cruzar(int i1, int i2, vector<Individuo>& v_individuos);

        vector<Vitima> to_vitima(Individuo melhor);

        //bool comparar(Individuo i1, Individuo i2);

        //-----------------

};