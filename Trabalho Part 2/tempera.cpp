#include "tempera.h"

Tempera::Tempera(vector<Vitima> vit, int tempo, int iter=200) {
    vitimas = vit;
    this->tempo = tempo;
    iteracoes = iter;
    temp_inicial = 0;
    temp_final = 100;
}

vector<Vitima> Tempera::executar() {
    //preenche solução com tudo false
    vector<bool> solucao;
    vector<bool> melhor_solucao;
    solucao.reserve(vitimas.size());
    int i;
    for(i=0; i < vitimas.size(); i++) {
        solucao.push_back(false);
    }
    
    double melhor_fitness=0, prob;
    double fitness_ant = calcula_fitness(solucao), fitness_atual;
    int temperatura;
    for(i=0; i < iteracoes; i++) {
        int temperatura = schedule(i);
        vector<bool> sucessor = escolhe_acao(solucao);

        if(fitness_ant > melhor_fitness) {
            //cout << "iteracao " << i << " fitness = " << fitness_ant << endl;
            melhor_solucao = solucao;
            melhor_fitness = fitness_ant;
        }

        fitness_atual = calcula_fitness(sucessor);
        if(fitness_atual > fitness_ant) {
            solucao = sucessor;
            fitness_ant = fitness_atual;
        }
        else {
            //calcula probabilidade de pegar a solução ruim
            if(temperatura == 0) {
                solucao = sucessor;
                fitness_ant = fitness_atual;
                continue;
            }

            prob = (fitness_atual - fitness_ant)/temperatura;
            //diminui a chance de escolher a solução se estourar o tempo
            if(fitness_atual < 0)
                prob = (fitness_atual - (fitness_ant*temperatura/2))/temperatura;
            prob = exp(prob);
            int random_num = rand() % 10001;
            if(random_num < (int)(prob * 10000)) {
                solucao = sucessor;
                fitness_ant = fitness_atual;
            }
        }
        
    }

    return to_vitima(melhor_solucao);
}

//calcula temperatura
int Tempera::schedule(int tempo) {
    double razao = (double)(tempo-iteracoes)/(double)(iteracoes);
    razao = pow(razao, 2);
    return (((double)temp_final) + ((double)(temp_inicial - temp_final))*razao);
}

double Tempera::calcula_fitness(vector<bool> individuo) {
    double fitness = 0.1;
    int tempo_restante = tempo;
    for(int i = 0; i < individuo.size(); i++) {
        if(individuo[i]) {
            fitness += 1 + (vitimas[i].gravidade / 10);
            tempo_restante -= vitimas[i].tempo;
        }
    }
    if(tempo_restante < 0) {
        fitness = -(fitness / 20);
    }

    return fitness;
}

//aleatoriamente da toggle em uma posicao do vetor
vector<bool> Tempera::escolhe_acao(vector<bool> inicial) {
    vector<bool> resultado;
    resultado.reserve(inicial.size());
    for(int i=0; i < inicial.size(); i++) {
        resultado.push_back(inicial[i]);
    }

    int posicao_aleatoria = rand() % inicial.size();
    resultado[posicao_aleatoria] = !resultado[posicao_aleatoria];
    return resultado;
}

vector<Vitima> Tempera::to_vitima(vector<bool> ind) {
    vector<Vitima> solucao;
    for(int i=0; i < ind.size(); i++) {
        if(ind[i]) {
            solucao.push_back(vitimas[i]);
        }
    }

    return solucao;
}