#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <time.h> 

#include "vitima.h"
#include "genetico.h"
#include "tempera.h"

using namespace std;

void imprime_desempenho(vector<Vitima> v, vector<Vitima> s, int t)
{
    //cout << "----Desempenho----" << endl << endl;

    cout << "Valor acumulado das gravidades das vitimas salvas:\n";
    float soma = 0;
    for(int i = 0; i < s.size(); i++)
        soma += s[i].gravidade;
    cout << "Gravidade: " << soma << endl << endl;

    cout << "Numero de vitimas salvas pelo tempo gasto:\n";
    float tempo = 0;
    for(int i = 0; i < s.size(); i++)
        tempo += s[i].tempo;
    cout << "Tempo total: " << t << endl;
    cout << "Tempo gasto: " << tempo << endl;
    cout << "Total de vitimas: " << s.size() << endl;
    cout << "Vitimas por tempo gasto: " << s.size()/tempo << endl << endl;

    cout << "Numero de vítimas salvas em 5 extratos de gravidade pelo tempo gasto:\n";
    int s1 = 0;
    for(int i = 0; i < s.size(); i++)
        s1 += s[i].extrato_gravidade;
    int s2 = 0;
    for(int i = 0; i < v.size(); i++)
        s2 += v[i].extrato_gravidade;
    cout << "Total de extratos de gravidade de vitimas salvas: " << s1 << endl;
    cout << "Total de extratos de gravidade: " << s2 << endl;
    cout << "Extratos de gravidade pelo tempo gasto: " << s1/(tempo * s2) << endl << endl;


}

void imprime_vitimas(vector<Vitima> vitimas)
{
    cout << "Vitimas: " << endl;
    for(long unsigned int i = 0; i < vitimas.size(); i++)
        cout << "Pos: " << vitimas[i].posx << "," << vitimas[i].posy << 
        " Tempo: " << vitimas[i].tempo << ", Gravidade:" << vitimas[i].gravidade << 
        " Extrato de gravidade: " << vitimas[i].extrato_gravidade << endl;
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
    //cout << "Tempo: " << *tempo << endl;
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

        int extrato = 0;
        if(aux < 0.2)
            extrato = 1;
        else if(aux < 0.4)
            extrato = 2;
        else if(aux < 0.6)
            extrato = 3;
        else if(aux < 0.8)
            extrato = 4;
        else
            extrato = 5;
        vitimas[x].extrato_gravidade = extrato;

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
    Genetico algoritmo1 = Genetico(vitimas, tempo, 100, 6, 0.9, 0.04, Padrao);
    algoritmo1.estrategiaCruz = EstrategiaCruzamento::Padrao;

    Genetico algoritmo2 = Genetico(vitimas, tempo, 100, 6, 0.9, 0.04, Aleatorio);
    algoritmo2.estrategiaCruz = EstrategiaCruzamento::Aleatorio;


    double fitness1=0, fitness2=0, maior1=0, maior2=0;
    int vit1=0, vit2=0;
    vector<Vitima> solucao1, solucao2, melhor1, melhor2;
    for(int i=0; i < 2; i++) {
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
    srand (time(NULL));
    int tempo = 0;
    vector<Vitima> vitimas;

    incializa(&tempo, vitimas);
    Genetico gen = Genetico(vitimas, tempo, 200, 50, 0.9, 0.04, Padrao);
    gen.algoritmo_genetico();

    /*
    
    Tempera tempera = Tempera(vitimas, tempo, 10000);
    vector<Vitima> solucao = tempera.executar();
    cout << "----Desempenho Tempera----" << endl << endl;
    imprime_desempenho(vitimas, solucao, tempo);

    Genetico gen = Genetico(vitimas, tempo, 500, vitimas.size()*5, 0.9, 0.04, Padrao);
    solucao = gen.algoritmo_genetico();
    cout << "----Desempenho Genético - Cruzamento Simples----" << endl << endl;
    imprime_desempenho(vitimas, solucao, tempo);

    gen = Genetico(vitimas, tempo, 500, vitimas.size()*5, 0.9, 0.04, Aleatorio);
    solucao = gen.algoritmo_genetico();
    cout << "----Desempenho Genético - Cruzamento Aleatorio----" << endl << endl;
    imprime_desempenho(vitimas, solucao, tempo);
    */

    //vector<Vitima> solucao = comparar_algoritmos(vitimas, tempo);
    //imprime_vitimas(vitimas);
    
    // cout << endl << "Imprimindo solução" << endl;
    // cout << solucao.size() << " vitimas salvas" << endl;

    // imprime_vitimas(solucao);

}