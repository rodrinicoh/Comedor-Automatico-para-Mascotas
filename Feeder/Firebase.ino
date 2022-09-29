#include "FirebaseDriver.h"

#include <ArduinoJson.h>

const char *FIREBASE_HOST = "comedor-7390e-default-rtdb.firebaseio.com";
const char *FIREBASE_AUTH = "E7VlYqw0CYihXDpzA7l9sfteYdFnz6uGEDZyUFNn";

FirebaseData firebaseData;

void setup_firebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int save_hour_minute_pair(String hora_string,int hour, int minute){
  //Guardar valores en la base de datos
  if ( (!Firebase.setInt(firebaseData, hora_string + "/hour", hour)) || (!Firebase.setInt(firebaseData, hora_string + "/minute", minute)) ){
    return -1;
  }
  
  return 1;
}

int get_hour(String hour){
  Firebase.getInt(firebaseData, hour + "/hour");
  if(firebaseData.dataType() == "int")
    return firebaseData.intData();
   else
    return -1;
}

int get_minute(String hour){
  Firebase.getInt(firebaseData, hour + "/minute");
  if(firebaseData.dataType() == "int")
    return firebaseData.intData();
   else
    return -1;
}

void guardar_tamaño_racion(int tamaño){
  Firebase.setInt(firebaseData, "tamanio_Racion", tamaño);
}

int obtener_tamanio_racion(){
  Firebase.getInt(firebaseData, "tamanio_Racion");
  if(firebaseData.dataType() == "int")
    return firebaseData.intData();
  else
    return 3;
}

void delete_hour_minute_pair(String hour){
  Firebase.deleteNode(firebaseData, hour);
}
