#include "Estructuras.h"
#include "conexion.h"

// Creación real de los vectores globales
vector<Sesion> sesiones;
vector<Participante> participantes;
vector<Inscripcion> inscripciones;
//Uso de la funcion  antitontos
int leerEnteroSeguro(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail()) {  //ci.fail limpia y vuelve a preguntar sin que el programa cierre de golpe
            cin.clear(); // Limpia el estado del cin permitiendo volver a aceptar la entrada de datos (desbloquea el cin)
            cin.ignore(10000, '\n'); // ignora hasta el soguiente salto de linea (borra la entrada que quedó en el teclado
            cout << "Error: Entrada invalida. Por favor, ingrese un numero entero.\n";
        } else {
            cin.ignore(10000, '\n'); // si todo sale bien limpia el enter sobrante
            return valor; //retornará el resultado si todo cumple
        }
    }
}

int buscarSesionPorId(string id) {
    for (size_t i = 0; i < sesiones.size(); i++) {
        if (sesiones[i].id == id) return i;
    }
    return -1; /* esta funcion vaa recorrer los vectores de sesion en busca deuna coincidencia si la encuentra devuelve
    una posicion numerica, y si no devuelve -1 que nos dice "no existe"*/
}

int buscarParticipantePorId(string id) {
    for (size_t i = 0; i < participantes.size(); i++) {
        if (participantes[i].id == id) return i;
    }
    return -1; /* De igual manera, recorrerá los vectores en busca de coincidencias exactas
    si lo encuentra devolverá su posición numerica, si no devuelve el -1*/
}

void buscarSesionPorTitulo() { //con una fraccion de la palbra puede buscar dl titulo
    cin.ignore(); //limpia el enter que quedó al seleccionar el case 5 en el main
    string criterio;
    cout << "\n--- BUSCAR SESION POR PALABRA CLAVE / TITULO ---\n";
    cout << "Ingrese parte del titulo de la conferencia: ";
    getline(cin, criterio); // con el podemos leer palabras complejas y utilizando espacios ejem: "curso c++"

    bool encontrada = false;//Empieza en falso si alguna sesion coincide cambia flaso, y si al terminar de revisar el vectos no encuentra coincidencias seguirá en false, avisandoque no hubo coincidencias.
    cout << "\nResultados de la busqueda:\n";
    for (const auto& s : sesiones) { /*auto le dice a c++ que deduzca el tipo de dato, y como vector
            sesiones tiene elemntos de tipo sesion no hay necesidad de escribir sesion&*/

        if (s.titulo.find(criterio) != string::npos) { //van a recorrer las sesiones dentro del vector
            cout << "ID: [" << s.id << "] " << s.titulo //s.titulo, de la estructura solo extraeremos el titulo
                 << " | Horario: " << s.horaInicio << " - " << s.horaFin
                 << " hrs | Aforo: " << s.cupoActual << "/" << s.cupoMaximo << "\n";
            encontrada = true;
        }
    }
    if (!encontrada) {
        cout << "No se encontraron sesiones que coincidan con ese criterio.\n"; // mensaje que mostrara si no se encuentran coincidencias
    }
}

void ordenarSesionesPorHorario() {
    if (sesiones.size() < 2) { // si el numero estra entre 0 y 1 no habrá nacesidad de ordenr nada, caso contraio cuando el umero sea >= 2
        cout << "\nNo hay suficientes sesiones para ordenar.\n";
        return;
    }
    for (size_t i = 0; i < sesiones.size() - 1; i++) {
        for (size_t j = 0; j < sesiones.size() - i - 1; j++) { // estos ciclos anidades van a comparse entre si la sesion inicia nates que otra entonces habrá un ordenamiento y si intercambiarán de posicin
            if (sesiones[j].horaInicio > sesiones[j + 1].horaInicio) {
                Sesion temp = sesiones[j]; //uso de la variable temp. que temporalmente servirá para cambiar de lugar entre sesiones
                sesiones[j] = sesiones[j + 1];
                sesiones[j + 1] = temp;
            }
        }
    }
    cout << "\n¡Sesiones ordenadas cronologicamente con exito!\n";
}

bool yaEstaInscrito(string idPart, string idSes) { //va a revisar la lista de inscripciones para comprobra si un participante en especifico ya tiene un cupo asignado dentro de la sesion
    for (const auto& ins : inscripciones) {
        if (ins.idParticipante == idPart && ins.idSesion == idSes) return true;
    }
    return false;// devolverá verdadero o falso
}

bool hayChoqueDeHorario(const Sesion& s1, const Sesion& s2) {
    return (s1.horaInicio < s2.horaFin && s2.horaInicio < s1.horaFin);
} // comprobará si un horario de cierta sesion se cruza con otra a la que el usuario ya está inscrita


//funciones de operacion
void registrarParticipante() { //como tal vamos a pedir los datos del participante.
    Participante p;
    cout << "\n------- REGISTRAR NUEVO PARTICIPANTE -------\n";
    cout << "Ingrese ID o carne del participante: ";
    cin >> p.id;

    if (buscarParticipantePorId(p.id) != -1) {
        cout << "Error: Ya existe un participante registrado con este ID.\n";
        return;
    }

    // para la base de datos, se solicta ahora nombre, correo electronico y numero de telefono
    cin.ignore();
    cout << "Ingrese Nombre completo: ";
    getline(cin, p.nombre);

    string correo, telefono;
    cout << "Ingrese Correo electronico: ";
    getline(cin, correo);
    cout << "Ingrese Telefono: ";
    getline(cin, telefono);

    // 1. Guardamos en el vector local y ejecutamos el comando SQL inserte para guardar en la base de datos
    participantes.push_back(p);

    // 2. Guardamos en la Base de Datos MySQL
    MYSQL* conn = conectarBD();
    if (conn != NULL) {
        // Consulta SQL con comillas simples en el ID y los textos
        string query = "INSERT INTO participantes (id, nombre, correo, telefono) VALUES ('" +
                       p.id + "', '" + p.nombre + "', '" + correo + "', '" + telefono + "')";

        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "¡Participante registrado con exito en memoria y en la Base de Datos!\n";
        } else {
            cout << "Aviso: Se guardo localmente, pero hubo un error al sincronizar con MySQL: " << mysql_error(conn) << endl;
        }

        mysql_close(conn); // Cerramos la conexión
    } else {
        cout << "¡Participante registrado con exito en memoria (Sin conexion a Base de Datos)!\n";
    }
}

void crearSesion() { // permite la creacion de la conferencia
    Sesion s;
    cout << "\n------- CREAR NUEVA CONFERENCIA / SESION -------\n";
    cout << "Ingrese ID de la sesion: ";
    cin >> s.id;
    if (buscarSesionPorId(s.id) != -1) {
        cout << "Error: Ya existe una sesion registrada con este ID.\n"; // evalua si el ID ya existe o no
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

    // 1. Guardamos en el vector local
    sesiones.push_back(s);

    // 2. Guardamos en la Base de Datos MySQL, se registrará en la tabla sesiones
    MYSQL* conn = conectarBD();
    if (conn != NULL) {
        string query = "INSERT INTO sesiones (id, titulo, hora_inicio, hora_fin, cupo_maximo, cupo_actual) VALUES ('" +
                       s.id + "', '" + s.titulo + "', " + to_string(s.horaInicio) + ", " +
                       to_string(s.horaFin) + ", " + to_string(s.cupoMaximo) + ", " + to_string(s.cupoActual) + ")";

        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "¡Sesion creada con exito en memoria y en la Base de Datos!\n";
        } else {
            cout << "Aviso: Se guardo localmente, pero hubo un error al sincronizar con MySQL: " << mysql_error(conn) << endl;
        }
        mysql_close(conn);
    } else {
        cout << "¡Sesion creada con exito en memoria (Sin conexion a Base de Datos)!\n";
    }
}

void inscribirParticipante() { /*Aqui evaluamos todas las condiciones, que el participante exista, que la sesion exista, que
    el usuario no esté inscrito con anterioridad, que no exista choque de horarios, si cumple con todo actualiza y ejecuta un insert*/
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

    // 1. Guardamos en el vector local e incrementamos cupo
    Inscripcion nuevaIns = {idPart, idSes};
    inscripciones.push_back(nuevaIns);
    sesionObjetivo.cupoActual++;

    // 2. Guardamos la inscripción en MySQL y actualizamos el cupo actual de la sesión
    MYSQL* conn = conectarBD();
    if (conn != NULL) {
        string queryIns = "INSERT INTO inscripciones (id_participante, id_sesion) VALUES ('" + idPart + "', '" + idSes + "')";
        string queryUpd = "UPDATE sesiones SET cupo_actual = cupo_actual + 1 WHERE id = '" + idSes + "'";

        if (mysql_query(conn, queryIns.c_str()) == 0 && mysql_query(conn, queryUpd.c_str()) == 0) {
            cout << "¡Inscripcion confirmada con exito en memoria y en la Base de Datos!\n";
        } else {
            cout << "Aviso: Se registro localmente, pero hubo un error al sincronizar con MySQL: " << mysql_error(conn) << endl;
        }
        mysql_close(conn);
    } else {
        cout << "¡Inscripcion confirmada con exito en memoria (Sin conexion a Base de Datos)!\n";
    }
}

void mostrarEstadoSesiones() { //Aqui se imprime el reporte completoen pantalla
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

    // Credenciales de ejemplo
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


void cargarDatosDesdeBD() { //Cuando main se ejecute realiza consultas select masivas en mySQL para rellenar los vectores en memoria RAM y asi el programa recuerde todo lo qeu se registró

    MYSQL* conn = conectarBD(); //Abre la comunicacion
    if (conn == NULL) return; // En dado caso XAMPP este apagado, para que salga cin return y que el programa funcione al menos en memoria local sin trabarse

    // 1. Cargar Participantes desde MySQL a la memoria
    if (mysql_query(conn, "SELECT id, nombre FROM participantes") == 0) {
        MYSQL_RES* res = mysql_store_result(conn); //MYSQL_RES* res paquete de memoria
        if (res != NULL) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                Participante p;
                p.id = row[0] ? row[0] : "";
                p.nombre = row[1] ? row[1] : "";
                participantes.push_back(p);
            }
            mysql_free_result(res);
        }
    }

    // 2. Cargar Sesiones desde MySQL a la memoria
    if (mysql_query(conn, "SELECT id, titulo, hora_inicio, hora_fin, cupo_maximo, cupo_actual FROM sesiones") == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res != NULL) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {//Cada renglón (row)Funciona como una lista
                Sesion s;
                s.id = row[0] ? row[0] : "";
                s.titulo = row[1] ? row[1] : "";
                s.horaInicio = row[2] ? stoi(row[2]) : 0;
                s.horaFin = row[3] ? stoi(row[3]) : 0; // stoi (String to integer) convierte texto "900" en numero entero para poder guardarlo
                s.cupoMaximo = row[4] ? stoi(row[4]) : 0;
                s.cupoActual = row[5] ? stoi(row[5]) : 0;
                sesiones.push_back(s);
            } //push_back() mete cada registro construido a su vector
            mysql_free_result(res);
        }
    }

    // 3. Cargar Inscripciones desde MySQL a la memoria
    if (mysql_query(conn, "SELECT id_participante, id_sesion FROM inscripciones") == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res != NULL) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                Inscripcion ins;
                ins.idParticipante = row[0] ? row[0] : "";
                ins.idSesion = row[1] ? row[1] : "";
                inscripciones.push_back(ins);
            }
            mysql_free_result(res);
        }
    }

    mysql_close(conn);
}
