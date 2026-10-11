#include "RedSocial.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

// Constructor: Reserva memoria dinamica para la matriz bool**
RedSocial::RedSocial(int capacidad) : capacidadMaxima(capacidad) {
    matrizAmistad = new bool*[capacidadMaxima];
    for (int i = 0; i < capacidadMaxima; i++) {
        matrizAmistad[i] = new bool[capacidadMaxima];
    }
    inicializarMatriz();
}

void RedSocial::inicializarMatriz() {
    for (int i = 0; i < capacidadMaxima; i++) {
        for (int j = 0; j < capacidadMaxima; j++) {
            matrizAmistad[i][j] = false;
        }
    }
}

// Destructor: Libera la memoria de la matriz dinamica y de los usuarios
RedSocial::~RedSocial() {
    // Liberar cada fila de la matriz
    for (int i = 0; i < capacidadMaxima; i++) {
        delete[] matrizAmistad[i];
    }
    delete[] matrizAmistad;

    // Liberar los objetos Usuario asignados en memoria
    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        delete usuarios.obtener(i);
    }
}

bool RedSocial::agregarUsuario(Usuario* nuevoUsuario) {
    if (nuevoUsuario == nullptr || usuarios.obtenerTamano() >= capacidadMaxima) {
        return false;
    }
    if (buscarUsuario(nuevoUsuario->getUsername()) != nullptr) {
        return false; // Ya existe ese username
    }
    usuarios.agregarAlFinal(nuevoUsuario);
    return true;
}

Usuario* RedSocial::buscarUsuario(const std::string& username) {
    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        if (usuarios.obtener(i)->getUsername() == username) {
            return usuarios.obtener(i);
        }
    }
    return nullptr;
}

int RedSocial::obtenerIndiceUsuario(const std::string& username) const {
    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        if (usuarios.obtener(i)->getUsername() == username) {
            return i;
        }
    }
    return -1;
}

// Registro de amistad bidireccional en la matriz bool**
bool RedSocial::registrarAmistad(const std::string& user1, const std::string& user2) {
    int idx1 = obtenerIndiceUsuario(user1);
    int idx2 = obtenerIndiceUsuario(user2);

    if (idx1 != -1 && idx2 != -1 && idx1 != idx2) {
        matrizAmistad[idx1][idx2] = true;
        matrizAmistad[idx2][idx1] = true;

        Usuario* u1 = usuarios.obtener(idx1);
        Usuario* u2 = usuarios.obtener(idx2);
        u1->agregarAmigo(u2);
        u2->agregarAmigo(u1);
        return true;
    }
    return false;
}

bool RedSocial::sonAmigos(const std::string& user1, const std::string& user2) const {
    int idx1 = obtenerIndiceUsuario(user1);
    int idx2 = obtenerIndiceUsuario(user2);
    if (idx1 != -1 && idx2 != -1) {
        return matrizAmistad[idx1][idx2];
    }
    return false;
}

int RedSocial::contarAmigosEnComun(const std::string& user1, const std::string& user2) const {
    int idx1 = obtenerIndiceUsuario(user1);
    int idx2 = obtenerIndiceUsuario(user2);
    if (idx1 == -1 || idx2 == -1) return 0;

    int comun = 0;
    for (int k = 0; k < usuarios.obtenerTamano(); k++) {
        if (matrizAmistad[idx1][k] && matrizAmistad[idx2][k]) {
            comun++;
        }
    }
    return comun;
}

// Algoritmo 1: Sorteo de mejores amigos al iniciar sesion
ListaDinamica<Usuario*> RedSocial::obtenerMejoresAmigosSorteados(const std::string& username) {
    ListaDinamica<Usuario*> seleccionados;
    Usuario* u = buscarUsuario(username);
    if (u == nullptr) return seleccionados;

    ListaDinamica<Usuario*>& amigos = u->getAmigos();
    if (amigos.obtenerTamano() == 0) return seleccionados;

    // Seleccionamos hasta 3 amigos al azar
    for (int i = 0; i < amigos.obtenerTamano() && i < 3; i++) {
        seleccionados.agregarAlFinal(amigos.obtener(i));
    }
    return seleccionados;
}

// Algoritmo 2: Ranking Top-K de sugerencias de amistad
ListaDinamica<Usuario*> RedSocial::obtenerSugerenciasK(const std::string& username, int k) {
    ListaDinamica<Usuario*> sugerencias;
    int idxUser = obtenerIndiceUsuario(username);
    if (idxUser == -1) return sugerencias;

    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        if (i != idxUser && !matrizAmistad[idxUser][i]) {
            int comun = contarAmigosEnComun(username, usuarios.obtener(i)->getUsername());
            if (comun > 0) {
                sugerencias.agregarAlFinal(usuarios.obtener(i));
                if (sugerencias.obtenerTamano() == k) break;
            }
        }
    }
    return sugerencias;
}

int RedSocial::obtenerCantidadUsuarios() const {
    return usuarios.obtenerTamano();
}

bool RedSocial::cargarUsuariosDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string username, fechaNac, email, foto, ciudad, pais, fechaInsc;

        std::getline(ss, username, ';');
        std::getline(ss, fechaNac, ';');
        std::getline(ss, email, ';');
        std::getline(ss, foto, ';');
        std::getline(ss, ciudad, ';');
        std::getline(ss, pais, ';');
        std::getline(ss, fechaInsc, ';');

        if (!username.empty()) {
            Usuario* nuevo = new Usuario(username, fechaNac, email, foto, ciudad, pais, fechaInsc);
            agregarUsuario(nuevo);
        }
    }
    archivo.close();
    return true;
}

bool RedSocial::cargarRelacionesDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string u1, u2;

        std::getline(ss, u1, ';');
        std::getline(ss, u2, ';');

        if (!u1.empty() && !u2.empty()) {
            registrarAmistad(u1, u2);
        }
    }
    archivo.close();
    return true;
}
