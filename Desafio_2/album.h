#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <cstddef>
#include "foto.h"
#include "ListaDinamica.h"


class Album{
private:
    std::string nombre;
    std::string rutaRaiz;
    std::string privacidad;

    // lista que contiene punteros a los objetos Foto
    ListaDinamica<Foto*> fotos;
    //indice para el carrusel de fotos
    int indiceActual;
    // atributo de auditoria
    size_t bytesOcupados;

public:
    //Construtores y destructor
    Album();
    Album(std::string nombre, std::string rutaRaiz, std::string privacidad);
    ~Album();

    //Getters y setters
    std::string getNombre() const;
    std::string getRuta() const;
    std::string getPrivacidad() const;
    int getIndice() const;

    void setPrivacidad(std::string nuevaPrivacidad);

    // Getters de auditoria
    size_t getMemoria() const;
    static const char* getLibreriasUsadas();

    //Gestion de las fotos
    void agregarFoto(Foto* nuevaFoto);
    int cantidadFotos() const;

    //Carrusel
    Foto* obtenerFotoActual();
    void fotoSiguiente();
    void fotoAnterior();

};

#endif // ALBUM_H
