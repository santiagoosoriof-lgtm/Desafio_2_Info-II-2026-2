#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <cstddef>
#include <foto.h>
#include <ListaDinamica.h>
using namespace std;

class Album{
private:
    string nombre;
    string rutaRaiz;
    string privacidad;

    // lista que contiene punteros a los objetos Foto
    ListaDinamica<Foto*> fotos;
    //indice para el carrusel de fotos
    int indiceActual;
    // atributo de auditoria
    size_t bytesOcupados;

public:
    //Construtores y destructor
    Album();
    Album(string nombre, string rutaRaiz, string privacidad);
    ~Album();

    //Getters y setters
    string getNombre() const;
    string getRuta() const;
    string getPrivacidad() const;
    int getIndice() const;

    void setPrivacidad(string nuevaPrivacidad);

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
