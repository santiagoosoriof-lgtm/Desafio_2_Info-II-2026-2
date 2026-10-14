#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <cstddef>
#include "ListaDinamica.h"
#include "mensaje.h"

// Declaraciones adelantadas para evitar errores de inclusion circular
class Album;
class Mensaje;

class Usuario {
private:
    std::string username;
    std::string fechaNacimiento;
    std::string email;
    std::string fotoPerfilRuta;
    std::string ciudad;
    std::string pais;
    std::string fechaInscripcion;

    // Listas relacionales usando nuestra propia ListaDinamica de punteros
    ListaDinamica<Usuario*> amigos;
    ListaDinamica<Usuario*> solicitudesPendientes;
    ListaDinamica<Album*> albumes;
    ListaDinamica<Mensaje*> mensajesEnviados;
    ListaDinamica<Mensaje*> mensajesRecibidos;

    // Atributos de auditoria
    size_t bytesOcupados;

public:
    // Constructores
    Usuario();
    Usuario(std::string username, std::string fechaNacimiento, std::string email,
            std::string fotoPerfilRuta, std::string ciudad, std::string pais, std::string fechaInscripcion);

    // Constructor de Copia
    Usuario(const Usuario& otro);

    // Destructor
    ~Usuario();

    // Getters
    std::string getUsername() const;
    std::string getFechaNacimiento() const;
    std::string getEmail() const;
    std::string getFotoPerfilRuta() const;
    std::string getCiudad() const;
    std::string getPais() const;
    std::string getFechaInscripcion() const;

    int getCantidadAlbums() const;
    ListaDinamica<Album*> &getAlbumes();
    const ListaDinamica<Album*> &getAlbumes() const;
    ListaDinamica<Usuario*>& getSolicitudesPendientes();
    const ListaDinamica<Usuario*>& getSolicitudesPendientes() const;

    // Setters
    void setFotoPerfilRuta(const std::string& ruta);
    void setCiudad(const std::string& c);
    void setPais(const std::string& p);

    // Gestion de Amigos
    void agregarAmigo(Usuario* nuevoAmigo);
    void agregarSolicitud(Usuario* solicitante);
    bool esAmigoDe(Usuario* otro) const;

    ListaDinamica<Usuario*>& getAmigos();
    const ListaDinamica<Usuario*>& getAmigos() const;

    // Sobrecarga de Operadores == y != (Comparan por Username unico)
    bool operator==(const Usuario& otro) const;
    bool operator!=(const Usuario& otro) const;

    //metodos de auditoria
    static const char* getLibreriasUsadas();
    size_t getMemoria() const;
};

#endif
