#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include <cstddef>


class Mensaje
{
private:
    std::string emisor;
    std::string receptor;
    std::string texto;
    long fechaenvio;
    bool leido;
    //Atributo de auditoria
    size_t bytesOcupados;

public:
    Mensaje();
    Mensaje(const std::string& emisor, const std::string& receptor, const std::string& texto, long fechaenvio);
    ~Mensaje();

    //Getters ans setters
    std::string getEmisor() const;
    std::string getReceptor() const;
    std::string getTexto() const;
    long getFechaEnvio() const;
    bool estaleido() const;
    void setLeido(bool estado);

    //getters para auditoria de memoria y librerias
    size_t getMemoria() const;  //Nos da la memoria ocupada por el objeto
    static const char* getLibreriasUsadas();

};

#endif // MENSAJE_H
