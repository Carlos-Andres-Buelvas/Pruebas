#include "huesped.h"
#include "fecha.h"
#include "reserva.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern int totalIteraciones;
extern int totalMemoria;

using namespace std;

Huesped::Huesped() : documento(""), nombre(""), antiguedad(0),
    puntuacion(0.0), clave(""), cantidadReservas(0), capacidadReservas(10)
    {
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
}

Huesped::Huesped(string doc, string nom, int antig, float punt)
    : documento(doc), nombre(nom), antiguedad(antig), puntuacion(punt),
    clave(""), cantidadReservas(0), capacidadReservas(10) {
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
}

Huesped::Huesped(const Huesped& otro)
    : documento(otro.documento), nombre(otro.nombre),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    clave(otro.clave), cantidadReservas(otro.cantidadReservas),
    capacidadReservas(otro.capacidadReservas) {
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
    for (int i = 0; i < cantidadReservas; ++i){
        totalIteraciones++;
        reservas[i] = otro.reservas[i];
    }
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
        totalMemoria += sizeof(Reserva*) * capacidadReservas;
        for (int i = 0; i < cantidadReservas; ++i){
            totalIteraciones++;
            reservas[i] = otro.reservas[i];
        }
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
        totalIteraciones++;
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
        totalIteraciones++;
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j){
                totalIteraciones++;
                reservas[j] = reservas[j + 1];
            }
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

void Huesped::eliminarReservaPorCodigo(const string& codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        if (reservas[i] && reservas[i]->getCodigo() == codigo) {
            // Evitar uso de puntero inválido
            for (int j = i; j < cantidadReservas - 1; ++j){
                totalIteraciones++;
                reservas[j] = reservas[j + 1];
            }
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

void Huesped::mostrar() const {
    cout << "\nHuésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    if (cantidadReservas == 0) {
        cout << "No tiene reservaciones activas.\n";
        return;
    }

    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        cout << i + 1 << ". ";
        reservas[i]->mostrarComprobante();
    }
}

// ----------- Utilidades -----------
void Huesped::redimensionarReservas() {
    capacidadReservas *= 2;
    Reserva** temp = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
    for (int i = 0; i < cantidadReservas; ++i){
        totalIteraciones++;
        temp[i] = reservas[i];
    }
    delete[] reservas;
    reservas = temp;
}

void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        totalIteraciones++;
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < cantidadReservas; ++j) {
            totalIteraciones++;
            if (reservas[j]->getCodigo() == actual->getCodigo())
                reservas[j] = actual;
        }
    }
}

void Huesped::cargarDesdeArchivo(const std::string& archivo,
                                 Huesped*& arreglo,
                                 int& cantidad,
                                 int& capacidad)
{
    std::ifstream in(archivo);
    if (!in.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir " << archivo << "\n";
        return;
    }

    std::string linea;
    std::getline(in, linea); // Saltar encabezado
    cantidad = 0;
    capacidad = 10;
    arreglo = new Huesped[capacidad];
    totalMemoria += sizeof(Huesped) * capacidad;

    while (std::getline(in, linea)) {
        totalIteraciones++;
        std::stringstream ss(linea);
        std::string doc, nombre, clave, antigStr, puntStr;

        std::getline(ss, doc, ';');
        std::getline(ss, nombre, ';');
        std::getline(ss, antigStr, ';');
        std::getline(ss, puntStr, ';');
        std::getline(ss, clave, ';');

        int antig = std::stoi(antigStr);
        float punt = std::stof(puntStr);

        Huesped nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Huesped* nuevoArr = new Huesped[capacidad];
            totalMemoria += sizeof(Huesped) * capacidad;
            for (int i = 0; i < cantidad; ++i){
                totalIteraciones++;
                nuevoArr[i] = arreglo[i];
            }
            delete[] arreglo;
            arreglo = nuevoArr;
        }

        arreglo[cantidad++] = nuevo;
    }

    in.close();
}

Huesped::~Huesped() {
    delete[] reservas;
}
