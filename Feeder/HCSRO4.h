#ifndef HCSRO4_C
#define HCSRO4_C

//Variables utilizadas para el sensor de distancia
int TRIG = 4;      // trigger en pin D2
int ECO = 16;      // echo en pin D0
int DURACION;
int DISTANCIA;

//Configura el sensor
void setup_sensor_distancia();

//Calcula la distancia medida por el sensor y retorna un resultado en cm
int calcular_distancia();

#endif
