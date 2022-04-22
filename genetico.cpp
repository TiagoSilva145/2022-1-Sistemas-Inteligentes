#include "genetico.h"

Genetico::Genetico(const vector<Vitima> vit, int tempo, 
    int iteracoes=300, int num_ind=300, float crossover=0.9, float mutacao=0.04) {
        this->vitimas = vit;
        this->iteracoes = iteracoes;
        this->num_individuos = num_ind;
        this->crossover = crossover;
        this->mutacao = mutacao;
        this->tempo = tempo;
        this-> estrategiaCruz = EstrategiaCruzamento::Padrao;

        if(num_ind < 0) {
            this->num_individuos=300;
        }
        if(crossover <= 0 || crossover > 1) {
            this->crossover = 0.9;
        }
        if(mutacao <= 0 || mutacao > 1) {
            this->mutacao=0.04;
        }
    }

vector<Vitima> Genetico::algoritmo_genetico() {
    vector<vector<bool>> individuos = preenche_individuos_iniciais(num_individuos*2);
    melhor_fitness = 0;
    pair<double, vector<bool>> melhor;
    double maior_fitness = 0;
    int i;
    for(i=0; i < iteracoes; i++) {
        vector<double> fitness = fitness_individuos(individuos);

        melhor = melhor_individuo(fitness, individuos);

        //imprime se mudar o maior para não floodar a tela
        if(melhor.first > maior_fitness) {
            //cout << "fitness melhor individuo geração " << i << " = " << melhor.first << endl;
            maior_fitness = melhor.first;
            melhor_fitness = melhor.first;
        }

        vector<pair<vector<bool>, vector<bool>>> cruzamento = seleciona_individuos(individuos, fitness);

        individuos = cruza_individuos(cruzamento);
    }
    
    return to_vitima(melhor.second);
}

vector<Vitima> Genetico::to_vitima(vector<bool> ind) {
    vector<Vitima> solucao;
    for(int i=0; i < ind.size(); i++) {
        if(ind[i]) {
            solucao.push_back(vitimas[i]);
        }
    }

    return solucao;
}

pair<double, vector<bool>> Genetico::melhor_individuo(vector<double> fitness, vector<vector<bool>> individuos) {
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
vector<vector<bool>> Genetico::preenche_individuos_iniciais(int qtd_individuos) {
    vector<vector<bool>> individuos;
    individuos.reserve(qtd_individuos);

    int num_vitimas = vitimas.size();
    int i, j, distribuicao;
    for(i=0; i < qtd_individuos; i++) {
        //probabilidade de um true
        distribuicao = rand() % 11;

        vector<bool> aux;
        aux.reserve(num_vitimas);

        //preenche o vetor aleatoriamente com true ou false
        for(j=0; j < num_vitimas; j++) {
            bool x = (rand() % 11) > distribuicao ? true : false;
            aux.push_back(x);
        }
        individuos.push_back(aux);
    }

    return individuos;
}

vector<double> Genetico::fitness_individuos(vector<vector<bool>> individuos) {
    vector<double> fitness;
    fitness.reserve(individuos.size());

    int i;
    for(i=0; i < individuos.size(); i++) {
        fitness.push_back(calcula_fitness(individuos[i]));
    }

    return fitness;
}

//fitness valoriza maximizar o número de vítimas e o número da gravidade 
//desvaloriza muito se passou do limite de tempo
double Genetico::calcula_fitness(vector<bool> individuo) {
    double fitness = 0.1;
    int tempo_restante = tempo;
    for(int i = 0; i < individuo.size(); i++) {
        if(individuo[i]) {
            fitness += 1 + (vitimas[i].gravidade / 10);
            tempo_restante -= vitimas[i].tempo;
        }
    }
    if(tempo_restante < 0) {
        fitness = fitness / 20;
    }

    return fitness;
}

vector<pair<vector<bool>, vector<bool>>> Genetico::seleciona_individuos(vector<vector<bool>> indivuduos, vector<double> fitness) {
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

pair<double, vector<bool>> Genetico::selecionar_individuo_probabilidade(list<pair<double, vector<bool>>> &ind, double &soma_fit) {
    //Seleciona um individuo pela probabilidade
    int random = rand() % 10001;
    int interv_inicial=0, interv_final=0;
    list<pair<double, vector<bool>>>::iterator iter;
    pair<double, vector<bool>> selecionado = (*ind.begin());

    for(iter=ind.begin(); iter != ind.end(); ++iter) {
        
        interv_final = ceil(((iter->first / soma_fit) * 10000)) + interv_inicial;

        if((random >= interv_inicial && random < interv_final) || (interv_inicial > 10000)) {
            selecionado = *iter;
            ind.erase(iter);
            soma_fit -= iter->first;
            break;
        }
        interv_inicial = interv_final;
    }

    return selecionado;
}

vector<vector<bool>> Genetico::cruza_individuos(vector<pair<vector<bool>, vector<bool>>> cruzamento) {
    vector<vector<bool>> individuos;

    for(int i=0; i < cruzamento.size(); i++) {
        individuos.push_back(cruzamento[i].first);
        individuos.push_back(cruzamento[i].second);

        //Testa se individuos vao cruzar
        pair<vector<bool>, vector<bool>> filhos;
        if((rand() % 100) <= ceil(crossover * 100)) {
            if(estrategiaCruz == EstrategiaCruzamento::Padrao)
                filhos = cruzar_padrao(cruzamento[i].first, cruzamento[i].second);
            else if (estrategiaCruz == EstrategiaCruzamento::Aleatorio)
                filhos = cruzar_aleatorio(cruzamento[i].first, cruzamento[i].second);
            
            mutar_individuo(&filhos);
            individuos.push_back(filhos.first);
            individuos.push_back(filhos.second);
        }
    }

    return individuos;
}
//cruza dois individuos gerando dois filhos
pair<vector<bool>, vector<bool>> Genetico::cruzar_padrao(vector<bool> ind1, vector<bool> ind2) {
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

pair<vector<bool>, vector<bool>> Genetico::cruzar_aleatorio(vector<bool> ind1, vector<bool> ind2) {
    pair<vector<bool>, vector<bool>> filhos;
    for(int i=0; i < ind1.size(); i++) {
      if(rand() % 2 == 0) {
          filhos.first.push_back(ind1[i]);
      }
      else {
          filhos.first.push_back(ind2[i]);
      }

      if(rand() % 2 == 0) {
          filhos.second.push_back(ind1[i]);
      }
      else {
          filhos.second.push_back(ind2[i]);
      }
    };

    return filhos;
}
//Testa para cada gene se vai ter mutação
void Genetico::mutar_individuo(pair<vector<bool>, vector<bool>> *filhos) {
    for(int i=0; i < filhos->first.size(); i++) {
        if((rand() % 100) <= ceil(mutacao * 100)) {
            filhos->first[i] = !(filhos->first[i]);
        }
        if((rand() % 100) <= ceil(mutacao * 100)) {
            filhos->second[i] = !(filhos->second[i]);
        }
    }
}