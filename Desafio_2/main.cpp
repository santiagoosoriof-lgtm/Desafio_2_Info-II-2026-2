
#include "RastreadorMemoria.h"
#include "mensaje.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
    RastreadorMemoria::IniciarSegmento("main.cpp, Modulotest, <iostream>");
        for(int i = 0; i < 1000; i++){
        RastreadorMemoria::RegistroIteracion();
        }
        RastreadorMemoria::RegistroAsignacionMemoria(1000);
        RastreadorMemoria::ReporteEficiencia();
        RastreadorMemoria::RegistroLiberacionMemoria(1000);
        RastreadorMemoria::ReporteEficiencia();


        Mensaje* chat1 = new Mensaje("Santiago","Juan","Hola Juan, Como estas? este es un mensaje de prueba de la clase Mensaje", 10102026);
        RastreadorMemoria::RegistroAsignacionMemoria(sizeof(Mensaje)+chat1->getLongitudTexto());
        cout << " -> Emisor:   " << (*chat1).getEmisor() << "\n";
        cout << " -> Receptor: " << (*chat1).getReceptor() << "\n";
        cout << " -> Texto:    " << (*chat1).getTexto() << "\n";
        cout << " -> Fecha:    " << (*chat1).getFechaEnvio() << "\n";
        cout << " -> Leido:    " << ((*chat1).estaleido() ? "Si" : "No") << "\n\n";

        string textogigante ="";
        for(int i = 0; i < 60; i++ ){
            textogigante += "0123456789";
            RastreadorMemoria::RegistroIteracion();
        }



        Mensaje* chat2 = new Mensaje("Santiago","Juan", textogigante, 10102026);
        RastreadorMemoria::RegistroAsignacionMemoria(sizeof(Mensaje)+chat2->getLongitudTexto());

        cout << " -> Emisor:   " << (*chat2).getEmisor() << "\n";
        cout << " -> Receptor: " << (*chat2).getReceptor() << "\n";
        cout << " -> Texto:    " << (*chat2).getTexto() << "\n";
        cout << " -> Fecha:    " << (*chat2).getFechaEnvio() << "\n";
        cout << " -> Leido:    " << ((*chat2).estaleido() ? "Si" : "No") << "\n\n";

        RastreadorMemoria::ReporteEficiencia();
        RastreadorMemoria::RegistroLiberacionMemoria(sizeof(Mensaje)+chat1->getLongitudTexto());
        delete chat1;
        RastreadorMemoria::RegistroLiberacionMemoria(sizeof(Mensaje)+chat2->getLongitudTexto());
        delete chat2;
        RastreadorMemoria::ReporteEficiencia();

        return 0;
}
