#include "huesped.h"
#include "fecha.h"
#include "reserva.h"
#include <iostream>
using namespace std;

Huesped::Huesped() : documento(""), nombre(""), clave(""),
    antiguedad(0), puntuacion(0.0),
    capacidadReservas(5), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];
}

Huesped::Huesped(string doc, string nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""),
    antiguedad(antig), puntuacion(punt),
    capacidadReservas(5), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];
}

Huesped::Huesped(const Huesped& otro)
    : documento(otro.documento), nombre(otro.nombre),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    clave(otro.clave), capacidadReservas(otro.capacidadReservas),
    cantidadReservas(otro.cantidadReservas) {
    reservas = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        reservas[i] = otro.reservas[i];
}

Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservas;

        documento = otro.documento;
        nombre = otro.nombre;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        clave = otro.clave;
        capacidadReservas = otro.capacidadReservas;
        cantidadReservas = otro.cantidadReservas;

        reservas = new Reserva*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            reservas[i] = otro.reservas[i];
    }
    return *this;
}

// ----------- Getters & Setters -----------
string Huesped::getDocumento() const { return documento; }
string Huesped::getNombre() const { return nombre; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }
string Huesped::getClave() const { return clave; }

void Huesped::setDocumento(const string& doc) { documento = doc; }
void Huesped::setAntiguedad(int antig) { antiguedad = antig; }
void Huesped::setPuntuacion(float punt) { puntuacion = punt; }
void Huesped::setClave(const string& c) { clave = c; }

// ----------- Reservas -----------
bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion()))
        return false;

    if (cantidadReservas >= capacidadReservas)
        redimensionarReservas();

    reservas[cantidadReservas++] = nueva;
    return true;
}

bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int y1 = entrada.getAnio();
    int start1 = entrada.aDiaDelAnio();
    int end1 = start1 + duracion;

    for (int i = 0; i < cantidadReservas; ++i) {
        Reserva* r = reservas[i];
        Fecha fe = r->getFechaEntrada();

        if (fe.getAnio() != y1) continue;

        int start2 = fe.aDiaDelAnio();
        int end2 = start2 + r->getDuracion();

        if (!(end1 <= start2 || end2 <= start1))
            return true;
    }
    return false;
}

void Huesped::eliminarReserva(Reserva* r) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

void Huesped::eliminarReservaPorCodigo(const string& codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] && reservas[i]->getCodigo() == codigo) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

void Huesped::mostrar() const {
    cout << "Huésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    bool tiene = false;
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] && reservas[i]->getHuesped()->getDocumento() == documento) {
            cout << i + 1 << ". ";
            reservas[i]->mostrarComprobante();
            tiene = true;
        }
    }
    if (!tiene)
        cout << "No tiene reservaciones activas.\n";
}

// ----------- Utilidades -----------
void Huesped::redimensionarReservas() {
    capacidadReservas *= 2;
    Reserva** temp = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        temp[i] = reservas[i];
    delete[] reservas;
    reservas = temp;
}

void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < cantidadReservas; ++j) {
            if (reservas[j]->getCodigo() == actual->getCodigo())
                reservas[j] = actual;
        }
    }
}

Huesped::~Huesped() {
    delete[] reservas;
}
