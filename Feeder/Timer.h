#ifndef TIMER_C
#define TIMER_C



#define T1C_enable_bit 7
#define T1C_automatic_reaload_when_zero 6
// prescale-divider, 0: divided by 1, 1: divided by 16, 2 or 3: divided by 256
#define T1C_prescaler_bit_one 3
#define T1C_prescaler_bit_two 2
#define MAX_VALUE_TIMER1 8388607
#define MIN_VALUE_TIMER2 11679


//Congiura el timer 1 previo a ser utilizado
void timer1_setup();
//Comienza la ejecución del timer 1
void start_timer1();
//Consulta el tiempo transcurrido desde el inicio del timer 1 hasta el momento y retorna un resultado en segundos
float get_time_since_last_enabled1();
//Reinicia el conteo del timer 1
void reset_timer1();

//Congiura el timer 2 previo a ser utilizado
void timer2_setup();
//Comienza la ejecución del timer 2
void start_timer2();
//Consulta el tiempo transcurrido desde el inicio del timer 2 hasta el momento y retorna un resultado en segundos
float get_time_since_last_enabled2();
//Reinicia el conteo del timer 2
void reset_timer2();

#endif
