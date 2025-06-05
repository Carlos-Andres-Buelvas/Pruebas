#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <locale>
using namespace std;

#include "anfitrion.h"
#include "huesped.h"
#include "reserva.h"
#include "alojamiento.h"
#include "fecha.h"
#include "controlador.h"

extern int totalIteraciones;
extern int totalMemoria;

int siguienteNumeroReserva;

Controlador::Controlador() {
    anfitriones = nullptr;
    alojamientos = nullptr;
    huespedes = nullptr;
    reservas = new Reserva[10];
    totalMemoria += sizeof(Reserva) * 10;

    siguienteNumeroReserva = obtenerSiguienteNumeroReserva();

    Anfitrion::cargarDesdeArchivo("anfitriones.txt", anfitriones, cantAnfitriones, capAnfitriones);
    Huesped::cargarDesdeArchivo("huespedes.txt", huespedes, cantHuespedes, capHuespedes);
    Alojamiento::cargarDesdeArchivo("alojamientos.txt", alojamientos, cantAlojamientos, capAlojamientos, anfitriones, cantAnfitriones);
    Reserva::cargarDesdeArchivo("reservas.txt", reservas, cantReservas, capReservas, alojamientos, cantAlojamientos, huespedes, cantHuespedes);
}

void Controlador::iniciar(){
    menuPrincipal();
}

void Controlador::menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1. Iniciar sesión\n";
        cout << "0. Salir\n";
        cout << "Opción: ";

        // Validar entrada
        if (!(cin >> opcion)) {
            cin.clear(); // limpiar el error
            cin.ignore(10000, '\n'); // descartar entrada inválida
            cout << "[ERROR] Entrada inválida. Por favor ingrese un número.\n";
            continue; // volver a mostrar el menú
        }

        try {
            switch (opcion) {
            case 1:
                iniciarSesion();
                break;
            case 0:
                cout << "\nGracias por usar el sistema. ¡Hasta luego!\n";
                break;
            default:
                throw std::invalid_argument("Opción no válida.");
            }
        } catch (const std::invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        } catch (const std::exception& e) {
            cout << "[ERROR inesperado] " << e.what() << "\n";
        }

    } while (opcion != 0);
}

void Controlador::iniciarSesion() {
    string doc, claveIngresada;

    try {
        cout << "\n--- Iniciar sesión ---\n";

        cout << "Ingrese su número de documento: ";
        if (!(cin >> doc)) {
            throw std::runtime_error("Error al leer el documento.");
        }

        // Validación: el documento debe contener solo dígitos
        for (char c : doc) {
            if (!isdigit(c)) {
                throw std::invalid_argument("El documento debe contener solo números.");
            }
        }

        cout << "Ingrese su clave: ";
        if (!(cin >> claveIngresada)) {
            throw std::runtime_error("Error al leer la clave.");
        }

        // Buscar en huéspedes
        for (int i = 0; i < cantHuespedes; ++i) {
            totalIteraciones++;
            if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == claveIngresada) {
                mostrarMenuHuesped(&huespedes[i]);
                return;
            }
        }

        // Buscar en anfitriones
        for (int i = 0; i < cantAnfitriones; ++i) {
            totalIteraciones++;
            if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == claveIngresada) {
                mostrarMenuAnfitrion(&anfitriones[i]);
                return;
            }
        }

        cout << "[ERROR] Documento o clave incorrectos.\n";

    } catch (const std::invalid_argument& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ENTRADA INVÁLIDA] " << e.what() << "\n";
    } catch (const std::exception& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[EXCEPCIÓN] " << e.what() << "\n";
    }
}

void Controlador::mostrarMenuHuesped(Huesped* h) {
    int opcion;

    try {
        do {
            cout << "\n=== MENÚ HUÉSPED ===\n";
            cout << "Bienvenido, " << h->getNombre() << " (documento: " << h->getDocumento() << ")\n";
            cout << "1. Buscar alojamientos disponibles\n";
            cout << "2. Reservar alojamiento\n";
            cout << "3. Ver antigüedad y puntuación\n";
            cout << "4. Mostrar y/o anular una reservación\n";
            cout << "0. Cerrar sesión\n";
            cout << "Opción: ";

            if (!(cin >> opcion)) {
                throw std::invalid_argument("Entrada no válida. Se esperaba un número.");
            }

            switch (opcion) {
            case 1:
                buscarYReservarAlojamiento(h);
                break;
            case 2:
                reservarAlojamiento(h);
                break;
            case 3:
                cout << "Antigüedad: " << h->getAntiguedad() << " meses\n";
                cout << "Puntuación: " << h->getPuntuacion() << "/5.0\n";
                break;
            case 4: {
                cout << "\n--- Reservaciones activas ---\n";
                int seleccionables[cantReservas], totalMostrar = 0;

                for (int i = 0; i < cantReservas; ++i) {
                    totalIteraciones++;
                    if (reservas[i].getHuesped()->getDocumento() == h->getDocumento()) {
                        cout << (totalMostrar + 1) << ". ";
                        reservas[i].mostrarComprobante();
                        seleccionables[totalMostrar++] = i;
                    }
                }

                if (totalMostrar == 0) {
                    cout << "No tiene reservaciones activas.\n";
                    break;
                }

                int eleccion;
                cout << "Seleccione el número de la reserva a anular (0 para cancelar): ";

                if (!(cin >> eleccion)) {
                    throw std::invalid_argument("Entrada no válida al seleccionar reserva.");
                }

                if (eleccion < 1 || eleccion > totalMostrar) {
                    cout << "Selección cancelada o fuera de rango.\n";
                    break;
                }

                int idx = seleccionables[eleccion - 1];
                Reserva* r = &reservas[idx];
                string codigo = r->getCodigo();
                anularReservacion(codigo, h, nullptr);
                break;
            }
            case 0:
                cout << "Sesión finalizada.\n";
                break;
            default:
                cout << "Opción inválida.\n";
            }

        } while (opcion != 0);

    } catch (const std::exception& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[EXCEPCIÓN] " << e.what() << "\n";
    }
}

void Controlador::mostrarMenuAnfitrion(Anfitrion* a) {
    int opcion;

    try {
        do {
            cout << "\n=== MENÚ ANFITRIÓN ===\n";
            cout << "Bienvenido, " << a->getNombre() << " (documento: " << a->getDocumento() << ")\n";
            cout << "1. Mostrar mis alojamientos\n";
            cout << "2. Ver antigüedad y puntuación\n";
            cout << "3. Anular una reservación asociada a mis alojamientos\n";
            cout << "4. Consultar reservaciones en rango de fechas\n";
            cout << "5. Actualizar histórico de reservas\n";
            cout << "6. Mostrar histórico de reservas\n";
            cout << "0. Cerrar sesión\n";
            cout << "Opción: ";

            if (!(cin >> opcion)) {
                throw std::invalid_argument("Entrada inválida. Se esperaba un número.");
            }

            switch (opcion) {
            case 1:
                a->mostrar();
                break;

            case 2:
                cout << "\nAntigüedad: " << a->getAntiguedad() << " meses\n";
                cout << "Puntuación: " << a->getPuntuacion() << "/5.0\n";
                break;

            case 3: {
                string cod;
                cout << "Código de la reservación a anular: ";
                cin >> cod;
                anularReservacion(cod, nullptr, a);
                break;
            }

            case 4: {
                int d1, m1, a1, d2, m2, a2;
                cout << "Ingrese fecha inicial (D M A): ";
                if (!(cin >> d1 >> m1 >> a1)) throw std::invalid_argument("Fecha inicial inválida.");

                cout << "Ingrese fecha final (D M A): ";
                if (!(cin >> d2 >> m2 >> a2)) throw std::invalid_argument("Fecha final inválida.");

                Fecha ini(d1, m1, a1), fin(d2, m2, a2);

                int total = 0;
                for (int i = 0; i < cantReservas; ++i) {
                    totalIteraciones++;
                    Reserva& r = reservas[i];
                    if (r.getAlojamiento()->getAnfitrion() == a &&
                        r.getFechaEntrada().esMayorQue(ini) &&
                        r.getFechaEntrada().esMenorQue(fin)) {
                        cout << "\n[" << ++total << "] ";
                        r.mostrarComprobante();
                    }
                }

                if (total == 0)
                    cout << "No se encontraron reservaciones en ese rango.\n";

                break;
            }

            case 5: {
                int d, m, a_;
                cout << "--- Actualizar histórico ---\n";
                cout << "Ingrese fecha de corte (D M A): ";
                if (!(cin >> d >> m >> a_)) throw std::invalid_argument("Fecha inválida.");

                Fecha fechaCorte(d, m, a_);

                depurarReservas(fechaCorte);
                sobrescribirArchivoReservas();

                cout << "Histórico actualizado. Solo se conservaron reservas desde la fecha indicada.\n";
                break;
            }

            case 6:
                this->mostrarHistorico();
                break;

            case 0:
                cout << "Sesión finalizada.\n";
                break;

            default:
                cout << "Opción inválida.\n";
            }

        } while (opcion != 0);

    } catch (const std::exception& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n[EXCEPCIÓN] " << e.what() << "\n";
        // Reintentar
        mostrarMenuAnfitrion(a);
    }
}

void Controlador::asegurarCapacidadReservas() {
    if (cantReservas >= capReservas) {
        capReservas *= 2;
        Reserva* nuevo = new Reserva[capReservas];
        totalMemoria += sizeof(Reserva) * capReservas;
        for (int i = 0; i < cantReservas; ++i)
            nuevo[i] = reservas[i];
        delete[] reservas;
        reservas = nuevo;

        // Reparar punteros en los huéspedes
        for (int i = 0; i < cantHuespedes; ++i)
            huespedes[i].repararPunterosReservas(reservas, cantReservas);       
    }
}

void Controlador::buscarYReservarAlojamiento(Huesped* h) {
    try {
        cout << "--- Buscar y reservar alojamiento ---\n";
        string municipio;
        int d, m, a, noches;

        cout << "Municipio: ";
        cin >> ws;
        getline(cin, municipio);

        cout << "Fecha de entrada (D M A): ";
        if (!(cin >> d >> m >> a)) throw std::invalid_argument("Fecha inválida.");
        cout << "Duración (noches): ";
        if (!(cin >> noches)) throw std::invalid_argument("Duración inválida.");

        Fecha entrada(d, m, a);
        Fecha hoy = getFechaSistema();
        Fecha maxPermitida = hoy.sumarDias(365);

        if (entrada.esMenorQue(hoy) || entrada.esMayorQue(maxPermitida)) {
            cout << "La fecha de entrada debe estar entre hoy y dentro de 12 meses máximo.\n";
            return;
        }

        // Filtros opcionales
        char aplicarFiltro;
        float precioMax = -1, puntMin = -1;
        cout << "¿Desea aplicar filtros de precio y puntuación? (s/n): ";
        cin >> aplicarFiltro;
        if (aplicarFiltro == 's' || aplicarFiltro == 'S') {
            cout << "Ingrese precio máximo por noche (-1 si no desea aplicar): ";
            if (!(cin >> precioMax)) throw std::invalid_argument("Precio inválido.");
            cout << "Ingrese puntuación mínima del anfitrión (-1 si no desea aplicar): ";
            if (!(cin >> puntMin)) throw std::invalid_argument("Puntuación inválida.");
        }

        // Recolectar alojamientos disponibles
        const int MAX_DISPONIBLES = cantReservas;
        Alojamiento* disponibles[MAX_DISPONIBLES];
        int cantDisponibles = 0;

        for (int i = 0; i < cantAlojamientos; ++i) {
            totalIteraciones++;
            Alojamiento& a = alojamientos[i];

            if (a.getMunicipio() == municipio &&
                a.estaDisponible(entrada, noches) &&
                (precioMax < 0 || a.getPrecioNoche() <= precioMax) &&
                (puntMin < 0 || a.getAnfitrion()->getPuntuacion() >= puntMin)) {
                if (cantDisponibles < MAX_DISPONIBLES)
                    disponibles[cantDisponibles++] = &a;
            }
        }

        if (cantDisponibles == 0) {
            cout << "No hay alojamientos disponibles.\n";
            return;
        }

        // Mostrar lista
        cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
        for (int i = 0; i < cantDisponibles; ++i) {
            totalIteraciones++;
            cout << i + 1 << ".\n";
            disponibles[i]->mostrar();
            cout << "Puntuación del anfitrión: "
                 << disponibles[i]->getAnfitrion()->getPuntuacion()
                 << " / 5.0\n---------------------\n";
        }

        int opcion;
        cout << "Seleccione número (0 cancelar): ";
        if (!(cin >> opcion)) throw std::invalid_argument("Opción inválida.");
        if (opcion < 1 || opcion > cantDisponibles) {
            cout << "Reserva cancelada.\n";
            return;
        }

        Alojamiento* aloj = disponibles[opcion - 1];

        // Método de pago
        string metodo;
        cout << "Metodo de pago\n1. PSE\n2. TCredito\nOpción: ";
        cin >> ws;
        getline(cin, metodo);
        if (metodo != "1" && metodo != "2") {
            cout << "Opción inválida. Reserva cancelada.\n";
            return;
        }
        metodo = (metodo == "1") ? "PSE" : "TCredito";

        // Anotación
        string nota;
        cout << "Ingrese anotación (max 1000 chars): ";
        cin >> ws;
        getline(cin, nota);

        extern int siguienteNumeroReserva;
        std::ostringstream oss;
        oss << "RSV" << std::setw(3) << std::setfill('0') << siguienteNumeroReserva++;
        string codigo = oss.str();
        Fecha fechaPago = getFechaSistema();
        float monto = noches * aloj->getPrecioNoche();

        if (h->hayConflicto(entrada, noches)) {
            cout << "Conflicto de fechas. No se realizó la reserva.\n";
            return;
        }

        asegurarCapacidadReservas();

        reservas[cantReservas] = Reserva(codigo, aloj, h, entrada, noches, metodo, fechaPago, monto, nota);
        Reserva* ptrReserva = &reservas[cantReservas++];
        h->agregarReserva(ptrReserva);
        aloj->reservarDias(entrada, noches);
        guardarReservaIndividual(*ptrReserva);

        ptrReserva->mostrarComprobante();

    } catch (const std::exception& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n[ERROR] " << e.what() << "\n";
        cout << "Por favor, intente de nuevo.\n";
        buscarYReservarAlojamiento(h);
    }
}

void Controlador::reservarAlojamiento(Huesped* h) {
    try {
        cout << "--- Reservar alojamiento ---\n";
        string cod;
        int d, m, a, dur;

        cout << "Código del alojamiento: ";
        cin >> ws;
        getline(cin, cod);

        cout << "Fecha entrada (D M A): ";
        if (!(cin >> d >> m >> a)) throw std::invalid_argument("Fecha inválida.");
        cout << "Duración (noches): ";
        if (!(cin >> dur)) throw std::invalid_argument("Duración inválida.");

        Fecha entrada(d, m, a);
        Fecha hoy = getFechaSistema();
        Fecha maxPermitida = hoy.sumarDias(365);

        if (entrada.esMenorQue(hoy) || entrada.esMayorQue(maxPermitida)) {
            cout << "La fecha debe estar entre hoy y 12 meses hacia adelante.\n";
            return;
        }

        // Buscar alojamiento
        Alojamiento* aloj = nullptr;
        for (int i = 0; i < cantAlojamientos; ++i) {
            totalIteraciones++;
            if (alojamientos[i].getCodigo() == cod) {
                aloj = &alojamientos[i];
                break;
            }
        }

        if (!aloj) {
            cout << "Alojamiento no encontrado.\n";
            return;
        }

        if (!aloj->estaDisponible(entrada, dur)) {
            cout << "El alojamiento no está disponible.\n";
            return;
        }

        // Método de pago
        string metodo;
        cout << "Método de pago\n1. PSE\n2. TCredito\nOpción: ";
        cin >> ws;
        getline(cin, metodo);
        if (metodo != "1" && metodo != "2") {
            cout << "Opción inválida. Reserva cancelada.\n";
            return;
        }
        metodo = (metodo == "1") ? "PSE" : "TCredito";

        // Anotación
        string nota;
        cout << "Ingrese anotación (máx 1000 chars): ";
        cin >> ws;
        getline(cin, nota);

        extern int siguienteNumeroReserva;
        std::ostringstream oss;
        oss << "RSV" << std::setw(3) << std::setfill('0') << siguienteNumeroReserva++;
        string codigo = oss.str();
        Fecha fechaPago = getFechaSistema();
        float monto = dur * aloj->getPrecioNoche();

        // Validar conflicto de fechas
        if (h->hayConflicto(entrada, dur)) {
            cout << "El huésped ya tiene una reserva en esas fechas.\n";
            return;
        }

        // Redimensionar si es necesario
        asegurarCapacidadReservas();

        // Crear y almacenar
        reservas[cantReservas] = Reserva(codigo, aloj, h, entrada, dur, metodo, fechaPago, monto, nota);
        Reserva* ptr = &reservas[cantReservas++];
        h->agregarReserva(ptr);
        aloj->reservarDias(entrada, dur);
        guardarReservaIndividual(*ptr);

        ptr->mostrarComprobante();

    } catch (const std::exception& e) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n[ERROR] " << e.what() << "\n";
        cout << "Por favor, intente nuevamente.\n";
        reservarAlojamiento(h); // Llamado recursivo para reintentar
    }
}

int Controlador::obtenerSiguienteNumeroReserva(){
    ifstream archivo("reservas.txt");
    if (!archivo.is_open()) return 1;

    string linea, ultimoCodigo;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t pos = linea.find(';');
        if (pos != string::npos)
            ultimoCodigo = linea.substr(0, pos);
    }
    archivo.close();

    if (ultimoCodigo.size() < 4) return 1;
    int numero = stoi(ultimoCodigo.substr(3));
    return numero + 1;
}

void Controlador::anularReservacion(const string& codigo, Huesped* h, Anfitrion* a) {
    for (int i = 0; i < cantReservas; ++i) {
        totalIteraciones++;
        if (reservas[i].getCodigo() == codigo) {
            bool autorizado = false;
            Huesped* h_afectado = nullptr;

            if (h && reservas[i].getHuesped()->getDocumento() == h->getDocumento()) {
                autorizado = true;
                h_afectado = h;
            } else if (a && reservas[i].getAlojamiento()->getAnfitrion()->getDocumento() == a->getDocumento()) {
                autorizado = true;
                h_afectado = reservas[i].getHuesped();
            }

            if (!autorizado) {
                cout << "No tiene permiso para anular esta reservación.\n";
                return;
            }

            // Eliminar del huésped
            if (h_afectado)
                h_afectado->eliminarReservaPorCodigo(codigo);

            // Compactar arreglo global de reservas
            for (int j = i; j < cantReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            cantReservas--;

            // Reparar punteros internos del huésped (opcional si ya se eliminó la reserva)
            if (h_afectado && cantReservas > 0)
                h_afectado->repararPunterosReservas(reservas, cantReservas);

            sobrescribirArchivoReservas();
            cout << "Reservación anulada con éxito.\n";
            return;
        }
    }

    cout << "Reservación no encontrada.\n";
}

void Controlador::mostrarHistorico() {
    std::ifstream archivo("historico_reservas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo histórico.\n";
        return;
    }

    std::string linea;
    int contador = 1;
    std::cout << "\n--- Histórico de reservaciones ---\n";
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        std::cout << contador++ << ". " << linea << "\n";
    }

    archivo.close();
    if (contador == 1) std::cout << "No hay reservas históricas registradas.\n";
}

void Controlador::guardarReservaIndividual(const Reserva& r) {

    // Abrir archivo en modo binario para evitar corrupción de caracteres
    std::ofstream archivo("reservas.txt", std::ios::app | std::ios::binary);
    if (!archivo.is_open()) {
        std::cout << "Error al guardar la reserva en archivo.\n";
        return;
    }

    archivo << r.getCodigo() << ";"
            << r.getAlojamiento()->getCodigo() << ";"
            << r.getHuesped()->getDocumento() << ";"
            << r.getFechaEntrada().getDia() << "/"
            << r.getFechaEntrada().getMes() << "/"
            << r.getFechaEntrada().getAnio() << ";"
            << r.getDuracion() << ";"
            << r.getMetodoPago() << ";"
            << r.getFechaPago().getDia() << "/"
            << r.getFechaPago().getMes() << "/"
            << r.getFechaPago().getAnio() << ";"
            << r.getMonto() << ";"
            << r.getAnotacion() << "\n";

    archivo.close();
}

void Controlador::sobrescribirArchivoReservas() {
    std::ofstream archivo("reservas.txt");
    if (!archivo.is_open()) {
        std::cout << "Error al escribir en reservas.txt\n";
        return;
    }

    archivo << "codigoReserva;codAlojamiento;docHuesped;fechaEntrada;duracion;metodoPago;fechaPago;monto;anotacion\n";

    for (int i = 0; i < cantReservas; ++i) {
        const Reserva& r = reservas[i];

        // Filtrar reservas anuladas
        if (r.getCodigo() == "" || r.getAlojamiento() == nullptr || r.getHuesped() == nullptr)
            continue;

        archivo << r.getCodigo() << ";"
                << r.getAlojamiento()->getCodigo() << ";"
                << r.getHuesped()->getDocumento() << ";"
                << r.getFechaEntrada().getDia() << "/"
                << r.getFechaEntrada().getMes() << "/"
                << r.getFechaEntrada().getAnio() << ";"
                << r.getDuracion() << ";"
                << r.getMetodoPago() << ";"
                << r.getFechaPago().getDia() << "/"
                << r.getFechaPago().getMes() << "/"
                << r.getFechaPago().getAnio() << ";"
                << r.getMonto() << ";"
                << r.getAnotacion() << "\n";
    }

    archivo.close();
}

void Controlador::depurarReservas(const Fecha& fechaCorte) {
    std::ofstream archivo("historico_reservas.txt", std::ios::app);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo histórico.\n";
        return;
    }

    int nuevaCantidad = 0;
    for (int i = 0; i < cantReservas; ++i) {
        if (reservas[i].getFechaEntrada().esMayorQue(fechaCorte) ||
            reservas[i].getFechaEntrada().toEntero() == fechaCorte.toEntero()) {
            reservas[nuevaCantidad++] = reservas[i];
        } else {
            archivo << reservas[i].getCodigo() << ";"
                    << reservas[i].getAlojamiento()->getCodigo() << ";"
                    << reservas[i].getHuesped()->getDocumento() << ";"
                    << reservas[i].getFechaEntrada().getDia() << "/"
                    << reservas[i].getFechaEntrada().getMes() << "/"
                    << reservas[i].getFechaEntrada().getAnio() << ";"
                    << reservas[i].getDuracion() << ";"
                    << reservas[i].getMetodoPago() << ";"
                    << reservas[i].getFechaPago().getDia() << "/"
                    << reservas[i].getFechaPago().getMes() << "/"
                    << reservas[i].getFechaPago().getAnio() << ";"
                    << reservas[i].getMonto() << ";"
                    << reservas[i].getAnotacion() << "\n";
        }
    }

    cantReservas = nuevaCantidad;
    archivo.close();
}


Controlador::~Controlador() {
    delete[] reservas;
    delete[] huespedes;
    delete[] alojamientos;
    delete[] anfitriones;
}
