#ifndef FOTO_H
#define FOTO_H

#include <string>
#include <cstddef>
using namespace std;

class Album; //Declaracion adelantada

class Foto{
private:
    string fechaCaptura;
    string rutaAbsoluta;
    int tamanoArchivoBytes;
    int likes;
    Album* albumDueno; //Album que contiene la foto
    //Atributo de auditoria
    size_t bytesOcupados;

public:
    Foto();
    Foto(string fechaCaptura, string rutaAbsoluta, int tamanoArchivoBytes);
    ~Foto();

    //Getters y setters


    string getFechaCaptura() const;
    string getRutaAbsoluta() const;
    int getTamanoBytes() const;
    int getLikes() const;
    Album* getAlbumDueno() const;

    void setLike();
    void setUnlike();
    void setAlbumDueno(Album* nuevoDueno);

    //getters para auditoria de memoria y librerias
    size_t getMemoria() const;
    static const char* getLibreriasUsadas();

};

#endif // FOTO_H
