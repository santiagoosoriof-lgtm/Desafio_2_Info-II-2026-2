// Generador auxiliar de dataset masivo
#include <iostream>
#include <fstream>
using namespace std;

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
            int numAmigos = 50 + (rand() % 10);
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
            for (int i = 0; i < 300; i++) {
                int remitente = rand() % 1000;
                int destinatario = rand() % 1000;
                if (remitente != destinatario) {
                    fSol << "user_" << remitente << ";user_" << destinatario << "\n";
                }
            }
            fSol.close();
        }

        // 4. Crear Albumes.txt
        ofstream fAlb("albumes.txt");
        if (fAlb.is_open()) {
            for (int i = 0; i < 150; i++) {
                // Generamos un album de vacaciones publico
                fAlb << "user_" << i << ";Vacaciones San Andres;Publico;/media/user_" << i << "/; "
                     << "playa.png|20261010|5242880|5;"   // Foto 1: nombre|fecha|peso
                     << "hotel.png|20261011|3145728|0;"   // Foto 2
                     << "buceo.png|20261012|4194304|10\n"; // Foto 3 (Cierra con salto de línea)

                // Generamos un segundo album amigos
                fAlb << "user_" << i << ";Recuerdos;Amigos;/secure/user_" << i << "/; "
                     << "Abuela.png|20260101|1048576|0;"
                     << "casa Infancia.png|20260515|2097152|20\n";

                // Generamos un segundo álbum privado
                fAlb << "user_" << i << ";Fotos Personales;Privado;/secure/user_" << i << "/; "
                     << "avatar.png|20260101|1048576|0;"
                     << "documento.png|20260515|2097152|3\n";
            }
            fAlb.close();
        }

        // 5. Crear Mensajes.txt
        ofstream fMsn("mensajes.txt");
        if (fMsn.is_open()) {
            // Caso de prueba para user_0
            fMsn << "user_1;user_0;Hola user_0, ¿vamos a estudiar en la UdeA?;20261005;0\n";
            fMsn << "user_1;user_0;Recuerda llevar impreso el informe del Hito 2;20261007;0\n";
            fMsn << "user_0;user_1;¿A que hora es la sustentacion con Anibal?;20261010;1\n";
            fMsn << "user_0;user_1;¡Hola! Si, salgo para la Universidad en 10 minutos;20261014;1\n";
            fMsn << "user_1;user_0;Hola user_0, ¿vamos a estudiar en la UdeA?;20260901;0\n";
            fMsn << "user_1;user_0;Recuerda llevar impreso el informe del Hito 2;20260908;0\n";
            fMsn << "user_0;user_1;¿A que hora es la sustentacion con Anibal?;20260915;1\n";
            fMsn << "user_0;user_1;¡Hola! Si, salgo para la Universidad en 10 minutos;20260920;1\n";

            // (1000 interacciones)
            for (int i = 0; i < 1000; i++) {
                int emisor = rand() % 1000;
                int receptor = rand() % 1000;
                if (emisor != receptor) {
                    // Fechas aletorias en 2026 (AAAAMMDD)
                    int diaAleatorio = 1 + (rand() % 15);
                    int mesAleatorio = 1 + (rand() % 11)*100;
                    long fechaFicticia = 20260000 + mesAleatorio + diaAleatorio;

                    fMsn << "user_" << emisor << ";user_" << receptor << ";"
                         << "Mensaje simulado numero " << i << " de prueba.;"
                         << fechaFicticia << ";"
                         << (rand() % 2) << "\n"; // 0 o 1 aleatorio para el estado leído
                }
            }
            fMsn.close();
        }

        cout << "Dataset creado exitosamente!\n";
    }
}
