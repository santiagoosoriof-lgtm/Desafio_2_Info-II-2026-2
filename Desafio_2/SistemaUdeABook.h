#ifndef SISTEMAUDEABOOK_H
#define SISTEMAUDEABOOK_H

#include "RedSocial.h"
#include "Usuario.h"
#include "mensajeria.h"

class SistemaUdeABook {
private:
    RedSocial red;
    Usuario* usuarioActivo;
    Mensajeria* gestorMensajeria;

    void cargarBaseDeDatos();
    void mostrarMejoresAmigos();
    void menuAdministrarAmistades();
    void menuVisitarPerfilAmigo();
    void menuVerSugerenciasK();
    void menuAdministrarMensajes();

public:
    SistemaUdeABook(int capacidadRed = 1000);
    ~SistemaUdeABook();

    void iniciar();
};

#endif // SISTEMAUDEABOOK_H
