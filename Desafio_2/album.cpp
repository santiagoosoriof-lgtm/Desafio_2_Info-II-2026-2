#include "album.h"

Album::Album() {
    this-> nombre ="Album generico";
    this-> rutaRaiz ="";
    this-> privacidad="Privado";
    this-> indiceActual=-1;
    this-> bytesOcupados = sizeof(Album);
}

Album::Album(std::string nombre, std::string rutaRaiz, std::string privacidad){
    this-> nombre =nombre;
    this-> rutaRaiz = rutaRaiz;
    this-> privacidad= privacidad;
    this-> indiceActual=-1;

    size_t caracterescadenas = nombre.length() + rutaRaiz.length() + privacidad.length();

    this-> bytesOcupados = sizeof(Album) + caracterescadenas;
}

Album::~Album(){
    int totalFotos = fotos.obtenerTamano();
    for (int i = 0; i < totalFotos; i++) {
        delete fotos.obtener(i); // Libera la memoria de cada objeto Foto
    }
    fotos.limpiar(); // Limpia los nodos de la lista dinámica
}

//Getters y setters
std::string Album::getNombre() const{return this->nombre;}
std::string Album::getRuta() const{return this-> rutaRaiz;}
std::string Album::getPrivacidad() const{return this-> privacidad;}
int Album::getIndice() const{return this-> indiceActual;}

void Album::setPrivacidad(std::string nuevaPrivacidad){ this-> privacidad = nuevaPrivacidad;}

// Getters de auditoria
size_t Album::getMemoria() const{ return this-> bytesOcupados;}
const char* Album::getLibreriasUsadas(){return "album.h, ListaDinamica.h, foto.h, <string>, <cstddef>";}

//Gestion de las fotos
void Album::agregarFoto(Foto* nuevaFoto){
    if(nuevaFoto == nullptr)return;

    //verificamos que la foto no exista en otro album
    if(nuevaFoto->getAlbumDueno() != nullptr){
        std::cout << "\n[ERROR] La foto con ruta [" << nuevaFoto->getRutaAbsoluta()
        << "] ya pertenece al album '" << nuevaFoto->getAlbumDueno()->getNombre() << "'.\n";
        std::cout << "No se puede duplicar en el album '" << this->nombre << "'.\n\n";
        return;
    }

    this-> fotos.agregarAlFinal(nuevaFoto);  //Agregamos la foto al album
    nuevaFoto->setAlbumDueno(this);  //le otorgamos un valor al atributo album dueño de la foto

    //se actualiza el indice de la foto en el album
        if(this->indiceActual == -1){
            this-> indiceActual = 0;
        }

}
int Album::cantidadFotos() const{
    return this->fotos.obtenerTamano();
}

//Carrusel
Foto* Album::obtenerFotoActual(){
    if(this->indiceActual == -1 || this->fotos.obtenerTamano() == 0){
        return nullptr;
    }
    return this-> fotos.obtener(this->indiceActual);
}

void Album::fotoSiguiente(){
    int total = this -> fotos.obtenerTamano();
    if(total <= 1) return;
    // se garantiza el caso en el que estamos en la ultima foto y volvemos a la primera
    this-> indiceActual = (this->indiceActual + 1) % total;
}
void Album::fotoAnterior(){
    int total = this -> fotos.obtenerTamano();
    if(total <= 1) return;
    //se garantriza el caso en el que estamos en la primer foto y retrocedemos --> vamos a la ultima foto
    this-> indiceActual = (this->indiceActual - 1 + total) % total;
}
