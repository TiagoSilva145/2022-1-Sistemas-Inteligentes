#include "labirinto.h"
#include "roboSalv.h"
#include "roboVasc.h"

#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void inicializarLabirinto(RoboVasc** v, RoboSalv** s, Labirinto** l)
{
    ifstream arq;
    arq.open("config.txt", ios::in);
    string linha;

    if(!arq)
    {
        cout << "Arquivo nao encontrado";
        exit;
    }

    vector<int> dados;

    while(getline(arq,linha))
    {
        int i = static_cast<int>(linha.find('='))+ 1;
        int aux = 0;
        while(i < linha.size())
        {
            aux *= 10;
            aux += linha[i]-48;
            i++;
        }
        dados.push_back(aux);
    }

    *l = new Labirinto(dados[0], dados[1]);

    *v = new RoboVasc(dados[0], dados[1], dados[4], dados[2], *l);

    *s = new RoboSalv(dados[6], dados[5], dados[3]);

}

int main()
{
    using namespace std::this_thread;
    using namespace std::chrono;

    RoboVasc* vasc;
    RoboSalv* salv;
    Labirinto* lab;

    inicializarLabirinto(&vasc, &salv, &lab);

    vasc->imprimirDados();
    salv->imprimirDados();
    lab->imprimeLabirinto();

    while(vasc->completou == false) {
        system("clear");
        vasc->deliberar();
        lab->imprimeLabirinto();
        vasc->imprimirMapa();
        sleep_for(milliseconds(300));
    }
    
    return 0;
}

