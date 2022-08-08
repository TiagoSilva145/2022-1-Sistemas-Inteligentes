#include "labirinto.h"


bool Labirinto::validaAcao(Robo* robo, int posx, int posy) {
    if(robo->bateria > 0 && robo->tempo > 0) {
        if(posx >= 0 && posx < labirinto.size()) {
            if(posy >=0 && posy < labirinto[posx].size()) {
                if(labirinto[posx][posy] > 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

Percepcao Labirinto::mover(Robo* robo, int movX, int movY) {
    Percepcao per;
    int movimentoX=robo->posx+movX, movimentoY=robo->posy+movY;
    if(movX && movY) {
        robo->bateria -= 1.5;
        robo->tempo -= 1.5;
    }
    else {
        robo->bateria -= 1.0;
        robo->tempo -= 1.0;
    }
    per.posx = movimentoX;
    per.posy = movimentoY;
    
    if(validaAcao(robo, movimentoX, movimentoY)) {
        per.objeto = labirinto[movimentoX][movimentoY];
        per.sucesso = true;
        labirinto[robo->posx][robo->posy] = 1;
        //TODO - Pode sobrepor a vítima no labirinto
        labirinto[movimentoX][movimentoY] = -2;
        robo->posx = per.posx;
        robo->posy = per.posy;
    }
    else {
        per.sucesso = false;
    }

    return per;
}

Percepcao Labirinto::moverNorte(Robo* robo) {
    cout << "Acao tomada: moverNorte" << endl;
    int movX=-1, movY=0;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverSul(Robo* robo) {
    cout << "Acao tomada: moverSul" << endl;
    int movX=1, movY=0;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverLeste(Robo* robo) {
    cout << "Acao tomada: moverLeste" << endl;
    int movX=0, movY=1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverOeste(Robo* robo) {
    cout << "Acao tomada: moverOeste" << endl;
    int movX=0, movY=-1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverNordeste(Robo* robo) {
    cout << "Acao tomada: moverNordeste" << endl;
    int movX=-1, movY=1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverNoroeste(Robo* robo) {
    cout << "Acao tomada: moverNoroeste" << endl;
    int movX=-1, movY=-1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverSudeste(Robo* robo) {
    cout << "Acao tomada: moverSudeste" << endl;
    int movX=1, movY=1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::moverSudoeste(Robo* robo) {
    cout << "Acao tomada: moverSudoeste" << endl;
    int movX=1, movY=-1;
    return mover(robo, movX, movY);
}
Percepcao Labirinto::pegarVitima(Robo* robo) {
cout << "Acao tomada: pegarVitima" << endl;
}

vector<int>* Labirinto::LerSinaisVitais(Robo* robo) {
    cout << "Acao tomada: LerSinaisVitais" << endl;
    vector<int>* sinais;
    robo->bateria -= 2;
    robo->tempo -= 2;

    if(robo->bateria > 0 && robo->tempo > 0) {
        if(robo->posx >= 0 && robo->posx < labirinto.size()) {
            if(robo->posy >=0 && robo->posy < labirinto[robo->posx].size()) {
                if(labirinto[robo->posx][robo->posy] > 1) {
                    return sinais;
                }
            }
        }
    }
    return nullptr;
}