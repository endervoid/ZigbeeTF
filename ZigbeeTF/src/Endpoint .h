#ifndef _ENDPOINT_h
#define _ENDPOINT_h

typedef uint8_t zEndPoint_t;
typedef uint16_t zProfileId_t;
typedef uint16_t zDeviceId_t;
typedef uint8_t zCounter_t;
typedef uint8_t cId_t;
typedef uint8_t cOut_t;

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#endif

/// Helper macro for calculating the number of entries in an array.
#define _TABLE_ENTRIES(array)    ((sizeof (array)) / (sizeof (*array)))



struct SimpleDescriptor_t
{
	zEndPoint_t EndPoint;//Endpoint number 1-240 
	zProfileId_t AppProfId;//Supported Profile ID 
	zDeviceId_t AppDeviceId;//Support Device ID 
	uint8_t AppDevVer : 4;//The version of the device description executed 
	uint8_t Reserved : 4;//Reserved

	zCounter_t AppNumInClusters;//The number of input clusters supported by the terminal 
	cId_t* pAppInClusterList;//Pointer to the input Cluster ID list 
	zCounter_t AppNumOutClusters;//The number of output clusters supported by the terminal 
	cId_t* pAppOutClusterList;//Pointer to the output Cluster ID list 
} ;//Simple descriptor

typedef struct
{
	uint8_t endPoint;//Endpoint number 1-240 	
	SimpleDescriptor_t* simpleDesc;//simple descriptor 	
	uint8_t flags;
} EndPointDesc_t;//endpoint descriptor 



template <int nOfEndpoints>
struct EndpointList_t{

	static const uint8_t number = nOfEndpoints;
	EndPointDesc_t endpoints[nOfEndpoints];
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

#endif
