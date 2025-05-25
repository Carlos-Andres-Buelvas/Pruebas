#include "alojamiento.h"
#include "anfitrion.h"
#include <iostream>
using namespace std;

Alojamiento::Alojamiento() {
    codigo = "";
    departamento = "";
    municipio = "";
    nombre = "";
    tipo = "";
    direccion = "";
    anfitrion = nullptr;
    precioNoche = 0.0f;
    for (int i = 0; i < 6; ++i)
        amenidades[i] = false;

    capFechas = 30;
    cantFechas = 0;
    fechasOcupadas = new Fecha[capFechas];
}

Alojamiento::Alojamiento(string cod, string dep, string mun, string nom, string tipo_,
                         string dir, Anfitrion* a, float precio, const bool amen[6]) {
    codigo = cod;
    departamento = dep;
    municipio = mun;
    nombre = nom;
    tipo = tipo_;
    direccion = dir;
    anfitrion = a;
    precioNoche = precio;
    for (int i = 0; i < 6; ++i)
        amenidades[i] = amen[i];

    capFechas = 30;
    cantFechas = 0;
    fechasOcupadas = new Fecha[capFechas];
}

string Alojamiento::getMunicipio() const { return municipio; }
Anfitrion* Alojamiento::getAnfitrion() const { return anfitrion; }

string Alojamiento::getCodigo() const { return codigo; }
void Alojamiento::setCodigo(const string& cod) { codigo = cod; }

float Alojamiento::getPrecioNoche() const { return precioNoche; }
void Alojamiento::setPrecioNoche(float precio) { precioNoche = precio; }

float Alojamiento::getPrecio() const { return precioNoche; }

bool Alojamiento::contieneFecha(const Fecha& f) const {
    for (int i = 0; i < cantFechas; ++i) {
        if (fechasOcupadas[i].toEntero() == f.toEntero())
            return true;
    }
    return false;
}

void Alojamiento::agregarFecha(const Fecha& f) {
    if (contieneFecha(f)) return;

    if (cantFechas >= capFechas) {
        capFechas *= 2;
        Fecha* nuevo = new Fecha[capFechas];
        for (int i = 0; i < cantFechas; ++i)
            nuevo[i] = fechasOcupadas[i];
        delete[] fechasOcupadas;
        fechasOcupadas = nuevo;
    }

    fechasOcupadas[cantFechas++] = f;
}

bool Alojamiento::estaDisponible(const Fecha& entrada, int duracion) const {
    for (int i = 0; i < duracion; ++i) {
        Fecha f = entrada.sumarDias(i);
        if (contieneFecha(f))
            return false;
    }
    return true;
}

void Alojamiento::reservarDias(const Fecha& entrada, int duracion) {
    for (int i = 0; i < duracion; ++i)
        agregarFecha(entrada.sumarDias(i));
}

void Alojamiento::liberarDias(const Fecha& inicio, int noches) {
    for (int i = 0; i < noches; ++i) {
        Fecha f = inicio.sumarDias(i);
        for (int j = 0; j < cantFechas; ++j) {
            if (fechasOcupadas[j].toEntero() == f.toEntero()) {
                for (int k = j; k < cantFechas - 1; ++k)
                    fechasOcupadas[k] = fechasOcupadas[k + 1];
                cantFechas--;
                break;
            }
        }
    }
}

void Alojamiento::mostrar() const {
    cout << "Código: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Ubicación: " << departamento << ", " << municipio << endl;
    cout << "Tipo: " << tipo << " | Dirección: " << direccion << endl;
    cout << "Precio por noche: $" << precioNoche << endl;
    cout << "Amenidades: ";

    const string nombres[6] = {
        "ascensor", "piscina", "aire acondicionado",
        "caja fuerte", "parqueadero", "patio"
    };

    bool hayAlguna = false;
    for (int i = 0; i < 6; ++i) {
        if (amenidades[i]) {
            if (hayAlguna) cout << ", ";
            cout << nombres[i];
            hayAlguna = true;
        }
    }

    if (!hayAlguna) cout << "ninguna";
    cout << endl;
}

// Constructor de copia
Alojamiento::Alojamiento(const Alojamiento& otro) {
    codigo = otro.codigo;
    departamento = otro.departamento;
    municipio = otro.municipio;
    nombre = otro.nombre;
    tipo = otro.tipo;
    direccion = otro.direccion;
    precioNoche = otro.precioNoche;
    anfitrion = otro.anfitrion;

    for (int i = 0; i < 6; ++i)
        amenidades[i] = otro.amenidades[i];

    capFechas = otro.capFechas;
    cantFechas = otro.cantFechas;
    fechasOcupadas = new Fecha[capFechas];
    for (int i = 0; i < cantFechas; ++i)
        fechasOcupadas[i] = otro.fechasOcupadas[i];
}

// Operador de asignación
Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this != &otro) {
        delete[] fechasOcupadas;

        codigo = otro.codigo;
        departamento = otro.departamento;
        municipio = otro.municipio;
        nombre = otro.nombre;
        tipo = otro.tipo;
        direccion = otro.direccion;
        precioNoche = otro.precioNoche;
        anfitrion = otro.anfitrion;

        for (int i = 0; i < 6; ++i)
            amenidades[i] = otro.amenidades[i];

        capFechas = otro.capFechas;
        cantFechas = otro.cantFechas;
        fechasOcupadas = new Fecha[capFechas];
        for (int i = 0; i < cantFechas; ++i)
            fechasOcupadas[i] = otro.fechasOcupadas[i];
    }
    return *this;
}

Alojamiento::~Alojamiento() {
    delete[] fechasOcupadas;
}
