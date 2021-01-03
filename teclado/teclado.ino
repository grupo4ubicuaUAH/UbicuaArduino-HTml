
#define RELAY D8

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
int position = 0;
bool permiso = false;
const byte ROWS = 4; // 4 filas,declaracion de filas
const byte COLS = 4; // declaracion de columnas

// Inicilaiza la conexion WIFI

char ssid[] = "MOVISTAR_8BB3";     // your network SSID (name)
char password[] = "AA4351C77B2ED6E4E87D"; // your network key


// Inicializa el bot de Telegram
#define BOTtoken "1426153334:AAHQMPbEOecaeAlmYdB_5BDiP50AYkacBug"
WiFiClient espClient;
PubSubClient client(espClient);
WiFiClient clienteBot

WiFiClientSecure client(clienteBot);
UniversalTelegramBot bot(BOTtoken, clienteBot);


//const char* ssid = "MOVISTAR_8BB3";
//const char* password = "AA4351C77B2ED6E4E87D";
const char* mqttServer = "81.40.105.254";
const int mqttPort = 1883;
const char* mqttUser = "ubicua";
const char* mqttPassword = "SistemaSeguridad123";



char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},

  { '7', '8', '9', 'C'},
  { '*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {D0, D1, D2, D3};
byte colPins[COLS] = {D4, D5, D6, D7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //Instancia del propio Keypad
char key;
char pass[7];
char passwordR[7] = "240599";
byte counter = 0;

int Lock = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  pinMode(RELAY, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  initMQTTServer();

}

void loop() {
  client.loop();
  // put your main code here, to run repeatedly:

  key = keypad.getKey();
  if (key) {
    pass[counter] = key;
    counter++;
    Serial.print(key);
  }
  if (counter == 6) {
    if (!strcmp(pass, passwordR) and permiso == true) {

      Serial.println("La contraseña introducida es correcta");
      digitalWrite(RELAY, HIGH);
    } else {
      Serial.println("Contraseña incorrecta");
      digitalWrite(RELAY, LOW);
    }
    counter = 0;
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("Nuevos mensajes"); // Para ver el estado en el puerto serie
  Serial.println(String(numNewMessages));  // Para ver el estado en el puerto serie
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name =="") from_name = "Guest";
        //————-
        //Aquí puedes añadir más comandos para tu control o modificarlos
        //————-

    if (text == "/camara") { //MUESTRA LA CAMARA
      bot.sendMessage(chat_id, "Camara ON, ");
      }
    if (text == "/apagacamara") {
    bot.sendMessage(chat_id, "Camara OFF, ");
    }

    if (text == "/start") {    //Es el mensaje que recibimos en el celular cuando conectamos
    String welcome = "Bienvenido al Bot Telegram,  + from_name + ";
    welcome += "Puede usar los siguientes comandos: \n\n";
    welcome += " / camara : Para encender la camara\n";
    welcome += " / apagacamara : Para apagar la camara\n";
    bot.sendMessage(chat_id, welcome, "");
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  String persona = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    persona = persona + (char) payload[i];
  }
  Serial.println();
  Serial.println(persona);
  Serial.println("-----------------------");
  if (persona.equals(persona)) {
    permiso = true;
  }

}

void initMQTTServer() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect("ESP86", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.publish("esp8266/test", "hello emqx");
  client.subscribe("esp/test");
}
