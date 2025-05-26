#include "reserva.h"
#include "alojamiento.h"
#include "huesped.h"
#include <iostream>
#include <cstring>

// Constructor por defecto
Reserva::Reserva()
    : codigo(""), alojamiento(nullptr), huesped(nullptr),
    fechaEntrada(), duracion(0), metodoPago(""),
    fechaPago(), monto(0)
{
    anotacion[0] = '\0';
}

// Constructor parametrizado
Reserva::Reserva(const std::string& cod, Alojamiento* alo, Huesped* h,
                 const Fecha& entrada, int dur, const std::string& metodo,
                 const Fecha& pago, int m, const std::string& nota)
    : codigo(cod), alojamiento(alo), huesped(h),
    fechaEntrada(entrada), duracion(dur), metodoPago(metodo),
    fechaPago(pago), monto(m)
{
    std::strncpy(anotacion, nota.c_str(), 1000);
    anotacion[1000] = '\0'; // Seguridad: null-terminar
}

// Constructor de copia
Reserva::Reserva(const Reserva& otra)
    : codigo(otra.codigo), alojamiento(otra.alojamiento),
    huesped(otra.huesped), fechaEntrada(otra.fechaEntrada),
    duracion(otra.duracion), metodoPago(otra.metodoPago),
    fechaPago(otra.fechaPago), monto(otra.monto)
{
    std::strcpy(anotacion, otra.anotacion);
}

// Operador de asignación
Reserva& Reserva::operator=(const Reserva& otra) {
    if (this != &otra) {
        codigo = otra.codigo;
        alojamiento = otra.alojamiento;
        huesped = otra.huesped;
        fechaEntrada = otra.fechaEntrada;
        duracion = otra.duracion;
        metodoPago = otra.metodoPago;
        fechaPago = otra.fechaPago;
        monto = otra.monto;
        std::strcpy(anotacion, otra.anotacion);
    }
    return *this;
}

// Getters
std::string Reserva::getCodigo() const { return codigo; }
Alojamiento* Reserva::getAlojamiento() const { return alojamiento; }
Huesped* Reserva::getHuesped() const { return huesped; }
Fecha Reserva::getFechaEntrada() const { return fechaEntrada; }
int Reserva::getDuracion() const { return duracion; }
std::string Reserva::getMetodoPago() const { return metodoPago; }
Fecha Reserva::getFechaPago() const { return fechaPago; }
int Reserva::getMonto() const { return monto; }
std::string Reserva::getAnotacion() const { return std::string(anotacion); }

// Mostrar comprobante
void Reserva::mostrarComprobante() const {
    Fecha salida = fechaEntrada.sumarDias(duracion);
    Fecha hoySistema = getFechaSistema();

    std::cout << "------------------------------\n";
    std::cout << "=== COMPROBANTE DE RESERVA ===\n";
    std::cout << "Reserva: " << codigo << '\n';
    std::cout << "Nombre del huésped: " << (huesped ? huesped->getNombre() : "N/A") << '\n';
    std::cout << "Alojamiento: " << (alojamiento ? alojamiento->getCodigo() : "N/A") << '\n';
    std::cout << "Duración: " << duracion << " noche(s)\n";
    std::cout << "Entrada: "; fechaEntrada.mostrarExtendido();
    std::cout << "Salida: "; salida.mostrarExtendido();
    std::cout << "Método de pago: " << metodoPago << '\n';
    std::cout << "Pago realizado el: "; hoySistema.mostrarExtendido();
    std::cout << "Monto: $" << monto << '\n';
    std::cout << "Anotación: " << anotacion << '\n';
    std::cout << "------------------------------\n";
}

// Verifica coincidencia de reserva
bool Reserva::coincideCon(const Fecha& entrada, int dur) const {
    return fechaEntrada.toEntero() == entrada.toEntero() && duracion == dur;
}
