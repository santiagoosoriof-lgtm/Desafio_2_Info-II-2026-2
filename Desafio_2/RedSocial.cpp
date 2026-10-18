#include "RedSocial.h"
#include "RastreadorMemoria.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

// Constructor: Inicializa la red y audita su tamaño base
RedSocial::RedSocial(int capacidad) : capacidadMaxima(capacidad) {
    this->bytesOcupados = sizeof(RedSocial);
    RastreadorMemoria::RegistroAsignacionMemoria(this->bytesOcupados);
}

// Destructor: Libera los usuarios y la lista dinámica
RedSocial::~RedSocial() {
    RastreadorMemoria::RegistroLiberacionMemoria(this->bytesOcupados);

    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        Usuario* u = usuarios.obtener(i);
        if (u != nullptr) {
            RastreadorMemoria::RegistroLiberacionMemoria(u->getMemoria());
            delete u;
        }
    }
    usuarios.limpiar();
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
        RastreadorMemoria::RegistroIteracion();
        if (usuarios.obtener(i)->getUsername() == username) {
            return usuarios.obtener(i);
        }
    }
    return nullptr;
}

int RedSocial::obtenerIndiceUsuario(const std::string& username) const {
    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        RastreadorMemoria::RegistroIteracion();
        if (usuarios.obtener(i)->getUsername() == username) {
            return i;
        }
    }
    return -1;
}

// Registro de amistad bidireccional en las listas de adyacencia de cada Usuario
bool RedSocial::registrarAmistad(const std::string& user1, const std::string& user2) {
    Usuario* u1 = buscarUsuario(user1);
    Usuario* u2 = buscarUsuario(user2);

    if (u1 != nullptr && u2 != nullptr && u1 != u2) {
        u1->agregarAmigo(u2);
        u2->agregarAmigo(u1);
        return true;
    }
    return false;
}

bool RedSocial::sonAmigos(const std::string& user1, const std::string& user2) const {
    RedSocial* self = const_cast<RedSocial*>(this);
    Usuario* u1 = self->buscarUsuario(user1);
    Usuario* u2 = self->buscarUsuario(user2);

    if (u1 != nullptr && u2 != nullptr) {
        return u1->esAmigoDe(u2);
    }
    return false;
}

// Contar amigos en común recorriendo la lista de amigos reales O(amigos)
int RedSocial::contarAmigosEnComun(const std::string& user1, const std::string& user2) const {
    RedSocial* self = const_cast<RedSocial*>(this);
    Usuario* u1 = self->buscarUsuario(user1);
    Usuario* u2 = self->buscarUsuario(user2);

    if (u1 == nullptr || u2 == nullptr) return 0;

    int comun = 0;
    ListaDinamica<Usuario*>& amigosU1 = u1->getAmigos();

    for (int i = 0; i < amigosU1.obtenerTamano(); i++) {
        RastreadorMemoria::RegistroIteracion();
        Usuario* amigoDeU1 = amigosU1.obtener(i);
        if (u2->esAmigoDe(amigoDeU1)) {
            comun++;
        }
    }
    return comun;
}

// Algoritmo 1: Sorteo de mejores amigos con probabilidades ponderadas (1.0x, 2.5x, 3.5x)
ListaDinamica<Usuario*> RedSocial::obtenerMejoresAmigosSorteados(const std::string& username) {
    ListaDinamica<Usuario*> seleccionados;
    Usuario* u = buscarUsuario(username);
    if (u == nullptr) return seleccionados;

    ListaDinamica<Usuario*>& amigos = u->getAmigos();
    int totalAmigos = amigos.obtenerTamano();
    if (totalAmigos == 0) return seleccionados;

    double* pesos = new double[totalAmigos];
    double sumaTotalPesos = 0.0;

    size_t bytesTemporales = (sizeof(double) * totalAmigos) + (sizeof(bool) * totalAmigos);
    RastreadorMemoria::RegistroAsignacionMemoria(bytesTemporales);

    for (int i = 0; i < totalAmigos; i++) {
        RastreadorMemoria::RegistroIteracion();
        Usuario* amigo = amigos.obtener(i);
        int comunes = contarAmigosEnComun(username, amigo->getUsername());

        if (comunes <= 10) {
            pesos[i] = 1.0;
        } else if (comunes >= 11 && comunes <= 20) {
            pesos[i] = 2.5;
        } else {
            pesos[i] = 3.5;
        }
        sumaTotalPesos += pesos[i];
    }

    bool* yaSeleccionado = new bool[totalAmigos];
    for (int i = 0; i < totalAmigos; i++) yaSeleccionado[i] = false;

    int objetivosASeleccionar = (totalAmigos < 3) ? totalAmigos : 3;

    for (int s = 0; s < objetivosASeleccionar; s++) {
        RastreadorMemoria::RegistroIteracion();
        double dardo = (rand() % 10000 / 10000.0) * sumaTotalPesos;
        double acumulado = 0.0;

        for (int i = 0; i < totalAmigos; i++) {
            RastreadorMemoria::RegistroIteracion();
            if (yaSeleccionado[i]) continue;

            acumulado += pesos[i];
            if (dardo <= acumulado) {
                seleccionados.agregarAlFinal(amigos.obtener(i));
                yaSeleccionado[i] = true;
                sumaTotalPesos -= pesos[i];
                break;
            }
        }
    }

    RastreadorMemoria::RegistroLiberacionMemoria(bytesTemporales);
    delete[] pesos;
    delete[] yaSeleccionado;

    return seleccionados;
}

// Algoritmo 2: Ranking Top-K de sugerencias
ListaDinamica<Usuario*> RedSocial::obtenerSugerenciasK(const std::string& username, int k) {
    ListaDinamica<Usuario*> sugerencias;
    Usuario* uActivo = buscarUsuario(username);
    if (uActivo == nullptr) return sugerencias;

    for (int i = 0; i < usuarios.obtenerTamano(); i++) {
        RastreadorMemoria::RegistroIteracion();
        Usuario* candidato = usuarios.obtener(i);

        if (candidato != uActivo && !uActivo->esAmigoDe(candidato)) {
            int comun = contarAmigosEnComun(username, candidato->getUsername());
            if (comun > 0) {
                sugerencias.agregarAlFinal(candidato);
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
    std::ifstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        RastreadorMemoria::RegistroIteracion();
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
            if (buscarUsuario(username) != nullptr || obtenerCantidadUsuarios() >= capacidadMaxima) {
                continue;
            }

            Usuario* nuevo = new Usuario(username, fechaNac, email, foto, ciudad, pais, fechaInsc);
            agregarUsuario(nuevo);
            RastreadorMemoria::RegistroAsignacionMemoria(nuevo->getMemoria());
        }
    }
    archivo.close();
    return true;
}

bool RedSocial::cargarRelacionesDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        RastreadorMemoria::RegistroIteracion();
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

bool RedSocial::cargarSolicitudesDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        RastreadorMemoria::RegistroIteracion();
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string solicitanteStr, destinatarioStr;

        std::getline(ss, solicitanteStr, ';');
        std::getline(ss, destinatarioStr, ';');

        if (!solicitanteStr.empty() && !destinatarioStr.empty()) {
            Usuario* solicitante = buscarUsuario(solicitanteStr);
            Usuario* destinatario = buscarUsuario(destinatarioStr);

            if (solicitante != nullptr && destinatario != nullptr) {
                destinatario->agregarSolicitud(solicitante);
            }
        }
    }
    archivo.close();
    return true;
}

bool RedSocial::guardarSolicitudesEnArchivo(const std::string& rutaArchivo) {
    std::ofstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de solicitudes.\n";
        return false;
    }

    int totalUsuarios = usuarios.obtenerTamano();

    for (int i = 0; i < totalUsuarios; i++) {
        RastreadorMemoria::RegistroIteracion();
        Usuario* user = usuarios.obtener(i);

        if (user != nullptr) {
            ListaDinamica<Usuario*>& pendientes = user->getSolicitudesPendientes();
            int totalPendientes = pendientes.obtenerTamano();

            for (int j = 0; j < totalPendientes; j++) {
                RastreadorMemoria::RegistroIteracion();
                Usuario* solicitante = pendientes.obtener(j);

                if (solicitante != nullptr) {
                    archivo << solicitante->getUsername() << ";"
                            << user->getUsername() << "\n";
                }
            }
        }
    }
    archivo.close();
    return true;
}

bool RedSocial::guardarRelacionesEnArchivo(const std::string& rutaArchivo) {
    std::ofstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de relaciones.\n";
        return false;
    }

    int totalUsuarios = usuarios.obtenerTamano();

    for (int i = 0; i < totalUsuarios; i++) {
        Usuario* user = usuarios.obtener(i);
        if (user != nullptr) {
            ListaDinamica<Usuario*>& amigos = user->getAmigos();
            int totalAmigos = amigos.obtenerTamano();

            for (int j = 0; j < totalAmigos; j++) {
                Usuario* amigo = amigos.obtener(j);

                // Filtrado no trivial: se escribe A;B si user1 < user2 para evitar duplicados en el .txt
                if (amigo != nullptr && user->getUsername() < amigo->getUsername()) {
                    archivo << user->getUsername() << ";" << amigo->getUsername() << "\n";
                }
            }
        }
    }
    archivo.close();
    return true;
}

size_t RedSocial::getMemoria() const {
    return this->bytesOcupados;
}

const char* RedSocial::getLibreriasUsadas() {
    return "RedSocial.h, Usuario.h, ListaDinamica.h, RastreadorMemoria.h, <iostream>, <fstream>, <sstream>, <cstdlib>, <cstddef>";
}