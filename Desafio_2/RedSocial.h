#pragma once

#include <string>
#include "ListaDinamica.h"
#include "Usuario.h"

class RedSocial {
private:
    ListaDinamica<Usuario*> usuarios;
    int capacidadMaxima;
    size_t bytesOcupados;

public:
    RedSocial(int capacidad = 1000);
    ~RedSocial();

    bool agregarUsuario(Usuario* nuevoUsuario);
    Usuario* buscarUsuario(const std::string& username);
    int obtenerIndiceUsuario(const std::string& username) const;

    bool registrarAmistad(const std::string& user1, const std::string& user2);
    bool sonAmigos(const std::string& user1, const std::string& user2) const;
    int contarAmigosEnComun(const std::string& user1, const std::string& user2) const;

    ListaDinamica<Usuario*> obtenerMejoresAmigosSorteados(const std::string& username);
    ListaDinamica<Usuario*> obtenerSugerenciasK(const std::string& username, int k);

    int obtenerCantidadUsuarios() const;
    bool cargarUsuariosDesdeArchivo(const std::string& rutaArchivo);
    bool cargarRelacionesDesdeArchivo(const std::string& rutaArchivo);

    size_t getMemoria() const;
    static const char* getLibreriasUsadas();
};