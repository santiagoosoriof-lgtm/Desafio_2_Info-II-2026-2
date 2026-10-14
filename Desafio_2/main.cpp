#include "RedSocial.h"
#include "Mensajeria.h"
#include "album.h"
#include "foto.h"
#include "fecha.h"
#include "RastreadorMemoria.h"

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

    RastreadorMemoria::IniciarSegmento("main.cpp, RedSocial.h, <fstream>");

    cout << "=========================================\n";
    cout << "     INICIALIZACION Y PERSISTENCIA CORE \n";
    cout << "=========================================\n";

    crearArchivosDePrueba();

    // Inicializar la red social (El constructor ya registra la matriz internamente)
    RedSocial red(100);

    // Cargamos los archivos planos (Inyectan iteraciones y memoria de usuarios)
    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
    cout << "-> Datos de Usuarios y Relaciones cargados correctamente.\n";

    // Imprimimos el reporte del primer segmento (Carga Masiva)
    RastreadorMemoria::ReporteEficiencia();


    string libsMultimedia = string(Album::getLibreriasUsadas());
    RastreadorMemoria::IniciarSegmento(libsMultimedia);

    cout << "=========================================\n";
    cout << "      PRUEBAS DE GALERIA Y CARRUSEL      \n";
    cout << "=========================================\n";

    // Simulamos la creación de una galería para santiago_gomez
    Album* albumVacaciones = new Album("Vacaciones San Andres", "/home/santi/media/", "Publico");
    RastreadorMemoria::RegistroAsignacionMemoria(albumVacaciones->getMemoria());

    Foto* fPlaya = new Foto("20261011", "/media/playa.png", 5242880); // 5MB simulados
    Foto* fHotel = new Foto("20261012", "/media/hotel.png", 3145728); // 3MB simulados

    RastreadorMemoria::RegistroAsignacionMemoria(fPlaya->getMemoria());
    RastreadorMemoria::RegistroAsignacionMemoria(fHotel->getMemoria());

    // Agregamos las fotos aplicando el control de exclusividad
    albumVacaciones->agregarFoto(fPlaya);
    albumVacaciones->agregarFoto(fHotel);

    // Navegación del carrusel circular dinámico
    cout << " -> Foto inicial:   " << albumVacaciones->obtenerFotoActual()->getRutaAbsoluta() << "\n";
    albumVacaciones->fotoSiguiente();
    cout << " -> Al avanzar:     " << albumVacaciones->obtenerFotoActual()->getRutaAbsoluta() << "\n";
    albumVacaciones->fotoSiguiente(); // Giro circular adelante
    cout << " -> Giro adelante:  " << albumVacaciones->obtenerFotoActual()->getRutaAbsoluta() << "\n";

    RastreadorMemoria::ReporteEficiencia();


    RastreadorMemoria::IniciarSegmento(Mensajeria::getLibreriasUsadas());

    cout << "=========================================\n";
    cout << "      PRUEBAS DE CHAT Y MENSALERIA       \n";
    cout << "=========================================\n";

    Mensajeria chatGlobal;
    string fechaEntradaTexto1 = "2026-10-10";
    string fechaEntradaTexto2 = "2026-10-12";
    long fecha1Long = Fecha::cadenaALongInvertido(fechaEntradaTexto1); // Devuelve 20261010
    long fecha2Long = Fecha::cadenaALongInvertido(fechaEntradaTexto2); // Devuelve 20261012

    /
    chatGlobal.enviarMensaje("santiago_gomez", "juan_perez", "Hola Juan, ya monte mi rastreador.", fecha1Long);
    chatGlobal.enviarMensaje("juan_perez", "santiago_gomez", "Excelente Santi, probemos el chat.", fecha2Long);
    chatGlobal.enviarMensaje("santiago_gomez", "juan_perez", "Mensaje antiguo de prueba.", fecha1Long);


    chatGlobal.getNuevosmsn("juan_perez");
    chatGlobal.verBandejaCompleta("juan_perez");


    chatGlobal.buscarporFecha("juan_perez",20261010, 20261013);

    RastreadorMemoria::ReporteEficiencia();

    cout << "=========================================\n";
    cout << "     LIBERACION FINAL DE RECURSOS        \n";
    cout << "=========================================\n";

    // Antes de borrar los punteros, notificamos al rastreador para mantener la simetría
    RastreadorMemoria::RegistroLiberacionMemoria(albumVacaciones->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(fPlaya->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(fHotel->getMemoria());

    delete albumVacaciones;
    delete fPlaya;
    delete fHotel;

    cout << "-> Estructuras dinamicas del Heap liberadas correctamente.\n";
    return 0;
}