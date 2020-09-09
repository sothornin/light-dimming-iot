#include <ESP8266WiFi.h>

String apiKey = "2DP7D0HVS66UCT5N";     //  Enter your Write API key from ThingSpeak
WiFiClient client;
const char *ssid =  "Dechawu";     // replace with your wifi ssid and wpa2 key
const char *pass =  "thornin2018";
const char* server = "api.thingspeak.com";

int ledPin = D5;
int sensor = A0;
float x = 0;
void setup() {

  Serial.begin(9600);   // initialize serial communication at 9600 BPS
  delay(10);


  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  pinMode(ledPin, OUTPUT);
}

void loop() {
  float temp = analogRead(sensor);
  float bright = temp / (1023/255);
if(temp<100){
bright =0;
    for (int i = x; x > bright; x--)
    { analogWrite(ledPin, x);
      delay(10);
    }
  
  x=0;
  
} 
 else{
  if (x < bright) {
    for (int i = x; x < bright; x++)
    { analogWrite(ledPin, x);
      delay(10);
    }
  }
  if (x > bright) {
    for (int i = x; x > bright; x--)
    { analogWrite(ledPin, x);
      delay(10);
    }
  }
  else
    analogWrite(ledPin, x);

  x = bright;
 }
  float voltage = x * (5.0 / 255.0);
  if (isnan(temp))
  {
    Serial.println("Failed to read from LDR sensor!");
    return;
  }

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "&field2=";
    postStr += String(voltage);
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Brightness: ");
    Serial.print(temp);
    Serial.print(" Voltage: ");
    Serial.print(voltage);
    Serial.println("v. Send to Thingspeak.");
  }
  client.stop();

  Serial.println("Waiting...");
}
