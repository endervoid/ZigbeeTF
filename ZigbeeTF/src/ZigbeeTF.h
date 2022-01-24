/*
 Name:		ZigbeeTF.h
 Created:	16.01.2022 14:47:32
 Author:	tfi
 Editor:	http://www.visualmicro.com
*/

#ifndef _ZigbeeTF_h
#define _ZigbeeTF_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <XBee.h>
#include <Printers.h>

#define CO2_TX D1
#define CO2_RX D2





#include "Binary.h"
#include "ZigbeeDefines.h"

enum PacketProfile
{
	ZDO,
	ZCL,
	UNKNOWN
};




bool matchZdoReply_(ZBExplicitRxResponse& rx, uintptr_t data);

//#include "Fifo.h"
#include "ZDO.h"



//TODO Packed
//struct LookupEntry{
//    uint8_t endpoint;
//    uint16_t profile;
//}

#include "Endpoint .h"


#define PROFILE_ZCL_STRING F("ZCL")
#define PROFILE_ZDO_STRING F("ZDO")
#define PROFILE_UNHANDLED_STRING F("UKN")

//#define HEADER_VERBOSE;

template<int nOfEndpoints>
struct ExplicitRxPacketDispatcher {
	XBeeAddress64 remoteAddr64;
	uint16_t remoteAddr16;
	uint16_t clusterId;
	uint16_t profileId;
	uint8_t srcEndpoint;
	uint8_t dstEndpoint;
	PacketProfile profile;

	ZDOHandler< nOfEndpoints> zdoHandler;
	EndpointList_t<nOfEndpoints> _endpoints;

	
	ExplicitRxPacketDispatcher() {

	}


	void registerEndpoints(EndpointList_t<nOfEndpoints>& endpoints) {
		_endpoints = endpoints;		
		//SerialDebug.print(F("Number of Endpoints: "));
		//SerialDebug.println(endpoints.number);
		//printHex(SerialDebug, endpoints.endpoints[0].endPoint);
		//SerialDebug.println();
		//printHex(SerialDebug, endpoints.endpoints[1].endPoint);
		//SerialDebug.println();
		//SerialDebug.println(endpoints.endpoints[0].simpleDesc->AppNumInClusters);
		zdoHandler.setEndpointList(&_endpoints);
	}

	void printHeader() {
		//print Header Information
		SerialDebug.print(F("<-- "));
		if (profileId == WPAN_PROFILE_ZDO) {
			SerialDebug.print(PROFILE_ZDO_STRING);
		}
		else if (profileId == WPAN_PROFILE_ZCL) {
			SerialDebug.print(PROFILE_ZCL_STRING);
		}
		else {
			SerialDebug.print(PROFILE_UNHANDLED_STRING);
		}

		SerialDebug.print(F(" From(0x"));
		printHex(SerialDebug, remoteAddr64);
		SerialDebug.print(F(")"));
		SerialDebug.print(F("(EP:0x"));
		printHex(SerialDebug, srcEndpoint);
		SerialDebug.print(F("->0x"));
		printHex(SerialDebug, dstEndpoint);
		SerialDebug.print(F(") Cluster( 0x"));
		printHex(SerialDebug, clusterId);
		SerialDebug.print(F(" ) Profile ( 0x"));
		printHex(SerialDebug, profileId);
		SerialDebug.print(F(" )"));
		SerialDebug.println();
	}

	void handleRxPacket(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee) {	


		remoteAddr64 = rx.getRemoteAddress64();
		remoteAddr16 = rx.getRemoteAddress16();
		clusterId = rx.getClusterId();
		profileId = rx.getProfileId();
		srcEndpoint = rx.getSrcEndpoint();
		dstEndpoint = rx.getDstEndpoint();

#ifdef HEADER_VERBOSE
		printHeader();
#endif

		if (
			rx.getSrcEndpoint() == 0 &&
			rx.getDstEndpoint() == 0 &&
			profileId == WPAN_PROFILE_ZDO)
		{
			zdoHandler.handleRequest(rx, xbee);

		}
		else if ((profileId == WPAN_PROFILE_ZCL)) {


			union {
				struct {
					uint8_t frameControl : 8;
					uint8_t transSeqNumber : 8;
					uint8_t commandIdentifier : 8;
				};
				uint32_t zclframe;
			} typedef ZCLFrame_NoManufacturer;

			union {
				struct {
					uint8_t frameType : 2;
					uint8_t manufacturerSpec : 1;
					uint8_t direction : 1;
					uint8_t disableDefaultResponse : 1;
					uint8_t reserved : 3;
				};
				uint8_t fcf;
			} typedef FrameControlField;

			Buffer b(rx.getData(), rx.getDataLength());

			uint8_t frameControl = b.get(0)[0];
			FrameControlField bitfield;
			bitfield.fcf = frameControl;

			uint32_t zclf = ((uint32_t*)b.get(0))[0];
			uint8_t* ztest = b.get(0);
			ZCLFrame_NoManufacturer  zclFrame;
			zclFrame.zclframe = zclf;

			if (bitfield.manufacturerSpec == 1) {
#ifdef HEADER_VERBOSE
				SerialDebug.println();
				SerialDebug.println(F("Manufacturer specific is ON"));
#endif
				return;
			}
			else {
#ifdef HEADER_VERBOSE
				SerialDebug.println();
				printHex(SerialDebug, bitfield.frameType);
				SerialDebug.println();
				printHex(SerialDebug, bitfield.manufacturerSpec);
				SerialDebug.println();
				printHex(SerialDebug, bitfield.direction);
				SerialDebug.println();
				printHex(SerialDebug, bitfield.disableDefaultResponse);
				SerialDebug.println();
				printHex(SerialDebug, bitfield.reserved);
				SerialDebug.println();
				SerialDebug.println(F("-----------------"));
				printHex(SerialDebug, zclFrame.frameControl);
				SerialDebug.println();
				printHex(SerialDebug, zclFrame.transSeqNumber);
				SerialDebug.println();
				printHex(SerialDebug, zclFrame.commandIdentifier);
				SerialDebug.println();
				SerialDebug.println(F("-----------------"));
#endif
				if (zclFrame.commandIdentifier == 0x00) {
					//read attributes
					uint8_t payloadLen = b.len() - 3;
					if (payloadLen > 0) {
						printHex(SerialDebug, payloadLen);
						SerialDebug.println();
						SerialDebug.println(F("-----------------"));
						uint16_t* payload = (uint16_t*)b.get(3);
						for (int i = 0; i < payloadLen; i += 2) {
							SerialDebug.print("0x");
							printHex(SerialDebug, payload[i]);
							SerialDebug.println();
						}
						SerialDebug.println(F("-----------------"));
					}

				}
			}


		}
		else {
			return;
			

		}	
	}
};




#endif

