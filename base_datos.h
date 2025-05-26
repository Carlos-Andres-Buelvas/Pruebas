#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include "reserva.h"
#include "fecha.h"

/**
 * @brief Carga la base de datos desde los archivos de texto.
 *
 * @param anfitriones Arreglo dinámico de anfitriones.
 * @param cantAnfitriones Cantidad actual de anfitriones.
 * @param alojamientos Arreglo dinámico de alojamientos.
 * @param cantAlojamientos Cantidad actual de alojamientos.
 * @param huespedes Arreglo dinámico de huéspedes.
 * @param cantHuespedes Cantidad actual de huéspedes.
 * @param reservas Arreglo dinámico de reservas.
 * @param cantReservas Cantidad actual de reservas.
 * @param capacidad Capacidad máxima actual del arreglo de reservas.
 */
void cargarBaseDatos(Anfitrion*& anfitriones, int& cantAnfitriones,
                     Alojamiento*& alojamientos, int& cantAlojamientos,
                     Huesped*& huespedes, int& cantHuespedes,
                     Reserva*& reservas, int& cantReservas, int& capacidad);

/**
 * @brief Elimina del arreglo las reservas finalizadas antes de una fecha dada.
 *
 * @param reservas Arreglo dinámico de reservas.
 * @param cantidad Cantidad actual de reservas.
 * @param fechaCorte Fecha límite para conservar las reservas.
 */
void depurarReservas(Reserva*& reservas, int& cantidad, const Fecha& fechaCorte);

/**
 * @brief Muestra en pantalla un resumen histórico del sistema.
 */
void mostrarHistorico();

#endif // BASE_DATOS_H
