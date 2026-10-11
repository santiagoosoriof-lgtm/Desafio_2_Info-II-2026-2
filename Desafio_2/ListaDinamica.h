#pragma once

#include <iostream>

template <typename T>
class Nodo {
private:
    T dato;
    Nodo<T>* siguiente;

public:
    Nodo(const T& d) : dato(d), siguiente(nullptr) {}

    T& getDato() { return dato; }
    const T& getDato() const { return dato; }
    void setDato(const T& d) { dato = d; }

    Nodo<T>* getSiguiente() const { return siguiente; }
    void setSiguiente(Nodo<T>* sig) { siguiente = sig; }
};

template <typename T>
class ListaDinamica {
private:
    Nodo<T>* primero;
    Nodo<T>* ultimo;
    int tamano;

public:
    // Constructor
    ListaDinamica() : primero(nullptr), ultimo(nullptr), tamano(0) {}

    // Destructor: Libera la memoria dinámica para evitar memory leaks
    ~ListaDinamica() {
        limpiar();
    }

    // Agrega un elemento al final de la lista
    void agregarAlFinal(const T& elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);
        if (primero == nullptr) {
            primero = nuevo;
            ultimo = nuevo;
        } else {
            ultimo->setSiguiente(nuevo);
            ultimo = nuevo;
        }
        tamano++;
    }

    // Obtiene una referencia al elemento en el índice especificado
    T& obtener(int indice) {
        if (indice < 0 || indice >= tamano) {
            std::cerr << "Error: Indice fuera de rango [" << indice << "]" << std::endl;
            static T errorVal;
            return errorVal;
        }
        Nodo<T>* actual = primero;
        for (int i = 0; i < indice; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getDato();
    }

    // Versión constante de obtener
    const T& obtener(int indice) const {
        if (indice < 0 || indice >= tamano) {
            std::cerr << "Error: Indice fuera de rango [" << indice << "]" << std::endl;
            static T errorVal;
            return errorVal;
        }
        Nodo<T>* actual = primero;
        for (int i = 0; i < indice; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getDato();
    }

    // Elimina el elemento en la posición dada y reconecta los punteros
    bool eliminar(int indice) {
        if (indice < 0 || indice >= tamano || primero == nullptr) {
            return false;
        }

        Nodo<T>* aBorrar = nullptr;

        if (indice == 0) {
            aBorrar = primero;
            primero = primero->getSiguiente();
            if (primero == nullptr) {
                ultimo = nullptr;
            }
        } else {
            Nodo<T>* actual = primero;
            for (int i = 0; i < indice - 1; i++) {
                actual = actual->getSiguiente();
            }
            aBorrar = actual->getSiguiente();
            actual->setSiguiente(aBorrar->getSiguiente());
            if (aBorrar == ultimo) {
                ultimo = actual;
            }
        }

        delete aBorrar; // Liberación explícita de memoria
        tamano--;
        return true;
    }

    // Devuelve la cantidad de elementos
    int obtenerTamano() const {
        return tamano;
    }

    bool estaVacia() const {
        return tamano == 0;
    }

    // Borra todos los nodos
    void limpiar() {
        Nodo<T>* actual = primero;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->getSiguiente();
            delete actual;
            actual = siguiente;
        }
        primero = nullptr;
        ultimo = nullptr;
        tamano = 0;
    }
};
