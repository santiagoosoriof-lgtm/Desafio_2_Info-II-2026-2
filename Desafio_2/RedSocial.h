#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include "Usuario.h"
#include "ListaDinamica.h"

class RedSocial {
private:
    ListaDinamica<Usuario*> usuarios; // Lista de punteros a todos los usuarios
    bool** matrizAmistad;             // Matriz dinamica de adyacencia (bool**)
    int capacidadMaxima;

    void inicializarMatriz();

public:
    RedSocial(int capacidad = 100);
    ~RedSocial();
    // Persistencia (Lectura de archivos planos)
    bool cargarUsuariosDesdeArchivo(const std::string& rutaArchivo);
    bool cargarRelacionesDesdeArchivo(const std::string& rutaArchivo);


    // Gestion de Usuarios
    bool agregarUsuario(Usuario* nuevoUsuario);
    Usuario* buscarUsuario(const std::string& username);
    int obtenerIndiceUsuario(const std::string& username) const;

    // Gestion del Grafo de Amistades (Matriz bool**)
    bool registrarAmistad(const std::string& user1, const std::string& user2);
    bool sonAmigos(const std::string& user1, const std::string& user2) const;
    int contarAmigosEnComun(const std::string& user1, const std::string& user2) const;

    // Algoritmos asignados a Juan
    ListaDinamica<Usuario*> obtenerMejoresAmigosSorteados(const std::string& username);
    ListaDinamica<Usuario*> obtenerSugerenciasK(const std::string& username, int k);

    int obtenerCantidadUsuarios() const;
};

#endif
