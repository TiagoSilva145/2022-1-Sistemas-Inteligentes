#pragma once
#include "robo.h"

class RoboSalv: public Robo
{
    private:
    int carga;
    vector<vector<int>> *labirinto;

    public:

    RoboSalv(int carg, float bat, float temp);
    ~RoboSalv();

    void imprimirDados();
};