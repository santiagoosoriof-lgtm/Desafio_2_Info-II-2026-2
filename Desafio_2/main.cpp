#include "RedSocial.h"
#include "mensajeria.h"
#include "album.h"
#include "foto.h"
#include "fecha.h"
#include "RastreadorMemoria.h"
#include "SistemaUdeABook.h"
#include <iostream>
#include <string>
#include <fstream>

void generarDatasetMasivo1000();

int main() {
   // generarDatasetMasivo1000(); // Solo se corre una vez para crear los archivos de prueba

    SistemaUdeABook* sistema = new SistemaUdeABook();
    sistema->iniciar();
    delete sistema;

    RastreadorMemoria::IniciarSegmento("RedSocial.h, mensajeria.h, album.h, foto.h, fecha.h, "
                                       "RastreadorMemoria.h, SistemaUdeABook.h,<iostream>, <string>, <fstream>");
    RastreadorMemoria::ReporteEficiencia();

    return 0;
}