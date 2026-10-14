#include "mensajeria.h"
#include "RastreadorMemoria.h"
#include "fecha.h"
#include <iostream>

//constructor y destructor
Mensajeria::Mensajeria() {
    this-> bytesOcupados =sizeof(Mensajeria);
    RastreadorMemoria::RegistroAsignacionMemoria(this->bytesOcupados);
}
Mensajeria::~Mensajeria(){

    RastreadorMemoria::RegistroLiberacionMemoria(this->bytesOcupados);

    //borramos cada uno de los mensajes en la lista del usuario
    for(int i = 0; i < bandejaMensajes.obtenerTamano(); i++){
        Mensaje* msn = bandejaMensajes.obtener(i);
        if(msn != nullptr){
            RastreadorMemoria::RegistroLiberacionMemoria(msn->getMemoria());
            delete msn;
        }

    }

}

//metodos de auditoria
size_t Mensajeria::getMemoria() const{ return this-> bytesOcupados;}

const char* Mensajeria::getLibreriasUsadas(){
    return "mensajeria.h, RastreadorMemoria.h, <iostream>,"
           " <string>, <cstddef>, ListaDinamica.h, Mensaje.h";}

//metodos de la mensajeria
void Mensajeria::enviarMensaje(const string& emisor, const string& receptor, const string& texto, long fechaEnvio){
    //creamos el mensaje
    Mensaje* nuevomsn = new Mensaje(emisor, receptor, texto, fechaEnvio);
    RastreadorMemoria::RegistroAsignacionMemoria(nuevomsn->getMemoria());

    //insertar el mensaje en la lista
    bandejaMensajes.agregarAlFinal(nuevomsn);

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