
#define RELAY D8
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>
int position = 0;
bool permiso = false;
const byte ROWS = 4; // 4 filas,declaracion de filas
const byte COLS = 4; // declaracion de columnas
const char* ssid = "MOVISTAR_8BB3";
const char* password = "AA4351C77B2ED6E4E87D";
const char* mqttServer = "138.201.164.206";
const int mqttPort = 1883;
const char* mqttUser = "root";
const char* mqttPassword = " EE6zGqxZL5anUmst";
WiFiClient espClient;
PubSubClient client(espClient);
String lista[10];
int countador = 0;
String persona = "";
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  key = keypad.getKey();
  if (key) {
    pass[counter] = key;
    counter++;
    Serial.print(key);
  }
  if (counter == 6) {
    if (!strcmp(pass, passwordR) and permiso == true) {

      Serial.println("La contraseña introducida es correcta");
      
      usuarioEntra(persona);
      digitalWrite(RELAY, HIGH);
    } else {
      Serial.println("Contraseña incorrecta");
      entradaDenegada();
      digitalWrite(RELAY, LOW);
    }
    counter = 0;
    delay(5000);
    permiso = false;
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  
  String topico = "";
  String auxiliar = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  /*
  for (int i = 0; i <= sizeof(topic); i++) {
    Serial.println((char) topic[i]);
    topico = topico + (char) topic[i];
    Serial.println("Tu lo del topico ha funcionado" + topico);
  }
  */
  if (topico.equals("lista")) {
    for (int i = 0; i < length; i++) { 
      auxiliar = auxiliar + (char) payload[i];
    }
    lista[countador] = auxiliar;
    countador = countador + 1;
  }
  else {
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
      persona = persona + (char) payload[i];
    }
    Serial.println();
    Serial.println(persona);
    Serial.println("-----------------------");
    for (int j = 0; (!permiso and j <= sizeof(lista)); j++) {
      Serial.println(lista[j]);
      String auxiliar2 = lista[0];
      if (lista[j].indexOf(persona)){
        Serial.print("Ahora puedes meter la contraseña");
        permiso = true;
        Serial.print(permiso);
      }
    }
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
 
  client.subscribe("esp/test");
  client.subscribe("listaPersonasEntrada/entrada");
}
void usuarioEntra(String persona){
 
  client.publish("Entrada/entradaCasa",(char*) persona.c_str());
}
void entradaDenegada(){
  String comsg;
  
  client.publish("Entrada/entradaCasa","Se ha querido entrar con una contraseña incorrecta");
}
