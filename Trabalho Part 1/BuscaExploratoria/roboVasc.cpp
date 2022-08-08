#include "roboVasc.h"
#include "labirinto.h"

RoboVasc::RoboVasc(int maxLin, int maxCol, float bat, float temp, Labirinto *amb) : Robo(bat, temp)
{
    ultimaPer.posx = 0;
    ultimaPer.posy = 0;
    ultimaPer.sucesso = false;

    for (int i = 1; i <= 4; i++)
        acoes.push_back(i);
    ambiente = amb;
    for (int i = 0; i < maxLin; i++)
    {
        vector<int> *aux = new vector<int>;
        for (int j = 0; j < maxCol; j++)
            aux->push_back(-1);
        mapa.push_back(*aux);
    }
    mapa[0][0] = 1;
}

RoboVasc::~RoboVasc()
{
    int x = mapa[0].size();
    for (int i = 0; i < x; i++)
        mapa[i].clear();
    mapa.clear();
}

void RoboVasc::imprimirDados()
{
    cout << "Robo Vasculhador dados: "
         << "bateria = " << bateria
         << "; tempo = " << tempo
         << "; tamanho = " << mapa.size() << " X " << mapa[0].size() << endl;
}

void RoboVasc::deliberar()
{
    if (ultimaPer.sucesso)
    {
        mapa[posx][posy] = ultimaPer.objeto;
        if (ultimaPer.objeto > 1)
        {
            // TODO - ler sinais vitais
            // auto sinais = ambiente->LerSinaisVitais(this);
        }
    }
    else
    {
        if ((ultimaPer.posx >= 0 && ultimaPer.posx < mapa.size()) && 
        (ultimaPer.posy >= 0 && ultimaPer.posy < mapa[ultimaPer.posx].size()))
            if (mapa[ultimaPer.posx][ultimaPer.posy] == -1)
                mapa[ultimaPer.posx][ultimaPer.posy] = 0;
        int aux = acoes.front();
        acoes.remove(acoes.front());
        acoes.push_back(aux);
    }
    decidirMovimento();
}

void RoboVasc::decidirMovimento()
{
    int acaoEscolhida = acoes.front();
    list<int>::iterator it;
    for (it = acoes.begin(); it != acoes.end(); ++it)
    {
        if (explorado(*it) == -1) {
            acaoEscolhida = *it;
            break;
        }
    }
    switch (acaoEscolhida)
    {
    case 1:
        ultimaPer = ambiente->moverNorte(this);
        break;
    case 2:
        ultimaPer = ambiente->moverSul(this);
        break;
    case 3:
        ultimaPer = ambiente->moverLeste(this);
        break;
    case 4:
        ultimaPer = ambiente->moverOeste(this);
        break;
    case 5:
        ultimaPer = ambiente->moverNordeste(this);
        break;
    case 6:
        ultimaPer = ambiente->moverNoroeste(this);
        break;
    case 7:
        ultimaPer = ambiente->moverSudeste(this);
        break;
    case 8:
        ultimaPer = ambiente->moverSudoeste(this);
        break;
    default:
        break;
    }
}

int RoboVasc::explorado(int mov)
{
    switch (mov)
    {
    case 1:
        if (IndiceSeguroMatriz(posx - 1, posy, mapa.size(), mapa[0].size()))
            return mapa[posx - 1][posy];
        break;
    case 2:
        if (IndiceSeguroMatriz(posx + 1, posy, mapa.size(), mapa[0].size()))
            return mapa[posx + 1][posy];
        break;
    case 3:
        if (IndiceSeguroMatriz(posx, posy + 1, mapa.size(), mapa[0].size()))
            return mapa[posx][posy + 1];
        break;
    case 4:
        if (IndiceSeguroMatriz(posx, posy - 1, mapa.size(), mapa[0].size()))
            return mapa[posx][posy - 1];
        break;
    case 5:
        if (IndiceSeguroMatriz(posx - 1, posy + 1, mapa.size(), mapa[0].size()))
            return mapa[posx - 1][posy + 1];
        break;
    case 6:
        if (IndiceSeguroMatriz(posx - 1, posy - 1, mapa.size(), mapa[0].size()))
            return mapa[posx - 1][posy - 1];
        break;
    case 7:
        if (IndiceSeguroMatriz(posx + 1, posy + 1, mapa.size(), mapa[0].size()))
            return mapa[posx + 1][posy + 1];
        break;
    case 8:
        if (IndiceSeguroMatriz(posx + 1, posy - 1, mapa.size(), mapa[0].size()))
            return mapa[posx + 1][posy - 1];
        break;
    default:
        return 0;
        break;
    }
}

bool RoboVasc::IndiceSeguroMatriz(int i, int j, int maxI, int maxJ)
{
    if (i >= 0 && i < maxI && j >= 0 && j < maxJ)
    {
        return true;
    }
    return false;
}