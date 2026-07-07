#include "Telescopio.hpp"
#include "Astrodata.hpp"
#include <iostream>
#include <string>


Telescopio::Telescopio()
:ang_horizontal(0),ang_vertical(0)
{
    
}

void Telescopio::definirAnguloHorizontal(float angulo){
    
        this->ang_horizontal = angulo;
    
}

void Telescopio::definirAnguloVertical(float angulo){
    
        this->ang_vertical = angulo;
       
   
}
float Telescopio::obterAnguloVertical() const{
    return this->ang_vertical;
}
float Telescopio::obterAnguloHorizontal() const{
    return this->ang_horizontal;
}
void Telescopio::Mover_Telescopio(std::string planeta){
    Astrodata data;
    data.obterDados(planeta);
    Telescopio::definirAnguloHorizontal((float)(data.obterHorinzontal()));
    Telescopio::definirAnguloVertical((float)(data.obterVertical()));
}