/* seguimiento de las mejoras en el proyecto
//menu prinicpal

*/
#include "Estructuras.h"

int main() {
    string rol="";

    //Buvcle de login antes de entrar al sistema
    while (rol==""){
        rol = iniciarSesion();
    }
    int opcion;
    do {
        cout << "\n===================================================\n";
        cout << "         SISTEMA DE GESTION DE CONFERENCIAS        \n";
        cout << "                   VARIANTE B                      \n";
        cout << "             Rol Actual: " << rol << "             \n";
        cout << "=====================================================\n";


        if (rol == "admin"){
            // El administrador puede hacer todo
            cout << "1. Registrar Participante\n";
            cout << "2. Crear Sesion / Conferencia\n";
            cout << "3. Inscribir Participante a Sesion\n";
            cout << "4. Ver Estado de Sesiones y Asistentes\n";
            cout << "5. Buscar Sesion por Titulo\n";
            cout << "6. Ordenar Sesiones por Horario\n";
            cout << "7. Salir del Sistema\n";
        } else {
            // El registrador solo tiene acceso limitado
            cout << "1. Registrar Participante\n";
            cout << "3. Inscribir Participante a Sesion\n";
            cout << "4. Ver Estado de Sesiones y Asistentes\n";
            cout << "5. Buscar Sesion por Titulo\n";
            cout << "7. Salir del Sistema\n";
        }
        opcion = leerEnteroSeguro("Seleccione una opcion: ");
        //Control de opciones segun el rol
        if (rol == "registrador" && (opcion == 2 || opcion == 6)) {
            cout << "Acceso denegado: Esta opcion es exclusiva para Administradores.\n";
            continue;
        }
        switch (opcion) {
            case 1: registrarParticipante(); break;
            case 2: crearSesion(); break;
            case 3: inscribirParticipante(); break;
            case 4: mostrarEstadoSesiones(); break;
            case 5: buscarSesionPorTitulo(); break;
            case 6: ordenarSesionesPorHorario(); break;
            case 7: cout << "Saliendo del sistema. ¡Feliz Dia! \n"; break;
            default: cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 7);

    return 0;
}
