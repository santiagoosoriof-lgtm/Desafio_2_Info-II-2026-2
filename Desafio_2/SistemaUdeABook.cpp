#include "SistemaUdeABook.h"
#include "RastreadorMemoria.h"
#include "fecha.h"
#include "album.h"
#include "foto.h"
#include "mensajeria.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

SistemaUdeABook::SistemaUdeABook(int capacidadRed) : red(capacidadRed), usuarioActivo(nullptr) {
    gestorMensajeria = new Mensajeria();
}

SistemaUdeABook::~SistemaUdeABook() {
    // Liberamos la memoria que ocupa el objeto gestor de mensajeria
    if (gestorMensajeria != nullptr) {
        delete gestorMensajeria;
        gestorMensajeria = nullptr;
    }
}

void SistemaUdeABook::cargarBaseDeDatos() {
    RastreadorMemoria::IniciarSegmento("RedSocial.h, <fstream>, <sstream>");

    cout << "Cargando datos del sistema UdeABook desde archivos planos...\n";

    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
    red.cargarSolicitudesDesdeArchivo("solicitudes.txt");
    red.cargarAlbumesDesdeArchivo("albumes.txt");

    if (gestorMensajeria != nullptr) {
        gestorMensajeria->cargarArchivoMensajes("Mensajes.txt");
    }

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::mostrarMejoresAmigos() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(RedSocial::getLibreriasUsadas());

    cout << "\n==================================================\n";
    cout << "           FOTOS DE PERFIL DE AMIGOS             \n";
    cout << "==================================================\n";

    ListaDinamica<Usuario*> mejores = red.obtenerMejoresAmigosSorteados(usuarioActivo->getUsername());

    if (mejores.estaVacia()) {
        cout << " (Aun no tienes amigos agregados o con coincidencia)\n";
    } else {
        for (int i = 0; i < mejores.obtenerTamano(); i++) {
            Usuario* amigo = mejores.obtener(i);
            cout << " [" << (i + 1) << "] " << amigo->getUsername()
                 << " -> Foto: " << amigo->getFotoPerfilRuta() << "\n";
        }
    }
    cout << "==================================================\n";

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::menuAdministrarAmistades() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(RedSocial::getLibreriasUsadas());
    int op = -1;
    do {
        cout << "\n==================================================\n";
        cout << "           GESTION DE SOLICITUDES Y AMIGOS       \n";
        cout << "==================================================\n";
        cout << " 1. Enviar solicitud de amistad\n";
        cout << " 2. Ver y gestionar solicitudes de amistad\n";
        cout << " 3. Volver al menu principal UdeaBook\n";
        cout << " Su opcion: ";
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            op = -1;
            cout << "Opcion invalida. Intente de nuevo.\n";
            continue;
        }

        if (op == 1) {
            string destinoUsername;
            cout << "\nIngrese el username del usuario a solicitar: ";
            cin >> destinoUsername;

            if (destinoUsername == usuarioActivo->getUsername()) {
                cout << "[Aviso]: No puedes enviarte una solicitud a ti mismo.\n";
                RastreadorMemoria::ReporteEficiencia();
                return;
            }

            if (red.sonAmigos(usuarioActivo->getUsername(), destinoUsername)) {
                cout << "[Aviso]: Ya tienes una relacion de amistad activa con " << destinoUsername << ".\n";
            } else {
                Usuario* destinoUser = red.buscarUsuario(destinoUsername);
                if (destinoUser == nullptr) {
                    cout << "[Error] : El usuario '" << destinoUsername << "' no esta registrado en UdeABook.\n";
                } else {
                    ListaDinamica<Usuario*>& pendientesDestino = destinoUser->getSolicitudesPendientes();
                    bool yaSolicitado = false;

                    for (int i = 0; i < pendientesDestino.obtenerTamano(); i++) {
                        RastreadorMemoria::RegistroIteracion();
                        if (pendientesDestino.obtener(i)->getUsername() == usuarioActivo->getUsername()) {
                            yaSolicitado = true;
                            break;
                        }
                    }

                    if (yaSolicitado) {
                        cout << "[Aviso]: Ya le has enviado una solicitud de amistad a " << destinoUsername << " y esta pendiente.\n";
                    } else {
                        destinoUser->agregarSolicitud(usuarioActivo);
                        cout << "Solicitud de Amistad enviada exitosamente a " << destinoUsername << "!\n";
                        red.guardarSolicitudesEnArchivo("solicitudes.txt");
                    }
                }
            }
        } else if (op == 2) {
            ListaDinamica<Usuario*>& pendientes = usuarioActivo->getSolicitudesPendientes();
            int totalPendientes = pendientes.obtenerTamano();

            if (totalPendientes == 0) {
                cout << " No tienes solicitudes pendientes.\n";
                cout << "--------------------------------------------------\n";
                RastreadorMemoria::ReporteEficiencia();
                return;
            }

            cout << "\n--------------------------------------------------\n";
            cout << "         SOLICITUDES DE AMISTAD RECIBIDAS        \n";
            cout << "--------------------------------------------------\n";
            cout << " -> Tienes " << totalPendientes << " solicitud(es) en espera.\n\n";

            for (int i = 0; i < totalPendientes; i++) {
                RastreadorMemoria::RegistroIteracion();
                Usuario* solicitante = pendientes.obtener(i);
                cout << "   [" << (i + 1) << "] " << solicitante->getUsername() << "\n";
            }
            cout << "--------------------------------------------------\n";

            int seleccion = -1;
            do {
                cout << "\nSeleccione el numero de la solicitud a gestionar (presione 0 para volver al inicio): ";
                cin >> seleccion;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    seleccion = -1;
                    continue;
                }
                RastreadorMemoria::RegistroIteracion();
            } while (seleccion < 0 || seleccion > totalPendientes);

            if (seleccion == 0) {
                RastreadorMemoria::ReporteEficiencia();
                return;
            }

            int indiceElegido = seleccion - 1;
            Usuario* solicitanteElegido = pendientes.obtener(indiceElegido);

            int decision = -1;
            do {
                cout << "\n ¿Que desea hacer con la solicitud de " << solicitanteElegido->getUsername() << "?\n";
                cout << " 1. Aceptar solicitud\n";
                cout << " 2. Rechazar solicitud\n";
                cout << " 3. Volver al menu\n";
                cout << " Su opcion: ";
                cin >> decision;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    decision = -1;
                    continue;
                }
                RastreadorMemoria::RegistroIteracion();
            } while (decision < 1 || decision > 3);

            if (decision == 1) {
                red.registrarAmistad(usuarioActivo->getUsername(), solicitanteElegido->getUsername());
                cout << "\n Ahora eres amigo de " << solicitanteElegido->getUsername() << ".\n";
                pendientes.eliminar(indiceElegido);
                red.guardarRelacionesEnArchivo("relaciones.txt");
                red.guardarSolicitudesEnArchivo("solicitudes.txt");
            } else if (decision == 2) {
                pendientes.eliminar(indiceElegido);
                cout << "\nSolicitud de " << solicitanteElegido->getUsername() << " rechazada exitosamente.\n";
                red.guardarSolicitudesEnArchivo("solicitudes.txt");
            }
        }
        RastreadorMemoria::ReporteEficiencia();
    } while (op != 3);
}

void SistemaUdeABook::menuVisitarPerfilAmigo() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(Album::getLibreriasUsadas());

    string usuarioUsername;
    cout << "\nIngrese el username del usuario a visitar: ";
    cin >> usuarioUsername;

    Usuario* usuarioDestino = red.buscarUsuario(usuarioUsername);
    if (usuarioDestino == nullptr) {
        cout << "Error: El usuario no existe en la plataforma.\n";
        RastreadorMemoria::ReporteEficiencia();
        return;}

    bool esAmigo = red.sonAmigos(usuarioActivo->getUsername(), usuarioDestino->getUsername());

    cout << "\n==================================================\n";
    cout << "           PERFIL DE: " << usuarioDestino->getUsername() << "\n";
    cout << "==================================================\n";

    int totalAlbumes = usuarioDestino->getCantidadAlbums();

    if (totalAlbumes == 0) {
        cout << " [Aviso]: Este amigo aun no tiene albumes creados.\n";
        cout << "==================================================\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    cout << " -> Este usuario tiene " << totalAlbumes << " album(es) registrado(s).\n\n";
    for (int i = 0; i < totalAlbumes; i++) {
        RastreadorMemoria::RegistroIteracion();
        Album* alb = usuarioDestino->getAlbumes().obtener(i);
        cout << "   [" << (i + 1) << "] " << alb->getNombre()
             << " (" << alb->cantidadFotos() << " fotos) - [" << alb->getPrivacidad() << "]\n";
    }
    cout << "==================================================\n";

    int seleccion = -1;
    do {
        cout << "\nSeleccione el numero del album que desea abrir (presione 0 para salir): ";
        cin >> seleccion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            seleccion = -1;
            continue;
        }
    } while (seleccion < 0 || seleccion > totalAlbumes);

    if (seleccion == 0) {
        cout << "Saliendo al menu principal...\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    Album* albumSeleccionado = usuarioDestino->getAlbumes().obtener(seleccion - 1);
    std::string privacidad = albumSeleccionado->getPrivacidad(); //obtenemos privacidad


    if (privacidad == "Privado") {
        cout << "El album seleccionado es privado.\n";
    } else {
        if (albumSeleccionado->cantidadFotos() == 0) {
            cout << "\n[Aviso]: El album '" << albumSeleccionado->getNombre() << "' esta vacio.\n";
            RastreadorMemoria::ReporteEficiencia();
            return;
        }else if(privacidad == "Amigos" && !esAmigo){
        cout << "Este album solo es visible para los amigos de " << usuarioDestino->getUsername() << ".\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }else {
        // Si el album es "Publico" o si es de "Amigos"
        if (albumSeleccionado->cantidadFotos() == 0) {
            cout << "\n[Aviso]: El album '" << albumSeleccionado->getNombre() << "' esta vacio.\n";
            RastreadorMemoria::ReporteEficiencia();
            return;
        }
        char controlCarrusel = ' ';
        while (controlCarrusel != 'M' && controlCarrusel != 'm') {
            RastreadorMemoria::RegistroIteracion();

            Foto* fotoActual = albumSeleccionado->obtenerFotoActual();

            cout << "\n--------------------------------------------------\n";
            cout << " ALBUM: " << albumSeleccionado->getNombre() << " (Visualizando contenido)\n";
            cout << "--------------------------------------------------\n";
            cout << " -> Ruta Logica Linux: " << fotoActual->getRutaAbsoluta() << "\n";
             cout << " -> Capturada el:      " << Fecha::longAFormatoHumano(Fecha::StringNumtoLong(fotoActual->getFechaCaptura())) << "\n";
            cout << " -> Likes actuales:    " << fotoActual->getLikes() << "\n";
            cout << "--------------------------------------------------\n";
            cout << " [A] Anterior  //  [S] Siguiente  //  [L] Dar Like  //  [U] Quitar Like  //  [M] Volver a Menu\n";
            cout << " Seleccione accion: ";
            cin >> controlCarrusel;

            if (controlCarrusel == 'A' || controlCarrusel == 'a') {
                albumSeleccionado->fotoAnterior();
                cout << "Retrocediendo...\n";
            } else if (controlCarrusel == 'S' || controlCarrusel == 's') {
                albumSeleccionado->fotoSiguiente();
                cout << "Avanzando...\n";
            } else if (controlCarrusel == 'L' || controlCarrusel == 'l') {
                fotoActual->setLike();
                cout << "¡Le diste like a la foto!\n";
            } else if (controlCarrusel == 'U' || controlCarrusel == 'u') {
                fotoActual->setUnlike();
                cout << "¡Le quitaste el like a la foto!\n";
            } else if (controlCarrusel != 'M' && controlCarrusel != 'm') {
                cout << "Accion no valida.\n";
            }
        }
    red.guardarAlbumesEnArchivo("albumes.txt");
    }

        cout << "\nCerrando galeria multimedia...\n";
        RastreadorMemoria::ReporteEficiencia();
    }
}

void SistemaUdeABook::menuVerSugerenciasK() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(RedSocial::getLibreriasUsadas());

    int k;
    cout << "\nIngrese la cantidad K de sugerencias a consultar (1-20): ";
    cin >> k;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        k = 5;
    }

    if (k < 1) k = 1;
    if (k > 20) k = 20;

    ListaDinamica<Usuario*> sug = red.obtenerSugerenciasK(usuarioActivo->getUsername(), k);

    cout << "\n--- SUGERENCIAS DE AMISTAD (TOP-" << k << ") ---\n";
    if (sug.estaVacia()) {
        cout << "No hay sugerencias con amigos en comun disponibles.\n";
    } else {
        for (int i = 0; i < sug.obtenerTamano(); i++) {
            Usuario* u = sug.obtener(i);
            int comun = red.contarAmigosEnComun(usuarioActivo->getUsername(), u->getUsername());
            cout << " " << (i + 1) << ". " << u->getUsername()
                 << " (" << comun << " amigos en comun)\n";
        }
    }

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::menuAdministrarMensajes() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(Mensajeria::getLibreriasUsadas());
    int op = -1;
    do {
        cout << "\n--- ADMINISTRAR MENSAJES ---\n";
        cout << "1. Enviar mensaje\n";
        cout << "2. Ver notificaciones (Nuevos)\n";
        cout << "3. Ver Bandeja de entrada completa\n";
        cout << "4. Buscar mensajes por rango de fechas\n";
        cout << "5. Menu Inicial UdeABook\n";
        cout << "Opcion: ";
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            op = -1;
            cout << "Opcion invalida.\n";
            continue;
        }

        if (op == 1) {
            string destinatario, texto;
            cout << "Destinatario (username): ";
            cin >> destinatario;
            cout << "si el texto es mayor a 512 caracteres se cortara el mensaje hasta este tamanio maximo): \n";
            cout << "Texto (max 512 caracteres): ";
            cin.ignore();
            getline(cin, texto);

            if (texto.empty()) {
                cout << "[Error]: No se pueden enviar mensajes vacios.\n";
                continue;
            }

            Usuario* receptor = red.buscarUsuario(destinatario);

            if (receptor == nullptr) {
                cout << "[Error]: El usuario destinatario no existe en UdeABook.\n";
            } else {
                long fechaHoy = Fecha::obtenerFechaActual();
                gestorMensajeria->enviarMensaje(usuarioActivo, receptor, texto, fechaHoy);
                cout << "Mensaje enviado exitosamente.\n";
                gestorMensajeria->guardarMensajesEnArchivo("mensajes.txt");
            }
        } else if (op == 2) {
            gestorMensajeria->getNuevosmsn(usuarioActivo->getUsername());
            gestorMensajeria->guardarMensajesEnArchivo("mensajes.txt");
        } else if (op == 3) {
            gestorMensajeria->verBandejaCompleta(usuarioActivo->getUsername());
            gestorMensajeria->guardarMensajesEnArchivo("mensajes.txt");
        } else if (op == 4) {
            string fIniciomsn, fFinmsn;
            cout << "Ingrese fecha de inicio (AAAA-MM-DD): ";
            cin >> fIniciomsn;
            cout << "Ingrese fecha de fin (AAAA-MM-DD): ";
            cin >> fFinmsn;

            long fInicio = Fecha::cadenaALongInvertido(fIniciomsn);
            long fFin = Fecha::cadenaALongInvertido(fFinmsn);

            gestorMensajeria->buscarporFecha(usuarioActivo->getUsername(), fInicio, fFin);
            gestorMensajeria->guardarMensajesEnArchivo("mensajes.txt");
        }

        RastreadorMemoria::ReporteEficiencia();
    } while (op != 5);
}

void SistemaUdeABook::iniciar() {
    cargarBaseDeDatos();

    string username;
    cout << "\n==================================================\n";
    cout << "          BIENVENIDO A LA RED UdeABook           \n";
    cout << "==================================================\n";
    cout << "Ingrese su username para iniciar sesion (ej: user_0): ";
    cin >> username;

    usuarioActivo = red.buscarUsuario(username);

    if (usuarioActivo == nullptr) {
        cout << "Error: Usuario no registrado en la plataforma.\n";
        return;
    }

    char opcion = ' ';
    while (opcion != 'S' && opcion != 's') {
        cout << "\nBienvenido a UdeABook // " << usuarioActivo->getUsername() << "\n";
        mostrarMejoresAmigos();

        cout << "\nMenu de opciones:\n";
        cout << " 1.- Administrar amistades\n";
        cout << " 2.- Visitar perfil de un usuario\n";
        cout << " 3.- Ver sugerencias de amistad (Top-K)\n";
        cout << " 4.- Administrar mensajes\n";
        cout << "\n .- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        cout << " .- Ingrese 'S' o 's' para cerrar su sesion.\n";
        cout << "\nSu opcion: ";
        cin >> opcion;

        if (opcion == '1') {
            menuAdministrarAmistades();
        } else if (opcion == '2') {
            menuVisitarPerfilAmigo();
        } else if (opcion == '3') {
            menuVerSugerenciasK();
        } else if (opcion == '4') {
            menuAdministrarMensajes();
        } else if (opcion == 'N' || opcion == 'n') {
            cout << "\nRefrescando seleccion de mejores amigos...\n";
        } else if (opcion == 'S' || opcion == 's') {
            cout << "\nGuardando Cambios...\n";
            if (gestorMensajeria != nullptr) {
                gestorMensajeria->guardarMensajesEnArchivo("Mensajes.txt");
            }
            red.guardarRelacionesEnArchivo("relaciones.txt");
            red.guardarSolicitudesEnArchivo("solicitudes.txt");
            red.guardarAlbumesEnArchivo("albumes.txt");
            cout << "\nCerrando sesion en UdeABook... ¡Hasta pronto!\n";
        } else {
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }
}