#include "anfitrion.h"
#include <iostream>

Anfitrion::Anfitrion()
    : documento(""), nombre(""), clave(""), antiguedad(0), puntuacion(0.0f),
    capacidadAlojamientos(5), cantidadAlojamientos(0) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

Anfitrion::Anfitrion(const std::string& doc, const std::string& nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""), antiguedad(antig), puntuacion(punt),
    capacidadAlojamientos(5), cantidadAlojamientos(0) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

Anfitrion::Anfitrion(const Anfitrion& otro)
    : documento(otro.documento), nombre(otro.nombre), clave(otro.clave),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    capacidadAlojamientos(otro.capacidadAlojamientos),
    cantidadAlojamientos(otro.cantidadAlojamientos) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        alojamientos[i] = otro.alojamientos[i];  // Copia superficial
    }
}

Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this != &otro) {
        delete[] alojamientos;

        documento = otro.documento;
        nombre = otro.nombre;
        clave = otro.clave;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        capacidadAlojamientos = otro.capacidadAlojamientos;
        cantidadAlojamientos = otro.cantidadAlojamientos;

        alojamientos = new Alojamiento*[capacidadAlojamientos];
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            alojamientos[i] = otro.alojamientos[i];
        }
    }
    return *this;
}

Anfitrion::~Anfitrion() {
    delete[] alojamientos;
    alojamientos = nullptr;
}

// Getters
std::string Anfitrion::getDocumento() const { return documento; }
std::string Anfitrion::getNombre() const { return nombre; }
std::string Anfitrion::getClave() const { return clave; }
int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

// Setters
void Anfitrion::setDocumento(const std::string& doc) { documento = doc; }
void Anfitrion::setClave(const std::string& c) { clave = c; }
void Anfitrion::setAntiguedad(int antig) { antiguedad = antig; }
void Anfitrion::setPuntuacion(float punt) { puntuacion = punt; }

// Función para agregar alojamiento
void Anfitrion::agregarAlojamiento(Alojamiento* nuevo) {
    if (cantidadAlojamientos >= capacidadAlojamientos) {
        int nuevaCapacidad = capacidadAlojamientos * 2;
        Alojamiento** nuevoArray = new Alojamiento*[nuevaCapacidad];

        for (int i = 0; i < cantidadAlojamientos; ++i)
            nuevoArray[i] = alojamientos[i];

        delete[] alojamientos;
        alojamientos = nuevoArray;
        capacidadAlojamientos = nuevaCapacidad;
    }

    alojamientos[cantidadAlojamientos++] = nuevo;
}

// Mostrar información del anfitrión
void Anfitrion::mostrar() const {
    std::cout << "Anfitrión: " << documento
              << " | Antigüedad: " << antiguedad << " meses"
              << " | Puntuación: " << puntuacion << std::endl;

    std::cout << "Alojamientos asignados: " << cantidadAlojamientos << std::endl;

    for (int i = 0; i < cantidadAlojamientos; ++i) {
        if (alojamientos[i])
            alojamientos[i]->mostrar();
    }
}
