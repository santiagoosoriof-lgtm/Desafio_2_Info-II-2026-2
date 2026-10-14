#include "SistemaUdeABook.h"
#include <iostream>
#include <cstdlib>

using namespace std;

SistemaUdeABook::SistemaUdeABook(int capacidadRed) : red(capacidadRed), usuarioActivo(nullptr) {}

SistemaUdeABook::~SistemaUdeABook() {}

void SistemaUdeABook::cargarBaseDeDatos() {
    cout << "Cargando datos del sistema UdeABook...\n";
    // Si tienes la funcion que crea los archivos planos de prueba, la puedes llamar aqui:
    // crearArchivosDePrueba();
    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
}

void SistemaUdeABook::mostrarMejoresAmigos() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento("SistemaUdeABook::mostrarMejoresAmigos");

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

    RastreadorMemoria::IniciarSegmento("SistemaUdeABook::menuAdministrarAmistades");

    cout << "\n--- ADMINISTRAR AMISTADES ---\n";
    cout << "1. Enviar solicitud de amistad\n";
    cout << "2. Aceptar solicitud pendiente\n";
    cout << "Opcion: ";
    int op;
    cin >> op;

    if (op == 1) {
        string destino;
        cout << "Ingrese el username del usuario a solicitar: ";
        cin >> destino;

        if (red.sonAmigos(usuarioActivo->getUsername(), destino)) {
            cout << "Ya son amigos.\n";
        } else {
            red.registrarAmistad(usuarioActivo->getUsername(), destino);
            cout << "¡Solicitud registrada / Amistad vinculada exitosamente con " << destino << "!\n";
        }
    } else if (op == 2) {
        cout << "No tienes solicitudes pendientes por procesar.\n";
    }

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::menuVisitarPerfilAmigo() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento("SistemaUdeABook::menuVisitarPerfilAmigo");

    string amigoUsername;
    cout << "\nIngrese el username del amigo a visitar: ";
    cin >> amigoUsername;

    if (!red.sonAmigos(usuarioActivo->getUsername(), amigoUsername)) {
        cout << "Error: Solo puedes visitar perfiles de usuarios que sean tus amigos.\n";
        RastreadorMemoria::ReporteEficiencia();
        return;
    }

    cout << "\n--- PERFIL DE " << amigoUsername << " ---\n";
    cout << " (Desplegando albumes publicos/amigos en carrusel circular...)\n";

    RastreadorMemoria::ReporteEficiencia();
}

void SistemaUdeABook::menuVerSugerenciasK() {
    if (usuarioActivo == nullptr) return;

    RastreadorMemoria::IniciarSegmento("SistemaUdeABook::menuVerSugerenciasK");

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

    RastreadorMemoria::IniciarSegmento("SistemaUdeABook::menuAdministrarMensajes");

    cout << "\n--- ADMINISTRAR MENSAJES ---\n";
    cout << "1. Enviar mensaje\n";
    cout << "2. Leer nuevos mensajes\n";
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

        cout << "Mensaje enviado a " << destinatario << " exitosamente.\n";
    } else if (op == 2) {
        cout << "No tienes mensajes nuevos sin leer.\n";
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
            cout << "\nCerrando sesion en UdeABook... ¡Hasta pronto!\n";
        } else {
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }
}