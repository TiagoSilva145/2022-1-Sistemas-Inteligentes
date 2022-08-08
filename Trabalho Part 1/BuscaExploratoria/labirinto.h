#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include "robo.h"
using namespace std;

class Percepcao;

class Labirinto
{
    private:

    vector<vector<int>> labirinto;
    int vitimaCount;
    vector<vector<float>> vitais;

    public:

    Labirinto(int x, int y);
    ~Labirinto();

    void imprimeLabirinto();
    void PreencheLabirinto();
    void preencheSinaisVitais();
    void imprimeSinaisVitais();

    //Ações

    Percepcao moverNorte(Robo* robo);
    Percepcao moverSul(Robo* robo);
    Percepcao moverLeste(Robo* robo);
    Percepcao moverOeste(Robo* robo);
    Percepcao moverNordeste(Robo* robo);
    Percepcao moverNoroeste(Robo* robo);
    Percepcao moverSudeste(Robo* robo);
    Percepcao moverSudoeste(Robo* robo);
    Percepcao pegarVitima(Robo* robo);
    vector<int>* LerSinaisVitais(Robo* robo);

    bool validaAcao(Robo* robo, int posx, int posy); 
    Percepcao mover(Robo* robo, int movX, int movY);
};