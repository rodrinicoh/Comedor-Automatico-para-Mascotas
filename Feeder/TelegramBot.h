#ifndef TELEGRAMBOT_C
#define TELEGRAMBOT_C


//Librerías para el uso del bot de telegram
#include <UniversalTelegramBot.h>

#define BOTtoken "1815478810:AAGfJXQLf0GkqO0jJHafknaCHmCm_acm7_k"  // Bot Token (Se obtiene mediante Botfather)

//Con @myidbot se obtiene el ID de una persona o un grupo. La primera vez, se debe presionar start en el bot antes de que se puedan enviar y recibir mensajes
#define CHAT_ID "300591313"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);



// Checks for new messages every 1 second.
int botRequestDelay = 1;

#endif
