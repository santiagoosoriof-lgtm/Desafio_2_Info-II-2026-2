#include "RedSocial.h"
#include "Mensajeria.h"
#include "album.h"
#include "foto.h"
#include "fecha.h"
#include "RastreadorMemoria.h"
#include "SistemaUdeABook.h"

#include <iostream>
#include <string>
#include <fstream> // Necesario para la lectura/escritura de archivos planos

using namespace std;

// Función de Juan para asegurar que existan los archivos planos de prueba
void crearArchivosDePrueba() {
    ifstream checkUser("usuarios.txt");
    if (!checkUser.good()) {
        ofstream fUsuarios("usuarios.txt");
        if (fUsuarios.is_open()) {
            fUsuarios << "juan_perez;2000-05-10;juan@udea.edu.co;foto1.jpg;Medellin;Colombia;2026-10-01\n";
            fUsuarios << "santiago_gomez;2001-08-15;santi@udea.edu.co;foto2.jpg;Envigado;Colombia;2026-10-02\n";
            fUsuarios << "maria_lopez;1999-12-01;maria@udea.edu.co;foto3.jpg;Rionegro;Colombia;2026-10-03\n";
            fUsuarios.close();
        }
    }

    ifstream checkRel("relaciones.txt");
    if (!checkRel.good()) {
        ofstream fRelaciones("relaciones.txt");
        if (fRelaciones.is_open()) {
            fRelaciones << "juan_perez;santiago_gomez\n";
            fRelaciones << "santiago_gomez;maria_lopez\n";
            fRelaciones.close();
        }
    }
}

int main() {

    SistemaUdeABook sistema(1000);
    sistema.iniciar();

    return 0;
}