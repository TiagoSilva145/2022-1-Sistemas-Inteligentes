#pragma once

class Vitima
{
    public:
    int posx;
    int posy;
    float gravidade;
    float tempo;
    int extrato_gravidade;

    Vitima()
    {
        posx = posy = 0;
        tempo = gravidade = extrato_gravidade = 0;
    }
    ~Vitima()
    {

    }
};