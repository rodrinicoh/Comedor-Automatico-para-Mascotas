#include "Timer.h"


void timer1_setup(){
  noInterrupts();
  
  T1C = (0<<T1C_enable_bit);
  
  T1C |= (1<<T1C_automatic_reaload_when_zero); //si es igual a 1, no reinicia automáticamente
  //prescaler a 256
  T1C |= (1<<T1C_prescaler_bit_one);
  T1C |= (1<<T1C_prescaler_bit_two);
  interrupts();
  
}

void start_timer1(){
  noInterrupts();
  T1C |= (1<<T1C_enable_bit);
  interrupts();
}

//80MHz de clock
//valor máximo de clock de 8388607
//1 tick tarda 0.0125microseg
//si usamos un prescaler de 256, 1 tick tarda 3.2microsegundos=0.0000032seg

float get_time_since_last_enabled1(){
  float time_in_seconds;
  time_in_seconds=MAX_VALUE_TIMER1-T1V;
  time_in_seconds=time_in_seconds*0.0000032;
  return time_in_seconds;
}

void reset_timer1(){
  noInterrupts();
  T1L =MAX_VALUE_TIMER1;
}

void timer2_setup(){
  noInterrupts();
  T2C = (0<<T1C_enable_bit);
  
  T2C |= (1<<T1C_automatic_reaload_when_zero); //si es igual a 1, no reinicia automáticamente
  //prescaler a 256
  T2C |= (1<<T1C_prescaler_bit_one);
  T2C |= (1<<T1C_prescaler_bit_two);
  interrupts();
  
}

void start_timer2(){
  noInterrupts();
  T2C |= (1<<T1C_enable_bit);
  interrupts();
}



float get_time_since_last_enabled2(){
  float time_in_seconds;
  time_in_seconds=T2V;
  time_in_seconds=time_in_seconds*0.0000032;
  return time_in_seconds;
}

void reset_timer2(){
  noInterrupts();
  T2L =MIN_VALUE_TIMER2;
  interrupts();
}
