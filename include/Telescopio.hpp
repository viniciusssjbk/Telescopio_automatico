#include <string>
#include <iostream>

class Telescopio
{
private:
    float ang_horizontal;
    float ang_vertical;

public:
   Telescopio();

   void definirAnguloHorizontal(float angulo);
   void definirAnguloVertical(float angulo);
   float obterAnguloVertical() const;
   float obterAnguloHorizontal() const;
   void Mover_Telescopio(const std::string planeta);



};