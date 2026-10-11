#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
using namespace std;


class Mensaje
{
private:
    string emisor;
    string receptor;
    string texto;
    long fechaenvio;
    bool leido;
    int longitudTexto;

public:
    Mensaje();
    Mensaje(const string& emisor, const string& receptor, const string& texto, long fechaenvio);
    ~Mensaje();

    //Getters ans setters
    string getEmisor() const;
    string getReceptor() const;
    string getTexto() const;
    long getFechaEnvio() const;
    bool estaleido() const;
    int getLongitudTexto() const;

    void setLeido(bool estado);
};

#endif // MENSAJE_H
