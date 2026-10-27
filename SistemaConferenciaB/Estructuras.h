#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define ESTRUCTURAS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Sesion {
    string id;
    string titulo;
    int horaInicio;
    int horaFin;
    int cupoMaximo;
    int cupoActual;
};

struct Participante {
    string id;
    string nombre;
};

struct Inscripcion {
    string idParticipante;
    string idSesion;
};

// Declaración de vectores globales (usando extern para compartirlos)
extern vector<Sesion> sesiones;
extern vector<Participante> participantes;
extern vector<Inscripcion> inscripciones;

// Prototipos de funciones
int leerEnteroSeguro(string mensaje);
int buscarSesionPorId(string id);
int buscarParticipantePorId(string id);
void buscarSesionPorTitulo();
void ordenarSesionesPorHorario();
bool yaEstaInscrito(string idPart, string idSes);
bool hayChoqueDeHorario(const Sesion& s1, const Sesion& s2);
void registrarParticipante();
void crearSesion();
void inscribirParticipante();
void mostrarEstadoSesiones();
string iniciarSesion();
void cargarDatosDesdeBD();
#endif // ESTRUCTURAS_H_INCLUDED






