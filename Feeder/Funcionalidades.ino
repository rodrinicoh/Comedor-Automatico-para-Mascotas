#include "Funcionalidades.h"


int checkHora(int hora){
  int toRet = 0;
  if((hora >=0) && (hora<=23 ) ){
    toRet = 1;
  }
  return toRet;
}

int checkMinuto(int minute){
  int toRet = 0;
  if ( ( minute >=0 ) && (minute <=59)){
    toRet=1;
  }
  return toRet;
}


int convertir_a_entero(String text){
  int aux=text.toInt();
  if( (text != "0") && (text != "00") && (aux == 0) ){
    return -1;
  }
  return aux;
}
