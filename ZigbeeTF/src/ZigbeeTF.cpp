/*
 Name:		ZigbeeTF.cpp
 Created:	16.01.2022 14:47:32
 Author:	tfi
 Editor:	http://www.visualmicro.com
*/

#include "ZigbeeTF.h"

bool matchZdoReply_(ZBExplicitRxResponse& rx, uintptr_t data) {
    uint8_t* payload = rx.getFrameData() + rx.getDataOffset();
    uint8_t transactionId = (uint8_t)data;

    return rx.getSrcEndpoint() == 0 &&
        rx.getDstEndpoint() == 0 &&
        rx.getProfileId() == WPAN_PROFILE_ZDO &&
        payload[0] == transactionId;
}


