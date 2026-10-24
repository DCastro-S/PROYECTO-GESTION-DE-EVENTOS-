#include "Estructuras.h"

// Creación real de los vectores globales
vector<Sesion> sesiones;
vector<Participante> participantes;
vector<Inscripcion> inscripciones;

int leerEnteroSeguro(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Entrada invalida. Por favor, ingrese un numero entero.\n";
        } else {
            cin.ignore(10000, '\n');
            return valor;
        }
    }
}

int buscarSesionPorId(string id) {
    for (size_t i = 0; i < sesiones.size(); i++) {
        if (sesiones[i].id == id) return i;
    }
    return -1;
}

int buscarParticipantePorId(string id) {
    for (size_t i = 0; i < participantes.size(); i++) {
        if (participantes[i].id == id) return i;
    }
    return -1;
}

void buscarSesionPorTitulo() {
    cin.ignore();
    string criterio;
    cout << "\n--- BUSCAR SESION POR PALABRA CLAVE / TITULO ---\n";
    cout << "Ingrese parte del titulo de la conferencia: ";
    getline(cin, criterio);

    bool encontrada = false;
    cout << "\nResultados de la busqueda:\n";
    for (const auto& s : sesiones) {
        if (s.titulo.find(criterio) != string::npos) {
            cout << "ID: [" << s.id << "] " << s.titulo
                 << " | Horario: " << s.horaInicio << " - " << s.horaFin
                 << " hrs | Aforo: " << s.cupoActual << "/" << s.cupoMaximo << "\n";
            encontrada = true;
        }
    }
    if (!encontrada) {
        cout << "No se encontraron sesiones que coincidan con ese criterio.\n";
    }
}

void ordenarSesionesPorHorario() {
    if (sesiones.size() < 2) {
        cout << "\nNo hay suficientes sesiones para ordenar.\n";
        return;
    }
    for (size_t i = 0; i < sesiones.size() - 1; i++) {
        for (size_t j = 0; j < sesiones.size() - i - 1; j++) {
            if (sesiones[j].horaInicio > sesiones[j + 1].horaInicio) {
                Sesion temp = sesiones[j];
                sesiones[j] = sesiones[j + 1];
                sesiones[j + 1] = temp;
            }
        }
    }
    cout << "\n¡Sesiones ordenadas cronologicamente con exito!\n";
}

bool yaEstaInscrito(string idPart, string idSes) {
    for (const auto& ins : inscripciones) {
        if (ins.idParticipante == idPart && ins.idSesion == idSes) return true;
    }
    return false;
}

bool hayChoqueDeHorario(const Sesion& s1, const Sesion& s2) {
    return (s1.horaInicio < s2.horaFin && s2.horaInicio < s1.horaFin);
}

void registrarParticipante() {
    Participante p;
    cout << "\n------- REGISTRAR NUEVO PARTICIPANTE -------\n";
    cout << "Ingrese ID o carne del participante: ";
    cin >> p.id;
    if (buscarParticipantePorId(p.id) != -1) {
        cout << "Error: Ya existe un participante registrado con este ID.\n";
        return;
    }
    cin.ignore();
    cout << "Ingrese Nombre completo: ";
    getline(cin, p.nombre);
    participantes.push_back(p);
    cout << "¡Participante registrado con exito!\n";
}

void crearSesion() {
    Sesion s;
    cout << "\n------- CREAR NUEVA CONFERENCIA / SESION -------\n";
    cout << "Ingrese ID de la sesion: ";
    cin >> s.id;
    if (buscarSesionPorId(s.id) != -1) {
        cout << "Error: Ya existe una sesion registrada con este ID.\n";
        return;
    }
    cin.ignore();
    cout << "Ingrese Titulo de la conferencia: ";
    getline(cin, s.titulo);
    s.horaInicio = leerEnteroSeguro("Hora de Inicio (formato 24h, ej. 900): ");
    s.horaFin = leerEnteroSeguro("Hora de Fin (formato 24h, ej. 1030): ");
    if (s.horaInicio >= s.horaFin) {
        cout << "Error: La hora de inicio debe ser menor que la hora de fin.\n";
        return;
    }
    s.cupoMaximo = leerEnteroSeguro("Cupo Maximo de asistentes: ");
    s.cupoActual = 0;
    sesiones.push_back(s);
    cout << "¡Sesion creada con exito!\n";
}

void inscribirParticipante() {
    cout << "\n------- INSCRIPCION A CONFERENCIA -------\n";
    string idPart, idSes;
    cout << "Ingrese ID del Participante: ";
    cin >> idPart;
    int idxPart = buscarParticipantePorId(idPart);
    if (idxPart == -1) {
        cout << "Error: Participante no encontrado en el sistema.\n";
        return;
    }
    cout << "Ingrese ID de la Sesion: ";
    cin >> idSes;
    int idxSes = buscarSesionPorId(idSes);
    if (idxSes == -1) {
        cout << "Error: Sesion no encontrada en el sistema.\n";
        return;
    }
    Sesion& sesionObjetivo = sesiones[idxSes];
    if (sesionObjetivo.cupoActual >= sesionObjetivo.cupoMaximo) {
        cout << "Operacion rechazada: La sesion ha alcanzado su cupo maximo de aforo.\n";
        return;
    }
    if (yaEstaInscrito(idPart, idSes)) {
        cout << "Operacion rechazada: El participante ya esta inscrito en esta sesion.\n";
        return;
    }
    for (const auto& ins : inscripciones) {
        if (ins.idParticipante == idPart) {
            int idxSesPrevia = buscarSesionPorId(ins.idSesion);
            if (idxSesPrevia != -1) {
                const Sesion& sesionPrevia = sesiones[idxSesPrevia];
                if (hayChoqueDeHorario(sesionObjetivo, sesionPrevia)) {
                    cout << "CONFLICTO DE HORARIO DETECTADO:\n"
                         << "    La sesion '" << sesionObjetivo.titulo << "' se cruza con '"
                         << sesionPrevia.titulo << "'.\n";
                    return;
                }
            }
        }
    }
    Inscripcion nuevaIns = {idPart, idSes};
    inscripciones.push_back(nuevaIns);
    sesionObjetivo.cupoActual++;
    cout << "¡Inscripcion confirmada exitosamente!\n";
}

void mostrarEstadoSesiones() {
    cout << "\n===================================================\n";
    cout << "         ESTADO ACTUAL DE LAS CONFERENCIAS         \n";
    cout << "===================================================\n";
    if (sesiones.empty()) {
        cout << "No hay sesiones registradas en el sistema.\n";
        return;
    }
    for (const auto& s : sesiones) {
        cout << "ID: [" << s.id << "] " << s.titulo << "\n"
             << "    Horario: " << s.horaInicio << " hrs - " << s.horaFin << " hrs\n"
             << "    Aforo: " << s.cupoActual << " / " << s.cupoMaximo << " inscritos\n";
        cout << "    Asistentes: ";
        bool alguno = false;
        for (const auto& ins : inscripciones) {
            if (ins.idSesion == s.id) {
                int idxP = buscarParticipantePorId(ins.idParticipante);
                if (idxP != -1) {
                    cout << participantes[idxP].nombre << " | ";
                    alguno = true;
                }
            }
        }
        if (!alguno) cout << "Ninguno";
        cout << "\n---------------------------------------------------\n";
    }
}


// Función simple de Login que retorna el rol ("admin" o "registrador" o "" si falla)
string iniciarSesion() {
    string usuario, password;
    cout << "\n===================================================\n";
    cout << "                LOGIN DE ACCESO                    \n";
    cout << "            Registrador/Administrador              \n";
    cout << "=====================================================\n";
    cout << "Usuario: ";
    cin >> usuario;
    cout << "Contrasena: ";
    cin >> password;

    // Credenciales quemadas de ejemplo (puedes cambiarlas)
    if (usuario == "admin" && password == "admin123") {
        cout << "¡Bienvenido Administrador!\n";
        return "admin";
    }
    else if (usuario == "registrador" && password == "reg123") {
        cout << "¡Bienvenido Registrador!\n";
        return "registrador";
    }
    else {
        cout << "Error: Usuario o contrasena incorrectos.\n";
        return "";
    }
}
