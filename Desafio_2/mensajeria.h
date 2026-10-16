#ifndef MENSAJERIA_H
#define MENSAJERIA_H

#include <string>
#include <cstddef>
#include "ListaDinamica.h"
#include "Usuario.h"
#include "mensaje.h"


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
    bool cargarArchivoMensajes(const std::string& rutaArchivo);
    void enviarMensaje(Usuario *emisor, Usuario *receptor, const std::string& texto, long fechaEnvio);
    void getNuevosmsn(const std::string& usuarioReceptor);
    void verBandejaCompleta(const std::string& usuarioReceptor);
    void buscarporFecha(const std::string& usuarioReceptor,long fechaInicio, long fechaFin);
    bool guardarMensajesEnArchivo(const std::string& rutaArchivo);
};

#endif // MENSAJERIA_H
