#include "genetico.h"

Genetico::Genetico(const vector<Vitima> vit, int tempo,
                   int iteracoes = 300, int num_ind = 50, float crossover = 0.9, float mutacao = 0.04, EstrategiaCruzamento e = Padrao)
{
    this->vitimas = vit;
    this->iteracoes = iteracoes;
    this->num_individuos = num_ind;
    this->crossover = crossover;
    this->mutacao = mutacao;
    this->tempo = tempo;
    this->estrategiaCruz = e;

    if (num_ind < 0)
    {
        this->num_individuos = 300;
    }
    if (crossover <= 0 || crossover > 1)
    {
        this->crossover = 0.9;
    }
    if (mutacao <= 0 || mutacao > 1)
    {
        this->mutacao = 0.04;
    }
}

bool comparar(Individuo i1, Individuo i2)
{
    return i1.fit > i2.fit; 
}

vector<Vitima> Genetico::algoritmo_genetico()
{
    vector<Individuo> v_individuos = preenche_individuos_iniciais(num_individuos);

    //Para cada geracao
    for (int i = 0; i < iteracoes; i++)
    {
        cout << "Iteracao: " << i << endl;
        //Tam = tamanho do vetor
        int tam = v_individuos.size();
        //Para tam / 2 cruzamentos
        for (int i = 0; i < tam / 2; i++)
        {
            // Seleciona os dois individuos que vao cruzar
            int i1 = seleciona_individuo(v_individuos, tam);
            int i2 = seleciona_individuo(v_individuos, tam);
            while(i1 == i2)
                i2 = seleciona_individuo(v_individuos, tam);

            //Faz o cruzamento de ambos e coloca no vetor
            cruzar(i1, i2, v_individuos);
        }

        //Ordena o vetor em ordem decrescente de fitnees
        sort(v_individuos.begin(), v_individuos.end(), comparar);

        //Apaga todas as posicos maiores que tam no vetor
        v_individuos.erase(v_individuos.begin()+tam,v_individuos.end());
        cout << "Individuos:" << endl;
        for (int i = 0; i < v_individuos.size()/2; i++)
        {
            cout << "i" << i << ": ";
            for (int j = 0; j < v_individuos[i].vitimas.size(); j++)
            {
                if (v_individuos[i].vitimas[j])
                    cout << "1 ";
                else
                    cout << "0 ";
            }
            cout << "F: " << v_individuos[i].fit;
            cout << endl;
        }
    }

    return to_vitima(v_individuos[0]);
}

// Gera individuos aleatórios
vector<Individuo> Genetico::preenche_individuos_iniciais(int qtd_individuos)
{
    vector<Individuo> individuos;
    individuos.reserve(qtd_individuos);

    int num_vitimas = vitimas.size();
    int i, j, distribuicao;
    for (i = 0; i < qtd_individuos; i++)
    {
        // probabilidade de um true
        distribuicao = rand() % 11;

        Individuo aux;
        aux.vitimas.reserve(num_vitimas);

        // preenche o vetor aleatoriamente com true ou false
        for (j = 0; j < num_vitimas; j++)
        {
            bool x = (rand() % 11) > distribuicao ? true : false;
            aux.vitimas.push_back(x);
        }
        aux.fit = calcula_fitness(aux);
        individuos.push_back(aux);
    }

    return individuos;
}

// fitness valoriza maximizar o valor da gravidade
// desvaloriza muito se passou do limite de tempo
double Genetico::calcula_fitness(Individuo individuo)
{
    double fitness = 0.1;
    int tempo_restante = tempo;
    for (int i = 0; i < individuo.vitimas.size(); i++)
    {
        if (individuo.vitimas[i])
        {
            fitness += vitimas[i].gravidade;
            tempo_restante -= vitimas[i].tempo;
        }
    }
    if (tempo_restante < 0)
    {
        fitness = fitness / 20;
    }

    return fitness;
}

int Genetico::seleciona_individuo(vector<Individuo> geracao, int tam)
{
    double total_fit = 0;
    for (int i = 0; i < tam; i++)
        total_fit += geracao[i].fit;

    float selecionado = (float)(rand() % 100) / 100 * total_fit;

    double limite = 0;
    int i = 0;
    while(i < tam && limite < total_fit)
    {
        limite += geracao[i].fit;
        if(selecionado < limite)
            return i;
        i++;
    }
    return tam-1;
}

void Genetico::cruzar(int i1, int i2, vector<Individuo>& v_individuos)
{
    if(((float)(rand() % 100) / 100) > crossover)
    {
        v_individuos.push_back(v_individuos[i1]);
        v_individuos.push_back(v_individuos[i2]);
        return;
    }

    double pivo = rand() % (vitimas.size() - 2);
    pivo++;

    Individuo f1;
    Individuo f2;
    //Repete os genes ate o pivo
    for(int i = 0; i < pivo; i++)
    {
        f1.vitimas.push_back(v_individuos[i1].vitimas[i]);
        f2.vitimas.push_back(v_individuos[i2].vitimas[i]);
    }

    //Inverte os genes do pivo
    for(int i = pivo; i < vitimas.size(); i++)
    {
        f2.vitimas.push_back(v_individuos[i1].vitimas[i]);
        f1.vitimas.push_back(v_individuos[i2].vitimas[i]);
    }

    //Faz a mutacao
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(((float)(rand() % 100) / 100) <= mutacao)
            f1.vitimas[i] = !f1.vitimas[i];
        if(((float)(rand() % 100) / 100) <= mutacao)
            f2.vitimas[i] = !f2.vitimas[i];
    }

    //Calcula os fitness e coloca no vetor da geracao
    f1.fit = calcula_fitness(f1);
    f2.fit = calcula_fitness(f2);
    
    v_individuos.push_back(f1);
    v_individuos.push_back(f2);
}

vector<Vitima> Genetico::to_vitima(Individuo melhor)
{
    vector<Vitima> solucao;
    for (int i = 0; i < melhor.vitimas.size(); i++)
    {
        if (melhor.vitimas[i])
        {
            solucao.push_back(vitimas[i]);
        }
    }

    return solucao;
}
