#include <iostream>
#include "RastreadorMemoria.h"

using namespace std;

// Inicialización de variables estáticas
long RastreadorMemoria::iteracionesTotales = 0;
size_t RastreadorMemoria::memoriaAsignadaBytes = 0;
string RastreadorMemoria::libreriasEjecutadas = "";

// Reinicia contadores y define qué librerías/módulos se usarán en la funcionalidad
void RastreadorMemoria::IniciarSegmento(const string& modulosYLibrerias) {
    iteracionesTotales = 0;
    libreriasEjecutadas = modulosYLibrerias;
}

// Guarda las iteraciones de una funcionalidad
void RastreadorMemoria::RegistroIteracion() {
    iteracionesTotales++;
}

// Lleva la cuenta de la memoria asignada
void RastreadorMemoria::RegistroAsignacionMemoria(size_t bytes) {
    memoriaAsignadaBytes += bytes;
}

// Lleva la cuenta de la memoria liberada
void RastreadorMemoria::RegistroLiberacionMemoria(size_t bytes) {
    if (memoriaAsignadaBytes >= bytes) {
        memoriaAsignadaBytes -= bytes;
    } else {
        memoriaAsignadaBytes = 0;
    }
}

// Reporte de eficiencia
void RastreadorMemoria::ReporteEficiencia() {
    cout << "\n==================================================\n";
    cout << "          REPORTE DE EFICIENCIA Y RECURSOS        \n";
    cout << "==================================================\n";
    cout << " -> Componentes/Librerias:  " << libreriasEjecutadas << "\n";
    cout << " -> Iteraciones ejecutadas: " << iteracionesTotales << "\n";
    cout << " -> Memoria dinamica activa: " << memoriaAsignadaBytes << " Bytes ";
    cout << "(" << (memoriaAsignadaBytes / 1024.0) << " KB)\n";
    cout << "==================================================\n\n";
}