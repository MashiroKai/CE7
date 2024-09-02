#include <Windows.h> 
#include <rs232.h>
#include <tcpsupp.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <time.h> 
#include "toolbox.h"
#include "asynctmr.h"
#include <cvirte.h>		
#include <userint.h>
#include "EPH5275_PNP.h"

#include "ftd2xx.h"
#include "ftd2xx_wp.h"

#define TB 0.1
static void *asyncCBData = "这是一个异步定时器示例！5555555555";

static int timerid;
static int PnlHandle;

int CVICALLBACK asynCB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);

char  CHAPathName[512],file_receive_s[50]={"RECORD\\状态数据"};

int year,month,day;
int hour,mini,sec;
int pre_sec=-1;

time_t t_start,t_stop;
int Tcnt=0;
int total_bytes=0;
int rx_num=0;

unsigned char a[64];
int on_off=0;
 


unsigned char s1[100000]="";
unsigned char s2[100000]="";
unsigned char s3[100000]=""; 
unsigned char s4[100000]="";
unsigned char sx[100000]="";
unsigned char sdate[100];
unsigned char stime[100];

unsigned char buff[1000000]; //遥测

FILE  *fp_sci;

FILE  *fp_rec;
FILE  *fp_blackbox;

int st_load=-1;
int freed=0;
int closed=0;

FT_HANDLE g_ftHandle;  
HMODULE g_hmodule;
DWORD dwRxBytes;
int LoadDLL();
	
typedef FT_STATUS (WINAPI *PtrToOpen)(int, FT_HANDLE *);
PtrToOpen g_pOpen; 
FT_STATUS Open(int);

typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
PtrToListDevices g_pListDevices; 
FT_STATUS ListDevices(PVOID, PVOID, DWORD);

typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
PtrToClose g_pClose;
FT_STATUS Close();

typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
PtrToRead g_pRead;
FT_STATUS Read(LPVOID, DWORD, LPDWORD);

typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
PtrToWrite g_pWrite;
FT_STATUS Write(LPVOID, DWORD, LPDWORD);

typedef FT_STATUS (WINAPI *PtrToSetUSBParameters)(FT_HANDLE,ULONG,ULONG);
PtrToSetUSBParameters g_pSetUSBPara;
FT_STATUS SetUSBPara(ULONG,ULONG);

typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
PtrToSetTimeouts g_pSetTimeouts;
FT_STATUS SetTimeouts(ULONG, ULONG);

typedef FT_STATUS (WINAPI *PtrToSetBitmode)(FT_HANDLE, UCHAR, UCHAR);
PtrToSetBitmode g_pSetBitmode;
FT_STATUS SetBitmode(UCHAR, UCHAR);


typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE,DWORD *);
PtrToGetQueueStatus g_pGetQueueStatus;
FT_STATUS  FT232_GetQueueStatus(void);

/*


	FTD2XX_API
		FT_STATUS WINAPI FT_SetTimeouts(
		FT_HANDLE ftHandle,
		ULONG ReadTimeout,
		ULONG WriteTimeout
		);    


	FTD2XX_API
		FT_STATUS WINAPI FT_SetBitMode(
		FT_HANDLE ftHandle,
		UCHAR ucMask,
		UCHAR ucEnable
		);
*/

int LoadDLL()
{
	g_hmodule = LoadLibrary("ftd2xx.dll");
	if(g_hmodule == NULL)
		return WP_LOAD_ERROR;
	g_pOpen = (PtrToOpen)GetProcAddress(g_hmodule, "FT_Open");
	if (g_pOpen == NULL)
	{
		return WP_OPEN_ERROR;
	}
	//....
	g_pWrite = (PtrToWrite)GetProcAddress(g_hmodule, "FT_Write");
	if (g_pWrite == NULL)
	{
		return WP_WRITE_ERROR;
	}

	g_pRead = (PtrToRead)GetProcAddress(g_hmodule, "FT_Read");
	if (g_pRead == NULL)
	{
		return WP_READ_ERROR;
	}
	
	g_pListDevices = (PtrToListDevices)GetProcAddress(g_hmodule, "FT_ListDevices");
	if(g_pListDevices == NULL)
	{
		return WP_LIST_ERROR;
	}

	g_pClose = (PtrToClose)GetProcAddress(g_hmodule, "FT_Close");
	if (g_pClose == NULL)
	{
		return WP_CLOSE_ERROR;
	}
	
	g_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(g_hmodule, "FT_SetTimeouts");
	if (g_pSetTimeouts == NULL)
	{
		return WP_SETTIMEOUT_ERROR;
	}
	g_pSetBitmode = (PtrToSetBitmode)GetProcAddress(g_hmodule, "FT_SetBitMode");
	if (g_pSetBitmode == NULL)
	{
		return WP_SetBitmode_ERROR;
	}
	
	g_pSetUSBPara = (PtrToSetUSBParameters)GetProcAddress(g_hmodule, "FT_SetUSBParameters");
	if (g_pSetUSBPara == NULL)
	{
		return WP_SETUSBPARA_ERROR;
	}
	
	
	g_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(g_hmodule, "FT_GetQueueStatus");
	if(g_pGetQueueStatus == NULL)
	{
	   return WP_GETQUEUESTATUS_ERROR;
	}
	return WP_OK;
}
int FreeDLL()
{
	return FreeLibrary(g_hmodule);
}

FT_STATUS  open(int DeviceID)
{
	if(!g_pOpen)
	{
		return FT_INVALID_HANDLE;		
	}
	return (*g_pOpen)(DeviceID, &g_ftHandle );
}
FT_STATUS Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)  
{																		   
	if (!g_pRead)
	{
		return FT_INVALID_HANDLE;
	}

	return (*g_pRead)(g_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	
FT_STATUS Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!g_pWrite)
	{
		return FT_INVALID_HANDLE;
	}
	
	return (*g_pWrite)(g_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}	
FT_STATUS ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!g_pListDevices)
	{
		return FT_INVALID_HANDLE;
	}
	return (*g_pListDevices)(pArg1, pArg2, dwFlags);
}	
FT_STATUS Close()
{
	if (!g_pClose)
	{
		return FT_INVALID_HANDLE;
	}
	
	return (*g_pClose)(g_ftHandle);
}	
FT_STATUS SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!g_pSetTimeouts)
	{
		return FT_INVALID_HANDLE;
	}
	return (*g_pSetTimeouts)(g_ftHandle, dwReadTimeout, dwWriteTimeout);
}
	
FT_STATUS SetBitmode(UCHAR dwucMask, UCHAR dwucEnable)
{
	if (!g_pSetBitmode)
	{
		return FT_INVALID_HANDLE;
	}
	return (*g_pSetBitmode)(g_ftHandle, dwucMask, dwucEnable);
}

FT_STATUS SetUSBPara(ULONG dinTransferSize,ULONG doutTransferSize)
{
	if(!g_pSetUSBPara)
	{
		return FT_INVALID_HANDLE; 
	}
	return (*g_pSetUSBPara)(g_ftHandle,dinTransferSize,doutTransferSize);
}

FT_STATUS FT232_GetQueueStatus(void)
{
	if(!g_pGetQueueStatus)
	{
	   return FT_INVALID_HANDLE;
	}
	return (*g_pGetQueueStatus)(g_ftHandle,&dwRxBytes);

}
void display_date(void)
{
	char ww[100];
	GetSystemDate(&month,&day,&year);
	sdate[0]='\0';
	ww[0]='\0';Fmt(ww,"%s<%d",year);strcat(sdate,ww);strcat(sdate,"年");
	ww[0]='\0';Fmt(ww,"%s<%d",month);if(month<10) strcat(sdate,"0");strcat(sdate,ww);strcat(sdate,"月");
	ww[0]='\0';Fmt(ww,"%s<%d",day);  if(day<10)   strcat(sdate,"0");strcat(sdate,ww);strcat(sdate,"日");
}
void display_time(void)
{
	char ww[100];
	GetSystemTime(&hour,&mini,&sec);
	stime[0]='\0';
	ww[0]='\0';
	Fmt(ww,"%s<%d",hour);if(hour<10) strcat(stime,"0");strcat(stime,ww);strcat(stime,":");
	ww[0]='\0';Fmt(ww,"%s<%d",mini);if(mini<10) strcat(stime,"0");strcat(stime,ww);strcat(stime,":");
	ww[0]='\0';Fmt(ww,"%s<%d",sec); if(sec<10)  strcat(stime,"0");strcat(stime,ww);
}
void display_date2(void)
{
	char ww[100];
	GetSystemDate(&month,&day,&year);
	sdate[0]='\0';
	ww[0]='\0';Fmt(ww,"%s<%d",year);strcat(sdate,ww);strcat(sdate,"_");
	ww[0]='\0';Fmt(ww,"%s<%d",month);if(month<10) strcat(sdate,"0");strcat(sdate,ww);strcat(sdate,"_");
	ww[0]='\0';Fmt(ww,"%s<%d",day);  if(day<10)   strcat(sdate,"0");strcat(sdate,ww);strcat(sdate,"_");
}
void display_time2(void)
{
	char ww[100];
	GetSystemTime(&hour,&mini,&sec);
	stime[0]='\0';
	ww[0]='\0';
	Fmt(ww,"%s<%d",hour);if(hour<10) strcat(stime,"0");strcat(stime,ww);strcat(stime,"_");
	ww[0]='\0';Fmt(ww,"%s<%d",mini);if(mini<10) strcat(stime,"0");strcat(stime,ww);strcat(stime,"_");
	ww[0]='\0';Fmt(ww,"%s<%d",sec); if(sec<10)  strcat(stime,"0");strcat(stime,ww);
}


int main (int argc, char *argv[])
{
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;
	if ((PnlHandle = LoadPanel (0, "EPH5275_PNP.uir", PANEL)) < 0)
		return -1;
	timerid = NewAsyncTimer (TB, -1, 0, asynCB, asyncCBData);
	DisplayPanel (PnlHandle);

	st_load=LoadDLL();
	fp_rec=fopen("record.bin","wb");
	RunUserInterface (); 
	
	
	DiscardAsyncTimer (timerid); 
	DiscardPanel (PnlHandle);
	return 0;
}
int CVICALLBACK config_usb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int flag;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetSystemDate(&month,&day,&year);
			GetSystemTime(&hour,&mini,&sec);
			s1[0]='\0';strcat(s1,"\n\n");
			s2[0]='\0';Fmt(s2,"%s<%d",year);strcat(s1,s2);strcat(s1,"年");
			s2[0]='\0';Fmt(s2,"%s<%d",month);strcat(s1,s2);strcat(s1,"月");
			s2[0]='\0';Fmt(s2,"%s<%d",day);strcat(s1,s2);strcat(s1,"日");
			s2[0]='\0';Fmt(s2,"%s<%d",hour);strcat(s1,s2);strcat(s1,"点");
			s2[0]='\0';Fmt(s2,"%s<%d",mini);strcat(s1,s2);strcat(s1,"分");
			s2[0]='\0';Fmt(s2,"%s<%d",sec);strcat(s1,s2);strcat(s1,"秒");
			SetCtrlVal (panel, PANEL_TEXTBOX,s1);

			GetCtrlVal(panel,PANEL_TOGGLEBUTTON_5,&flag);
			if(flag==1)
			{
				if(st_load!=0)
				{
					s2[0]='\0';Fmt(s2,"%s<%d",st_load);
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nDLL Load error, error code: ");
					SetCtrlVal (panel, PANEL_TEXTBOX,s2);
				}
				else
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nDLL Load Success!!");
				}
				int numDevs;
				FT_STATUS st_list = ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
				if (st_list == FT_OK) 
				{
					s2[0]='\0';Fmt(s2,"%s<%d",numDevs);
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nUSB Devices num : ");
					SetCtrlVal (panel, PANEL_TEXTBOX,s2); 
				}
				else
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nList Error!!");
					printf("%d", st_list);
				}
				char Buffer[64];
				int usb_port_num=0;
				for(int i=0;i<numDevs;i++)
				{
					DWORD devIndex = i;
					Buffer[0]='\0'; 
					st_list = ListDevices((PVOID)devIndex,Buffer,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
					if (st_list == FT_OK) 
					{
						s2[0]='\0';Fmt(s2,"%s<%d",i);
						SetCtrlVal (panel, PANEL_TEXTBOX,"\n\n Device ID : ");
						SetCtrlVal (panel, PANEL_TEXTBOX,s2);
						SetCtrlVal (panel, PANEL_TEXTBOX," Device Name  : ");
						SetCtrlVal (panel, PANEL_TEXTBOX,Buffer);
						if(Buffer[0]=='F' && Buffer[1]=='T' && (Buffer[2]=='4' || Buffer[2]=='7' || Buffer[2]=='8') )  
						{
							usb_port_num=i;SetCtrlVal (panel, PANEL_TEXTBOX,"\n This Device is FT232H !!!");
						}
					}
					else
					{
						s2[0]='\0';Fmt(s2,"%s<%d",i);
						SetCtrlVal (panel, PANEL_TEXTBOX,"\n\n Device ID : ");
						SetCtrlVal (panel, PANEL_TEXTBOX,s2);
						SetCtrlVal (panel, PANEL_TEXTBOX," Get Name Failed!!");
						//printf("%d", st_list);
					}
				}
				
				
				FT_STATUS st_open = open(usb_port_num);
				if(st_open == FT_OK) 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nOpen SUCCESS!!");
					SetCtrlVal (panel, PANEL_LED,1);
				}
				else 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nOpen Failed!!");
					SetCtrlVal (panel, PANEL_LED,0); 
					printf("%d", st_open);
					SetCtrlVal(panel,PANEL_TOGGLEBUTTON_5,0);
				}
				
				FT_STATUS st_settimeout = SetTimeouts(1,10);
				if(st_settimeout == FT_OK) 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nTimeOut Set SUCCESS!!");
				}
				else 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nTimeOut Set Failed!!");
				}
				FT_STATUS st_setbitmode = SetBitmode(0xff,FT_BITMODE_SYNC_FIFO);
				if(st_setbitmode == FT_OK) 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nBitMode Set SUCCESS!!");
				}
				else 
				{
					SetCtrlVal (panel, PANEL_TEXTBOX,"\nBitMode Set Failed!!");
				}
	            
			}
			else
			{
				freed=1;closed=1;
				FT_STATUS st_close=Close();
				if(st_close == FT_OK) SetCtrlVal (panel, PANEL_TEXTBOX,"\nClose SUCCESS!!");else printf("%d", st_close);
				
				SetCtrlVal (panel, PANEL_LED,0);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK asyn_timerctrl (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int enable=0;
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal(panel,PANEL_TOGGLEBUTTON_7,&enable);
			if(enable==1)
			{
				char sci_name[1000];
				sci_name[0]='\0';
				strcat(sci_name,"SCI\\sci");  
                //strcat(sci_name,"D:\\VirtalMac\\shared\\JLY202105_3381\\sci");  
                //strcat(sci_name,"D:\\VirtalMac\\shared\\X_Rays2021\\sci");
				display_date2();
				display_time2();
				strcat(sci_name,sdate);
				strcat(sci_name,stime);
				strcat(sci_name,".bin");
				fp_sci=fopen(sci_name,"wb");
				on_off=1;
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_INTERVAL, TB);
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 1);
				rx_num=0;
			}
			else
			{
				on_off=0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK asynCB (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{

	int realnum=0;
	unsigned char buff_sci[1000000];
	unsigned char times[100];
	FT_STATUS st_write;
	FT_STATUS st_read;
    switch (event)
    {
		case EVENT_TIMER_TICK:
			
			t_start=clock(); 
			FT232_GetQueueStatus();
			while(dwRxBytes>0)
			{
					st_read = Read(buff_sci,dwRxBytes,&realnum);
					if(st_read!= FT_OK){SetCtrlVal (PnlHandle, PANEL_TEXTBOX, "\nFT232H READ ERROR!!\n"); break;}
					total_bytes=total_bytes+realnum;
					if(realnum>0) fwrite(buff_sci,1,realnum,fp_sci);
					FT232_GetQueueStatus();
					
			}
			s3[0]='\0';
			if(Tcnt%10==9)
			{
				display_date();display_time();
				strcat(s3,"\n");strcat(s3,sdate);
				strcat(s3,"  ");strcat(s3,stime);
				
				strcat(s3, "\nFT232H READ Successful: ");
				s2[0]='\0';Fmt(s2,"%s<%d",total_bytes);strcat(s3,s2);
				strcat(s3," Bytes");
				
				if(total_bytes>0) rx_num++;
				
				strcat(s3," rx_num="); 
				s2[0]='\0';Fmt(s2,"%s<%d",rx_num);strcat(s3,s2);
				
				/*
				if(rx_num>=1000)
				{

						rx_num=0;
						fclose(fp_sci);
						char sci_name[1000];
						sci_name[0]='\0';
						strcat(sci_name,"D:\\VirtalMac\\shared\\JLY202105_3381\\sci"); 
						display_date2();
						display_time2();
						strcat(sci_name,sdate);
						strcat(sci_name,stime);
						strcat(sci_name,".bin");
						fp_sci=fopen(sci_name,"wb");
				}
				*/
				total_bytes=0;
			    t_stop=clock();
				s2[0]='\0';Fmt(s2,"%s<%d",t_stop -t_start);//strcat(s3,"\n interval time:");strcat(s3,s2);strcat(s3," ms");
				strcat(s3,"\n");
				SetCtrlVal (PnlHandle, PANEL_TEXTBOX, s3);
			}
			
			if(Tcnt%10==9) 
			{
				if(on_off==0) 
				{
					SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 0);
					fclose(fp_sci);
				}
			}
			Tcnt++;	
			break;
	}
 return 0;
}

int CVICALLBACK clr_box (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox (panel, PANEL_TEXTBOX, "");
			ResetTextBox (panel, PANEL_TEXTBOX, "");
			break;
	}
	return 0;
}

int CVICALLBACK QuitSoft (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			//if(fp_sci!=NULL) fclose(fp_sci);
				if(fp_rec!=NULL) fclose(fp_rec);
				if(fp_blackbox!=NULL) fclose(fp_blackbox);
			if (closed==0 && g_ftHandle!=NULL) Close();
			
			int free_dll=FreeDLL();
			if(free_dll!=0) 
			{
				SetCtrlVal (panel, PANEL_TEXTBOX,"\nFree DLL SUCCESS!!");
			}
			else 
				printf("\nFree DLL Failed!!");
			QuitUserInterface (0);
			break;
		}
	return 0;
}

int CVICALLBACK check (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char CHAPathNameJS[500];
	FILE  *fp_dc;
	FILE  *fp_fee[8];
	int fee_id=0;
	int fee_num[8];
	int error_num[8];
	char fee_data[30952];
	int pnum=0;
	char data,pdata,ppdata,pppdata;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			CHAPathName[0]='\0';
			int rt=FileSelectPopup ("D:\\VirtalMac\\shared\\JLY202105_3381", "*.*", "*.*", "选择待解析文件",VAL_SELECT_BUTTON, 0, 0, 1, 0, CHAPathName);
			if(rt!=0)
		    {
			   fp_dc = fopen (CHAPathName, "rb");
			   SetCtrlVal (panel, PANEL_TEXTBOX, "\n解析文件：");
			   SetCtrlVal (panel, PANEL_TEXTBOX, CHAPathName);
			   SetCtrlVal (panel, PANEL_TEXTBOX, "\n生成各FEE文件：\n");
			   for(int i_fee=0;i_fee<8;i_fee++)
			   {
				   fee_num[i_fee]=0;
				   error_num[i_fee]=0;
				   for(int i=0;i<500;i++)
				   {
					   if(CHAPathName[i]=='.')
					   {
						   CHAPathNameJS[i+0]='_';
						   CHAPathNameJS[i+1]='f';
						   CHAPathNameJS[i+2]='e';
						   CHAPathNameJS[i+3]='e';
						   CHAPathNameJS[i+4]='0'+i_fee;
						   CHAPathNameJS[i+5]='.';
						   CHAPathNameJS[i+6]='b';
						   CHAPathNameJS[i+7]='i';
						   CHAPathNameJS[i+8]='n';
						   CHAPathNameJS[i+9]='\0';
						   break;
					   }
					   CHAPathNameJS[i]=CHAPathName[i];
				   }
				   fp_fee[i_fee]=fopen(CHAPathNameJS, "wb");
				   SetCtrlVal (panel, PANEL_TEXTBOX,CHAPathNameJS);
				   SetCtrlVal (panel, PANEL_TEXTBOX,"\n");
			   }
			   while(! feof(fp_dc))
			   {
				   int num=fread(fee_data,1,30952,fp_dc);
				   
				   pnum++;
				   if(num!=30952) 
				   {
					   if(num!=0) SetCtrlVal (panel, PANEL_TEXTBOX,"包长不正确！！！\n");
					   char TStr[500];
					   TStr[0]='\0';
					   sprintf(TStr,"pnum=%d\n",pnum);
					   SetCtrlVal (panel, PANEL_TEXTBOX,TStr);
					   break;
				   }
				   
				   fee_id=fee_data[12]/16;
				   if(fee_id<0 || fee_id>8) 
				   {
					   char TStr[500];
					   TStr[0]='\0';
					   sprintf(TStr,"fee_id=%d\n",fee_id);
					   SetCtrlVal (panel, PANEL_TEXTBOX,TStr);
				   }
				   else
				   {
					   fwrite(fee_data,1,30952,fp_fee[fee_id]);
					   fee_num[fee_id]++;
				   }
				   
			   }
			   fclose(fp_dc);
			   for(int i_fee=0;i_fee<8;i_fee++)
			   {
				   char TStr[500];
				   TStr[0]='\0';
				   sprintf(TStr,"fee%d 包计数: %d",i_fee,fee_num[i_fee]);  
				   SetCtrlVal (panel, PANEL_TEXTBOX,TStr);
				   if(i_fee%2==0)  SetCtrlVal (panel, PANEL_TEXTBOX,"  ");
				   else			   SetCtrlVal (panel, PANEL_TEXTBOX,"\n");
				   fclose(fp_fee[i_fee]);
			   }
			   SetCtrlVal (panel, PANEL_TEXTBOX,"解包完成！！！\n");
			}
			else
			{
				SetCtrlVal (panel, PANEL_TEXTBOX, "未选择解析文件或文件为空！！！\n");
			}
			break;
	}
	return 0;
}
