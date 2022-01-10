#include <iostream>
#include "Sensors/Sensors.h"
int main()
{
    Sensors prueba;
    prueba.setNum_channel(3);
    std::cout << prueba.getNum_channel();
    return 0;
}