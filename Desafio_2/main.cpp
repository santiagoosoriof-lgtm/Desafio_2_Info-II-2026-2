
#include "RastreadorMemoria.h"
#include <iostream>
using namespace std;

int main()
{
    RastreadorMemoria::IniciarSegmento("main.cpp, Modulotest, <iostream>");
        for(int i = 0; i < 1000; i++){
        RastreadorMemoria::RegistroIteracion();
        }
        RastreadorMemoria::RegistroAsignacionMemoria(4096);
        RastreadorMemoria::ReporteEficiencia();
        RastreadorMemoria::RegistroLiberacionMemoria(1000);
        RastreadorMemoria::ReporteEficiencia();
    return 0;
}
