#ifndef FECHA_H
#define FECHA_H

#include <string>
#include <cstddef>


class Fecha {
private:
    // Es una clase que el usuario no puede llamar solo se utiliza internamente
    Fecha() = delete;
    ~Fecha() = delete;

    // Métodos auxiliares manuales para emular funciones de conversión
    static int stringAEnteroManual(const std::string& cadena, int inicio, int fin);
    static std::string enteroAStringManual(int numero);

public:
    //Obtiene la fecha actual
    static long obtenerFechaActual();

    // Convierte una cadena "AAAA-MM-DD" o "AAAA/MM/DD" a long numérico AAAAAMMDD
    static long cadenaALongInvertido(const std::string& fechaCadena);

    //Convierte una cadena "AAAAMMDD" a long AAAAMMDD
    static long StringNumtoLong(const std::string& cadenaNumeros);

    // Convierte el long numérico AAAAAMMDD a formato amigable con el usuario
    static std::string longAFormatoHumano(long fechaLong);

    // metodo de auditoria
    static const char* getLibreriasUsadas();
};

#endif // FECHA_H