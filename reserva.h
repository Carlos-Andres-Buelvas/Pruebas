#ifndef RESERVA_H
#define RESERVA_H

#include <string>
#include "fecha.h"

// Declaraciones adelantadas
class Huesped;
class Alojamiento;

class Reserva {
private:
    std::string codigo;
    Alojamiento* alojamiento;
    Huesped* huesped;
    Fecha fechaEntrada;
    int duracion;
    std::string metodoPago;
    Fecha fechaPago;
    int monto;
    char anotacion[1001];

public:
    // Constructores
    Reserva();
    Reserva(const std::string& cod, Alojamiento* alo, Huesped* h,
            const Fecha& entrada, int dur, const std::string& metodo,
            const Fecha& pago, int monto, const std::string& nota);
    Reserva(const Reserva& otra); // Constructor de copia

    // Operador de asignación
    Reserva& operator=(const Reserva& otra);

    // Getters
    std::string getCodigo() const;
    Alojamiento* getAlojamiento() const;
    Huesped* getHuesped() const;
    Fecha getFechaEntrada() const;
    int getDuracion() const;
    std::string getMetodoPago() const;
    Fecha getFechaPago() const;
    int getMonto() const;
    std::string getAnotacion() const;

    // Métodos funcionales
    void mostrarComprobante() const;
    bool coincideCon(const Fecha& entrada, int duracion) const;
};

#endif // RESERVA_H
