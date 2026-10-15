#ifndef RASTREADORMEMORIA_H
#define RASTREADORMEMORIA_H

#include <string>

class RastreadorMemoria {
private:
    static long iteracionesTotales;
    static size_t memoriaAsignadaBytes;
    static std::string libreriasEjecutadas; // Almacena el listado de dependencias activas

    //Constructor privado para que no se pueda instanciar la clase
    RastreadorMemoria () = delete;
    // Destructor privado no se puede destruir un objeto que no haya sido creado
    ~RastreadorMemoria() = delete;

public:
    static void IniciarSegmento(const std::string& modulosYLibrerias);
    static void RegistroIteracion();
    static void RegistroAsignacionMemoria(size_t bytes);
    static void RegistroLiberacionMemoria(size_t bytes);
    static void ReporteEficiencia();
};
#endif // RASTREADORMEMORIA_H
