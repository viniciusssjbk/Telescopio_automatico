
#include <iostream>

class Astrodata
{
private:
    float Horinzontal;
    float Vertical;

public:
    Astrodata();
    void obterDados(std::string planeta);
    std::string obterTempoAtual(int minutosAdicionais = 0);
    float obterHorinzontal();
    float obterVertical();
};