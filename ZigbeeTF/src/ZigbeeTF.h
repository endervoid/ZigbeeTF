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

#include "Fifo.h"
#include "ZDO.h"



//TODO Packed
//struct LookupEntry{
//    uint8_t endpoint;
//    uint16_t profile;
//}

struct ExplicitRxPacketDispatcher {
    ZDOHandler zdoHandler;
	void handleRxPacket(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee) {
       // HandlerLookup[0x01][PROFILE][CLUSTER]
        //SerialDebug.print(F("Received ExplicitRxResponse packet from "));
        XBeeAddress64 remoteAddr64 = rx.getRemoteAddress64();
        uint16_t remoteAddr16 = rx.getRemoteAddress16();
        auto clusterId = rx.getClusterId();
        auto profileId = rx.getProfileId();
        auto srcEndpoint = rx.getSrcEndpoint();
        auto dstEndpoint = rx.getDstEndpoint();
        PacketProfile profile;
        char profileString[4]="";

        //TODO dont use string!!!!

        if (
            rx.getSrcEndpoint() == 0 &&
            rx.getDstEndpoint() == 0 &&
            profileId == WPAN_PROFILE_ZDO)
        {
            profile = ZDO;            
            memcpy(&profileString, "ZDO", sizeof profileString);
        }
        else if ((profileId == WPAN_PROFILE_ZCL)) {
            profile = ZCL;
        }
        else {
            profile = UNKNOWN;
            memcpy(&profileString, "UKN", sizeof profileString);
        }
        
        //print Header Information
        SerialDebug.print(F("<-- "));
        SerialDebug.print(profileString);
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

        if (profile == ZDO) {
            zdoHandler.handleRequest(rx, xbee);
        }
        else if (profile = ZCL) {
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
                SerialDebug.println();
                SerialDebug.println(F("Manufacturer specific is ON"));
                
            }
            else {

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

            if (zclFrame.commandIdentifier == 0x00) {
                //read attributes
                uint8_t payloadLen = b.len()-3;
                if (payloadLen > 0) {
                    printHex(SerialDebug, payloadLen);
                    SerialDebug.println();
                    SerialDebug.println(F("-----------------"));
                    uint16_t* payload = (uint16_t*)b.get(3);
                    for (int i = 0; i < payloadLen / 2; i++) {
                        SerialDebug.print("0x");
                        printHex(SerialDebug, payload[i]);
                        SerialDebug.println();
                    }
                    SerialDebug.println(F("-----------------"));
                }

            }
            //printHex(SerialDebug, ztest[0]);
            //SerialDebug.println();
            //printHex(SerialDebug, ztest[1]);
            //SerialDebug.println();
            //printHex(SerialDebug, ztest[2]);
            //SerialDebug.println();
            ////printHex(SerialDebug, frameControl);
            ////SerialDebug.println();
        }



            
        }

        //if (profile == ZDO && clusterId == ZDO_CLUST_ACTIVE_EP_REQ) {

        //    Buffer b(rx.getData(), rx.getDataLength());

        //    uint8_t* zdpCommand = b.remove(3);
        //    ACTIVE_EP_REQ* req = (ACTIVE_EP_REQ*)zdpCommand;
        //    //char trans_sequence_number = zdpCommand[0];
        //    
        //    SerialDebug.print("  Transaction Sequence Number 0x");
        //    printHex(SerialDebug, req->transactionSequenceNumber);
        //    SerialDebug.print("(");
        //    SerialDebug.print(req->transactionSequenceNumber);
        //    SerialDebug.print(")");
        //    SerialDebug.println();
        //    SerialDebug.print("  NWKAddrOfInterest 0x");            
        //    printHex(SerialDebug, req->NWKAddrOfInterest);
        //    SerialDebug.println();

        //    if (!ZDO_CLUST_IS_RESPONSE(clusterId)) {
        //        SerialDebug.println("-->Responding");                
        //        ACTIVE_EP_RESP payload;
        //        payload.transactionSequenceNumber = req->transactionSequenceNumber;
        //        payload.NWKAddrOfInterest = req->NWKAddrOfInterest;
        //        //TODO: addresses need to match! Else discovery or 
        //        payload.Status = ZDO_STATUS_SUCCESS;
        //        payload.ActiveEPCount = 1;
        //        payload.ActiveEPList = 1;                
        //        SerialDebug.println(sizeof(payload));
        //        //remoteAddr64.set(0x0013A20041B5D535);

        //        XBeeAddress64 respAddr(0x0013A20041B5D535);   
        //        uint8_t content[] = { 1, 2, 3 };
        //        uint8_t* p = new uint8_t();
        //        p[0] = 0;
        //        ZBExplicitTxRequest tx(rx.getRemoteAddress64(),(uint8_t*)&payload,sizeof(payload));
        //        tx.setSrcEndpoint(WPAN_ENDPOINT_ZDO);
        //        tx.setDstEndpoint(WPAN_ENDPOINT_ZDO);
        //        tx.setClusterId(ZDO_CLUST_ACTIVE_EP_RESP);                
        //        tx.setProfileId(WPAN_PROFILE_ZDO);                
        //        tx.setFrameId(xbee.getNextFrameId());

        //        xbee.send(tx);

        //        return;

        //        uint8_t status = xbee.sendAndWait(tx,2000);
        //        

        //        
        //        // This waits up to 5000 seconds, since the default TX timeout (NH
        //        // value of 1.6s, times three retries) is 4.8s.
        //        ZBExplicitRxResponse rx2;
        //        //uint8_t status = xbee.waitFor(rx2, 5000,  matchZdoReply, req->transactionSequenceNumber, tx.getFrameId());
        //        switch (status) {
        //        case 0: // Success
        //            SerialDebug.print(F("Success sending to 0x"));
        //            printHex(SerialDebug, respAddr.getMsb());
        //            printHex(SerialDebug, respAddr.getLsb());
        //            SerialDebug.print(F(". Status: 0x"));
        //            printHex(SerialDebug, status);
        //            SerialDebug.println();
        //            break;
        //        case XBEE_WAIT_TIMEOUT:
        //            SerialDebug.print(F("No reply received from 0x"));
        //            printHex(SerialDebug, respAddr.getMsb());
        //            printHex(SerialDebug, respAddr.getLsb());
        //            SerialDebug.println(F("."));
        //            break;
        //        default:
        //            SerialDebug.print(F("Failed to send to 0x"));
        //            printHex(SerialDebug, remoteAddr64.getMsb());
        //            printHex(SerialDebug, remoteAddr64.getLsb());                    
        //            SerialDebug.print(F(". Status: 0x"));
        //            printHex(SerialDebug, status);
        //            SerialDebug.println();
        //            
        //        }


        //    }

        //    //SerialDebug.println();
        //    //SerialDebug.print(" NWKAddrOfInterest 0x");
        //}

	}
};




#endif

