#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
SoftwareSerial ESPserial(3, 1); // RXD0 | TXD0
//SoftwareSerial ESPserial(13, 15); // RXD2 | TXD2
unsigned char data = 0;
char buff[5]= {0,0,0,0,0};
char node[3]= {0,0,0};
unsigned int cnt = 0;
unsigned int chksum =0;

const char* ssid = "Kwangwoon_KT";            //Kwangwoon_KT
const char* password = "ilovekwu";         //ilovekwu
const char* mqtt_server = "223.194.8.103";   //114.70.63.55
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int flag = 0;
bool count =0;
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
}
void callback(char* topic, byte* payload, unsigned int length) {
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("nodesend", "hello world");
      // ... and resubscribe
      //client.subscribe("pisend");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  client.setServer(mqtt_server, 11056);
  client.setCallback(callback);
} 
void loop() {
  if(flag == 0){
    ESPserial.begin(9600);
    while(flag==0){
      if(data=='s'){
        cnt=0;
        for(int i=0;i<3;i++) node[i]=0;
      }
      if(ESPserial.available())
      {
        data = ESPserial.read();
        buff[cnt]=data;
        cnt++;
        if(cnt==4 && data=='e'){
          for(int i=0; i<3; i++) {
            node[i]=buff[i];
          }
          flag=1;
          count=0;
        }               
      }
    }
  }
  else if(flag == 1){
    Serial.begin(115200);                 //
    setup_wifi();                         //    
    if (!client.connected()) {
      reconnect();
    }
    //client.loop_start();
    
    snprintf (msg, 7, "%d%d%d",node[0]-48,node[1]-48,node[2]-48);
    //client.subscribe("pisend");
    client.publish("nodesend", msg);
    delay(1000);
    //client.loop_stop();
    
    Serial.end();
    //unsigned long now = millis();
    //if (now - lastMsg > 2000) {
    //  lastMsg = now;
    //  //snprintf (msg, 4,"%ld",node);      
    //}
    flag=0; 
  }    
}
