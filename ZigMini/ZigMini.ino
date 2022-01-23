#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>


#include <ZigbeeTF.h>
#include <XBee.h>
#include <Printers.h>

XBeeWithCallbacks xbee;


#define XBEE_TX D1
#define XBEE_RX D2



#define SerialDebug Serial
//#define SerialXBee SoftwareSerial
//SoftwareSerial SerialXBee(XBEE_TX, XBEE_RX);
SoftwareSerial SerialXBee(CO2_RX, CO2_TX);


//AltSoftSerial SerialXBee;


void processRxPacket(ZBRxResponse& rx, uintptr_t) {
    SerialDebug.print(F("Received packet from "));
    printHex(SerialDebug, rx.getRemoteAddress64());
    SerialDebug.println();
    SerialDebug.print(F("Payload: "));
    SerialDebug.write(rx.getData(), rx.getDataLength());
    SerialDebug.println();
}

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
    SerialXBee.begin(9600);
    //Serial1.begin(9600);
    //Serial3.begin(9600);
    //SerialDebug.begin(115200);
    digitalWrite(LED_BUILTIN, LOW); 
    xbee.begin(SerialXBee);
    delay(1);
    xbee.onResponse(printResponseCb, (uintptr_t)(Print*)&Serial);
    xbee.onZBRxResponse(processRxPacket);
    
    //AtCommandRequest req((uint8_t*)"VR");
    //xbee.send(req);


}

void sendPacket() {
    // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041B5D5A9);
    
    //txRequest.setAddress64(0x0013A20041B5D535);
    txRequest.setAddress64(0x000000000000FFFF);
    uint8_t payload[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
    txRequest.setPayload(payload, sizeof(payload));
    // And send it
    uint8_t status = xbee.sendAndWait(txRequest, 5000);
    if (status == 0) {
       SerialDebug.println(F("Succesfully sent packet"));
    }
    else {
        
        SerialDebug.print(F("Failed to send packet. Status: 0x" ));
        SerialDebug.println(status);
    }
}



void loop() {
    xbee.loop();
    //sendPacket();
    delay(1000);
    //for (int i=0; i<3; i++) {
      //SerialCO2.println(i);
    //}
  //  uint8_t serIn;
  //  if (Serial2.available()) {    
  //   // while (Serial.available()>0){
  //      
  //      serIn = Serial2.read();    
  //      String stringOne =  String(serIn, HEX);   
  //      stringOne += " ";
  //      Serial.print(stringOne); 
  //      //Serial.write(serIn); 
  //   // }     
  //  }
    //Serial1.print("a");
   // Serial2.print("a");
    //Serial3.print("a");

    //delay(1000);


    //Serial.print("a");


    // put your main code here, to run repeatedly:
   // digitalWrite(LED_BUILTIN, LOW);   // Arduino: turn the LED on (HIGH)
                                       // D1 Mini: turns the LED *off*
    //delay(1000);                       // wait for a second
    //digitalWrite(LED_BUILTIN, LOW);    // Arduino: turn the LED off (LOW)
                                       // D1 Mini: turns the LED *on*
    //delay(1000);                       // wait for a second
}
