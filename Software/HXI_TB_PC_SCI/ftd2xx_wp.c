#include "ftd2xx_wp.h"


FT_HANDLE g_ftHandle;  
HMODULE g_hmodule;
int LoadDLL();
	
typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
PtrToOpen g_pOpen; 
FT_STATUS Open(PVOID);

int LoadDLL()
{
	g_hmodule = LoadLibrary("Ftd2xx.dll");
	if(g_hmodule == NULL)  //Ftd2xx.dll无法加载
		return WP_LOAD_ERROR;
	//调用open
	g_pOpen = (PtrToOpen)GetProcAddress(g_hmodule, "FT_Open");
	if (g_pOpen == NULL)
	{
		return WP_OPEN_ERROR;
	}
	return WP_OK;
}

FT_STATUS  open(PVOID pvDevice)
{
	if(!g_pOpen)
	{
		return FT_INVALID_HANDLE;		
	}
	return (*g_pOpen)(pvDevice, &g_ftHandle );
}
