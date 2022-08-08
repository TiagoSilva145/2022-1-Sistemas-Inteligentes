#include "roboSalv.h"

RoboSalv::RoboSalv(int carg, float bat, float temp):
Robo(bat, temp)
{
    carga = carg;
    labirinto = nullptr;
}

RoboSalv::~RoboSalv()
{
    
}

void RoboSalv::imprimirDados()
{
    cout << "Robo Salvador dados: " << "bateria = " << bateria 
    << "; tempo = " << tempo << 
    "; carga = " << carga << endl;
}