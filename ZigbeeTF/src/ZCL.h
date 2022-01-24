#ifndef _ZDO_h
#define _ZDO_h

#include <XBee.h>
#include <Printers.h>
#include "ZigbeeDefines.h"

/*******************************************/
//
// ZCL FRAME Format
// Frame Control (8) | ManufacturerCode(0/16) | TransactionSeqNbr(8) | CommandIdentifier(8) ||| Payload
// ---------------------------------------------------------------------------------------    -------
//                                            ZCL HEADER                                      ZCL Payload
//
// Frame Control Field 8bit:
// FrameType(2)[0-1] | ManufacturerSpec(1)[2] | Direction(1)[3] | DisableDefaultResponse(1)[4] | Reserver(3)[5-7]

/*******************************************/


//
//struct ZCLAttributes {
//
//};
//
//struct ZHandler {
//	//void handleRequest(ZBExplicitRxResponse& rx, XBeeWithCallbacks& xbee);
//};


#endif