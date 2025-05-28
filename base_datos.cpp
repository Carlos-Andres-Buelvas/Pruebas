#include "base_datos.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

// ----------------------------
// Funciones auxiliares privadas
// ----------------------------

namespace {
void redimensionarAnfitriones(Anfitrion*& anfitriones, int& capacidad) {
    capacidad *= 2;
    Anfitrion* temp = new Anfitrion[capacidad];
    for (int i = 0; i < capacidad / 2; ++i)
        temp[i] = anfitriones[i];
    delete[] anfitriones;
    anfitriones = temp;
}

void redimensionarHuespedes(Huesped*& huespedes, int& capacidad) {
    capacidad *= 2;
    Huesped* temp = new Huesped[capacidad];
    for (int i = 0; i < capacidad / 2; ++i)
        temp[i] = huespedes[i];
    delete[] huespedes;
    huespedes = temp;
}

void redimensionarAlojamientos(Alojamiento*& alojamientos, int& capacidad) {
    capacidad *= 2;
    Alojamiento* temp = new Alojamiento[capacidad];
    for (int i = 0; i < capacidad / 2; ++i)
        temp[i] = alojamientos[i];
    delete[] alojamientos;
    alojamientos = temp;
}

void redimensionarReservas(Reserva*& reservas, int& capacidad) {
    capacidad *= 2;
    Reserva* temp = new Reserva[capacidad];
    for (int i = 0; i < capacidad / 2; ++i)
        temp[i] = reservas[i];
    delete[] reservas;
    reservas = temp;
}

Anfitrion* buscarAnfitrion(Anfitrion* anfitriones, int cant, const string& documento) {
    for (int i = 0; i < cant; ++i)
        if (anfitriones[i].getDocumento() == documento)
            return &anfitriones[i];
    return nullptr;
}

Huesped* buscarHuesped(Huesped* huespedes, int cant, const string& documento) {
    for (int i = 0; i < cant; ++i)
        if (huespedes[i].getDocumento() == documento)
            return &huespedes[i];
    return nullptr;
}

Alojamiento* buscarAlojamiento(Alojamiento* alojamientos, int cant, const string& codigo) {
    for (int i = 0; i < cant; ++i)
        if (alojamientos[i].getCodigo() == codigo)
            return &alojamientos[i];
    return nullptr;
}
}

// ----------------------------
// 1. Cargar todos los datos
// ----------------------------
void cargarBaseDatos(Anfitrion*& anfitriones, int& cantAnfitriones,
                     Alojamiento*& alojamientos, int& cantAlojamientos,
                     Huesped*& huespedes, int& cantHuespedes,
                     Reserva*& reservas, int& cantReservas, int& capacidad)
{
    cout << "[INFO] Iniciando carga de ANFITRIONES...\n";
    ifstream archAnfit("anfitriones.txt");
    if (!archAnfit) {
        cerr << "[ERROR] No se pudo abrir anfitriones.txt\n";
        return;
    }

    string linea;
    getline(archAnfit, linea); // encabezado
    int capAnfit = 10;
    anfitriones = new Anfitrion[capAnfit];
    cantAnfitriones = 0;

    while (getline(archAnfit, linea)) {
        //cout << "[INFO] Leyendo anfitrión: " << linea << endl;
        stringstream ss(linea);
        string doc, nombre, clave, antigStr, puntStr;
        getline(ss, doc, ';'); getline(ss, nombre, ';'); getline(ss, antigStr, ';');
        getline(ss, puntStr, ';'); getline(ss, clave, ';');

        int antig = stoi(antigStr); float punt = stof(puntStr);
        Anfitrion nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantAnfitriones >= capAnfit)
            redimensionarAnfitriones(anfitriones, capAnfit);

        anfitriones[cantAnfitriones++] = nuevo;
    }
    archAnfit.close();
    cout << "[OK] Anfitriones cargados: " << cantAnfitriones << endl;


    cout << "[INFO] Iniciando carga de ALOJAMIENTOS...\n";
    ifstream archAloj("alojamientos.txt");
    if (!archAloj) {
        cerr << "[ERROR] No se pudo abrir alojamientos.txt\n";
        return;
    }

    getline(archAloj, linea);
    int capAloj = 10;
    alojamientos = new Alojamiento[capAloj];
    cantAlojamientos = 0;

    while (getline(archAloj, linea)) {
        //cout << "[INFO] Leyendo alojamiento: " << linea << endl;
        stringstream ss(linea);
        string cod, nom, docAnfit, dep, mun, tipo, dir, precioStr, amenStr;
        getline(ss, cod, ';'); getline(ss, nom, ';'); getline(ss, docAnfit, ';');
        getline(ss, dep, ';'); getline(ss, mun, ';'); getline(ss, tipo, ';');
        getline(ss, dir, ';'); getline(ss, precioStr, ';'); getline(ss, amenStr, ';');

        float precio = stof(precioStr);
        bool amen[6] = {0};
        stringstream ssAmen(amenStr);
        string bit; int i = 0;
        while (getline(ssAmen, bit, ',') && i < 6)
            amen[i++] = (bit == "1");

        Anfitrion* anfitrion = buscarAnfitrion(anfitriones, cantAnfitriones, docAnfit);
        if (!anfitrion) {
            cerr << "[ERROR] Anfitrión no encontrado para alojamiento " << cod << endl;
            continue;
        }

        if (cantAlojamientos >= capAloj)
            redimensionarAlojamientos(alojamientos, capAloj);

        alojamientos[cantAlojamientos] = Alojamiento(cod, dep, mun, nom, tipo, dir, anfitrion, precio, amen);
        anfitrion->agregarAlojamiento(&alojamientos[cantAlojamientos]);
        cantAlojamientos++;
    }
    archAloj.close();
    cout << "[OK] Alojamientos cargados: " << cantAlojamientos << endl;


    cout << "[INFO] Iniciando carga de HUESPEDES...\n";
    ifstream archHuesp("huespedes.txt");
    if (!archHuesp) {
        cerr << "[ERROR] No se pudo abrir huespedes.txt\n";
        return;
    }

    getline(archHuesp, linea);
    int capHuesp = 10;
    huespedes = new Huesped[capHuesp];
    cantHuespedes = 0;

    while (getline(archHuesp, linea)) {
        //cout << "[INFO] Leyendo huésped: " << linea << endl;
        stringstream ss(linea);
        string doc, nombre, clave, antigStr, puntStr;
        getline(ss, doc, ';'); getline(ss, nombre, ';');
        getline(ss, antigStr, ';'); getline(ss, puntStr, ';'); getline(ss, clave, ';');

        int antig = stoi(antigStr); float punt = stof(puntStr);
        Huesped nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantHuespedes >= capHuesp)
            redimensionarHuespedes(huespedes, capHuesp);

        huespedes[cantHuespedes++] = nuevo;
    }
    archHuesp.close();
    cout << "[OK] Huéspedes cargados: " << cantHuespedes << endl;


    cout << "[INFO] Iniciando carga de RESERVAS...\n";
    ifstream archRes("reservas.txt");
    if (!archRes) {
        cerr << "[ERROR] No se pudo abrir reservas.txt\n";
        return;
    }

    getline(archRes, linea);
    int capRes = 10;
    reservas = new Reserva[capRes];
    cantReservas = 0;
    capacidad = capRes;

    while (getline(archRes, linea)) {
        cout << "[INFO] Leyendo reserva: " << linea << endl;
        stringstream ss(linea);
        string cod, codAloj, docHuesp, fechaIn, durStr, metodo, fechaPag, montoStr, nota;

        getline(ss, cod, ';'); getline(ss, codAloj, ';'); getline(ss, docHuesp, ';');
        getline(ss, fechaIn, ';'); getline(ss, durStr, ';'); getline(ss, metodo, ';');
        getline(ss, fechaPag, ';'); getline(ss, montoStr, ';'); getline(ss, nota, ';');

        Fecha entrada, pago;
        int d, m, a;
        sscanf(fechaIn.c_str(), "%d/%d/%d", &d, &m, &a); entrada = Fecha(d, m, a);
        sscanf(fechaPag.c_str(), "%d/%d/%d", &d, &m, &a); pago = Fecha(d, m, a);

        Huesped* h = buscarHuesped(huespedes, cantHuespedes, docHuesp);
        Alojamiento* aObj = buscarAlojamiento(alojamientos, cantAlojamientos, codAloj);
        if (!h || !aObj) {
            cerr << "[WARN] Reserva " << cod << " inválida (huésped o alojamiento no encontrado).\n";
            continue;
        }

        int dur = stoi(durStr);
        if (h->hayConflicto(entrada, dur)) {
            cerr << "[WARN] Reserva " << cod << " NO agregada por conflicto de fechas.\n";
            continue;
        }
        if (cantReservas > capRes-1)
            redimensionarReservas(reservas, capRes);

        reservas[cantReservas] = Reserva(cod, aObj, h, entrada, dur, metodo, pago, stoi(montoStr), nota);
        std::cout << "[DEBUG] Agregando reserva a huésped " << h->getDocumento() << " (idx: " << cantReservas << ")\n";
        std::cout << "[DEBUG] Reserva creada: " << reservas[cantReservas].getCodigo() << "\n";
        h->agregarReserva(&reservas[cantReservas]);
        aObj->reservarDias(entrada, dur);
        cantReservas++;
    }

    archRes.close();
    capacidad = capRes;
    cout << "[OK] Reservas cargadas: " << cantReservas << endl;
}
// ----------------------------
// 2. Buscar alojamientos disponibles
// ----------------------------
void buscarAlojamientosDisponibles(Alojamiento* alojamientos, int cantA,
                                   const Fecha& entrada, int duracion,
                                   const string& municipio,
                                   float precioMax, float puntMin)
{
    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    bool encontrados = false;

    for (int i = 0; i < cantA; ++i) {
        Alojamiento& a = alojamientos[i];

        if (a.getMunicipio() != municipio) continue;
        if (!a.estaDisponible(entrada, duracion)) continue;
        if (precioMax != -1 && a.getPrecioNoche() > precioMax) continue;
        if (puntMin != -1 && a.getAnfitrion()->getPuntuacion() < puntMin) continue;

        a.mostrar();
        cout << "-------------------------\n";
        encontrados = true;
    }

    if (!encontrados)
        cout << "No se encontraron alojamientos disponibles.\n";
}
