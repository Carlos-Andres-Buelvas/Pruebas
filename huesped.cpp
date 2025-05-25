#include "huesped.h"
#include <iostream>
#include "fecha.h"
#include "reserva.h"
using namespace std;

Huesped::Huesped() {
    documento = "";
    nombre = "";
    clave = "";
    antiguedad = 0;
    puntuacion = 0.0;
    capacidadReservas = 5;
    cantidadReservas = 0;
    reservas = new Reserva*[capacidadReservas];
}

Huesped::Huesped(string doc, string nom, int antig, float punt) {
    documento = doc;
    nombre = nom;
    // clave = "";
    antiguedad = antig;
    puntuacion = punt;
    capacidadReservas = 5;
    cantidadReservas = 0;
    reservas = new Reserva*[capacidadReservas];
}

// Constructor de copia
Huesped::Huesped(const Huesped& otro) {
    documento = otro.documento;
    nombre = otro.nombre;
    antiguedad = otro.antiguedad;
    puntuacion = otro.puntuacion;
    clave = otro.clave;
    capacidadReservas = otro.capacidadReservas;
    cantidadReservas = otro.cantidadReservas;

    reservas = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i) {
        reservas[i] = otro.reservas[i];
    }
}

// Operador de asignación
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
        for (int i = 0; i < cantidadReservas; ++i) {
            reservas[i] = otro.reservas[i];
        }
    }
    return *this;
}

string Huesped::getDocumento() const { return documento; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }
string Huesped::getNombre() const { return nombre; }

void Huesped::setDocumento(const string& doc) { documento = doc; }
void Huesped::setAntiguedad(int antig) { antiguedad = antig; }
void Huesped::setPuntuacion(float punt) { puntuacion = punt; }

void Huesped::mostrar() const {
    cout << "Huésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    if (cantidadReservas == 0) {
        cout << "No tiene reservaciones registradas.\n";
        return;
    }

    cout << "--- Reservaciones ---\n";
    for (int i = 0; i < cantidadReservas; ++i) {
        reservas[i]->mostrarComprobante();
    }
}

bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion())) {
        return false; // No se puede agregar por cruce de fechas
    }

    if (cantidadReservas >= capacidadReservas) {
        capacidadReservas *= 2;
        Reserva** temp = new Reserva*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            temp[i] = reservas[i];
        delete[] reservas;
        reservas = temp;
    }

    reservas[cantidadReservas++] = nueva;
    return true;
}

// --- Versión corregida de hayConflicto ---
bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int y1     = entrada.getAnio();
    int start1 = entrada.aDiaDelAnio();
    int end1   = start1 + duracion;  // exclusivo

    for (int i = 0; i < cantidadReservas; ++i) {
        Reserva* r2 = reservas[i];
        Fecha fe2 = r2->getFechaEntrada();
        int y2 = fe2.getAnio();
        if (y1 != y2) {
            // Diferente año, no pueden solaparse
            continue;
        }
        int start2 = fe2.aDiaDelAnio();
        int end2   = start2 + r2->getDuracion();

        // Si [start1,end1) y [start2,end2) se cruzan:
        if (!(end1 <= start2 || end2 <= start1)) {
            return true;
        }
    }
    return false;
}

void Huesped::eliminarReserva(Reserva* r) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            cantidadReservas--;
            break;
        }
    }
}

void Huesped::setClave(const string& c) {
    clave = c;
}

string Huesped::getClave() const {
    return clave;
}

void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < this->cantidadReservas; ++j) {
            if (this->reservas[j]->getCodigo() == actual->getCodigo()) {
                this->reservas[j] = actual;
            }
        }
    }
}

Huesped::~Huesped() {
    delete[] reservas;
}
