#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h> 

#include "vitima.h"
#include "genetico.h"

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

vector<Vitima> comparar_algoritmos(vector<Vitima> vitimas, int tempo) {
    Genetico algoritmo1 = Genetico(vitimas, tempo, 1000, 300, 0.9, 0.04);
    algoritmo1.estrategiaCruz = EstrategiaCruzamento::Padrao;

    Genetico algoritmo2 = Genetico(vitimas, tempo, 1000, 300, 0.9, 0.04);
    algoritmo2.estrategiaCruz = EstrategiaCruzamento::Aleatorio;


    double fitness1=0, fitness2=0, maior1=0, maior2=0;
    int vit1=0, vit2=0;
    vector<Vitima> solucao1, solucao2, melhor1, melhor2;
    for(int i=0; i < 100; i++) {
        solucao1 = algoritmo1.algoritmo_genetico();
        solucao2 = algoritmo2.algoritmo_genetico();

        fitness1 = algoritmo1.melhor_fitness;
        fitness2 = algoritmo2.melhor_fitness;

        cout << "Resultado 1, 2 = " << fitness1 << " " << fitness2 << endl;
        if(fitness1 > fitness2) {
            cout << "alg 1 ganhou" << endl;
            vit1++;
        }
        else if(fitness1 < fitness2) {
            cout << "alg 2 ganhou" << endl;
            vit2++;
        }
        else {
            cout << "empate" << endl;
        }

        if(fitness1 > maior1) {
            maior1 = fitness1;
            melhor1 = solucao1;
        }
        if(fitness2 > maior2) {
            maior2 = fitness2;
            melhor2 = solucao2;
        }
    }

    cout << "vitorias 1: " << vit1 << endl;
    cout << "vitorias 2: " << vit2 << endl; 
    cout << "max 1: " << maior1 << endl;
    cout << "max 2: " << maior2 << endl;

    if(maior1 > maior2) {
        return melhor1;
    }
    else {
        return melhor2;
    }
}

int main()
{
    int tempo = 0;
    vector<Vitima> vitimas;

    srand (time(NULL));

    incializa(&tempo, vitimas);
    
    //vector<Vitima> solucao = algoritmo.algoritmo_genetico();
    vector<Vitima> solucao = comparar_algoritmos(vitimas, tempo);
    imprime_vitimas(vitimas);

    
    cout << endl << "Imprimindo solução" << endl;
    cout << solucao.size() << " vitimas salvas" << endl;

    imprime_vitimas(solucao);
}