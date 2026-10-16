#include "foto.h"

Foto::Foto() {
    this-> fechaCaptura = "";
    this->rutaAbsoluta = "";
    this-> tamanoArchivoBytes = 0;
    this-> likes = 0;
    this-> bytesOcupados = sizeof(Foto);
}
Foto::Foto(std::string fechaCaptura, std::string rutaAbsoluta, int tamanoArchivoBytes){
    this-> fechaCaptura = fechaCaptura;
    this->rutaAbsoluta = rutaAbsoluta;
    this-> tamanoArchivoBytes = tamanoArchivoBytes;
    this-> likes = 0;
    this-> albumDueno = nullptr;

    size_t caracteresFecha = fechaCaptura.length();
    size_t caracteresRuta = rutaAbsoluta.length();

    this-> bytesOcupados = sizeof(Foto) + caracteresFecha + caracteresRuta;
}
Foto::~Foto(){

}

//Getters y setters


std::string Foto::getFechaCaptura()const { return this->fechaCaptura;}
std::string Foto::getRutaAbsoluta() const{return this->rutaAbsoluta;}
int Foto::getTamanoBytes() const{return this->tamanoArchivoBytes;}
int Foto::getLikes() const{return this->likes;}
Album* Foto::getAlbumDueno() const{return albumDueno;}

void Foto::setLike(){ this-> likes++;}
void Foto::setUnlike(){
    if(this-> likes > 0){
        this->likes--;}
    }
void Foto::setAlbumDueno(Album *nuevoDueno){albumDueno= nuevoDueno;}

// Implementacion getteres auditoria
size_t Foto::getMemoria() const{return this -> bytesOcupados;}
const char* Foto::getLibreriasUsadas(){return "foto.h, <string>, <cstddef>";}
