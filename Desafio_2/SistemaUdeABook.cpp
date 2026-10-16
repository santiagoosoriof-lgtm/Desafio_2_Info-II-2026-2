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

// Constructor
SistemaUdeABook::SistemaUdeABook(int capacidadRed) : red(capacidadRed), usuarioActivo(nullptr) {
    gestorMensajeria = new Mensajeria();
}

// Destructor
SistemaUdeABook::~SistemaUdeABook() {
    delete gestorMensajeria;
}

// Generador auxiliar de dataset masivo
void generarDatasetMasivo1000() {
    ifstream fCheck("usuarios.txt");
    bool necesitaGenerar = false;

    if (!fCheck.is_open()) {
        necesitaGenerar = true;
    } else {
        int lineas = 0;
        string l;
        while (getline(fCheck, l)) lineas++;
        fCheck.close();
        if (lineas < 1000) necesitaGenerar = true;
    }

    if (necesitaGenerar) {
        cout << "Generando dataset masivo de 1,000 usuarios y archivos planos...\n";

        // 1. Crear usuarios.txt
        ofstream fUser("usuarios.txt");
        for (int i = 0; i < 1000; i++) {
            fUser << "user_" << i << ";2000-01-01;user" << i << "@udea.edu.co;/users/udeabook/user_" << i
                  << "/albums/profile.png;Medellin;Colombia;2024-01-01\n";
        }
        fUser.close();

        // 2. Crear relaciones.txt
        ofstream fRel("relaciones.txt");
        for (int i = 0; i < 1000; i++) {
            int numAmigos = 5 + (rand() % 10);
            for (int a = 0; a < numAmigos; a++) {
                int amigoIdx = rand() % 1000;
                if (amigoIdx != i) {
                    fRel << "user_" << i << ";user_" << amigoIdx << "\n";
                }
            }
        }
        fRel.close();

        // 3. Crear solicitudes.txt
        ofstream fSol("solicitudes.txt");
        if (fSol.is_open()) {
            fSol << "user_1;user_0\n";
            fSol << "user_2;user_0\n";
            fSol.close();
        }

        // 4. Crear Galerias.txt
        ofstream fGal("Galerias.txt");
        if (fGal.is_open()) {
            fGal << "user_0;Vacaciones;Publico;/users/udeabook/user_0/albums/Vacaciones/playa.png;2026-10-01;1024;5\n";
            fGal << "user_0;Vacaciones;Publico;/users/udeabook/user_0/albums/Vacaciones/hotel.png;2026-10-02;2048;12\n";
            fGal.close();
        }

        // 5. Crear Mensajes.txt
        ofstream fMsn("Mensajes.txt");
        if (fMsn.is_open()) {
            fMsn << "user_1;user_0;Hola user_0, bienvenido a UdeABook!;20261010;0\n";
            fMsn.close();
        }

        cout << "¡Dataset masivo de 1,000 usuarios y los 5 archivos planos creados exitosamente!\n";
    }
}

void SistemaUdeABook::cargarBaseDeDatos() {
    RastreadorMemoria::IniciarSegmento("RedSocial.h, <fstream>, <sstream>");

    cout << "Cargando datos del sistema UdeABook...\n";
    generarDatasetMasivo1000(); // Revisa y genera los 5 archivos si faltan
    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
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

    cout << "\n==================================================\n";
    cout << "           GESTION DE SOLICITUDES Y AMIGOS       \n";
    cout << "==================================================\n";
    cout << " 1. Enviar solicitud de amistad\n";
    cout << " 2. Ver y gestionar solicitudes pendientes\n";
    cout << " Su opcion: ";
    int op;
    cin >> op;

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
                cout << "[Error]: El usuario '" << destinoUsername << "' no existe en la red.\n";
            } else {
                destinoUser->agregarSolicitud(usuarioActivo);
                cout << "Solicitud de Amistad enviada exitosamente a " << destinoUsername << "!\n";
            }
        }
    }
    else if (op == 2) {
        ListaDinamica<Usuario*>& pendientes = usuarioActivo->getSolicitudesPendientes();
        int totalPendientes = pendientes.obtenerTamano();

        if (totalPendientes == 0) {
            cout << " No tienes solicitudes pendientes por procesar.\n";
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
            cout << "\nSeleccione el numero de la solicitud a gestionar (presione 0 para volver): ";
            cin >> seleccion;
            RastreadorMemoria::RegistroIteracion();
        } while(seleccion < 0 || seleccion > totalPendientes);

        if(seleccion == 0) {
            RastreadorMemoria::ReporteEficiencia();
            return;
        }

        int indiceElegido = seleccion - 1;
        Usuario* solicitanteElegido = pendientes.obtener(indiceElegido);

        int decision = -1;
        do {
            cout << "\n¿Que desea hacer con la solicitud de " << solicitanteElegido->getUsername() << "?\n";
            cout << " 1. Aceptar solicitud\n";
            cout << " 2. Rechazar solicitud\n";
            cout << " 3. Volver al menu\n";
            cout << " Su opcion: ";
            cin >> decision;
            RastreadorMemoria::RegistroIteracion();
        } while(decision < 1 || decision > 3);

        if (decision == 1) {
            red.registrarAmistad(usuarioActivo->getUsername(), solicitanteElegido->getUsername());
            cout << "\n Ahora eres amigo de " << solicitanteElegido->getUsername() << ".\n";
            pendientes.eliminar(indiceElegido);
        }
        else if (decision == 2) {
            pendientes.eliminar(indiceElegido);
            cout << "\nSolicitud de " << solicitanteElegido->getUsername() << " rechazada exitosamente.\n";
        }
        else {
            cout << "Regresando al menu...\n";
        }
    }

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::menuVisitarPerfilAmigo() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento(Album::getLibreriasUsadas());

    string amigoUsername;
    cout << "\nIngrese el username del amigo a visitar: ";
    cin >> amigoUsername;

    if (!red.sonAmigos(usuarioActivo->getUsername(), amigoUsername)) {
        cout << "Error: Solo puedes visitar perfiles de usuarios que sean tus amigos.\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    Usuario* amigo = red.buscarUsuario(amigoUsername);
    if (amigo == nullptr) return;

    cout << "\n==================================================\n";
    cout << "           PERFIL DE: " << amigo->getUsername() << "\n";
    cout << "==================================================\n";

    int totalAlbumes = amigo->getCantidadAlbums();

    if (totalAlbumes == 0) {
        cout << " [Aviso]: Este amigo aun no tiene albumes creados.\n";
        cout << "==================================================\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    cout << " -> Este usuario tiene " << totalAlbumes << " album(es) registrado(s).\n\n";
    for (int i = 0; i < totalAlbumes; i++) {
        RastreadorMemoria::RegistroIteracion();
        Album* alb = amigo->getAlbumes().obtener(i);
        cout << "   [" << (i + 1) << "] " << alb->getNombre()
             << " (" << alb->cantidadFotos() << " fotos) - [" << alb->getPrivacidad() << "]\n";
    }
    cout << "==================================================\n";

    int seleccion = -1;
    do {
        cout << "\nSeleccione el numero del album que desea abrir (presione 0 para salir): ";
        cin >> seleccion;
    } while(seleccion < 0 || seleccion > totalAlbumes);

    if (seleccion == 0) {
        cout << "Saliendo al menu principal...\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    Album* albumSeleccionado = amigo->getAlbumes().obtener(seleccion - 1);
    if(albumSeleccionado->getPrivacidad() == "Privado"){
        cout << "El album seleccionado es privado\n";
    } else {
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
            cout << " -> Capturada el:      " << Fecha::longAFormatoHumano(Fecha::cadenaALongInvertido(fotoActual->getFechaCaptura())) << "\n";
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
                cout << "Le diste like a la foto!\n";
            } else if(controlCarrusel == 'U' || controlCarrusel == 'u'){
                fotoActual->setUnlike();
                cout << "Le quitaste el like a la foto!\n";
            } else if (controlCarrusel != 'M' && controlCarrusel != 'm') {
                cout << "Accion no valida.\n";
            }
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

    cout << "\n--- ADMINISTRAR MENSAJES ---\n";
    cout << "1. Enviar mensaje\n";
    cout << "2. Ver notificaciones (Nuevos)\n";
    cout << "3. Ver Bandeja de entrada completa\n";
    cout << "4. Buscar mensajes por rango de fechas\n";
    cout << "Opcion: ";
    int op;
    cin >> op;

    if (op == 1) {
        string destinatario, texto;
        cout << "Destinatario (username): ";
        cin >> destinatario;
        cout << "Texto (max 512 caracteres): ";
        cin.ignore();
        getline(cin, texto);

        Usuario* receptor = red.buscarUsuario(destinatario);

        if (receptor == nullptr) {
            cout << "[Error]: El usuario destinatario no existe en UdeABook.\n";
        } else {
            long fechaHoy = Fecha::obtenerFechaActual();
            gestorMensajeria->enviarMensaje(usuarioActivo, receptor, texto, fechaHoy);
            cout << "Mensaje enviado exitosamente.\n";
        }

    } else if (op == 2) {
        gestorMensajeria->getNuevosmsn(usuarioActivo->getUsername());

    } else if (op == 3) {
        gestorMensajeria->verBandejaCompleta(usuarioActivo->getUsername());

    } else if (op == 4) {
        string fIniciomsn, fFinmsn;
        cout << "Ingrese fecha de inicio (AAAA-MM-DD): ";
        cin >> fIniciomsn;
        cout << "Ingrese fecha de fin (AAAA-MM-DD): ";
        cin >> fFinmsn;

        long fInicio = Fecha::cadenaALongInvertido(fIniciomsn);
        long fFin = Fecha::cadenaALongInvertido(fFinmsn);

        gestorMensajeria->buscarporFecha(usuarioActivo->getUsername(), fInicio, fFin);
    }

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::iniciar() {
    cargarBaseDeDatos();

    string username;
    cout << "\n==================================================\n";
    cout << "          BIENVENIDO A LA RED UdeABook           \n";
    cout << "==================================================\n";
    cout << "Ingrese su username para iniciar sesion: ";
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
        cout << " 2.- Visitar perfil de un amigo\n";
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
            cout << "\nGuardando Cambios\n";
            if (gestorMensajeria != nullptr) {
                gestorMensajeria->guardarMensajesEnArchivo("Mensajes.txt");
            }
            cout << "\nCerrando sesion en UdeABook... Hasta pronto\n";
        } else {
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }
}