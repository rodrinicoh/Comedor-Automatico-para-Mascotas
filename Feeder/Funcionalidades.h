#ifndef TELEGRAMBOT_C
#define TELEGRAMBOT_C


//Controla si el número pasado por parámetro corresponde a una hora válida y retorna 1 si es así, 0 en caso contrario
int checkIfHour(int hora);

//Controla si el número pasado por parámetro corresponde a un minuto válido y retorna 1 si es así, 0 en caso contrario
int checkIfMinute(int minute);

//Convierte a entero el String pasado por parámetro, en caso de no poder hacerlo se retorna el valor -1
int convertir_a_entero(String text);


#endif
