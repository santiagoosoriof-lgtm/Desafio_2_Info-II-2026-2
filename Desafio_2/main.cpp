
#include "RastreadorMemoria.h"
#include "mensaje.h"
#include "foto.h"

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
        RastreadorMemoria::RegistroAsignacionMemoria(chat1->getMemoria());
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
        RastreadorMemoria::RegistroAsignacionMemoria(chat2->getMemoria());
        chat2->setLeido(true);
        cout << " -> Emisor:   " << (*chat2).getEmisor() << "\n";
        cout << " -> Receptor: " << (*chat2).getReceptor() << "\n";
        cout << " -> Texto:    " << (*chat2).getTexto() << "\n";
        cout << " -> Fecha:    " << (*chat2).getFechaEnvio() << "\n";
        cout << " -> Leido:    " << ((*chat2).estaleido() ? "Si" : "No") << "\n\n";

        RastreadorMemoria::ReporteEficiencia();
        RastreadorMemoria::RegistroLiberacionMemoria(chat1->getMemoria());
        delete chat1;
        RastreadorMemoria::RegistroLiberacionMemoria(chat2->getMemoria());
        delete chat2;
        RastreadorMemoria::ReporteEficiencia();

        Foto* foto1= new Foto("10102026","/home/santiago/udeabook/media/vacaciones.png",2621440);
        RastreadorMemoria::RegistroAsignacionMemoria(foto1->getMemoria());

        cout << " -> Fecha Captura: " << foto1->getFechaCaptura() << "\n";
        cout << " -> Ruta Linux:    " << foto1->getRutaAbsoluta() << "\n";
        cout << " -> Peso Imagen:   " << foto1->getTamanoBytes() << " Bytes (Simulado)\n";
        cout << " -> Likes Inicial: " << foto1->getLikes() << "\n";

        // Imprimimos el reporte para ver cuántos bytes base + dinámicos costó en RAM
        RastreadorMemoria::ReporteEficiencia();

        //probar los likes

        foto1->setLike();
        foto1->setLike();
        foto1->setLike();
        cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";
        foto1->setUnlike();
        cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";
        foto1->setUnlike();
        foto1->setUnlike();
        foto1->setUnlike();
        cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";

        RastreadorMemoria::RegistroLiberacionMemoria(foto1->getMemoria());
        delete foto1;

        RastreadorMemoria::ReporteEficiencia();

        return 0;
}
