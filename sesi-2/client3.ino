#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// TODO 
// 1. Setup Wifi config
// 2. Setup mqtt client config
// 3. Setup mqtt broker config
// 4. Change mqtt topic's message
// 5. Change Payload (optional)

// WIFI CONFIG
const char* SSID = "Smart Xirka";
const char* SSID_PASSWORD = "engineer@30";

// MQTT CLIENT CONFIG
// # APPKEY = app-DemoEsp32.DRr0p0m3l1UsG1hLgwT1QfVlmLA4FD
char CLIENT[]= "app-DemoEsp32";
char CLIENT_PASSWORD[]="DRr0p0m3l1UsG1hLgwT1QfVlmLA4FD";

// MQTT BROKER CONFIG
const char* BROKER_SERVER = "202.148.1.57";
const int BROKER_PORT = 1883;

// COUNTER
long lastMsg = 0;

// PAYLOAD 
float temperature = 0;
float humidity = 0;
char payload[512];

// LED PIN
const int BUILTIN_LED = 2;
volatile byte ledState = LOW;


void setup() {
  Serial.begin(115200);
  setupWifi();
  client.setServer(BROKER_SERVER, BROKER_PORT);
  client.setCallback(mqttCallback);
  pinMode(BUILTIN_LED, OUTPUT);
}

void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, SSID_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  if (String(topic) == "perangkat1/light") {
    Serial.print("Changing output to ");
    if(messageTemp == "{\"payload\":{\"message\":\"ON\"}}"){
      Serial.println("on");
      ledState = HIGH;
    }
    else if(messageTemp == "{\"payload\":{\"message\":\"OFF\"}}"){
      Serial.println("off");
      ledState = LOW;
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp32client", CLIENT, CLIENT_PASSWORD)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("perangkat1/light");
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
  // TURN ON / OFF LED   
  digitalWrite(BUILTIN_LED, ledState);
  
  // TRY TO CONNECT TO BROKER 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // HANDLER MESSAGE
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;

    int temperature = random(0, 30);
    int humidity = random(0, 20);
    sprintf(payload,"{\"payload\":{\"humidity\":%d, \"temperature\":%d}}", humidity, temperature);

    Serial.println(payload);
    client.publish("perangkat1", payload);
  }
}

//{"payload":{"message":"off"}}