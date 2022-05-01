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

vector<Vitima> Genetico::algoritmo_genetico()
{
    // vector<vector<bool>> individuos = preenche_individuos_iniciais(num_individuos*2);
    vector<Individuo> v_individuos = preenche_individuos_iniciais(num_individuos);

    /*
    melhor_fitness = 0;
    pair<double, vector<bool>> melhor;
    double maior_fitness = 0;
    for(int i=0; i < iteracoes; i++) {

        vector<double> fitness = fitness_individuos(individuos);

        melhor = melhor_individuo(fitness, individuos);

        imprime se mudar o maior para não floodar a tela
        if(melhor.first > maior_fitness) {
            //cout << "fitness melhor individuo geração " << i << " = " << melhor.first << endl;
            maior_fitness = melhor.first;
            melhor_fitness = melhor.first;
        }

        vector<pair<vector<bool>, vector<bool>>> cruzamento = seleciona_individuos(individuos, fitness);

        individuos = cruza_individuos(cruzamento);
    }
    */
    /*for (int i = 0; i < v_individuos.size(); i++)
    {
        for (int j = 0; j < v_individuos[i].vitimas.size(); j++)
        {
            if (v_individuos[i].vitimas[j])
                cout << "1 ";
            else
                cout << "0 ";
        }
        cout << "F: " << v_individuos[i].fit;
        cout << endl;
    }*/
    /*for(int i = 0; i < 3; i++)
    {
        seleciona_individuo(v_individuos, v_individuos.size());
    }
    return vitimas;*/
    //Para cada geracao
    for (int i = 0; i < iteracoes; i++)
    {
        cout << "Iteracao: " << i << endl;
        //Tam = tamanho do vetor
        int tam = v_individuos.size();
        cout << "Tam: " << v_individuos.size() << endl << "Tam/2: " << tam / 2 << endl;
        //Para tam / 2 cruzamentos
        for (int i = 0; i < tam / 2; i++)
        {
            // Seleciona os dois individuos que vao cruzar
            int i1 = seleciona_individuo(v_individuos, tam);
            int i2 = seleciona_individuo(v_individuos, tam);

            //Faz o cruzamento de ambos e coloca no vetor
            cruzar(i1, i2, v_individuos);

            cout << "Cruzou: " << i1 << " " << i2 << endl << endl;
        }

        cout << "Individuos:" << endl;
        for (int i = 0; i < v_individuos.size(); i++)
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

        //Ordena o vetor em ordem decrescente de fitnees
        sort(v_individuos.begin(), v_individuos.end(), comparar);

        cout << "Individuos ordenados:" << endl;
        for (int i = 0; i < v_individuos.size(); i++)
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

        //Apaga todas as posicos maiores que tam no vetor
    }

    // return to_vitima(melhor.second);
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

void Genetico::cruzamento(vector<Individuo> &geracao)
{
    int tam = geracao.size();
    for (int i = 0; i < tam; i++)
    {
        int i1 = seleciona_individuo(geracao, tam);
        int i2 = seleciona_individuo(geracao, tam);

    }
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
        {
            cout << "Selecionado: " << selecionado << " Individuo: " << i << " Fitness: " << geracao[i].fit << endl;
            return i;
        }
        i++;
    }
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
    // cout << "Pivo: " << pivo << endl; 
    pivo++;

    Individuo f1;
    Individuo f2;
    //Repete os genes ate o pivo
    for(int i = 0; i < pivo; i++)
    {
        // cout << "a";
        f1.vitimas.push_back(v_individuos[i1].vitimas[i]);
        f2.vitimas.push_back(v_individuos[i2].vitimas[i]);
    }
    // cout << endl;

    //Inverte os genes do pivo
    for(int i = pivo; i < vitimas.size(); i++)
    {
        // cout << "b";
        f2.vitimas.push_back(v_individuos[i1].vitimas[i]);
        f1.vitimas.push_back(v_individuos[i2].vitimas[i]);
    }
    // cout << endl;

    //Faz a mutacao
    // cout << "Mutacao: " << mutacao << endl;
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(((float)(rand() % 100) / 100) <= mutacao)
        {
            // cout << "Mutou f1 em" << i << endl; 
            f1.vitimas[i] = !f1.vitimas[i];
        }
        if(((float)(rand() % 100) / 100) <= mutacao)
        {
            // cout << "Mutou f2 em" << i << endl; 
            f2.vitimas[i] = !f2.vitimas[i];
        }
    }

    //Calcula os fitness e coloca no vetor da geracao
    f1.fit = calcula_fitness(f1);
    f2.fit = calcula_fitness(f2);
    
    v_individuos.push_back(f1);
    v_individuos.push_back(f2);

    /*
    cout << "i1: ";
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(v_individuos[i1].vitimas[i])
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
    cout << "i2: ";
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(v_individuos[i2].vitimas[i])
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
    cout << "f1: ";
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(f1.vitimas[i])
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
    cout << "f2: ";
    for(int i = 0; i < vitimas.size(); i++)
    {
        if(f2.vitimas[i])
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
    */
}

bool Genetico::comparar(Individuo i1, Individuo i2)
{
    return i1.fit > i2.fit; 
}

vector<Vitima> Genetico::to_vitima(vector<bool> ind)
{
    vector<Vitima> solucao;
    for (int i = 0; i < ind.size(); i++)
    {
        if (ind[i])
        {
            solucao.push_back(vitimas[i]);
        }
    }

    return solucao;
}

pair<double, vector<bool>> Genetico::melhor_individuo(vector<double> fitness, vector<vector<bool>> individuos)
{
    double maior = -1;
    vector<bool> melhor;
    for (int i = 0; i < individuos.size(); i++)
    {
        if (fitness[i] > maior)
        {
            maior = fitness[i];
            melhor = individuos[i];
        }
    }

    return pair<double, vector<bool>>(maior, melhor);
}

vector<double> Genetico::fitness_individuos(vector<vector<bool>> individuos)
{
    vector<double> fitness;
    fitness.reserve(individuos.size());

    int i;
    for (i = 0; i < individuos.size(); i++)
    {
        // fitness.push_back(calcula_fitness(individuos[i]));
    }

    return fitness;
}

vector<pair<vector<bool>, vector<bool>>> Genetico::seleciona_individuos(vector<vector<bool>> indivuduos, vector<double> fitness)
{
    vector<pair<vector<bool>, vector<bool>>> cruzamento;
    cruzamento.reserve((int)(indivuduos.size() / 2));

    // soma todos os fitness para fazer as probabilidades
    double soma_fit = 0;
    for (int i = 0; i < fitness.size(); i++)
    {
        soma_fit += fitness[i];
    }

    // converte vetor em uma lista
    list<pair<double, vector<bool>>> ind;
    for (int i = 0; i < indivuduos.size(); i++)
    {
        ind.push_back(pair<double, vector<bool>>(fitness[i], indivuduos[i]));
    }

    pair<double, vector<bool>> selecionado1, selecionado2;
    for (int i = 0; i < (int)num_individuos / 2; i++)
    {

        selecionado1 = selecionar_individuo_probabilidade(ind, soma_fit);
        selecionado2 = selecionar_individuo_probabilidade(ind, soma_fit);

        cruzamento.push_back(pair<vector<bool>, vector<bool>>(selecionado1.second, selecionado2.second));
    }

    return cruzamento;
}

pair<double, vector<bool>> Genetico::selecionar_individuo_probabilidade(list<pair<double, vector<bool>>> &ind, double &soma_fit)
{
    // Seleciona um individuo pela probabilidade
    int random = rand() % 10001;
    int interv_inicial = 0, interv_final = 0;
    list<pair<double, vector<bool>>>::iterator iter;
    pair<double, vector<bool>> selecionado = (*ind.begin());

    for (iter = ind.begin(); iter != ind.end(); ++iter)
    {

        interv_final = ceil(((iter->first / soma_fit) * 10000)) + interv_inicial;

        if ((random >= interv_inicial && random < interv_final) || (interv_inicial > 10000))
        {
            selecionado = *iter;
            ind.erase(iter);
            soma_fit -= iter->first;
            break;
        }
        interv_inicial = interv_final;
    }

    return selecionado;
}

vector<vector<bool>> Genetico::cruza_individuos(vector<pair<vector<bool>, vector<bool>>> cruzamento)
{
    vector<vector<bool>> individuos;

    for (int i = 0; i < cruzamento.size(); i++)
    {
        individuos.push_back(cruzamento[i].first);
        individuos.push_back(cruzamento[i].second);

        // Testa se individuos vao cruzar
        pair<vector<bool>, vector<bool>> filhos;
        if ((rand() % 100) <= ceil(crossover * 100))
        {
            if (estrategiaCruz == EstrategiaCruzamento::Padrao)
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
// cruza dois individuos gerando dois filhos
pair<vector<bool>, vector<bool>> Genetico::cruzar_padrao(vector<bool> ind1, vector<bool> ind2)
{
    int pivo = rand() % ind1.size();
    pair<vector<bool>, vector<bool>> filhos;
    for (int i = 0; i < ind1.size(); i++)
    {
        if (i < pivo)
        {
            filhos.first.push_back(ind1[i]);
            filhos.second.push_back(ind2[i]);
        }
        else
        {
            filhos.first.push_back(ind2[i]);
            filhos.second.push_back(ind1[i]);
        }
    };

    return filhos;
}

pair<vector<bool>, vector<bool>> Genetico::cruzar_aleatorio(vector<bool> ind1, vector<bool> ind2)
{
    pair<vector<bool>, vector<bool>> filhos;
    for (int i = 0; i < ind1.size(); i++)
    {
        if (rand() % 2 == 0)
        {
            filhos.first.push_back(ind1[i]);
        }
        else
        {
            filhos.first.push_back(ind2[i]);
        }

        if (rand() % 2 == 0)
        {
            filhos.second.push_back(ind1[i]);
        }
        else
        {
            filhos.second.push_back(ind2[i]);
        }
    };

    return filhos;
}
// Testa para cada gene se vai ter mutação
void Genetico::mutar_individuo(pair<vector<bool>, vector<bool>> *filhos)
{
    for (int i = 0; i < filhos->first.size(); i++)
    {
        if ((rand() % 100) <= ceil(mutacao * 100))
        {
            filhos->first[i] = !(filhos->first[i]);
        }
        if ((rand() % 100) <= ceil(mutacao * 100))
        {
            filhos->second[i] = !(filhos->second[i]);
        }
    }
}