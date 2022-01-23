
#ifndef _ZDO_h
#define _ZDO_h

#include <XBee.h>
#include <Printers.h>
#include "ZigbeeDefines.h"

struct ZDOHandler {
	void handleRequest(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee);
	void handleActEPReq(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee);
	void handleSimpleDescReq(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee);
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

PACKED_STRUCT SIMPLE_DESC_REQ {
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