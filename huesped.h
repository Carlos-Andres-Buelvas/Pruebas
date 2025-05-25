#ifndef HUESPED_H
#define HUESPED_H

#include <string>
#include "fecha.h"
using namespace std;

class Reserva;

class Huesped {
private:
    string documento;
    string nombre;
    int antiguedad;
    float puntuacion;
    Reserva** reservas; // arreglo de punteros a reservas
    int cantidadReservas;
    int capacidadReservas;
    string clave;

public:
    Huesped();
    Huesped(string doc, string nombre, int antig, float punt);

    // Getters
    string getDocumento() const;
    string getNombre() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    string getClave() const;

    // Setters
    void setDocumento(const string& doc);
    void setAntiguedad(int antig);
    void setPuntuacion(float punt);
    void setClave(const string& c);

    // Métodos
    bool agregarReserva(Reserva* nueva);
    bool hayConflicto(const Fecha& entrada, int duracion) const;
    void eliminarReserva(Reserva* r);
    void mostrar() const;

    // Constructor de copia
    Huesped(const Huesped& otro);

    // Operador de asignación
    Huesped& operator=(const Huesped& otro);

    // Repara punteros luego de redimensionar reservas[]
    void repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas);

    ~Huesped();
};

#endif // HUESPED_H
