#include <SoftwareSerial.h>
 
SoftwareSerial ESPserial(3, 1); // RXD0 | TXD0
//SoftwareSerial ESPserial(13, 15); // RXD2 | TXD2
unsigned char data = 0;
unsigned char buff[5]= {0,0,0,0,0};
unsigned char nodesend[3] ={0,0,0};
unsigned int cnt = 0;
unsigned int chksum =0;
void setup() {
  
} 
void loop() {
  ESPserial.begin(9600);
  if(data=='s') {
    cnt=0;
  }  
  if(ESPserial.available())
  {
    data = ESPserial.read();
    buff[cnt] = data;
    //ESPserial.println(buff[cnt]);
    cnt++;
    if(cnt==4 && data=='e'){
      //for(int i=0; i<5; i++) ESPserial.print('k');
      ESPserial.print("PM2.5: ");
      for(int i=0; i<3; i++) {
        nodesend[i]=(int)buff[i]-48;
        ESPserial.print(nodesend[i]);
      }
      ESPserial.println();
    }               
  }
}
