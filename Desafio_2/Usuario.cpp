#include "Usuario.h"

// Constructor por defecto
Usuario::Usuario()
    : username(""), fechaNacimiento(""), email(""), fotoPerfilRuta(""),
    ciudad(""), pais(""), fechaInscripcion("") {}

// Constructor parametrizado
Usuario::Usuario(std::string username, std::string fechaNacimiento, std::string email,
                 std::string fotoPerfilRuta, std::string ciudad, std::string pais, std::string fechaInscripcion)
    : username(username), fechaNacimiento(fechaNacimiento), email(email),
    fotoPerfilRuta(fotoPerfilRuta), ciudad(ciudad), pais(pais), fechaInscripcion(fechaInscripcion) {}

// Constructor de copia
Usuario::Usuario(const Usuario& otro) {
    username = otro.username;
    fechaNacimiento = otro.fechaNacimiento;
    email = otro.email;
    fotoPerfilRuta = otro.fotoPerfilRuta;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;

    // Copiamos los punteros a amigos de la lista
    for (int i = 0; i < otro.amigos.obtenerTamano(); i++) {
        amigos.agregarAlFinal(otro.amigos.obtener(i));
    }
}

// Destructor
Usuario::~Usuario() {
    // La liberacion principal de memoria de los usuarios la hace la RedSocial
}

// Getters
std::string Usuario::getUsername() const { return username; }
std::string Usuario::getFechaNacimiento() const { return fechaNacimiento; }
std::string Usuario::getEmail() const { return email; }
std::string Usuario::getFotoPerfilRuta() const { return fotoPerfilRuta; }
std::string Usuario::getCiudad() const { return ciudad; }
std::string Usuario::getPais() const { return pais; }
std::string Usuario::getFechaInscripcion() const { return fechaInscripcion; }

// Setters
void Usuario::setFotoPerfilRuta(const std::string& ruta) { fotoPerfilRuta = ruta; }
void Usuario::setCiudad(const std::string& c) { ciudad = c; }
void Usuario::setPais(const std::string& p) { pais = p; }

// Gestion de Amigos
void Usuario::agregarAmigo(Usuario* nuevoAmigo) {
    if (nuevoAmigo != nullptr && !esAmigoDe(nuevoAmigo)) {
        amigos.agregarAlFinal(nuevoAmigo);
    }
}

void Usuario::agregarSolicitud(Usuario* solicitante) {
    if (solicitante != nullptr) {
        solicitudesPendientes.agregarAlFinal(solicitante);
    }
}

bool Usuario::esAmigoDe(Usuario* otro) const {
    if (otro == nullptr) return false;
    for (int i = 0; i < amigos.obtenerTamano(); i++) {
        if (*amigos.obtener(i) == *otro) {
            return true;
        }
    }
    return false;
}

ListaDinamica<Usuario*>& Usuario::getAmigos() { return amigos; }
const ListaDinamica<Usuario*>& Usuario::getAmigos() const { return amigos; }

// Sobrecarga de Operadores == y !=
bool Usuario::operator==(const Usuario& otro) const {
    return username == otro.username;
}

bool Usuario::operator!=(const Usuario& otro) const {
    return !(*this == otro);
}