#include "foto.h"

Foto::Foto() {
    this-> fechaCaptura = "";
    this->rutaAbsoluta = "";
    this-> tamanoArchivoBytes = 0;
    this-> likes = 0;
    this-> bytesOcupados = sizeof(Foto);
}
Foto::Foto(string fechaCaptura, string rutaAbsoluta, int tamanoArchivoBytes){
    this-> fechaCaptura = fechaCaptura;
    this->rutaAbsoluta = rutaAbsoluta;
    this-> tamanoArchivoBytes = tamanoArchivoBytes;
    this-> likes = 0;

    int caracteresFecha = 0;
    while(fechaCaptura[caracteresFecha] != '\0'){
        caracteresFecha++;
    }

    int caracteresRuta = 0;
    while(rutaAbsoluta[caracteresRuta] != '\0'){
        caracteresRuta++;
    }

    this-> bytesOcupados = sizeof(Foto) + caracteresFecha + caracteresRuta;
}
Foto::~Foto(){

}

//Getters y setters

string Foto::getFechaCaptura()const { return this->fechaCaptura;}
string Foto::getRutaAbsoluta() const{return this->rutaAbsoluta;}
int Foto::getTamanoBytes() const{return this->tamanoArchivoBytes;}
int Foto::getLikes() const{return this->likes;}
size_t Foto::getMemoria() const{return this -> bytesOcupados;}

void Foto::setLike(){ this-> likes++;}
void Foto::setUnlike(){
    if(this-> likes > 0){
        this->likes--;
    }
}