#include "conexion.h"
#include <iostream>

using namespace std;

MYSQL* conectarBD() {
    MYSQL *conn;
    conn = mysql_init(NULL);

    if (conn == NULL) {
        cout << "Error al inicializar la estructura MySQL" << endl;
        return NULL;
    }

    // Conectamos a la base de datos local en XAMPP
    conn = mysql_real_connect(conn, "localhost", "root", "", "sistema_conferencias", 3306, NULL, 0);

    if (!conn) {
        cout << "Error de conexion: " << mysql_error(conn) << endl;
    }

    return conn;
}
