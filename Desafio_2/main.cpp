#include "RastreadorMemoria.h"
#include "mensaje.h"
#include "album.h"
#include "foto.h"
#include "RedSocial.h"

#include <iostream>
#include <string>
#include <fstream> // Necesario para la lectura/escritura de archivos planos

using namespace std;

// Función para asegurar que existan los archivos planos de prueba
void crearArchivosDePrueba() {
    ifstream checkUser("usuarios.txt");
    if (!checkUser.good()) {
        ofstream fUsuarios("usuarios.txt");
        if (fUsuarios.is_open()) {
            fUsuarios << "juan_perez;2000-05-10;juan@udea.edu.co;foto1.jpg;Medellin;Colombia;2026-10-01\n";
            fUsuarios << "santiago_gomez;2001-08-15;santi@udea.edu.co;foto2.jpg;Envigado;Colombia;2026-10-02\n";
            fUsuarios << "maria_lopez;1999-12-01;maria@udea.edu.co;foto3.jpg;Rionegro;Colombia;2026-10-03\n";
            fUsuarios.close();
        }
    }
}

int main(){
    // ============================================================
    //  CARGA Y PRUEBAS DEL MÓDULO CORE (JUAN)
    // ============================================================
    cout << "=========================================\n";
    cout << "   1. INICIALIZACION Y PERSISTENCIA CORE \n";
    cout << "=========================================\n";

    // Crear archivos de texto si no existen
    crearArchivosDePrueba();

    // Inicializar la red social y cargar persistencia
    RedSocial red(100);
    red.cargarUsuariosDesdeArchivo("usuarios.txt");
    red.cargarRelacionesDesdeArchivo("relaciones.txt");
    cout << "-> Datos de Usuarios y Relaciones cargados correctamente.\n\n";


    // ============================================================
    // 2. PRUEBAS DEL MÓDULO DE MENSAJES, FOTOS Y RASTREADOR (SANTIAGO)
    // ============================================================
    cout << "=========================================\n";
    cout << "   2. PRUEBAS DE MENSAJES Y MEMORIA      \n";
    cout << "=========================================\n";

    for(int i = 0; i < 1000; i++){
        RastreadorMemoria::RegistroIteracion();
    }
    RastreadorMemoria::IniciarSegmento(Mensaje::getLibreriasUsadas());
    RastreadorMemoria::RegistroAsignacionMemoria(1000);
    RastreadorMemoria::ReporteEficiencia();
    RastreadorMemoria::RegistroLiberacionMemoria(1000);
    RastreadorMemoria::ReporteEficiencia();

    Mensaje* chat1 = new Mensaje("Santiago","Juan","Hola Juan, Como estas? este es un mensaje de prueba de la clase Mensaje", 10102026);
    RastreadorMemoria::RegistroAsignacionMemoria(chat1->getMemoria());
    cout << " -> Emisor:   " << (*chat1).getEmisor() << "\n";
    cout << " -> Receptor: " << (*chat1).getReceptor() << "\n";
    cout << " -> Texto:    " << (*chat1).getTexto() << "\n";
    cout << " -> Fecha:    " << (*chat1).getFechaEnvio() << "\n";
    cout << " -> Leido:    " << ((*chat1).estaleido() ? "Si" : "No") << "\n\n";

    string textogigante ="";
    for(int i = 0; i < 60; i++ ){
        textogigante += "0123456789";
        RastreadorMemoria::RegistroIteracion();
    }

    Mensaje* chat2 = new Mensaje("Santiago","Juan", textogigante, 10102026);
    RastreadorMemoria::RegistroAsignacionMemoria(chat2->getMemoria());

    cout << " -> Emisor:   " << (*chat2).getEmisor() << "\n";
    cout << " -> Receptor: " << (*chat2).getReceptor() << "\n";
    cout << " -> Texto:    " << (*chat2).getTexto() << "\n";
    cout << " -> Fecha:    " << (*chat2).getFechaEnvio() << "\n";
    cout << " -> Leido:    " << ((*chat2).estaleido() ? "Si" : "No") << "\n\n";

    RastreadorMemoria::ReporteEficiencia();
    RastreadorMemoria::RegistroLiberacionMemoria(chat1->getMemoria());
    delete chat1;
    RastreadorMemoria::RegistroLiberacionMemoria(chat2->getMemoria());
    delete chat2;
    RastreadorMemoria::ReporteEficiencia();

    cout << "=========================================\n";
    cout << "   3. PRUEBAS DE FOTOS Y MEMORIA      \n";
    cout << "=========================================\n";

    Foto* foto1= new Foto("10102026","/home/santiago/udeabook/media/vacaciones.png",2621440);
    RastreadorMemoria::RegistroAsignacionMemoria(foto1->getMemoria());
    RastreadorMemoria::IniciarSegmento(Foto::getLibreriasUsadas());
    cout << " -> Fecha Captura: " << foto1->getFechaCaptura() << "\n";
    cout << " -> Ruta Linux:    " << foto1->getRutaAbsoluta() << "\n";
    cout << " -> Peso Imagen:   " << foto1->getTamanoBytes() << " Bytes (Simulado)\n";
    cout << " -> Likes Inicial: " << foto1->getLikes() << "\n";

    // Imprimimos el reporte para ver cuántos bytes base + dinámicos costó en RAM
    RastreadorMemoria::ReporteEficiencia();

    //probar los likes

    foto1->setLike();
    foto1->setLike();
    foto1->setLike();
    cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";
    foto1->setUnlike();
    cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";
    foto1->setUnlike();
    foto1->setUnlike();
    foto1->setUnlike();
    cout << "la foto tiene " << foto1->getLikes() <<" Me gusta\n";

    RastreadorMemoria::RegistroLiberacionMemoria(foto1->getMemoria());
    delete foto1;

    RastreadorMemoria::ReporteEficiencia();

    RastreadorMemoria::IniciarSegmento(Album::getLibreriasUsadas());

    cout << "==================================================\n";
    cout << "      PRUEBA UNITARIA: CLASE ALBUM (Ude@Book)    \n";
    cout << "==================================================\n\n";

    // -----------------------------------------------------------------
    // TEST 1: Creación de Álbum y Fotos Independientes
    // -----------------------------------------------------------------
    cout << "[Test 1] Creando Album y cargando contenido...\n";

    Album* miAlbum = new Album("Viaje San Andres", "/home/santiago/fotos/", "Publico");
    RastreadorMemoria::RegistroAsignacionMemoria(miAlbum->getMemoria());

    // Instanciamos 3 fotos independientes
    Foto* f1 = new Foto("20261011", "/media/playa.png", 1024);
    Foto* f2 = new Foto("20261011", "/media/hotel.png", 2048);
    Foto* f3 = new Foto("20261011", "/media/buceo.png", 4096);

    RastreadorMemoria::RegistroAsignacionMemoria(f1->getMemoria());
    RastreadorMemoria::RegistroAsignacionMemoria(f2->getMemoria());
    RastreadorMemoria::RegistroAsignacionMemoria(f3->getMemoria());

    // Metemos las 3 fotos al primer álbum
    miAlbum->agregarFoto(f1);
    miAlbum->agregarFoto(f2);
    miAlbum->agregarFoto(f3);

    cout << " -> Album creado: " << miAlbum->getNombre() << "\n";
    cout << " -> Cantidad de fotos cargadas: " << miAlbum->cantidadFotos() << " (Esperado: 3)\n\n";

    // -----------------------------------------------------------------
    // TEST 2: Validación de la Regla de Exclusividad (Seguridad)
    // -----------------------------------------------------------------
    cout << "[Test 2] Intentando duplicar f1 en un segundo album ...\n";

    Album* album2 = new Album("Fotos Duplicadas", "/home/juan/", "Privado");
    RastreadorMemoria::RegistroAsignacionMemoria(album2->getMemoria());

    // 🚨 Intento ilegal: meter f1 (que ya es de miAlbum) en albumHack
    album2->agregarFoto(f1);

    cout << " -> Cantidad en album hacker: " << album2->cantidadFotos() << " (Esperado: 0 por bloqueo)\n\n";

    // -----------------------------------------------------------------
    // TEST 3: Ejecución del Carrusel Circular (Aritmética Modular)
    // -----------------------------------------------------------------
    cout << "[Test 3] Probando navegacion infinita del carrusel...\n";

    // Foto inicial (índice 0)
    cout << " -> Foto Actual: " << miAlbum->obtenerFotoActual()->getRutaAbsoluta() << " (Esperado: /media/playa.png)\n";

    // Avanzamos 1 (índice 1)
    miAlbum->fotoSiguiente();
    cout << " -> Al avanzar:  " << miAlbum->obtenerFotoActual()->getRutaAbsoluta() << " (Esperado: /media/hotel.png)\n";

    // Avanzamos 1 (índice 2)
    miAlbum->fotoSiguiente();
    cout << " -> Al avanzar:  " << miAlbum->obtenerFotoActual()->getRutaAbsoluta() << " (Esperado: /media/buceo.png)\n";

    // 🚀 GIRO CIRCULAR DELANTERO: Estamos al final, si avanzamos debe volver a la primera (índice 0)
    miAlbum->fotoSiguiente();
    cout << " -> Giro Adelante Regresa a: " << miAlbum->obtenerFotoActual()->getRutaAbsoluta() << " (Esperado: /media/playa.png)\n";

    // 🚀 GIRO CIRCULAR TRASERO: Estamos en la primera, si retrocedemos debe saltar a la última (índice 2)
    miAlbum->fotoAnterior();
    cout << " -> Giro Atras Salta a:      " << miAlbum->obtenerFotoActual()->getRutaAbsoluta() << " (Esperado: /media/buceo.png)\n\n";

    // -----------------------------------------------------------------
    // LIMPIEZA DE MEMORIA DINÁMICA
    // -----------------------------------------------------------------

    RastreadorMemoria::ReporteEficiencia();
    cout << "[Test 4] Liberando estructuras y limpiando memoria RAM...\n";

    // Descontamos del rastreador antes del delete
    RastreadorMemoria::RegistroLiberacionMemoria(miAlbum->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(album2->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(f1->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(f2->getMemoria());
    RastreadorMemoria::RegistroLiberacionMemoria(f3->getMemoria());

    // Destrucción física
    delete miAlbum;
    delete album2;
    delete f1;
    delete f2;
    delete f3;

    // Reporte final del ResourceTracker: Debe quedar perfectamente en 0 Bytes
    cout << "--- STATUS ACUMULADO FIN DE EXECUCIÓN ---";
    RastreadorMemoria::ReporteEficiencia();

    cout << "==================================================\n";
    cout << "             FIN DE LA PRUEBA UNITARIA            \n";
    cout << "==================================================\n";

    return 0;
}