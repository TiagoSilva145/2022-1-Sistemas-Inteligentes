#include "robo.h"

Robo::Robo(float bat, float temp)
{
    posx = posy = 0;
    bateria = bat;
    tempo = temp;
    completou = false;
}

Robo::~Robo()
{
}

void Robo::imprimirMapa()
{
    cout << endl
         << "--- Mapa Robo ---" << endl;
    for (int i = 0; i < mapa.size(); i++)
    {
        string c = "  ";
        for (int j = 0; j < mapa[i].size(); j++)
        {
            switch (mapa[i][j])
            {
            case -1:
                c = "? ";
                break;
            case -2:
                c = "O ";
                break;
            case 0:
                c = "X ";
                break;
            case 1:
                c = ". ";
                break;
            default:
                c = "V ";
                break;
            }
            cout << c << " ";
        }
        cout << endl;
    }
}