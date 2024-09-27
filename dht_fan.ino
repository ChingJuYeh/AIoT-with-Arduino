  #include <WiFi.h>
  #include <PubSubClient.h>
  #include <ArduinoJson.h>
  #include "DHT.h"
  #define DHTPIN 14
  #define DHTTYPE DHT11

  DHT dht(DHTPIN, DHTTYPE);   
  int pin=16;
  const char*   ssid            = "Chiu";                        // your network SSID         
  const char*   pwd             = "c929rice";                                   // your network password
  int           i;
  const char*   mqtt_server     = "iiot.ideaschain.com.tw";
  const int     mqtt_port       = 1883;
  const char*   client_ID       = "eb3e9060-746c-11ee-bcc4-2b6af254e752";       // client ID
  const char*   username        = "1RtNd16FZ3f0tmn3Q1FV";                       // device access token
  char*         password        = "";                                           // no password needed
  const char*   subscribe_topic = "v1/devices/me/telemetry";                    // fixed format, do not modify
  const char*   publish_topic   = "v1/devices/me/telemetry";                    // fixed format, do not modify
  const char*   publish_payload = "{\"PM2.5\":\"10\",\"PM10\":\"20\"}";

  WiFiClient wifi_client;
  PubSubClient client(wifi_client);

  void printWiFiData(){
    Serial.print("");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi RSSI : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }

  // callback function : to deal with the received message
  void callback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] : ");
     for(i=0 ; i<length ; i++){
      Serial.print((char)payload[i]);
     }

    Serial.println();
    Serial.println("----------------------------------------");
  }
  // reconnect the server
  void reconnect(){
    //loop until we reconnected
    while(!client.connected()) {
      Serial.println("Attempting the MQTT connection ...");
      //Attempt to connect
      if(client.connect(client_ID, username, password)) {
        Serial.println("MQTT server connected");
        //publish announcement once we connect the server
        client.publish(publish_topic, publish_payload);
        //resubcribe topic
        client.subscribe(subscribe_topic);
      }
      else {
        Serial.print("fail connected , rc = ");
        Serial.println(client.state());
        Serial.println("Try connected after 5 seconds");
        delay(5000);
      }
    }
  }
  
  void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("DHT11 Test!");
    dht.begin();
    //WiFi.mode(WIFI_STA);
    //WiFi.begin(ssid, pwd);

    // while(WiFi.status() != WL_CONNECTED){
    //   Serial.print("Attempt to connect ");
    //   Serial.println(ssid);
    //   delay(5000);
    // } 

    // printWiFiData();  //print the wifi data and the signal strength

    // client.setServer(mqtt_server, mqtt_port); //client set the mqtt server and mqtt port
    // client.setCallback(callback); 
    //Allow hardware to sort itself out
    pinMode(pin,OUTPUT);
    
    //delay(1000);
  }

  void loop(){
    // if (!client.connected()) {
    //   reconnect();
    // }
    //code for DHT11 sensor 
    delay(1000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if(isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    float heat_index = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity : ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Heat index : ");
    Serial.print(heat_index);
    Serial.println(" %");

    if(t>=26){
      digitalWrite(pin,HIGH);
      delay(2000);
    }
    else digitalWrite(pin,LOW);
    //

    // //code for upload to IDEASChain server
    // char tempC_Inf[30];
    // char Hum_Inf[30];
    // char Heat_index_Inf[30];
    // char keyNameTem[] = "tempC";
    // char KeyNameHum[] = "Humid";
    // char KeyNameHID[] = "Heat_index";

    // sprintf(tempC_Inf, "{\"%s \":\"%f \"}" ,keyNameTem, t);
    // client.publish(publish_topic, tempC_Inf);
    // sprintf(Hum_Inf, "{\"%s \":\"%f \"}" ,KeyNameHum, h);
    // client.publish(publish_topic, Hum_Inf);
    // sprintf(Heat_index_Inf, "{\"%s \":\"%f \"}" ,KeyNameHID, heat_index);
    // client.publish(publish_topic, Heat_index_Inf);

    delay(300);
    //client.loop();

  }

// int pin=16;
// void setup() {
//   // put your setup code here, to run once:
//   pinMode(pin,OUTPUT);
//   Serial.begin(9600);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(pin,LOW);
//   delay(50);
//   digitalWrite(pin,HIGH);
//   delay(1550);
  
// }
