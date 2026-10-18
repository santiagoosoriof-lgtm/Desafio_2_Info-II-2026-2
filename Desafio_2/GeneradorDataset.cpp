#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int main() {
    srand(time(0));

    cout << "==========================================================" << endl;
    cout << "     GENERADOR MASIVO DE BASE DE DATOS UdeABook           " << endl;
    cout << "==========================================================" << endl;

    const int TOTAL_USUARIOS = 1000;

    // -----------------------------------------------------------------
    // 1. GENERAR usuarios.txt (1,000 usuarios)
    // -----------------------------------------------------------------
    cout << "1. Generando usuarios.txt (" << TOTAL_USUARIOS << " usuarios)..." << endl;
    ofstream fUsers("usuarios.txt");
    string ciudades[] = {"Medellin", "Bogota", "Cali", "Rionegro", "Envigado", "Itagui"};

    for (int i = 0; i < TOTAL_USUARIOS; i++) {
        string ciudad = ciudades[rand() % 6];
        fUsers << "user_" << i << ";2000-05-15;user" << i << "@udea.edu.co;"
               << "/users/udeabook/user_" << i << "/albums/profile.png;"
               << ciudad << ";Colombia;2024-01-15\n";
    }
    fUsers.close();

    // -----------------------------------------------------------------
    // 2. GENERAR relaciones.txt (Grafo de Amistades)
    // -----------------------------------------------------------------
    cout << "2. Generando relaciones.txt (Grafo denso de amistades)..." << endl;
    ofstream fRel("relaciones.txt");
    for (int i = 0; i < TOTAL_USUARIOS; i++) {
        int numAmigos = 8 + (rand() % 12); // Entre 8 y 20 amigos por usuario
        for (int a = 0; a < numAmigos; a++) {
            int amigoIdx = rand() % TOTAL_USUARIOS;
            if (amigoIdx != i) {
                fRel << "user_" << i << ";user_" << amigoIdx << "\n";
            }
        }
    }
    fRel.close();

    // -----------------------------------------------------------------
    // 3. GENERAR solicitudes.txt (Cientos de solicitudes pendientes)
    // -----------------------------------------------------------------
    cout << "3. Generando solicitudes.txt (Solicitudes pendientes de amistad)..." << endl;
    ofstream fSol("solicitudes.txt");
    for (int i = 0; i < 300; i++) {
        int solicitante = rand() % TOTAL_USUARIOS;
        int receptor = rand() % TOTAL_USUARIOS;
        if (solicitante != receptor) {
            fSol << "user_" << solicitante << ";user_" << receptor << "\n";
        }
    }
    fSol.close();

    // -----------------------------------------------------------------
    // 4. GENERAR Galerias.txt (Álbumes y Fotos masivas)
    // -----------------------------------------------------------------
    cout << "4. Generando Galerias.txt (Álbumes, Fotos y Carruseles)..." << endl;
    ofstream fGal("Galerias.txt");
    string albumNombres[] = {"Vacaciones", "Universidad", "Mascotas", "Paseos", "Eventos"};
    string privabilidades[] = {"Publico", "Amigos", "Privado"};

    // Generamos álbumes para los primeros 300 usuarios
    for (int u = 0; u < 300; u++) {
        int numAlbumes = 1 + (rand() % 3);
        for (int alb = 0; alb < numAlbumes; alb++) {
            string albNom = albumNombres[rand() % 5];
            string priv = privabilidades[rand() % 3];
            int numFotos = 2 + (rand() % 4); // 2 a 5 fotos por álbum

            for (int f = 0; f < numFotos; f++) {
                int likes = rand() % 50;
                int pesoKB = 512 + (rand() % 2048);
                fGal << "user_" << u << ";" << albNom << ";" << priv << ";"
                     << "/users/udeabook/user_" << u << "/albums/" << albNom << "/foto_" << f << ".png;"
                     << "2026-10-10;" << pesoKB << ";" << likes << "\n";
            }
        }
    }
    fGal.close();

    // -----------------------------------------------------------------
    // 5. GENERAR Mensajes.txt (Bandejas de Entrada y Notificaciones)
    // -----------------------------------------------------------------
    cout << "5. Generando Mensajes.txt (Historial de Mensajes y Notificaciones)..." << endl;
    ofstream fMsn("Mensajes.txt");
    string textosMsg[] = {
        "Hola! Como vas con el proyecto de Info 2?",
        "Nos vemos en el bloque 19 a las 2 pm?",
        "Ya revisaste los diagramas de clases?",
        "Te envie la solicitud de amistad, me la aceptas?",
        "Quedo super bien la entrega de C++!",
        "Feliz cumpleaños! Un abrazo."
    };

    for (int m = 0; m < 500; m++) {
        int emisor = rand() % TOTAL_USUARIOS;
        int receptor = rand() % TOTAL_USUARIOS;
        if (emisor != receptor) {
            string texto = textosMsg[rand() % 6];
            int leido = rand() % 2; // 0 = No leído (Notificación), 1 = Leído
            long fecha = 20261001 + (rand() % 15); // Fechas entre 2026-10-01 y 2026-10-15
            fMsn << "user_" << emisor << ";user_" << receptor << ";" << texto << ";" << fecha << ";" << leido << "\n";
        }
    }
    fMsn.close();

    cout << "==========================================================" << endl;
    cout << " ¡ÉXITO! Los 5 archivos planos han sido generados.      " << endl;
    cout << "==========================================================" << endl;

    return 0;
}