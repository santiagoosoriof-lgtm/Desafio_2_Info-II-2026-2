#pragma once

#include <iostream>
#include <string>
#include "RedSocial.h"
#include "Usuario.h"
#include "Album.h"
#include "foto.h"
#include "mensajeria.h"
#include "RastreadorMemoria.h"

class SistemaUdeABook {
private:
    RedSocial red;
    Usuario* usuarioActivo;
    Mensajeria* gestorMensajeria;

    // Métodos privados para cada submenú
    void mostrarMejoresAmigos();
    void menuAdministrarAmistades();
    void menuVisitarPerfilAmigo();
    void menuVerSugerenciasK();
    void menuAdministrarMensajes();

public:
    SistemaUdeABook(int capacidadRed = 1000);
    ~SistemaUdeABook();

    void cargarBaseDeDatos();
    void iniciar();
};
