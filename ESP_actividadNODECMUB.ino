#include <WiFi.h>
#include <PubSubClient.h>

// Wifi security
const char* ssid = "IoT_Ivo";
const char* password = "12345678";
// MQTT Broker IP address
const char* mqtt_server = "192.168.0.102";

const char* topico="A01422948";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float Dato_Enviar = 0;

// LED Pin
const int ledPin = 22;
const int pot=34;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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
  if (String(topic) == "guayabitos") {
    Serial.print("Changing output to ");
    if(messageTemp == "0"){
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "1"){
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Reconnect
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("guayabitos1A")) { //Nombre del cliente 
      Serial.println("connected");
      // Subscribe
      client.subscribe(topico);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Sent data
    value =analogRead(pot); 
    // Convert the value to a char array
    char tempString[8];
    dtostrf(value, 1, 2, tempString);
    Serial.print("Dato: ");
    Serial.println(tempString);
     client.publish(topico,"Sensor: ");
    client.publish(topico, tempString);

    }
}
