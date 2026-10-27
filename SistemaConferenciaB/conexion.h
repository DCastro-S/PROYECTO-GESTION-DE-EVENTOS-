#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED
#define HAVE_UCHAR
#define byte _my_byte
#include <mysql.h>
#undef byte


// Declaramos la función que manejará la conexión
MYSQL* conectarBD();


#endif // CONEXION_H_INCLUDED
