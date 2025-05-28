#include "alojamiento.h"
#include "anfitrion.h"
#include <iostream>

Alojamiento::Alojamiento() : anfitrion(nullptr), precioNoche(0.0f),
    cantFechas(0), capFechas(5000), fechasOcupadas(new Fecha[30]) {

    amenidades[0] = amenidades[1] = amenidades[2] = false;
    amenidades[3] = amenidades[4] = amenidades[5] = false;
}

Alojamiento::Alojamiento(std::string cod, std::string dep, std::string mun, std::string nom,
                         std::string tipo_, std::string dir, Anfitrion* a, float precio,
                         const bool amen[NUM_AMENIDADES]) :
    codigo(cod), departamento(dep), municipio(mun), nombre(nom), tipo(tipo_),
    direccion(dir), anfitrion(a), precioNoche(precio),
    cantFechas(0), capFechas(5000), fechasOcupadas(new Fecha[30]) {

    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = amen[i];
}

// Getters y setters
std::string Alojamiento::getCodigo() const { return codigo; }
void Alojamiento::setCodigo(const std::string& cod) { codigo = cod; }

std::string Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecioNoche() const { return precioNoche; }
void Alojamiento::setPrecioNoche(float precio) { precioNoche = precio; }

Anfitrion* Alojamiento::getAnfitrion() const { return anfitrion; }

// Funciones auxiliares
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

// Disponibilidad
bool Alojamiento::estaDisponible(const Fecha& entrada, int duracion) const {
    for (int i = 0; i < duracion; ++i)
        if (contieneFecha(entrada.sumarDias(i)))
            return false;
    return true;
}

void Alojamiento::reservarDias(const Fecha& entrada, int duracion) {
    int inicio = entrada.aDiaDelAnio();
    std::cout << "[DEBUG] Reservando desde " << inicio << " por " << duracion << " días\n";
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
                --cantFechas;
                break;
            }
        }
    }
}

// Mostrar información
void Alojamiento::mostrar() const {
    std::cout << "Código: " << codigo << '\n'
              << "Nombre: " << nombre << '\n'
              << "Ubicación: " << departamento << ", " << municipio << '\n'
              << "Tipo: " << tipo << " | Dirección: " << direccion << '\n'
              << "Precio por noche: $" << precioNoche << '\n'
              << "Amenidades: ";

    const std::string nombres[NUM_AMENIDADES] = {
        "ascensor", "piscina", "aire acondicionado",
        "caja fuerte", "parqueadero", "patio"
    };

    bool hayAlguna = false;
    for (int i = 0; i < NUM_AMENIDADES; ++i) {
        if (amenidades[i]) {
            if (hayAlguna) std::cout << ", ";
            std::cout << nombres[i];
            hayAlguna = true;
        }
    }
    if (!hayAlguna) std::cout << "ninguna";
    std::cout << '\n';
}

// Copia y asignación
Alojamiento::Alojamiento(const Alojamiento& otro) :
    codigo(otro.codigo), departamento(otro.departamento), municipio(otro.municipio),
    nombre(otro.nombre), tipo(otro.tipo), direccion(otro.direccion),
    anfitrion(otro.anfitrion), precioNoche(otro.precioNoche),
    capFechas(otro.capFechas), cantFechas(otro.cantFechas),
    fechasOcupadas(new Fecha[otro.capFechas]) {

    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = otro.amenidades[i];

    for (int i = 0; i < cantFechas; ++i)
        fechasOcupadas[i] = otro.fechasOcupadas[i];
}

Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this != &otro) {
        delete[] fechasOcupadas;

        codigo = otro.codigo;
        departamento = otro.departamento;
        municipio = otro.municipio;
        nombre = otro.nombre;
        tipo = otro.tipo;
        direccion = otro.direccion;
        anfitrion = otro.anfitrion;
        precioNoche = otro.precioNoche;

        for (int i = 0; i < NUM_AMENIDADES; ++i)
            amenidades[i] = otro.amenidades[i];

        capFechas = otro.capFechas;
        cantFechas = otro.cantFechas;
        fechasOcupadas = new Fecha[capFechas];
        for (int i = 0; i < cantFechas; ++i)
            fechasOcupadas[i] = otro.fechasOcupadas[i];
    }
    return *this;
}

// Destructor
Alojamiento::~Alojamiento() {
    delete[] fechasOcupadas;
}
