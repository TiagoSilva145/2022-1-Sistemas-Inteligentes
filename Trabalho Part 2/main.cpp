#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vitima.h"

using namespace std;

void imprime_vitimas(vector<Vitima>& vitimas)
{
    cout << "Vitimas: " << endl;
    for(long unsigned int i = 0; i < vitimas.size(); i++)
        cout << "Pos: " << vitimas[i].posx << "," << vitimas[i].posy << 
        " Tempo: " << vitimas[i].tempo << ", Gravidade:" << vitimas[i].gravidade << endl;
}

void incializa(int *tempo, vector<Vitima>& vitimas)
{
    ifstream arq;
    string linha;

    //-------------Inicializa o tempo do salvador
    arq.open("config.txt", ios::in);
    if(!arq)
    {
        cout << "Arquivo config nao encontrado";
        exit(1);
    }

    getline(arq,linha);
    getline(arq,linha);
    getline(arq,linha);
    getline(arq,linha);

    int i = static_cast<int>(linha.find('=')) + 1;
    int aux = 0;
    while (linha[i] >= '0' && linha[i] <= '9') // antigo == i < linha.size()
    {
        //cout << linha[i] - 48 << " ";
        aux *= 10;
        aux += linha[i] - 48;
        i++;
    }
    *tempo = aux;
    cout << "Tempo: " << *tempo << endl;
    arq.close();
    
    //-------------Inicializa o vetor de vitimas
    arq.open("ambiente.txt", ios::in);
    if(!arq)
    {
        cout << "Arquivo ambiente nao encontrado";
        exit(1);
    }

    getline(arq,linha);
    getline(arq,linha);

    i = 7;
    while((linha[i] >= '0' && linha[i] <= '9') || linha[i] == ',' || linha[i] == 32)
    {
        Vitima v;
        //Preenche o x
        while(linha[i] >= '0' && linha[i] <= '9')
        {
            v.posx *= 10;
            v.posx += linha[i] - 48;
            i++;
        }
        i++;
        //Preenche o y
        while(linha[i] >= '0' && linha[i] <= '9')
        {
            v.posy *= 10;
            v.posy += linha[i] - 48;
            i++;
        }
        vitimas.push_back(v);
        i++;
    }
    arq.close();

    //-------------Pega os valores da gravidade
    arq.open("sinaisvitais.txt", ios::in);
    if(!arq)
    {
        cout << "Arquivo sinais vitais nao encontrado";
        exit(1);
    }

    for(int x = 0; getline(arq,linha); x++)
    {
        i = 0;
        float aux = 0;

        while((linha[i] >= '0' && linha[i] <= '9') || linha[i] == '.' || linha[i] == 32)
        {
            aux = 0;
            int j = 10;
            while(linha[i] >= '0' && linha[i] <= '9')
            {
                aux *= 10;
                aux += linha[i] - 48;
                i++;
            }
            i++;
            while(linha[i] >= '0' && linha[i] <= '9')
            {
                aux += (float)(linha[i] - 48)/j;
                j *= 10;
                i++;
            }
            i++;
        }
        
        vitimas[x].gravidade = aux;
    }
    arq.close();

    //-------------Pega os valores do tempo
    arq.open("difacesso.txt", ios::in);
    if(!arq)
    {
        cout << "Arquivo dif acesso nao encontrado";
        exit(1);
    }

    for(int x = 0; getline(arq,linha); x++)
    {
        i = 0;
        float aux = 0;

        while((linha[i] >= '0' && linha[i] <= '9') || linha[i] == '.' || linha[i] == 32)
        {
            aux = 0;
            int j = 10;
            while(linha[i] >= '0' && linha[i] <= '9')
            {
                aux *= 10;
                aux += linha[i] - 48;
                i++;
            }
            i++;
            while(linha[i] >= '0' && linha[i] <= '9')
            {
                aux += (float)(linha[i] - 48)/j;
                j *= 10;
                i++;
            }
            i++;
        }
        
        vitimas[x].tempo = aux;
    }
    arq.close();
}

int main()
{
    int tempo = 0;
    vector<Vitima> vitimas;

    incializa(&tempo, vitimas);

    imprime_vitimas(vitimas);
}