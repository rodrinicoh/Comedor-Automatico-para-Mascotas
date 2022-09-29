
#include "Firebase.h"

#include <ArduinoJson.h>

//Configura las variables necesarias para el uso de la base de datos
void setup_firebase();

//guarda la hora y el minuto pasados por parámetro en la base de datos, retorna 1 si la operación es exitosa y -1 en caso contrario
int save_hour_minute_pair(int hour, int minute);
//Consulta si la hora pasada por parámetro se encuentra en la base de datos y la retorna
int get_hour(int hour);
//Consulta si el minuto asociado a la hora pasada por parámetro se encuentra en la base de datos y lo retorna
int get_minute(int hour);

//guarda el tamaño de ración pasado por parámetro en la base de datos
void guardar_tamaño_racion(int tamaño);
//Consulta el tamaño de ración en la base de datos y lo retorna
int obtener_tamanio_racion();

//Borra un par (hora,minuto) se la base de datos
void delete_hour_minute_pair(String hour);
