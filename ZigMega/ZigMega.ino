

#include "Fifo.h"
#include <ZigbeeTF.h>
#include <XBee.h>
#include <Printers.h>
//#include <binary.h>

XBeeWithCallbacks xbee;

#define SerialDebug Serial
#define SerialXBee Serial2




void processAtPacket(AtCommandResponse& rx, uintptr_t) {
    SerialDebug.println(F("Process: "));
    SerialDebug.print(F("ATCommandResponse 0x"));
    printHex(SerialDebug, rx.getErrorCode());
}



void processRxPacket(ZBRxResponse& rx, uintptr_t) {
    SerialDebug.println(F("Process: "));
    SerialDebug.print(F("Received packet from "));
    printHex(SerialDebug, rx.getRemoteAddress64());
    SerialDebug.println();
    SerialDebug.print(F("Payload: "));
    SerialDebug.write(rx.getData(), rx.getDataLength());
    SerialDebug.println();
    SerialDebug.print(F("Payload: "));
    Buffer b(rx.getData(), rx.getDataLength());
    uint8_t* zdpCommand = b.remove(3);
    ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*) zdpCommand;
    //char trans_sequence_number = zdpCommand[0];

    SerialDebug.println(req->transactionSequenceNumber);
    //SerialDebug.println(frame_control, HEX);
    //printHex(SerialDebug, frame_control);
    //SerialDebug.println();
    SerialDebug.println();
}

void processModemStatusResponse(ModemStatusResponse& rx, uintptr_t) {
    SerialDebug.print("ModemStatusResponse: 0x");
    printHex(SerialDebug,rx.getStatus());
    SerialDebug.println();
}

void invertEndian(uint8_t* buf, size_t len) {
    for (uint8_t i = 0, j = len - 1; i < len / 2; ++i, j--) {
        uint8_t tmp = buf[i];
        buf[i] = buf[j];        
        buf[j] = tmp;        
    }
}

void processExplicitRxPacket(ZBExplicitRxResponse& rx, uintptr_t) {
   // SerialDebug.println(F("Process: "));    
    /*if (rx.getSrcEndpoint() == 0xe8 && rx.getDstEndpoint() == 0xe8 &&
        rx.getProfileId() == 0xc105 && rx.getClusterId() == 0x0011) {
        SerialDebug.print(F("Normal packet from "));
        printHex(SerialDebug, rx.getRemoteAddress64());
        SerialDebug.println();
        SerialDebug.print(F("Payload: "));
        SerialDebug.write(rx.getData(), rx.getDataLength());
        SerialDebug.println();        
        return;
    }*/
    //SerialDebug.println(F("Normal2: "));
    
    ExplicitRxPacketDispatcher dispatcher;
    dispatcher.handleRxPacket(rx,xbee);
    return;
    
    //SerialDebug.print(F("Received ExplicitRxResponse packet from "));
    //printHex(SerialDebug, rx.getRemoteAddress64());
    //SerialDebug.println();
    //
    //SerialDebug.print(F("ClusterID: "));
    //printHex(SerialDebug,rx.getClusterId());
    //SerialDebug.println();
    //SerialDebug.print(F("ProfileID: "));
    //printHex(SerialDebug, rx.getProfileId());
    //SerialDebug.println();
    //SerialDebug.println(F("Payload: "));
    //Buffer b(rx.getData(), rx.getDataLength());
    //
    //uint8_t* zdpCommand = b.remove(3);
    //ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*)zdpCommand;
    ////char trans_sequence_number = zdpCommand[0];

    //SerialDebug.print("Transaction Sequence Number 0x");    
    //printHex(SerialDebug, req->transactionSequenceNumber);
    //SerialDebug.println();
    //SerialDebug.print(" NWKAddrOfInterest 0x");
    //printHex(SerialDebug, req->NWKAddrOfInterest);
    //SerialDebug.println();
    //SerialDebug.print(" NWKAddrOfInterest 0x");

 
   // invertEndian((uint8_t*)&req->NWKAddrOfInterest, 2);
   // printHex(SerialDebug, req->NWKAddrOfInterest);
   // SerialDebug.println();
    
}

void setup() {
    // put your setup code here, to run once:
    //pinMode(LED_BUILTIN, OUTPUT);
    SerialXBee.begin(9600);
    //Serial1.begin(9600);
    //Serial3.begin(9600);
    SerialDebug.begin(115200);
    SerialDebug.println("Starting...");
    //digitalWrite(LED_BUILTIN, LOW); 
    xbee.begin(SerialXBee);
    SerialDebug.println("XBee...");
    delay(500);
    SerialDebug.println("Sending...");
    //xbee.onResponse(printResponseCb, (uintptr_t)(Print*)&Serial);
    //xbee.onZBExplicitRxResponse(processExplicitRxPacket);
    //xbee.onZBRxResponse(processRxPacket);
    //xbee.onAtCommandResponse(processAtPacket);
    //xbee.onModemStatusResponse(processModemStatusResponse);
    xbee.onZBExplicitRxResponse(processExplicitRxPacket);
    //uint8_t value = 1;
    //AtCommandRequest req((uint8_t*)"AO", &value, sizeof(value));
    //xbee.send(req);
    //delay(1000);
   AtCommandRequest req2((uint8_t*)"NR");
    xbee.send(req2);
   // SerialDebug.println("OK...");
  
    //xbee.send(tx);
    //SerialXBee.print("+++");
    


}

void sendPacket() {    
    uint8_t content[] = { 1, 2, 3 };        
    ZBExplicitTxRequest tx;
    //tx.setSrcEndpoint(WPAN_ENDPOINT_ZDO);
    //tx.setDstEndpoint(WPAN_ENDPOINT_ZDO);
    tx.setSrcEndpoint(0x01);
    tx.setDstEndpoint(0x01);
    tx.setSrcEndpoint(0x00);
    tx.setDstEndpoint(0x00);
    //tx.setClusterId(ZDO_CLUST_ACTIVE_EP_RESP);
    tx.setClusterId(0x0000);
    //tx.setProfileId(WPAN_PROFILE_ZDO);
    tx.setProfileId(0x0104);
    //tx.setFrameId(xbee.getNextFrameId());
    //tx.setAddress16(0xFFFE);
    
    
    tx.setPayload(content);
    tx.setPayloadLength(sizeof(content));
    tx.setAddress64(0x0013A20041B5D535);

    // tx.setPayload(p);
    // tx.setPayloadLength(6);
    xbee.send(tx);
    //uint8_t status1 = xbee.send(tx, 5000);
    //SerialDebug.println(status1);
    SerialDebug.println("--");

    return;

    // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    txRequest.setAddress64(0x000000000000FFFF);
    uint8_t payload[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
    txRequest.setPayload(payload, sizeof(payload));
    // And send it
    uint8_t status = xbee.sendAndWait(txRequest, 5000);
    if (status == 0) {
        SerialDebug.println(F("Succesfully sent packet"));
    }
    else {
        SerialDebug.print(F("Failed to send packet. Status: 0x"));
    }
}

void loop() {
    
    xbee.loop();
    
   // sendPacket();
    delay(1000);
    //SerialDebug.println(".");
}
