#include "RastreadorMemoria.h"
#include "mensaje.h"
#include "RedSocial.h"
#include "Usuario.h"
#include <iostream>
#include <string>
#include <fstream> // Necesario para la lectura/escritura de archivos planos

using namespace std;

// Función para asegurar que existan los archivos planos de prueba
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

int main(){
    RastreadorMemoria::IniciarSegmento("main.cpp, Integracion_Core_y_Mensajes");

    // ============================================================
    // 1. CARGA Y PRUEBAS DEL MÓDULO CORE (JUAN)
    // ============================================================
    cout << "=========================================\n";
    cout << "   1. INICIALIZACION Y PERSISTENCIA CORE \n";
    cout << "=========================================\n";

    // Crear archivos de texto si no existen
    crearArchivosDePrueba();

    // Inicializar la red social y cargar persistencia
    RedSocial red(100);
    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
    cout << "-> Datos de Usuarios y Relaciones cargados correctamente.\n\n";


    // ============================================================
    // 2. PRUEBAS DEL MÓDULO DE MENSAJES Y RASTREADOR (SANTIAGO)
    // ============================================================
    cout << "=========================================\n";
    cout << "   2. PRUEBAS DE MENSAJES Y MEMORIA      \n";
    cout << "=========================================\n";

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