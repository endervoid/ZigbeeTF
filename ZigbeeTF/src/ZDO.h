
#ifndef _ZDO_h
#define _ZDO_h

#include <XBee.h>
#include <Printers.h>
#include "ZigbeeDefines.h"
#include "Endpoint .h"
#include "Binary.h"

PACKED_STRUCT ACTIVE_EP_REQ{
	uint8_t transactionSequenceNumber;
	uint16_t NWKAddrOfInterest;
};
//TODO: typedef PACKED_STRUCT zdo_active_ep_rsp_header_t{
PACKED_STRUCT ACTIVE_EP_RESP{
	uint8_t transactionSequenceNumber;
	uint8_t Status;
	uint16_t NWKAddrOfInterest;
	uint8_t ActiveEPCount;
	uint8_t ActiveEPList[];
};

PACKED_STRUCT SIMPLE_DESC_REQ{
	uint8_t transactionSequenceNumber;
	uint16_t NWKAddrOfInterest;
	uint8_t EndPoint;
};

PACKED_STRUCT SIMPLE_DESCR_HEADER{
	uint8_t Endpoint;
	uint16_t ApplicationProfileIdentifier;
	uint16_t ApplicationDeviceIdentifier;
	uint8_t ApplicationDeviceVersion;  //lower bits are reserved vvvvrrrr
	//variable cluster list afterwards
};

PACKED_STRUCT SIMPLE_DESC_RESP_HEADER{
	uint8_t transactionSequenceNumber;
	uint8_t Status;
	uint16_t NWKAddrOfInterest;
	uint8_t Length;
	//SIMPLE_DESCR_HEADER+variable cluster list afterwards
};

template<int nOfEndpoints>
struct ZDOHandler {
	EndpointList_t<nOfEndpoints>* _endpoints;

	void setEndpointList(EndpointList_t<nOfEndpoints>* endpoints) {
		_endpoints = endpoints;
	};
	

	void handleRequest(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee) {
		{
			auto clusterId = rx.getClusterId();
			switch (clusterId) {
			case ZDO_CLUST_ACTIVE_EP_REQ:
				handleActEPReq(rx, xbee);
				break;
			case ZDO_CLUST_SIMPLE_DESC_REQ:
				handleSimpleDescReq(rx, xbee);
				break;
			}
		};
	};

	void handleActEPReq(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee)
	{

		XBeeAddress64 remoteAddr64 = rx.getRemoteAddress64();
		uint16_t remoteAddr16 = rx.getRemoteAddress16();
		auto clusterId = rx.getClusterId();
		auto profileId = rx.getProfileId();
		auto srcEndpoint = rx.getSrcEndpoint();
		auto dstEndpoint = rx.getDstEndpoint();
		Buffer b(rx.getData(), rx.getDataLength());

		if (b.len() != sizeof(ACTIVE_EP_REQ)) {
			SerialDebug.println(F("ERROR: buffer len mismatch"));
			return;
		}
		ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*)(b.get(0));
		if (req == NULL) {
			SerialDebug.println(F("ERROR: buffer empty?"));
			return;
		}
		//ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*)zdpCommand;
		//ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*)(rx.getFrameData() + rx.getDataOffset());;

		SerialDebug.print(F("  Transaction Sequence Number 0x"));
		printHex(SerialDebug, req->transactionSequenceNumber);
		SerialDebug.print(F("("));
		SerialDebug.print(req->transactionSequenceNumber);
		SerialDebug.print(F(")"));
		SerialDebug.println();
		SerialDebug.print(F("  NWKAddrOfInterest 0x"));
		printHex(SerialDebug, req->NWKAddrOfInterest);
		SerialDebug.println();


		SerialDebug.println(F("-->Responding"));
		uint8_t activeEPCount = 2;
		uint8_t* payload = new uint8_t[5 + activeEPCount];

		ACTIVE_EP_RESP* resp = (ACTIVE_EP_RESP*)payload;

		resp->transactionSequenceNumber = req->transactionSequenceNumber;
		resp->NWKAddrOfInterest = req->NWKAddrOfInterest;
		//TODO: addresses need to match! Else discovery or 
		resp->Status = ZDO_STATUS_SUCCESS;
		resp->ActiveEPCount = activeEPCount;
		//resp->ActiveEPList = new uint8_t[payload.ActiveEPCount];
		resp->ActiveEPList[0] = 0x01;
		resp->ActiveEPList[1] = 0x11;
		//SerialDebug.println(sizeof(payload));

		ZBExplicitTxRequest tx(rx.getRemoteAddress64(), payload, 5 + activeEPCount);
		tx.setSrcEndpoint(WPAN_ENDPOINT_ZDO);
		tx.setDstEndpoint(WPAN_ENDPOINT_ZDO);
		tx.setClusterId(ZDO_CLUST_ACTIVE_EP_RESP);
		tx.setProfileId(WPAN_PROFILE_ZDO);
		tx.setFrameId(xbee.getNextFrameId());
		xbee.send(tx);
		delete[] payload;

		//test

	};
	void handleSimpleDescReq(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee)

	{
		XBeeAddress64 remoteAddr64 = rx.getRemoteAddress64();
		uint16_t remoteAddr16 = rx.getRemoteAddress16();
		auto clusterId = rx.getClusterId();
		auto profileId = rx.getProfileId();
		auto srcEndpoint = rx.getSrcEndpoint();
		auto dstEndpoint = rx.getDstEndpoint();

		//TODO: maybe use buffer for safety when length doesnt match type

		Buffer b(rx.getData(), rx.getDataLength());


		if (b.len() != sizeof(SIMPLE_DESC_REQ)) {
			SerialDebug.println(F("ERROR: buffer len mismatch"));
			SerialDebug.print(b.len());
			SerialDebug.print("!=");
			SerialDebug.print(sizeof(SIMPLE_DESC_REQ));
			SerialDebug.println();
			return;
		}
		SIMPLE_DESC_REQ* req = (SIMPLE_DESC_REQ*)(b.get(0));
		if (req == NULL) {
			SerialDebug.println(F("ERROR: buffer empty?"));
			return;
		}

		SerialDebug.print(F("  Transaction Sequence Number 0x"));
		printHex(SerialDebug, req->transactionSequenceNumber);
		SerialDebug.print(F("("));
		SerialDebug.print(req->transactionSequenceNumber);
		SerialDebug.print(F(")"));
		SerialDebug.println();
		SerialDebug.print(F("  NWKAddrOfInterest 0x"));
		printHex(SerialDebug, req->NWKAddrOfInterest);
		SerialDebug.println();
		SerialDebug.print(F("  Endpoint 0x"));
		printHex(SerialDebug, req->EndPoint);
		SerialDebug.print(F("("));
		SerialDebug.print(req->EndPoint);
		SerialDebug.print(F(")"));
		SerialDebug.println();



		uint8_t inputClusterCount = 1;
		uint8_t outputClusterCount = 1;

		uint8_t clusterListLength = (2 + inputClusterCount * 2 + outputClusterCount * 2);
		uint8_t totalLength = sizeof(SIMPLE_DESCR_HEADER) + sizeof(SIMPLE_DESC_RESP_HEADER) + clusterListLength;

		uint8_t* payload = new uint8_t[totalLength];
		SIMPLE_DESC_RESP_HEADER* resp_header = (SIMPLE_DESC_RESP_HEADER*)&payload[0];
		SIMPLE_DESCR_HEADER* desc_header = (SIMPLE_DESCR_HEADER*)&payload[sizeof(SIMPLE_DESC_RESP_HEADER)];

		desc_header->Endpoint = req->EndPoint;


		desc_header->ApplicationProfileIdentifier = 0x0104;// 41440;// 0x0014;

		desc_header->ApplicationDeviceIdentifier = 0x0100; //97;// 0x0014;

		desc_header->ApplicationDeviceVersion = 0x00;


		payload[sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER)] = inputClusterCount;


		uint16_t* pointer16;

		pointer16 = (uint16_t*)&payload[sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + 1];

		*pointer16 = 0x0000;// 33;// 0x0021;
		payload[1 + sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + inputClusterCount * 2] = outputClusterCount;


		pointer16 = (uint16_t*)&payload[2 + sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + inputClusterCount * 2];
		*pointer16 = 0x0019; //33;// 0x0022;

		resp_header->Length = sizeof(SIMPLE_DESCR_HEADER) + clusterListLength;
		resp_header->NWKAddrOfInterest = req->NWKAddrOfInterest;
		resp_header->Status = ZDO_STATUS_SUCCESS;
		resp_header->transactionSequenceNumber = req->transactionSequenceNumber;


		SerialDebug.println();
		SerialDebug.print(F("Seq num 0x"));
		printHex(SerialDebug, payload[0]); //SeqNum
		SerialDebug.println();
		SerialDebug.print(F("status 0x"));
		printHex(SerialDebug, payload[1]); //Status
		SerialDebug.println();
		SerialDebug.print(F("addr16 0x"));
		printHex(SerialDebug, *((uint16_t*)&payload[2])); //16bit Addr
		SerialDebug.println();
		SerialDebug.print(F("length 0x"));
		printHex(SerialDebug, payload[4]); //Length
		SerialDebug.println();
		SerialDebug.print(F("endpoint 0x"));
		printHex(SerialDebug, payload[5]); //Endpoint
		SerialDebug.println();
		SerialDebug.print(F("prof ident 0x"));
		printHex(SerialDebug, *((uint16_t*)&payload[6])); //prof Ident
		SerialDebug.println();
		SerialDebug.print(F("dev ident 0x"));
		printHex(SerialDebug, *((uint16_t*)&payload[8])); //dev Ident
		SerialDebug.println();
		SerialDebug.print(F("version 0x"));
		printHex(SerialDebug, uint8_t(payload[10] & 0xf)); //Version
		SerialDebug.println();
		SerialDebug.print(F("icc "));
		SerialDebug.print(payload[11]); //input cluster count
		SerialDebug.println();
		SerialDebug.print(F("ic1 0x"));
		printHex(SerialDebug, *((uint16_t*)&payload[12])); //input cluster 1
		SerialDebug.println();
		SerialDebug.print(F("occ "));
		SerialDebug.print(payload[14]); //output cluster count
		SerialDebug.println();
		SerialDebug.print(F("oc1 0x"));
		printHex(SerialDebug, *((uint16_t*)&payload[15])); //output cluster 1
		SerialDebug.println();


		ZBExplicitTxRequest tx(rx.getRemoteAddress64(), payload, totalLength);
		tx.setSrcEndpoint(WPAN_ENDPOINT_ZDO);
		tx.setDstEndpoint(WPAN_ENDPOINT_ZDO);
		tx.setClusterId(ZDO_CLUST_SIMPLE_DESC_RESP);
		tx.setProfileId(WPAN_PROFILE_ZDO);
		tx.setFrameId(xbee.getNextFrameId());
		xbee.send(tx);

		delete[] payload;
		
		SerialDebug.print(F("Number of Endpoints: "));
		SerialDebug.println(this->_endpoints->number);
		printHex(SerialDebug, this->_endpoints->endpoints[0].endPoint);
		SerialDebug.println();
		printHex(SerialDebug, this->_endpoints->endpoints[1].endPoint);
		SerialDebug.println();

		SimpleDescriptor_t zDescr;
		zDescr.EndPoint = 0x01;
		zDescr.AppProfId = 0x0104;
		zDescr.AppDevVer = 0x00;
		zDescr.AppDeviceId = 0x0100;
		zDescr.EndPoint = 0x01;
		zDescr.AppNumInClusters = 1;
		zDescr.AppNumOutClusters = 1;
		//delete[] payload.ActiveEPList;
	};

	void createPayload() {
		
		uint8_t clusterListLength = (2 + _endpoints->endpoints[0]->simpleDesc->AppNumInClusters * 2 + 
			_endpoints->endpoints[0]->simpleDesc->AppNumOutClusters * 2);
		//uint8_t totalLength = sizeof(SIMPLE_DESCR_HEADER) + sizeof(SIMPLE_DESC_RESP_HEADER) + clusterListLength;

		//uint8_t* payload = new uint8_t[totalLength];
		//SIMPLE_DESC_RESP_HEADER* resp_header = (SIMPLE_DESC_RESP_HEADER*)&payload[0];
		//SIMPLE_DESCR_HEADER* desc_header = (SIMPLE_DESCR_HEADER*)&payload[sizeof(SIMPLE_DESC_RESP_HEADER)];

		//desc_header->Endpoint = req->EndPoint;

		//desc_header->ApplicationProfileIdentifier = 0x0104;// 41440;// 0x0014;

		//desc_header->ApplicationDeviceIdentifier = 0x0100; //97;// 0x0014;

		//desc_header->ApplicationDeviceVersion = 0x00;
		//payload[sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER)] = inputClusterCount;

		//uint16_t* pointer16;

		//pointer16 = (uint16_t*)&payload[sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + 1];

		//*pointer16 = 0x0000;// 33;// 0x0021;
		//payload[1 + sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + inputClusterCount * 2] = outputClusterCount;


		//pointer16 = (uint16_t*)&payload[2 + sizeof(SIMPLE_DESC_RESP_HEADER) + sizeof(SIMPLE_DESCR_HEADER) + inputClusterCount * 2];
		//*pointer16 = 0x0019; //33;// 0x0022;

		//resp_header->Length = sizeof(SIMPLE_DESCR_HEADER) + clusterListLength;
		//resp_header->NWKAddrOfInterest = req->NWKAddrOfInterest;
		//resp_header->Status = ZDO_STATUS_SUCCESS;
		//resp_header->transactionSequenceNumber = req->transactionSequenceNumber;
	}
};

//const zbSimpleDescriptor_t Endpoint9_simpleDescriptor = {
//
//  9, /* Endpoint number */
//
//  0x4, 0x1, /* Application profile ID */
//
//  0x0, 0x1, /* Application device ID */
//
//  0, /* Application version ID */
//
//  5, /* Number of input clusters */
//
//  (uint8_t*)Endpoint9_InputClusterList, /* Input cluster list */
//
//  0, /* Number of output clusters */
//
//  (uint8_t*)Endpoint9_OutputClusterList, /* Output cluster list */
//
//};



//typedef struct zbZbSimpleDescriptor_tag
//{
//	/*End point ID */
//	zbEndPoint_t endPoint;
//	/*Application Profile ID*/
//	zbProfileId_t aAppProfId;
//	/*Appliacation Device ID*/
//	zbDeviceId_t aAppDeviceId;
//	/*Application Device Version And APS Flag*/
//	uint8_t appDevVerAndFlag;
//	/*Number of Input Cluster ID Supported by the End Point*/
//	zbCounter_t appNumInClusters;
//	/*Place Holder for the list of Input Cluster ID*/
//	uint8_t* pAppInClusterList;
//	/*Number of Output Cluster ID Supported by the End Point*/
//	zbCounter_t appNumOutClusters;
//	/*Place Holder for the list of Output Cluster ID*/
//	uint8_t* pAppOutClusterList;
//}zbZbSimpleDescriptor_t;



#endif