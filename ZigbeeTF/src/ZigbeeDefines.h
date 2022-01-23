#ifndef _ZigbeeDefines_h
#define _ZigbeeDefines_h

#define SerialDebug Serial
#define SerialXBee Serial2

#define PACKED_STRUCT struct __attribute__((__packed__))
typedef uint64_t addr64;

// Status values
#define ZDO_STATUS_SUCCESS					0x00
// 0x01 to 0x7F are reserved
#define ZDO_STATUS_INV_REQUESTTYPE		0x80
#define ZDO_STATUS_DEVICE_NOT_FOUND		0x81
#define ZDO_STATUS_INVALID_EP				0x82
#define ZDO_STATUS_NOT_ACTIVE				0x83
#define ZDO_STATUS_NOT_SUPPORTED			0x84
#define ZDO_STATUS_TIMEOUT					0x85
#define ZDO_STATUS_NO_MATCH				0x86
// 0x87 is reserved
#define zdo_status_no_entry				0x88
#define zdo_status_no_descriptor			0x89
#define zdo_status_insufficient_space	0x8a
#define zdo_status_not_permitted			0x8b
#define zdo_status_table_full				0x8c
#define zdo_status_not_authorized		0x8d

/// Cluster IDs with the high bit set are responses.
#define ZDO_CLUST_RESPONSE_MASK			0x8000
#define ZDO_CLUST_IS_RESPONSE(c)		(c & ZDO_CLUST_RESPONSE_MASK)

#define WPAN_ENDPOINT_ZDO			0x00
#define WPAN_PROFILE_ZDO				0x0000
#define WPAN_PROFILE_ZCL				0x0104

#define ZDO_CLUST_ACTIVE_EP_RESP 0x8005
#define ZDO_CLUST_ACTIVE_EP_REQ 0x0005
#define ZDO_CLUST_SIMPLE_DESC_REQ 0x0004
#define ZDO_CLUST_SIMPLE_DESC_RESP 0x8004


#endif