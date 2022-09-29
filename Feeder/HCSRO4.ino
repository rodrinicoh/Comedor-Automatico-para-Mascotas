#include "HCSRO4.h"
#include "Timer.h"


void setup_sensor_distancia(){
    pinMode(TRIG, OUTPUT);  // trigger como salida
    pinMode(ECO, INPUT);    // echo como entrada
}


int calcular_distancia(){
    digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar al pin conectado al trigger del sensor
    delay(1);
    noInterrupts();
    int ultima_medicion_timer = get_time_since_last_enabled2();
    
    while(get_time_since_last_enabled2() < ultima_medicion_timer+0.000001){
      //nada
    }
    interrupts();
    digitalWrite(TRIG, LOW);
    
    DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso alto en echo
    DISTANCIA = DURACION / 58;    // distancia medida en centimetros (el valor 58 lo da el fabricante)
    
    return DISTANCIA;
}
