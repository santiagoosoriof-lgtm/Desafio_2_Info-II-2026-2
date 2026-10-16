#include "mensaje.h"
using namespace std;

// constructor vacio
Mensaje::Mensaje() {
    this ->emisor = "";
    this ->receptor = "";
    this ->texto = "";
    this ->fechaenvio = 0;
    this ->leido = false;
    this ->bytesOcupados = sizeof(Mensaje); //Tamaño por defecto del objeto
}
//constructor completo
Mensaje::Mensaje(const string& emisor, const string& receptor, const string& usertexto, long fechaenvio){
    this ->emisor = emisor;
    this ->receptor = receptor;
    this ->fechaenvio = fechaenvio;
    this ->leido = false;

    this ->texto = ""; // iniciamos en vacio
    int longitudTexto = 0;
    size_t tamanoOriginal = usertexto.length();

    // El texto ingresado por el usuario se recorta si supera los 512 caracteres
    while (longitudTexto < tamanoOriginal && longitudTexto < 512) {
        this->texto += usertexto[longitudTexto];
        longitudTexto++;
    }
    this ->bytesOcupados = sizeof(Mensaje) + longitudTexto;

}
//Destructor de la clase
Mensaje::~Mensaje() {}

//Implementacion getters y setters
string Mensaje::getEmisor() const{return this->emisor;}
string Mensaje::getReceptor() const{return this->receptor;}
string Mensaje::getTexto() const{return this->texto;}
long Mensaje::getFechaEnvio() const{return this->fechaenvio;}
bool Mensaje::estaleido() const{return this->leido;}
void Mensaje::setLeido(bool estado){this->leido=estado;}

//Implementacion getters de auditoria
size_t Mensaje::getMemoria() const{return this->bytesOcupados;}
const char* Mensaje::getLibreriasUsadas(){return "mensaje.h, <string>, <cstdef> ";}
