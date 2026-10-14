#include "Usuario.h"
#include "RastreadorMemoria.h"

// Constructor por defecto
Usuario::Usuario()
    : username(""), fechaNacimiento(""), email(""), fotoPerfilRuta(""),
    ciudad(""), pais(""), fechaInscripcion(""), bytesOcupados(sizeof(Usuario)) {}

// Constructor parametrizado
Usuario::Usuario(std::string username, std::string fechaNacimiento, std::string email,
                 std::string fotoPerfilRuta, std::string ciudad, std::string pais, std::string fechaInscripcion)
    : username(username), fechaNacimiento(fechaNacimiento), email(email),
    fotoPerfilRuta(fotoPerfilRuta), ciudad(ciudad), pais(pais), fechaInscripcion(fechaInscripcion) {

    int caracteresTotales = 0;
    std::string campos[] = {username, fechaNacimiento, email, fotoPerfilRuta, ciudad, pais, fechaInscripcion};

    for(int i = 0; i < 7; i++){
        int j=0;
        while(campos[i][j]!= '\0'){
            caracteresTotales++;
            j++;
        }
    }
    this-> bytesOcupados = sizeof(Usuario) + caracteresTotales;

}


// Constructor de copia
Usuario::Usuario(const Usuario& otro) {
    username = otro.username;
    fechaNacimiento = otro.fechaNacimiento;
    email = otro.email;
    fotoPerfilRuta = otro.fotoPerfilRuta;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;

    // Copiamos los punteros a todas las listas del objeto
    for (int i = 0; i < otro.amigos.obtenerTamano(); i++) {
        amigos.agregarAlFinal(otro.amigos.obtener(i));
    }
    for (int i = 0; i < otro.solicitudesPendientes.obtenerTamano(); i++) {
        solicitudesPendientes.agregarAlFinal(otro.solicitudesPendientes.obtener(i));
    }
    for (int i = 0; i < otro.albumes.obtenerTamano(); i++) {
        albumes.agregarAlFinal(otro.albumes.obtener(i));
    }
    for (int i = 0; i < otro.mensajesEnviados.obtenerTamano(); i++) {
        mensajesEnviados.agregarAlFinal(otro.mensajesEnviados.obtener(i));
    }
    for (int i = 0; i < otro.mensajesRecibidos.obtenerTamano(); i++) {
        mensajesRecibidos.agregarAlFinal(otro.mensajesRecibidos.obtener(i));
    }

    //  Asignamos el mismo tamaño en memoria que calculó el objeto original
    this->bytesOcupados = otro.bytesOcupados;
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

int Usuario::getCantidadAlbums() const{
    return albumes.obtenerTamano();
}
ListaDinamica<Album*>& Usuario::getAlbumes() {
    return albumes;
}

const ListaDinamica<Album*>& Usuario::getAlbumes() const {
    return albumes;
}

ListaDinamica<Usuario*>& Usuario::getSolicitudesPendientes() {
    return solicitudesPendientes;
}

const ListaDinamica<Usuario*>& Usuario::getSolicitudesPendientes() const {
    return solicitudesPendientes;
}
ListaDinamica<Mensaje*>& Usuario::getMensajesEnviados() {
    return mensajesEnviados;
}

const ListaDinamica<Mensaje*>& Usuario::getMensajesEnviados() const {
    return mensajesEnviados;
}

ListaDinamica<Mensaje*>& Usuario::getMensajesRecibidos() {
    return mensajesRecibidos;
}

const ListaDinamica<Mensaje*>& Usuario::getMensajesRecibidos() const {
    return mensajesRecibidos;
}



// Getters de auditoria
size_t Usuario::getMemoria() const{ return this->bytesOcupados;}
const char* Usuario::getLibreriasUsadas(){
    return "Usuario.h, ListaDinamica.h, mensaje.h, RastreadorMemoria.h, <string>, <cstddef>";}

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
        RastreadorMemoria::RegistroIteracion();
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