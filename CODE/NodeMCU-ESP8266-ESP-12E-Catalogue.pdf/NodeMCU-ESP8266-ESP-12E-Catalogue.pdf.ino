// Reza Tanaki Zadeh


/***************************************************/
#include <ESP8266WiFi.h>  //ESP8266 library
#include "Adafruit_MQTT.h" // laybrary for Adafruite
#include "Adafruit_MQTT_Client.h" // also

//define Pins
#define relay1 D8
#define relay2 D5
#define relay3 D2

/************************* WiFi Define *********************************/

#define WLAN_SSID       "mertz"
#define WLAN_PASS       "rezatz1378"

/************************* Information to Connect to Adafruit IO *********************************/


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883               
#define AIO_USERNAME    "rezatz780210"
#define AIO_KEY         "95dbfce4fe9040cfabdca4d8c65868f2"

/************ Setting up your WiFi Client and MQTT Client ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.

WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Set Up a Feed to Subscribe ***************************************/


//Adafruit_MQTT_Publish myFirstValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/key");
Adafruit_MQTT_Subscribe light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");

/********************* Values ******************************/
// Need a changing value to send.  We will increment this value
// in the getVal function.  
//we start at zero and when it gets to 10 we start over.

uint32_t xVal=-1;

/*************************** Sketch Code ***********************************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
   

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&light1);
  mqtt.subscribe(&light2);
  mqtt.subscribe(&light3);
 
}


void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(20000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &light1)
    {
      Serial.print("onoff: ");
      Serial.println((char*) light1.lastread);
      int light1_state = atoi((char*)light1.lastread);
      digitalWrite(relay1 ,light1_state);
    }
  

  
     if (subscription == &light2)
    {
      Serial.print("onoff: ");
      Serial.println((char*) light2.lastread);
      int light2_state = atoi((char*)light2.lastread);
      digitalWrite(relay2 ,light2_state);
    }
  

  
     if (subscription == &light3)
    {
      Serial.print("onoff: ");
      Serial.println((char*) light3.lastread);
      int light3_state = atoi((char*)light3.lastread);
      digitalWrite(relay3 ,light3_state);
    }
  }
 /** // Now we can publish stuff!
  Serial.print(F("\nSending myValue "));
  Serial.print((xVal-1));
  Serial.print("...");
  if (! myFirstValue.publish(getVal())) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  **/

  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
  delay(1000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


//  Use this increment myVal

uint8_t getVal(){

  if(xVal == 11){
      xVal = 0;
  }
  return xVal++;
  
}

