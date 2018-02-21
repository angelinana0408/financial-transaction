#ifndef __INC_LCTCPCLIENT__H
#define __INC_LCTCPCLIENT__H

//ERROR CODE
#define ERROR_OK				0
#define ERROR_SYSTEM			1
#define ERROR_CONFIG			2
#define ERROR_DATA_INVALID		3
#define ERROR_CONNECTION		4
#define ERROR_SEND				5


#pragma pack(push,1)

#ifdef __cplusplus
extern "C" {
#endif

//API
int extern WINAPI Send2AMAS ( LPCSTR ISO8583,UINT length_8583,LPCSTR AppField,UINT length_field);

int extern WINAPI CloseAgentService ();

#ifdef __cplusplus
}
#endif

#pragma pack(pop)
#endif