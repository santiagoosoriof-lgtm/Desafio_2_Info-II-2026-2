#ifndef MENSAJERIA_H
#define MENSAJERIA_H

#include <string>
#include <cstddef>
#include "ListaDinamica.h"
#include "Usuario.h"
#include "mensaje.h"

using namespace std;

class Mensajeria{
private:
    ListaDinamica<Mensaje*> bandejaMensajes; //almacenamiento mensajes
    size_t bytesOcupados; //atributo auditoria

public:
    Mensajeria();
    ~Mensajeria();

    //metodos de auditoria
    size_t getMemoria() const;
    static const char* getLibreriasUsadas();

    //metodos de la mensajeria
    bool cargarArchivoMensajes(const string& rutaArchivo);
    void enviarMensaje(Usuario *emisor, Usuario *receptor, const string& texto, long fechaEnvio);
    void getNuevosmsn(const string& usuarioReceptor);
    void verBandejaCompleta(const string& usuarioReceptor);
    void buscarporFecha(const string& usuarioReceptor,long fechaInicio, long fechaFin);
    bool guardarMensajesEnArchivo(const std::string& rutaArchivo);
};

#endif // MENSAJERIA_H
