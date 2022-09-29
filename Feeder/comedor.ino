
//Librerías para conectarse a internet por wifi con diferentes micros
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>

//Librerías para el uso del  bot de telegram
#include <ArduinoJson.h>
#include "TelegramBot.h"

// Incluímos la librería para poder controlar el servo
#include <Servo.h>

//Librerías para consultar la hora en tiempo real
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "TiempoReal.h"

//Librería del driver de timer
#include "Timer.h"

//Funcionalidades generales del proyecto
#include "Funcionalidades.h"

//Librería para controlar el comportamiento del sensor de distancia
#include "HCSRO4.h"

//Librería para controlar la base de datos Firebase
#include "FirebaseDriver.h"

// Replace with your network credentials
const char* ssid = "Fibertel WiFi178 2.4GHz";
const char* password = "Rodrigo123.";




// Declaramos la variable para controlar el servo
Servo servoMotor;
int pin_servo = 5;
int parar_motor1 = 90;
int parar_motor2 = 95;
int mover_motor = 180;

//Variables para el control y manejo de mensajes
const int tamaño_chico = 3;
const int tamaño_mediano = 5;
const int tamaño_grande = 7;
int configurar = 0;
int configurar_tamaño_raciones = 0;
int servido = 0;
int hora_enviada=0;
int ultimaHoraAviso = 24;
int hayComida = 0;
int ultima_distancia = -1;
int tamaño_racion;


//Variables utilizadas en loop
String hora_string = "";
int hora = 24;
int minuto = 61;
int hora_actual = -1;
int numNewMessages;
int ultima_medicion_timer;


//Manejador de nuevos mensajes
void handleNewMessages(int numNewMessages) {
  
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  //Para cada mensaje recibido, controlamos si el usuario que lo envió está autorizado, cual es el comando ingresado y respondemos lo necesario
  for (int i=0; i<numNewMessages; i++) {
    //Chat id de la persona que envió el mensaje
    String chat_id = String(bot.messages[i].chat_id);
    //Si el id leído no es igual al id autorizado se envía un mensaje explicando la situación
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    
    //Obtenemos el mensaje recibido
    String text = bot.messages[i].text;
    Serial.println(text);

    //Obtenemos el nombre de la persona que envió el mensaje
    String from_name = bot.messages[i].from_name;

    
    //Si no estamos realizando una configuración de horario o de tamaño de ración, controlamos el comando enviado
    if ( (configurar== 0) && (configurar_tamaño_raciones == 0) ){

      //Iniciamos una configuración para determinar un horario donde se sirva la comida
      if(text == "/Configurar_horario"){
        bot.sendMessage(chat_id, "Ingrese la hora que desea configurar o /Cancelar para abortar la configuración", "");
        configurar =1;
      }

      //se muestra el menú principal
      if (text == "/menu") {
        String welcome = "Hola, " + from_name + ".\n";
        welcome += "Use los siguientes comandos para controlar el dispositivo\n\n";
        welcome += "/HayComida para ver si hay comida\n";
        welcome += "/MOTOR_ON para prender el motor \n";
        welcome += "/MOTOR_OFF para apagar el motor \n";
        welcome += "/Definir_Tamanio_Raciones para definir el tamaño de las raciones \n";
        welcome += "/Mostrar_Tamanio_Raciones para ver cual es el tamaño de las raciones \n";
        welcome += "/Configurar_horario para configurar el horario donde se da el alimento \n";
        welcome += "/Mostrar_horarios para mostrar los horarios donde se sirve comida \n";
        welcome += "/Borrar_Horario para borrar un horario configurado\n";
        bot.sendMessage(chat_id, welcome, "");
      }

      if(text== "/Borrar_Horario"){
        bot.sendMessage(chat_id, "Ingrese la hora de la configuración que busca borrar o /Cancelar para cancelar la operación", "");
        configurar = -1;
      }

      //Se notifica al usuario si hay comida o no
      if(text == "/HayComida"){
        if(hayComida == 0)
           bot.sendMessage(chat_id, "hay comida", "");
        else
             bot.sendMessage(chat_id, "No hay comida", "");
      }


      if( text == "/Mostrar_Tamanio_Raciones"){
        if(tamaño_racion == tamaño_chico){
          bot.sendMessage(chat_id, "Tamaño de ración chico", "");
        }else{
          if(tamaño_racion == tamaño_mediano){
            bot.sendMessage(chat_id, "Tamaño de ración mediano", "");
          }else{
            if(tamaño_racion == tamaño_grande){
              bot.sendMessage(chat_id, "Tamaño de ración grande", "");
            }
          }
        }
      }
      
      if(text == "/Definir_Tamanio_Raciones"){
        String opciones = "Elija un tamaño de reación  \n";
        opciones += "/Chica \n";
        opciones += "/Mediana \n";
        opciones += "/Grande \n";
        configurar_tamaño_raciones = 1;
        bot.sendMessage(chat_id, opciones, "");
      }
      
      
      //Se enciende el motor por 3 segundos
      if (text == "/MOTOR_ON") {
        bot.sendMessage(chat_id, "Se prende el motor", "");
        servoMotor.write(mover_motor);
        //delay(3000);
        noInterrupts();
        ultima_medicion_timer = get_time_since_last_enabled2();
        
        while(get_time_since_last_enabled2() < ultima_medicion_timer+tamaño_racion){
          //nada
        }
        interrupts();
        servoMotor.write(parar_motor2);
      }
      

      //Se apaga el motor
      if (text == "/MOTOR_OFF") {
        bot.sendMessage(chat_id, "Se apaga el motor", "");
        servoMotor.write(parar_motor1);
      }


      //Se muestran los horarios donde se sirve comida
      if(text == "/Mostrar_horarios"){
        int i;
        String hora_consulta = "";
        int horai, minutoi;
        String mensaje = "Los horarios configurados son: \n";
        horai=0;
        minutoi=0;
        i=0;
        while( i <24){
          horai=-1;
          hora_consulta = String(i);
          horai=get_hour(hora_consulta);
          if(horai != -1){
            minutoi=get_minute(hora_consulta);
            mensaje += "Hora: "+hora_consulta+" Minuto: "+String(minutoi)+ "\n";
          }
          i++;
        }
        bot.sendMessage(chat_id, mensaje, "");
      }

      
    }
    else{


      //Se comienza la configuración solicitando una hora
      if(configurar == 1){
        if(text == "/Cancelar"){
            configurar = 0;
            bot.sendMessage(chat_id, "Configuración abortada", "");
          }else{
            hora_enviada=convertir_a_entero(text);
            //Se controla si la hora ingresada es válida
            if( (checkHora(hora_enviada)) && (hora_enviada !=-1)){
              configurar = 2;
              bot.sendMessage(chat_id, "Ingrese el minuto", "");
            }else{
              bot.sendMessage(chat_id, "Valor incorrecto, ingrese una hora para terminar de configurar el dispositivo o /Cancelar para abortar la configuración", "");
            }
          }
        
      }else{
        if(configurar == 2) {
          if(text == "/Cancelar"){
            configurar = 0;
            bot.sendMessage(chat_id, "Configuración abortada", "");
          }else{
            int minuto_enviado = 0;
            //Continúa la configuración solicitando un minuto
            if(configurar == 2){
              minuto_enviado=convertir_a_entero(text);
              Serial.println(minuto_enviado);
              //Se controla si el minuto ingresado es válido
              if( (checkMinuto(minuto_enviado)) && (minuto_enviado != -1)){
                configurar = 0;
                save_hour_minute_pair(String(hora_enviada), hora_enviada, minuto_enviado);
                bot.sendMessage(chat_id, "Configuracion correcta", "");          
              }
              else{
                bot.sendMessage(chat_id, "Valor incorrecto, ingrese un minuto para terminar de configurar el dispositivo o /Cancelar para abortar la configuración", "");
              }
            }
          }
        }
      }
      

      if(configurar == -1){
        if(text == "/Cancelar"){
          configurar = 0;
          bot.sendMessage(chat_id, "Borrado abortado", "");
        }
        else{
          int hora_enviada =convertir_a_entero(text);
          if( (checkHora(hora_enviada)) && (hora_enviada != -1) ){
            delete_hour_minute_pair(String(hora_enviada) );
            bot.sendMessage(chat_id, "La hora enviada se a eliminado", "");
            configurar = 0;
          }else{
            bot.sendMessage(chat_id, "Valor incorrecto, ingrese una hora válida", "");
          }
        }
      }

      
      if(configurar_tamaño_raciones == 1){
        if( text == "/Chica"){
          tamaño_racion = tamaño_chico;
          bot.sendMessage(chat_id, "Se eligió el tamaño de ración chico", "");
          configurar_tamaño_raciones = 0;
          guardar_tamaño_racion(tamaño_racion);
        }else{
          if( text == "/Mediana"){
            tamaño_racion = tamaño_mediano;
            bot.sendMessage(chat_id, "Se eligió el tamaño de ración mediano", "");
            configurar_tamaño_raciones = 0;
            guardar_tamaño_racion(tamaño_racion);
          }else{
              if( text == "/Grande"){
                tamaño_racion = tamaño_grande;
                bot.sendMessage(chat_id, "Se eligió el tamaño de ración grande", "");
                configurar_tamaño_raciones = 0;
                guardar_tamaño_racion(tamaño_racion);
              }else{
                bot.sendMessage(chat_id, "Valor incorrecto, ingrese un tamaño de ración válido", "");
              }
          }
        }
      }

      
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      //Configuración para obtener el tiempo en UTC via NTP
    client.setTrustAnchors(&cert); // Certificado necesario para el funcionamiento de la api de telegram
  #endif

  
  //Conectamos a la señal Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  // Print Local IP Address
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Ingrese /menu para mostrar la lista de comandos", "");

  
  //Inicializamos el motor 
  servoMotor.attach(pin_servo);
  servoMotor.write(parar_motor1); 

  timeClient.begin();

  setup_sensor_distancia();

  setup_firebase(); 
  
  //obtenemos el tamaño de ración guardado en la base de datos
  tamaño_racion = obtener_tamanio_racion();

  timer2_setup();
  start_timer2();
  
  
}

void loop() {
  //Controlamos si se cumplió el delay que debe haber entre mensajes
  if (get_time_since_last_enabled2() > botRequestDelay)  {

    //COntrolamos y respondemos a los mensajes que hayan sido recibidos (si los hay)
    //bot.getUpdates recibe un offset por parámetro y retorna la cantidad de mensajes recibidos 
    //si el offset es bot.last_message_recieved+n se retorna una cantidad de mensajes recibidos que va desde 0 (ningún mensaje recibido) hasta n (n mensajes recibidos)
    //En este caso buscamos si hay 0 o 1 mensaje disponible
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    //Mientras hayan mensajes nuevos
    while(numNewMessages) {
      Serial.println("got response");
      //Se asigna el manejador d elso mensajes
      handleNewMessages(numNewMessages);
      //Se busca si aún hay mensajes recibidos
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    
    timeClient.update();
    
    //Controlamos la cantidad de comida y avisamos al usuario de ser necesario
    ultima_distancia = calcular_distancia();
    Serial.println(ultima_distancia);
    //Si distancia entre la tapa y el alimento es mayor a 13 cms.
    if ((ultima_distancia > 13)){ 
      //Si aún no se avisó que hay poco alimento en la hora actual, se notifica al usuario de la situación
      if(ultimaHoraAviso != timeClient.getHours()){
          bot.sendMessage(CHAT_ID, "Se está acabando la comida", "");
          ultimaHoraAviso=timeClient.getHours();
      }
      hayComida =1;
    }
    else{
      hayComida = 0;
    }

    //Reiniciamos el timer para calcular el delay entre mensajes
    reset_timer2();
    start_timer2();

    
  }


  //Obtenemos la hora actual y la convertimos en String para hacer la consulta en la base de datos
  timeClient.update();
  hora_actual =timeClient.getHours();
  hora_string=String(hora_actual);
  
  
  
  //controlamos si ya se sirvió comida en esta hora y minuto
  if(servido == 0){
    //Buscamos si la hora actual en la base de datos
    hora=get_hour(hora_string);
    //Controlamos si la hora está configurada en la base de datos
    if((hora != -1)){ 
      //obtenemos el minuto asociado a la hora en la base de datos
      minuto=get_minute(hora_string);
      //si el minuto asociado a la hora en la base de datos es igual al minuto actual, debemos servir comida
      if(minuto == timeClient.getMinutes()){
        servoMotor.write(mover_motor);
        
        noInterrupts();
        ultima_medicion_timer = get_time_since_last_enabled2();
        
        while(get_time_since_last_enabled2() < ultima_medicion_timer+tamaño_racion){
          //nada
        }
        interrupts();
        
        servoMotor.write(parar_motor2);
        servido = 1;
      }
    } 
  }
  else{
    
    if(get_minute(hora_string)!=timeClient.getMinutes()){
      servido=0;
    }
  }
  
  
  
  
}
