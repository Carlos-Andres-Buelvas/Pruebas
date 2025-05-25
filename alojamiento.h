#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>
#include "fecha.h"
using namespace std;

// Declaración adelantada para evitar ciclo de inclusión
class Anfitrion;

class Alojamiento {
private:
    string codigo;
    string departamento;
    string municipio;
    string nombre;
    string tipo;
    string direccion;
    Anfitrion* anfitrion;
    float precioNoche;
    bool amenidades[6];

    // Nuevo reemplazo para diasReservados
    Fecha* fechasOcupadas;
    int cantFechas;
    int capFechas;

    // Métodos auxiliares
    bool contieneFecha(const Fecha& f) const;
    void agregarFecha(const Fecha& f);

public:
    Alojamiento();
    Alojamiento(string cod, string dep, string mun, string nom, string tipo, string dir,
                Anfitrion* anfitrion, float precio, const bool amen[6]);

    // Getters y Setters
    string getCodigo() const;
    void setCodigo(const string& cod);
    float getPrecio() const;

    float getPrecioNoche() const;
    void setPrecioNoche(float precio);

    string getMunicipio() const;
    Anfitrion* getAnfitrion() const;

    // Funciones
    bool estaDisponible(const Fecha& entrada, int duracion) const;
    void reservarDias(const Fecha& entrada, int duracion);
    void liberarDias(const Fecha& inicio, int noches);
    void mostrar() const;

    Alojamiento(const Alojamiento& otro);                // Constructor de copia
    Alojamiento& operator=(const Alojamiento& otro);     // Operador de asignación

    ~Alojamiento();
};

#endif // ALOJAMIENTO_H
