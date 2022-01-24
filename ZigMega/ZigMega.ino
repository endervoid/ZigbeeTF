

//#include "Fifo.h"
#include <ZigbeeTF.h>
#include <XBee.h>
#include <Printers.h>
#include <binary.h>
#include <Endpoint .h>

XBeeWithCallbacks xbee;

#define SerialDebug Serial
#define SerialXBee Serial2




void processAtPacket(AtCommandResponse& rx, uintptr_t) {
	SerialDebug.println(F("Process: "));
	SerialDebug.print(F("ATCommandResponse 0x"));
	printHex(SerialDebug, rx.getErrorCode());
}



void processRxPacket(ZBRxResponse& rx, uintptr_t) {
	//SerialDebug.println(F("Process: "));
	//SerialDebug.print(F("Received packet from "));
	//printHex(SerialDebug, rx.getRemoteAddress64());
	//SerialDebug.println();
	//SerialDebug.print(F("Payload: "));
	//SerialDebug.write(rx.getData(), rx.getDataLength());
	//SerialDebug.println();
	//SerialDebug.print(F("Payload: "));
	//Buffer b(rx.getData(), rx.getDataLength());
	//uint8_t* zdpCommand = b.remove(3);
	//ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*) zdpCommand;
	////char trans_sequence_number = zdpCommand[0];

	//SerialDebug.println(req->transactionSequenceNumber);
	////SerialDebug.println(frame_control, HEX);
	////printHex(SerialDebug, frame_control);
	////SerialDebug.println();
	//SerialDebug.println();
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

ExplicitRxPacketDispatcher<2> dispatcher;

void processExplicitRxPacket(ZBExplicitRxResponse& rx, uintptr_t) {
	//SerialDebug.print(F("Payload: "));    
	
	dispatcher.handleRxPacket(rx,xbee);
	return;      
}

template<int nOfEndpoints>
EndpointList_t<nOfEndpoints> createEndpointList() {
	SimpleDescriptor_t* sDescr = new SimpleDescriptor_t();
	sDescr->AppDeviceId = 0x0100;
	sDescr->AppProfId = 0x0104;
	sDescr->EndPoint = 0x01;
	sDescr->AppDevVer = 0x00;
	sDescr->AppNumInClusters = 1;
	sDescr->AppNumOutClusters = 1;

	cId_t* inCluster = new cId_t[1];
	inCluster[0] = 0x0000;
	cOut_t* outCluster = new cId_t[1];
	outCluster[0] = 0x0104;

	sDescr->pAppInClusterList = inCluster;
	sDescr->pAppOutClusterList = outCluster;

	EndPointDesc_t ep1;
	ep1.flags = 0x00;
	ep1.endPoint = sDescr->EndPoint;
	ep1.simpleDesc = sDescr;

	sDescr = new SimpleDescriptor_t();
	sDescr->AppDeviceId = 0x0100;
	sDescr->AppProfId = 0x0104;
	sDescr->EndPoint = 0x0F;
	sDescr->AppDevVer = 0x00;
	sDescr->AppNumInClusters = 1;
	sDescr->AppNumOutClusters = 1;

	inCluster = new cId_t[1];
	inCluster[0] = 0x0000;
	outCluster = new cId_t[1];
	outCluster[0] = 0x0104;

	sDescr->pAppInClusterList = inCluster;
	sDescr->pAppOutClusterList = outCluster;

	EndPointDesc_t ep2;
	ep1.flags = 0x00;
	ep1.endPoint = sDescr->EndPoint;
	ep1.simpleDesc = sDescr;

	EndpointList_t<2> epList;
	epList.endpoints[0] = ep1;
	epList.endpoints[1] = ep2;

	

	return epList;

}

void setup() {
	
	SerialXBee.begin(9600);
	SerialDebug.begin(115200);	
	SerialDebug.println("Starting...");
	//digitalWrite(LED_BUILTIN, LOW); 

	xbee.begin(SerialXBee);

	SerialDebug.println("XBee...");
	//delay(500);
	//SerialDebug.println("Sending...");
	//xbee.onResponse(printResponseCb, (uintptr_t)(Print*)&Serial);
	xbee.onZBExplicitRxResponse(processExplicitRxPacket);
	//xbee.onZBRxResponse(processRxPacket);
	//xbee.onAtCommandResponse(processAtPacket);
	//xbee.onModemStatusResponse(processModemStatusResponse);
	//uint8_t value = 3;
	//AtCommandRequest req((uint8_t*)"AO", &value, sizeof(value));

	//xbee.onZBExplicitRxResponse(processExplicitRxPacket);
	
	//xbee.send(req);
	//delay(1000);

	 AtCommandRequest req2((uint8_t*)"NR");
	 xbee.send(req2);  

	 auto endpoints = createEndpointList<2>();
	 
	 dispatcher.registerEndpoints(endpoints);
	
}

void sendPacket() {    
	//uint8_t content[] = { 1, 2, 3 };        
	//ZBExplicitTxRequest tx;
	////tx.setSrcEndpoint(WPAN_ENDPOINT_ZDO);
	////tx.setDstEndpoint(WPAN_ENDPOINT_ZDO);
	//tx.setSrcEndpoint(0x01);
	//tx.setDstEndpoint(0x01);
	//tx.setSrcEndpoint(0x00);
	//tx.setDstEndpoint(0x00);
	////tx.setClusterId(ZDO_CLUST_ACTIVE_EP_RESP);
	//tx.setClusterId(0x0000);
	////tx.setProfileId(WPAN_PROFILE_ZDO);
	//tx.setProfileId(0x0104);
	////tx.setFrameId(xbee.getNextFrameId());
	////tx.setAddress16(0xFFFE);
	//
	//
	//tx.setPayload(content);
	//tx.setPayloadLength(sizeof(content));
	//tx.setAddress64(0x0013A20041B5D535);

	//// tx.setPayload(p);
	//// tx.setPayloadLength(6);
	//xbee.send(tx);
	////uint8_t status1 = xbee.send(tx, 5000);
	////SerialDebug.println(status1);
	//SerialDebug.println("--");

	//return;

	//// Prepare the Zigbee Transmit Request API packet
	//ZBTxRequest txRequest;
	//txRequest.setAddress64(0x000000000000FFFF);
	//uint8_t payload[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
	//txRequest.setPayload(payload, sizeof(payload));
	//// And send it
	//uint8_t status = xbee.sendAndWait(txRequest, 5000);
	//if (status == 0) {
	//    SerialDebug.println(F("Succesfully sent packet"));
	//}
	//else {
	//    SerialDebug.print(F("Failed to send packet. Status: 0x"));
	//}
}

void loop() {
	
   xbee.loop();
	
   // sendPacket();
}
