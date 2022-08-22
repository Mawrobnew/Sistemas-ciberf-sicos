
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Wifi security
const char* ssid = "IoT_Ivo"; //Red para conectarse a internet
const char* password = "12345678"; //contraseña de la red

// MQTT Broker IP address
const char* mqtt_server = "192.168.0.49"; //Ip del broker para mqtt

const char* topico="A01422948";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


// LED Pin
int Led = 15;
int Switch = 13;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setup_wifi();
  client.setServer(mqtt_server, 1883); //establece el servidor Mqtt y el puerto que se usará
  client.setCallback(callback);
  mqttconnect();
  pinMode(Led, OUTPUT);
  pinMode(Switch, INPUT);
}

void loop() {
    long now = millis();
    if (now - lastMsg > 50) {
      lastMsg = now;
      value= digitalRead(Switch);
      mqttconnect();
      char tempString[8];
      dtostrf(value, 1, 0, tempString);
      //Serial.println(tempString);
      client.publish(topico,"Switch state: ");
      client.publish(topico,tempString);
      //Serial.println(value);
    }
    client.loop();
}


void setup_wifi() {
  delay(10);
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //conectandose a la red wifi

  while (WiFi.status() != WL_CONNECTED) { //mientras establece la conexión marca puntos
    delay(500);
    Serial.print(".");
  }
   WiFi.setAutoReconnect(true);
   WiFi.persistent(true);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


  // If topic = "casa", check message is either "Uno" or "Dos". 
  // Changes the output state according to the message
  if(messageTemp == "Encendido"){
      Serial.println("Led2 On");
      digitalWrite(Led, HIGH);
    }
    else if(messageTemp == "Apagado"){
      Serial.println("Led2 off");
      digitalWrite(Led, LOW);
    }
}
void mqttconnect() {
  // Reconnect
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("guayabitos1B")) { //Nombre del cliente "GuayabitosB"
      Serial.println("connected");
      // Subscribe
      client.subscribe(topico);//topico 1
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
