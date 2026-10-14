#ifndef FECHA_H
#define FECHA_H

#include <string>
#include <cstddef>

using namespace std;


class Fecha {
private:
    // Es una clase que el usuario no puede llamar solo se utiliza internamente
    Fecha() = delete;
    ~Fecha() = delete;

    // Métodos auxiliares manuales para emular funciones de conversión
    static int stringAEnteroManual(const string& cadena, int inicio, int fin);
    static string enteroAStringManual(int numero);

public:
    // Convierte una cadena "AAAA-MM-DD" o "AAAA/MM/DD" a long numérico AAAAAMMDD
    static long cadenaALongInvertido(const string& fechaCadena);

    // Convierte el long numérico AAAAAMMDD a formato amigable con el usuario
    static string longAFormatoHumano(long fechaLong);

    // metodo de auditoria
    static const char* getLibreriasUsadas();
};

#endif // FECHA_H