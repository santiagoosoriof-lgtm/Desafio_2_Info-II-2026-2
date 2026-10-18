#include "mensajeria.h"
#include "RastreadorMemoria.h"
#include "fecha.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//constructor y destructor
Mensajeria::Mensajeria() {
    this-> bytesOcupados =sizeof(Mensajeria);
    RastreadorMemoria::RegistroAsignacionMemoria(this->bytesOcupados);
}
Mensajeria::~Mensajeria(){

    //borramos cada uno de los mensajes en la lista del usuario
    for(int i = 0; i < bandejaMensajes.obtenerTamano(); i++){
        Mensaje* msn = bandejaMensajes.obtener(i);
        if(msn != nullptr){
            RastreadorMemoria::RegistroLiberacionMemoria(msn->getMemoria());
            delete msn;
        }

    }
    RastreadorMemoria::RegistroLiberacionMemoria(this->bytesOcupados);
}

//metodos de auditoria
size_t Mensajeria::getMemoria() const{ return this-> bytesOcupados;}

const char* Mensajeria::getLibreriasUsadas(){
    return "mensajeria.h, RastreadorMemoria.h, <iostream>,"
           " <string>, <cstddef>, ListaDinamica.h, mensaje.h";}

//metodos de la mensajeria

bool Mensajeria::cargarArchivoMensajes(const string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        RastreadorMemoria::RegistroIteracion();
        if (linea.empty()) continue;

        stringstream ss(linea);
        string emisor, receptor, texto, fechaStr, leidoStr;

        // Identificamos los datos de la linea con el separador punto y coma ';'
        getline(ss, emisor, ';');
        getline(ss, receptor, ';');
        getline(ss, texto, ';');
        getline(ss, fechaStr, ';');
        getline(ss, leidoStr, ';');

        if (!emisor.empty() && !receptor.empty()) {
            // Convertimos la fecha a long
            long fechaLong = Fecha::StringNumtoLong(fechaStr);

            // Reconstruimos todos los mensajes en la RAM
            Mensaje* msn = new Mensaje(emisor, receptor, texto, fechaLong);
            msn->setLeido(leidoStr == "1");

            RastreadorMemoria::RegistroAsignacionMemoria(msn->getMemoria());
            bandejaMensajes.agregarAlFinal(msn);
        }
    }
    archivo.close();
    return true;
}

void Mensajeria::enviarMensaje(Usuario* emisor, Usuario* receptor, const string& texto, long fechaEnvio){
    if(emisor == nullptr || receptor == nullptr ){
        cout << "Debe ingresar un receptor" << endl;
        return;
    }

    //creamos el mensaje
    Mensaje* nuevomsn = new Mensaje(emisor-> getUsername(), receptor-> getUsername(), texto, fechaEnvio);
    RastreadorMemoria::RegistroAsignacionMemoria(nuevomsn->getMemoria());

    //insertar el mensaje en la lista
    bandejaMensajes.agregarAlFinal(nuevomsn);
    emisor->getMensajesEnviados().agregarAlFinal(nuevomsn);
    receptor->getMensajesRecibidos().agregarAlFinal(nuevomsn);

}
//obtener los nuevos mensajes de la bandeja de entrada
void Mensajeria::getNuevosmsn(const string& usuarioReceptor){
    cout << "\n--- MENSAJES NUEVOS (NO LEIDOS) PARA: " << usuarioReceptor << " ---\n";

    int totalmsn = bandejaMensajes.obtenerTamano();

    // Arreglos temporales para agrupar sin repetir
    string* emisoresUnicos = new string[totalmsn];
    int* conteoNuevos = new int[totalmsn];
    int cantidadEmisores = 0;

    size_t bytesTemporales = (sizeof(string) * totalmsn) + (sizeof(int) * totalmsn);
    RastreadorMemoria::RegistroAsignacionMemoria(bytesTemporales);

    for(int i = 0; i < totalmsn; i++){
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if( msn->getReceptor() == usuarioReceptor && !msn->estaleido()){
            string emisorActual = msn->getEmisor();
            bool encontrado = false;

            //buscamos si el emisor ya esta en el arreglo
            for (int j = 0; j < cantidadEmisores; j++) {
                RastreadorMemoria::RegistroIteracion();
                if (emisoresUnicos[j] == emisorActual) {
                    conteoNuevos[j]++;
                    encontrado = true;
                    break;
                }
            }

            // si el emisor lo vemos por primera vez
            if (encontrado == false) {
                emisoresUnicos[cantidadEmisores] = emisorActual;
                conteoNuevos[cantidadEmisores] = 1;
                cantidadEmisores++;
            }
        }
    }
        if(cantidadEmisores==0){
            cout << "No tienes mensajes nuevos en la bandeja de entrada." << endl;
        }else{
            for (int i = 0; i < cantidadEmisores; i++) {
                cout << " -> " << emisoresUnicos[i] << " (" << conteoNuevos[i] << " mensajes nuevos)\n";
            }

        }

        RastreadorMemoria::RegistroLiberacionMemoria(bytesTemporales);
        delete[] emisoresUnicos;
        delete[] conteoNuevos;

    }


void Mensajeria::verBandejaCompleta(const string& usuarioReceptor){
    cout << "\n--- BANDEJA DE ENTRADA PARA: " << usuarioReceptor << " ---\n";

    int totalmsn = bandejaMensajes.obtenerTamano();
    string* emisoresProcesados = new string[totalmsn];
    int cantidadProcesados = 0;

    size_t bytesTemporales = sizeof(string) * totalmsn;
    RastreadorMemoria::RegistroAsignacionMemoria(bytesTemporales);
    int totalMostrados = 0;

    for (int i = 0; i < totalmsn; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn->getReceptor() == usuarioReceptor) {
            string emisorActual = msn->getEmisor();
            bool yaImpreso = false;

            for (int j = 0; j < cantidadProcesados; j++) {
                RastreadorMemoria::RegistroIteracion();
                if (emisoresProcesados[j] == emisorActual) {
                    yaImpreso = true;
                    break;
                }
            }

            if (!yaImpreso) {
                cout << "\n CHAT CON: " << emisorActual
                     << "\n----------------------------------------\n";
                emisoresProcesados[cantidadProcesados] = emisorActual;
                cantidadProcesados++;

                for (int k = 0; k < totalmsn; k++) {
                    RastreadorMemoria::RegistroIteracion();
                    Mensaje* subMsn = bandejaMensajes.obtener(k);
                    if (subMsn->getReceptor() == usuarioReceptor && subMsn->getEmisor() == emisorActual) {
                        // Imprimimos fecha y estatus :Nuevo o Leído
                        cout << "   * [" << Fecha::longAFormatoHumano(subMsn->getFechaEnvio()) << "]\n";
                        cout << "   Mensaje : " << subMsn->getTexto()
                             << " [" << (subMsn->estaleido() ? "Leido" : "NUEVO") << "]\n";

                        //  se marca como leido: Porque el usuario ya lo vio en pantalla
                        subMsn->setLeido(true);
                        totalMostrados++;
                    }
                }
            }
        }
    }

    if (totalMostrados == 0) {
        cout << "No tienes mensajes registrados en tu bandeja de entrada.\n";
    }

    RastreadorMemoria::RegistroLiberacionMemoria(bytesTemporales);
    delete[] emisoresProcesados;

}
void Mensajeria::buscarporFecha(const string& usuarioReceptor, long fechaInicio, long fechaFin){
    cout << "\n--- HISTORIAL DE MENSAJES ENTRE: " << Fecha::longAFormatoHumano(fechaInicio)
         << " Y " << Fecha::longAFormatoHumano(fechaFin) << " ---\n";
    cout <<"--- PARA EL USUARIO: " << usuarioReceptor << " ---\n";
    int total = bandejaMensajes.obtenerTamano();
    int encontrados = 0;

    for (int i = 0; i < total; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msg = bandejaMensajes.obtener(i);
        long timestamp = msg->getFechaEnvio();

        if (msg->getReceptor() == usuarioReceptor && timestamp >= fechaInicio && timestamp <= fechaFin) {
            cout << "Emisor : [" << msg->getEmisor() << "] Fecha: "
                 << Fecha::longAFormatoHumano(timestamp) << ". Mensaje : " << msg->getTexto() << "\n";
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron mensajes registrados en ese rango de fechas.\n";
    }
}

bool Mensajeria::guardarMensajesEnArchivo(const std::string& rutaArchivo) {
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de mensajes.\n";
        return false;
    }

    int total = bandejaMensajes.obtenerTamano();
    for (int i = 0; i < total; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn != nullptr) {
            // Escribimos con el formato estandarizado: emisor;receptor;texto;fecha;leido
            archivo << msn->getEmisor() << ";"
                    << msn->getReceptor() << ";"
                    << msn->getTexto() << ";"
                    << msn->getFechaEnvio() << ";"
                    << (msn->estaleido() ? "1" : "0") << "\n";
        }
    }
    archivo.close();
    return true;
}