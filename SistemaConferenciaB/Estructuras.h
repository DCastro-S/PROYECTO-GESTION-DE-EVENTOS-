#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define ESTRUCTURAS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
// Definicion de estructuras sesion, participantes e inscripcion
struct Sesion {
    string id; // de tipo String porque este puede ser de codigo SS90
    string titulo; // de tipo String porque en el guardaremos un acadena de carcteres "Introduccion a c++"
    int horaInicio;// de tipo entero, usando formato de 24 horas (ej. 900 para las 09:00)
    int horaFin; // de tipo entero, usando formato de 24 horas (ej. 1030 para las 10:30)
    int cupoMaximo; // la capacidad limite de espacios disponibles.
    int cupoActual; // para saber la cantidad de cupos ya utilizados
};

struct Participante {
    string id; // de tipo estring el numero o crané del participante
    string nombre; // para el nombre del participante
};

struct Inscripcion {
    string idParticipante; // relaionará los ids de particpantes registrados
    string idSesion; // relacionará las ids del participante con el  de las sesiones registradas
};

/* Declaración de vectores globales (usando extern para compartirlos)
 por medio de vectores se guardará temporalmente la lista de participantes, sesiones e inscripciones*/
extern vector<Sesion> sesiones;
extern vector<Participante> participantes;
extern vector<Inscripcion> inscripciones;

// agrupacion de Prototipos de funciones (Implemnetacion de antintontos)
int leerEnteroSeguro(string mensaje);// eera un numero entero, si en dado caso el usuario coloca una letra, esta funcion limpiará y volverá a slicitar el entero
int buscarSesionPorId(string id); //
int buscarParticipantePorId(string id);
void buscarSesionPorTitulo();// para buscar las sesiones y participantes por id, así mismo las sesiones p
void ordenarSesionesPorHorario(); // impide que programemos una sesion que termine antes de la hora de inicio y viceversa
bool yaEstaInscrito(string idPart, string idSes); // impide inscribir a un participante nuevamente en la misma sesion
bool hayChoqueDeHorario(const Sesion& s1, const Sesion& s2);// si el participante se inscribe en el mismo hoario en dos sesiones nos mostrará el mensaje
void registrarParticipante();//acciones pricipales en donde pedimos el id y nombre
void crearSesion();// Aqui se crean las sesiones se pide id y nombre de la sesion
void inscribirParticipante();// si todo cumple, si el registro delpartcipante es correcto, si la sesion existe, si no hay choque de horarios entonces se habilita un espacio para guardar el registro
void mostrarEstadoSesiones(); //imprime el reporte en pantalla
string iniciarSesion(); // Aqui es en donde hacemos uso del login, pdeimos el usuraio y la contraseña
void cargarDatosDesdeBD(); // la peuqeña conexión a base de datos, y se ejecuta cuando empieza  funcionar e archivo main.
#endif // ESTRUCTURAS_H_INCLUDED






