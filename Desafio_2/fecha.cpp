#include "Fecha.h"
#include "RastreadorMemoria.h"

using namespace std;

// Convierte un segmento de un string a un numero entero de forma manual
int Fecha::stringAEnteroManual(const string& cadena, int inicio, int fin) {
    int resultado = 0;
    for (int i = inicio; i <= fin; i++) {
        RastreadorMemoria::RegistroIteracion();
        if (cadena[i] >= '0' && cadena[i] <= '9') {
            resultado = resultado * 10 + (cadena[i] - '0');
        }
    }
    return resultado;
}

// Convierte un numero entero a string
string Fecha::enteroAStringManual(int numero) {
    if (numero == 0) return "0";
    string resultado = "";
    int temporal = numero;
    while (temporal > 0) {
        RastreadorMemoria::RegistroIteracion();
        char digito = (temporal % 10) + '0';
        resultado = digito + resultado;
        temporal /= 10;
    }
    return resultado;
}

// Procesa una cadena "AAAA-MM-DD" y extrae el long AAAAMMDD
long Fecha::cadenaALongInvertido(const string& fechaCadena) {
    // Validamos que la cadena tenga contenido minimo
    if (fechaCadena[0] == '\0') return 19010101; // Fecha base por defecto

    // Extraemos manualmente ignorando los separadores '-' o '/'
    int anio = stringAEnteroManual(fechaCadena, 0, 3);
    int mes  = stringAEnteroManual(fechaCadena, 5, 6);
    int dia  = stringAEnteroManual(fechaCadena, 8, 9);

    // Construimos el numero invertido compuesto: (AAAA * 10000) + (MM * 100) + DD
    return (static_cast<long>(anio) * 10000) + (mes * 100) + dia; //20260000+1000+12-->20261012
}

// Transforma 20261013 en "13 de Octubre de 2026"
string Fecha::longAFormatoHumano(long fechaLong) {
    // Descomponemos el numero
    int anio = fechaLong / 10000;  //2026
    int mes  = (fechaLong % 10000) / 100; //1013/100
    int dia  = fechaLong % 100; //13

    std::string nombreMes = "";
    // Mapeo estructural de meses
    switch (mes) {
    case 1:  nombreMes = "Enero"; break;
    case 2:  nombreMes = "Febrero"; break;
    case 3:  nombreMes = "Marzo"; break;
    case 4:  nombreMes = "Abril"; break;
    case 5:  nombreMes = "Mayo"; break;
    case 6:  nombreMes = "Junio"; break;
    case 7:  nombreMes = "Julio"; break;
    case 8:  nombreMes = "Agosto"; break;
    case 9:  nombreMes = "Septiembre"; break;
    case 10: nombreMes = "Octubre"; break;
    case 11: nombreMes = "Noviembre"; break;
    case 12: nombreMes = "Diciembre"; break;
    default: nombreMes = "Enero"; break;
    }

    // Concatenamos el resultado
    string fechaEstetica = enteroAStringManual(dia) + " de " + nombreMes + " de " + enteroAStringManual(anio);
    return fechaEstetica;
}

const char* Fecha::getLibreriasUsadas() {
    return "Fecha.h, RastreadorMemoria.h, <string>, <cstddef>";
}
