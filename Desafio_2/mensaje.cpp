#include "mensaje.h"
using namespace std;

// constructor vacio
Mensaje::Mensaje() {
    this ->emisor = "";
    this ->receptor = "";
    this ->texto = "";
    this ->fechaenvio = 0;
    this ->leido = false;
    this ->longitudTexto = 0;
}
//constructor completo
Mensaje::Mensaje(const string& emisor, const string& receptor, const string& usertexto, long fechaenvio){
    this ->emisor = emisor;
    this ->receptor = receptor;
    this ->fechaenvio = fechaenvio;
    this ->leido = false;

    this ->texto = ""; // iniciamos en vacio
    this ->longitudTexto = 0;
    //el texto ingresado por el usuario se recorta si es muy largo y se asigna un valor al atributo longitud

    while(usertexto[this->longitudTexto] != '\0' && this->longitudTexto < 256){
        this-> texto += usertexto[this->longitudTexto];
        this->longitudTexto++;
    }
}
//Destructor de la clase
Mensaje::~Mensaje() {}

string Mensaje::getEmisor() const{
    return this->emisor;
}
string Mensaje::getReceptor() const{
    return this->receptor;
}
string Mensaje::getTexto() const{
    return this->texto;
}
long Mensaje::getFechaEnvio() const{
    return this->fechaenvio;
}
bool Mensaje::estaleido() const{
    return this->leido;
}
int Mensaje::getLongitudTexto() const{
    return this->longitudTexto;
}
void Mensaje::setLeido(bool estado){
    this->leido=estado;
}
