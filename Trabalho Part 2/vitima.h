#pragma once

class Vitima
{
    public:
    int posx;
    int posy;
    float gravidade;
    float tempo;

    Vitima()
    {
        posx = posy = 0;
        tempo = gravidade = 0;
    }
    ~Vitima()
    {

    }
};