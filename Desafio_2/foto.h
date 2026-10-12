#ifndef FOTO_H
#define FOTO_H

#include <string>
#include <cstddef>
using namespace std;

class Foto
{
private:
    string fechaCaptura;
    string rutaAbsoluta;
    int tamanoArchivoBytes;
    int likes;
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
    size_t getMemoria() const;

    void setLike();
    void setUnlike();
};

#endif // FOTO_H
