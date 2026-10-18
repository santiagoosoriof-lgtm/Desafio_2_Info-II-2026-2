#include "mensajeria.h"
#include "RastreadorMemoria.h"
#include "fecha.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// Constructor
Mensajeria::Mensajeria() {
    this->bytesOcupados = sizeof(Mensajeria);
    RastreadorMemoria::RegistroAsignacionMemoria(this->bytesOcupados);
}

// Destructor
Mensajeria::~Mensajeria() {
    RastreadorMemoria::RegistroLiberacionMemoria(this->bytesOcupados);

    // Borramos cada uno de los mensajes en la lista del usuario
    for (int i = 0; i < bandejaMensajes.obtenerTamano(); i++) {
        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn != nullptr) {
            RastreadorMemoria::RegistroLiberacionMemoria(msn->getMemoria());
            delete msn;
        }
    }
    bandejaMensajes.limpiar();
}

// Métodos de auditoría
size_t Mensajeria::getMemoria() const {
    return this->bytesOcupados;
}

const char* Mensajeria::getLibreriasUsadas() {
    return "mensajeria.h, RastreadorMemoria.h, <iostream>, <string>, <cstddef>, ListaDinamica.h, mensaje.h";
}

// Carga de persistencia desde Mensajes.txt
bool Mensajeria::cargarArchivoMensajes(const string& rutaArchivo) {
    ifstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        RastreadorMemoria::RegistroIteracion();
        if (linea.empty()) continue;

        stringstream ss(linea);
        string emisor, receptor, texto, fechaStr, leidoStr;

        getline(ss, emisor, ';');
        getline(ss, receptor, ';');
        getline(ss, texto, ';');
        getline(ss, fechaStr, ';');
        getline(ss, leidoStr, ';');

        if (!emisor.empty() && !receptor.empty()) {
            long fechaLong = Fecha::StringNumtoLong(fechaStr);

            Mensaje* msn = new Mensaje(emisor, receptor, texto, fechaLong);
            msn->setLeido(leidoStr == "1" || leidoStr == "true");

            RastreadorMemoria::RegistroAsignacionMemoria(msn->getMemoria());
            bandejaMensajes.agregarAlFinal(msn);
        }
    }
    archivo.close();
    return true;
}

// Envío de mensaje usando objetos Usuario*
void Mensajeria::enviarMensaje(Usuario* emisor, Usuario* receptor, const string& texto, long fechaEnvio) {
    if (emisor == nullptr || receptor == nullptr) {
        cout << "Debe ingresar un emisor y receptor validos." << endl;
        return;
    }

    // Creamos el mensaje en memoria RAM
    Mensaje* nuevomsn = new Mensaje(emisor->getUsername(), receptor->getUsername(), texto, fechaEnvio);
    RastreadorMemoria::RegistroAsignacionMemoria(nuevomsn->getMemoria());

    // Insertar el mensaje en la bandeja general y en las listas de los usuarios
    bandejaMensajes.agregarAlFinal(nuevomsn);
    emisor->getMensajesEnviados().agregarAlFinal(nuevomsn);
    receptor->getMensajesRecibidos().agregarAlFinal(nuevomsn);
}

// Obtener los nuevos mensajes no leídos
void Mensajeria::getNuevosmsn(const string& usuarioReceptor) {
    cout << "\n--- MENSAJES NUEVOS (NO LEIDOS) PARA: " << usuarioReceptor << " ---\n";

    int totalmsn = bandejaMensajes.obtenerTamano();

    string* emisoresUnicos = new string[totalmsn];
    int* conteoNuevos = new int[totalmsn];
    int cantidadEmisores = 0;

    size_t bytesTemporales = (sizeof(string) * totalmsn) + (sizeof(int) * totalmsn);
    RastreadorMemoria::RegistroAsignacionMemoria(bytesTemporales);

    for (int i = 0; i < totalmsn; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn->getReceptor() == usuarioReceptor && !msn->estaleido()) {
            string emisorActual = msn->getEmisor();
            bool encontrado = false;

            for (int j = 0; j < cantidadEmisores; j++) {
                RastreadorMemoria::RegistroIteracion();
                if (emisoresUnicos[j] == emisorActual) {
                    conteoNuevos[j]++;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                emisoresUnicos[cantidadEmisores] = emisorActual;
                conteoNuevos[cantidadEmisores] = 1;
                cantidadEmisores++;
            }
        }
    }

    if (cantidadEmisores == 0) {
        cout << "No tienes mensajes nuevos en la bandeja de entrada.\n";
    } else {
        for (int i = 0; i < cantidadEmisores; i++) {
            cout << " -> " << emisoresUnicos[i] << " (" << conteoNuevos[i] << " mensajes nuevos)\n";
        }
    }

    RastreadorMemoria::RegistroLiberacionMemoria(bytesTemporales);
    delete[] emisoresUnicos;
    delete[] conteoNuevos;
}

// Ver bandeja completa con historial
void Mensajeria::verBandejaCompleta(const string& usuarioReceptor) {
    cout << "\n--- BANDEJA DE ENTRADA Y CHATS PARA: " << usuarioReceptor << " ---\n";

    int totalmsn = bandejaMensajes.obtenerTamano();
    string* emisoresProcesados = new string[totalmsn];
    int cantidadProcesados = 0;

    size_t bytesTemporales = sizeof(string) * totalmsn;
    RastreadorMemoria::RegistroAsignacionMemoria(bytesTemporales);
    int totalMostrados = 0;

    for (int i = 0; i < totalmsn; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn->getReceptor() == usuarioReceptor || msn->getEmisor() == usuarioReceptor) {
            string otroUsuario = (msn->getReceptor() == usuarioReceptor) ? msn->getEmisor() : msn->getReceptor();
            bool yaImpreso = false;

            for (int j = 0; j < cantidadProcesados; j++) {
                RastreadorMemoria::RegistroIteracion();
                if (emisoresProcesados[j] == otroUsuario) {
                    yaImpreso = true;
                    break;
                }
            }

            if (!yaImpreso) {
                cout << "\n CHAT CON: " << otroUsuario
                     << "\n----------------------------------------\n";
                emisoresProcesados[cantidadProcesados] = otroUsuario;
                cantidadProcesados++;

                for (int k = 0; k < totalmsn; k++) {
                    RastreadorMemoria::RegistroIteracion();
                    Mensaje* subMsn = bandejaMensajes.obtener(k);

                    bool esMismaConversacion = (subMsn->getReceptor() == usuarioReceptor && subMsn->getEmisor() == otroUsuario) ||
                                               (subMsn->getEmisor() == usuarioReceptor && subMsn->getReceptor() == otroUsuario);

                    if (esMismaConversacion) {
                        cout << "   * [" << Fecha::longAFormatoHumano(subMsn->getFechaEnvio()) << "]\n";
                        if (subMsn->getEmisor() == usuarioReceptor) {
                            cout << "   (Enviado): " << subMsn->getTexto() << "\n";
                        } else {
                            cout << "   (Recibido): " << subMsn->getTexto()
                            << " [" << (subMsn->estaleido() ? "Leido" : "NUEVO") << "]\n";
                            subMsn->setLeido(true);
                        }
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

// Búsqueda por rango de fechas YYYYMMDD
void Mensajeria::buscarporFecha(const string& usuarioReceptor, long fechaInicio, long fechaFin) {
    cout << "\n--- HISTORIAL DE MENSAJES ENTRE: " << Fecha::longAFormatoHumano(fechaInicio)
    << " Y " << Fecha::longAFormatoHumano(fechaFin) << " ---\n";
    cout << "--- PARA EL USUARIO: " << usuarioReceptor << " ---\n";
    int total = bandejaMensajes.obtenerTamano();
    int encontrados = 0;

    for (int i = 0; i < total; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msg = bandejaMensajes.obtener(i);
        long timestamp = msg->getFechaEnvio();

        if ((msg->getReceptor() == usuarioReceptor || msg->getEmisor() == usuarioReceptor) &&
            timestamp >= fechaInicio && timestamp <= fechaFin) {

            cout << " [" << Fecha::longAFormatoHumano(timestamp) << "] ";
            if (msg->getEmisor() == usuarioReceptor) {
                cout << "(Enviado a " << msg->getReceptor() << "): ";
            } else {
                cout << "(Recibido de " << msg->getEmisor() << "): ";
            }
            cout << msg->getTexto() << "\n";
            encontrados++;
        }
    }

    if (encontrados == 0) {
        cout << "No se encontraron mensajes registrados en ese rango de fechas.\n";
    }
}

// Guardar en archivo
bool Mensajeria::guardarMensajesEnArchivo(const string& rutaArchivo) {
    ofstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo crear el archivo de mensajes.\n";
        return false;
    }

    int total = bandejaMensajes.obtenerTamano();
    for (int i = 0; i < total; i++) {
        RastreadorMemoria::RegistroIteracion();

        Mensaje* msn = bandejaMensajes.obtener(i);
        if (msn != nullptr) {
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