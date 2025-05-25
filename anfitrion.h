#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"
using namespace std;

class Anfitrion {
private:
    string documento;
    string nombre;
    string clave;
    int antiguedad;
    float puntuacion;
    Alojamiento** alojamientos;
    int cantidadAlojamientos;
    int capacidadAlojamientos;

public:
    Anfitrion();
    Anfitrion(string documento, string nombre, int antiguedad, float puntuacion);

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

    // Funciones
    void agregarAlojamiento(Alojamiento* nuevo);
    void mostrar() const;
    int getCantidadAlojamientos() const {
        return cantidadAlojamientos;
    }

    Alojamiento* getAlojamiento(int i) const {
        if (i >= 0 && i < cantidadAlojamientos)
            return alojamientos[i];
        return nullptr;
    }

    // Destructor
    ~Anfitrion();

    Anfitrion(const Anfitrion& otro);                 // Constructor de copia
    Anfitrion& operator=(const Anfitrion& otro);      // Operador de asignación

};

#endif // ANFITRION_H
