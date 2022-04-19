#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h> 

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

vector<Vitima> algoritmo_genetico(const vector<Vitima> vitimas, int iteracoes, int num_individuos, 
float crossover, float mutacao, int tempo);

int main()
{
    int tempo = 0;
    vector<Vitima> vitimas;

    srand (time(NULL));

    incializa(&tempo, vitimas);

    vector<Vitima> solucao = algoritmo_genetico(vitimas, 3000, 300, 0.9, 0.05, tempo);

    imprime_vitimas(vitimas);

    
    cout << endl << "Imprimindo solução" << endl;
    cout << solucao.size() << " vitimas salvas" << endl;

    imprime_vitimas(solucao);
}

//Algoritmo genérico começa aqui

vector<vector<bool>> preenche_individuos_iniciais(int num_individuos, int num_vitimas);

pair<double, vector<bool>> selecionar_individuo_probabilidade(list<pair<double, vector<bool>>> &ind, double &soma_fit);

vector<double> fitness_individuos(vector<vector<bool>> individuos, vector<Vitima> vitimas, int tempo);

double calcula_fitness(vector<bool> individuo, vector<Vitima> vitimas, int tempo);

vector<pair<vector<bool>, vector<bool>>> seleciona_individuos(vector<vector<bool>> indivuduos, 
vector<double> fitness, int num_individuos);

vector<vector<bool>> cruza_individuos(vector<pair<vector<bool>, vector<bool>>> cruzamento, 
float crossover, float mutacao);

pair<vector<bool>, vector<bool>> cruzar(vector<bool> ind1, vector<bool> ind2);

void mutar_individuo(pair<vector<bool>, vector<bool>> *filhos, float mutacao);

pair<double, vector<bool>> melhor_individuo(vector<double> fitness, vector<vector<bool>> individuos);

vector<Vitima> to_vitima(vector<bool> ind, vector<Vitima> vitimas);

vector<Vitima> algoritmo_genetico(const vector<Vitima> vitimas, int iteracoes, int num_individuos, 
float crossover, float mutacao, int tempo) {
    vector<vector<bool>> individuos = preenche_individuos_iniciais(num_individuos*2, vitimas.size());
    
    pair<double, vector<bool>> melhor;
    double maior_fitness = 0;
    int i;
    for(i=0; i < iteracoes; i++) {
        vector<double> fitness = fitness_individuos(individuos, vitimas, tempo);

        melhor = melhor_individuo(fitness, individuos);

        //imprime se mudar o maior para não floodar a tela
        if(melhor.first > maior_fitness) {
            cout << "fitness melhor individuo geração " << i << " = " << melhor.first << endl;
            maior_fitness = melhor.first;
        }

        vector<pair<vector<bool>, vector<bool>>> cruzamento = seleciona_individuos(individuos, fitness, num_individuos);

        individuos = cruza_individuos(cruzamento, crossover, mutacao);
    }
    
    return to_vitima(melhor.second, vitimas);
}

vector<Vitima> to_vitima(vector<bool> ind, vector<Vitima> vitimas) {
    vector<Vitima> solucao;
    for(int i=0; i < ind.size(); i++) {
        if(ind[i]) {
            solucao.push_back(vitimas[i]);
        }
    }

    return solucao;
}

pair<double, vector<bool>> melhor_individuo(vector<double> fitness, vector<vector<bool>> individuos) {
    double maior=-1;
    vector<bool> melhor;
    for(int i=0; i < individuos.size(); i++) {
        if(fitness[i] > maior) {
            maior = fitness[i];
            melhor = individuos[i];
        }
    }

    return pair<double, vector<bool>>(maior, melhor);
}

//Gera individuos aleatórios
vector<vector<bool>> preenche_individuos_iniciais(int num_individuos, int num_vitimas) {
    vector<vector<bool>> individuos;
    individuos.reserve(num_individuos);

    int i, j;
    for(i=0; i < num_individuos; i++) {
        vector<bool> aux;
        aux.reserve(num_vitimas);
        for(j=0; j < num_vitimas; j++) {
            //preenche o vetor aleatoriamente com true ou false
            bool x = (rand() % 2) == 1 ? true : false;
            aux.push_back(x);
        }
        individuos.push_back(aux);
    }

    return individuos;
}

vector<double> fitness_individuos(vector<vector<bool>> individuos, vector<Vitima> vitimas, int tempo) {
    vector<double> fitness;
    fitness.reserve(individuos.size());

    int i;
    for(i=0; i < individuos.size(); i++) {
        fitness.push_back(calcula_fitness(individuos[i], vitimas, tempo));
    }

    return fitness;
}

//fitness valoriza maximizar o número de vítimas e o número da gravidade 
//desvaloriza muito se passou do limite de tempo
double calcula_fitness(vector<bool> individuo, vector<Vitima> vitimas, int tempo) {
    double fitness = 0.1;
    for(int i = 0; i < individuo.size(); i++) {
        if(individuo[i]) {
            fitness += 1 + vitimas[i].gravidade;
            tempo -= vitimas[i].tempo;
        }
    }
    if(tempo < 0) {
        fitness = fitness / 20;
    }

    return fitness;
}

vector<pair<vector<bool>, vector<bool>>> seleciona_individuos(vector<vector<bool>> indivuduos, 
vector<double> fitness, int num_individuos) {
    vector<pair<vector<bool>, vector<bool>>> cruzamento;
    cruzamento.reserve((int)(indivuduos.size()/2));
    
    //soma todos os fitness para fazer as probabilidades
    double soma_fit=0;
    for(int i=0; i < fitness.size(); i++) {
        soma_fit += fitness[i];
    }

    //converte vetor em uma lista
    list<pair<double, vector<bool>>> ind;
    for(int i=0; i < indivuduos.size(); i++) {
        ind.push_back(pair<double, vector<bool>>(fitness[i], indivuduos[i]));
    }

    pair<double, vector<bool>> selecionado1, selecionado2;
    for(int i=0; i < (int)num_individuos/2; i++) {
        
        selecionado1 = selecionar_individuo_probabilidade(ind, soma_fit);
        selecionado2 = selecionar_individuo_probabilidade(ind, soma_fit);

        cruzamento.push_back(pair<vector<bool>, vector<bool>>(selecionado1.second, selecionado2.second));
    }

    return cruzamento;
}

pair<double, vector<bool>> selecionar_individuo_probabilidade(list<pair<double, vector<bool>>> &ind, double &soma_fit) {
    //Seleciona um individuo pela probabilidade
    int random = rand() % 10001;
    int interv_inicial=0, interv_final=0;
    list<pair<double, vector<bool>>>::iterator iter;
    pair<double, vector<bool>> selecionado = (*ind.begin());

    for(iter=ind.begin(); iter != ind.end(); ++iter) {
        
        interv_final = ceil(((iter->first / soma_fit) * 10000)) + interv_inicial;

        if((random >= interv_inicial && random < interv_final) || (random > 10000)) {
            selecionado = *iter;
            ind.erase(iter);
            soma_fit -= iter->first;
            break;
        }
        interv_inicial = interv_final;
    }

    return selecionado;
}

vector<vector<bool>> cruza_individuos(vector<pair<vector<bool>, vector<bool>>> cruzamento, 
float crossover, float mutacao) {
    vector<vector<bool>> individuos;

    for(int i=0; i < cruzamento.size(); i++) {
        individuos.push_back(cruzamento[i].first);
        individuos.push_back(cruzamento[i].second);

        //Testa se individuos vao cruzar
        if((rand() % 100) > ceil(crossover * 100)) {
            pair<vector<bool>, vector<bool>> filhos = cruzar(cruzamento[i].first, cruzamento[i].second);
            mutar_individuo(&filhos, mutacao);
            individuos.push_back(filhos.first);
            individuos.push_back(filhos.second);
        }
    }

    return individuos;
}
//cruza dois individuos gerando dois filhos
pair<vector<bool>, vector<bool>> cruzar(vector<bool> ind1, vector<bool> ind2) {
    int pivo = rand() % ind1.size();
    pair<vector<bool>, vector<bool>> filhos;
    for(int i=0; i < ind1.size(); i++) {
      if(i < pivo) {
          filhos.first.push_back(ind1[i]);
          filhos.second.push_back(ind2[i]);
      }
      else {
          filhos.first.push_back(ind2[i]);
          filhos.second.push_back(ind1[i]);
      }  
    };

    return filhos;
}
//Testa para cada gene se vai ter mutação
void mutar_individuo(pair<vector<bool>, vector<bool>> *filhos, float mutacao) {
    for(int i=0; i < filhos->first.size(); i++) {
        if((rand() % 100) > ceil(mutacao * 100)) {
            filhos->first[i] = !(filhos->first[i]);
        }
        if((rand() % 100) > ceil(mutacao * 100)) {
            filhos->second[i] = !(filhos->second[i]);
        }
    }
}