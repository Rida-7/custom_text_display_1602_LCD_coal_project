#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

const char* ssid = "OPPO A15s";
const char* password = "ridabatool09";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

/* 2. Define the API Key */
#define API_KEY "AIzaSyDnqeFgtIIRkZRltT05U2aWOqXyRFKOwXg"

/* 3. Define the RTDB URL */
#define DATABASE_URL "projectcoal-d7a68-default-rtdb.firebaseio.com/" 

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() 
{
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) 
  {
    signupOK = true;
  } 
  else 
  {
    delay(50);
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  // Print received MQTT message to UART
  for (int i = 0; i < length; i++) 
  {
    Serial.write(payload[i]);
    delay(750);
  }

  // Convert payload to String
  String mqttPayload = String((char*)payload);

  // Send the received data to Firebase
  if (Firebase.ready() && signupOK) 
  {
    // Write the MQTT payload to the database path test/mqtt_data
    if (Firebase.RTDB.setString(&fbdo, "test/mqtt_data", mqttPayload))
    {
     
    } 
    else
    {
      delay(50);
    }
  }
}

void reconnect() 
{
  while (!client.connected()) 
  {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) 
    {
      client.subscribe("2022-cs-118");
    } 
    else 
    {
      delay(5000);
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}
