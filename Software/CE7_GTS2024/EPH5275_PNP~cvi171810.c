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

double TB = 0.1;
double Obserinterval = 0 ;
static void *asyncCBData = "这是一个异步定时器示例！5555555555";
static void *ObserCBData = "这是一个异步定时器！55555555555555";
static void *SCICBData = "这是一个异步定时器！555555555555"; 
static void *PROCESSCBData = "这是一个异步定时器!5555"; 
static int timerid;
static int timerid2;
static int timerid3;
static int timerid4; 
static int PnlHandle;

int CVICALLBACK asynCB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK ObserCB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2); 
int CVICALLBACK SCICB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2); 
int CVICALLBACK PROCESSCB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2); 
	int bytes10ms =0;
	int data =0;
double yc(unsigned short int temp);
Point MakePoint (int x, int y);
void configmode(void);
void observemode(void);
void sendcmdfile(void);
void SD (void);
void ERROR1 (void);
void ERROR2 (void); 
void TimeCode(void);
void Hk (void);
void Pf (void);
void GTRESET (void);
void wr_fpbox1 (void);
void mutisend(void);
int COM_port0;
long baud;
int com_state0=-1; 
int byte_num=0;
	int rd_flag = 0; 
char  file_receive_s[50]={"RECORD\\状态数据"};

int year,month,day;
int hour,mini,sec;
int pre_sec=-1;
int SCICNT = 0; 
time_t t_start,t_stop;
int Tcnt=0;
int total_bytes=0; 
int save_interval;
int on_off=0;
float VA_I[8];
int I[8]={0}; 
unsigned char run_mod;
int p_num=0; 
int error_num=0;
int error_num2=0;
int rx_num=0;
int sci_rx_num=0;
int sci_rx_num_temp=0;
int observe=0;
int realnum=0;
int enable=0; 
int auto_st=0;

int SelCmdFileHandle;   
char SelCmdFilePathName[300];
char sci_name[1000]; 
int SendCmdFileEn=0;
int MutiSendCmdEn[100];
unsigned  int SendCmdInfoNum;
unsigned  int MutiSendCmdNum[100];
unsigned char TmpData[8000];
unsigned char FILECMD[100][80000];
unsigned char (*PTRFILE)[100];


unsigned char s1[100000]="";
unsigned char s2[100000]="";
unsigned char s3[100000]=""; 
unsigned char s4[100000]="";
unsigned char sx[100000]="";
unsigned char sdate[100];
unsigned char stime[100];

unsigned char buff[1000000]; //遥测


unsigned char  cmd1[8]  ={0x55,0xaa,0x5a,0xa5,1,0,0,0};
unsigned char  cmd2[8]  ={0x55,0xaa,0x5a,0xa5,2,0,0,0};
unsigned char  cmd3[8]  ={0x55,0xaa,0x5a,0xa5,3,0,0,0};
unsigned char  cmd4[8]  ={0x55,0xaa,0x5a,0xa5,4,0,0,0};
unsigned char  cmd5[8]  ={0x55,0xaa,0x5a,0xa5,5,0,0,0};
unsigned char  cmd6[8]  ={0x55,0xaa,0x5a,0xa5,6,0,0,0};
unsigned char  cmd7[8]  ={0x55,0xaa,0x5a,0xa5,7,0,0,0};
unsigned char  cmd8[1024]={0x55,0xaa,0x5a,0xa5,8,0,0,0};
unsigned char  cmd9[8]  ={0x55,0xaa,0x5a,0xa5,9,0,0,0};
unsigned char  cmd10[8] ={0x55,0xaa,0x5a,0xa5,10,0,0,0};



int request1 =0;
int request2 =0;
int request3 =0;
int request4 =0;
int request5 =0;
int request6 =0;
int request7 =0;
int request8 =0;
int request9 =0;
int request10=0;

unsigned int cmd_timer=0;
unsigned char cmds_head[8];
unsigned char cmds_content[1024];
unsigned char platform = 0;
int cmdlist_enable=0;
unsigned int neutron[128];
unsigned int gamma[64];


FILE  *fp_rec;
FILE  *fp_blackbox2;
FILE  *fp_blackbox1;
FILE  *fp_sci; 


FILE  *fp_cmd;
FILE  *fp_cmdjs;


float current0[86400];
float current1[86400];
float current2[86400];
int index_s=0;


unsigned char s[100000]="";

int st_load=-1;
int freed=0;
int closed=0;

FT_HANDLE g_ftHandle;  
HMODULE g_hmodule;
DWORD dwRxBytes;
char **fileList,**temp;
int numSelected = 0;

int HK_FLAG = -1;
int PF_FLAG = -1;
int TIME_FLAG = -1;
int SHUT_FLAG = -1;
int ERR1_FLAG = -1;
int ERR2_FLAG = -1;
int SCMF_FLAG = -1;
int Muti_FLAG = 0; 
int Muti_Num =0;
int OBSE_FLAG = -1;
int CONFIC_FLAG = -1;
int GT_FLAG = -1;
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
void config_usb(int usbmode)
{
				if(st_load!=0)
				{
					s2[0]='\0';Fmt(s2,"%s<%d",st_load);
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nDLL Load error, error code: ");
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,s2);
				}
				else
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nDLL Load Success!!");
				}
				int numDevs;
				FT_STATUS st_list = ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
				if (st_list == FT_OK) 
				{
					s2[0]='\0';Fmt(s2,"%s<%d",numDevs);
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nUSB Devices num : ");
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,s2); 
				}
				else
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nList Error!!");
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
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\n\n Device ID : ");
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,s2);
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB," Device Name  : ");
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,Buffer);
						if(Buffer[0]=='F' && Buffer[1]=='T' && (Buffer[2]=='4' || Buffer[2]=='7' || Buffer[2]=='8') )  
						{
							usb_port_num=i;SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\n This Device is FT232H !!!");
						}
					}
					else
					{
						s2[0]='\0';Fmt(s2,"%s<%d",i);
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\n\n Device ID : ");
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,s2);
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB," Get Name Failed!!");
						//printf("%d", st_list);
					}
				}
				
				
				FT_STATUS st_open = open(usb_port_num);
				if(st_open == FT_OK) 
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nOpen SUCCESS!!");
					SetCtrlVal (PnlHandle, PANEL_LED_USB,1);
				}
				else 
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nOpen Failed!!");
					SetCtrlVal (PnlHandle, PANEL_LED_USB,0); 
					printf("%d", st_open);
					SetCtrlVal(PnlHandle,PANEL_TOGGLEBUTTON_7,0);
				}
				
				FT_STATUS st_settimeout = SetTimeouts(0.1,0.1);
				if(st_settimeout == FT_OK) 
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nTimeOut Set SUCCESS!!");
				}
				else 
				{
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nTimeOut Set Failed!!");
				}
				if(usbmode == 1)
				{
					FT_STATUS st_setbitmode = SetBitmode(0xff,FT_BITMODE_SYNC_FIFO);
					if(st_setbitmode == FT_OK) 
					{
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nBitMode Set SUCCESS!!");
					}
					else 
					{
						SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB,"\nBitMode Set Failed!!");
					}
				}
}
double yc(unsigned short int temp)
{
		double Res = temp * 10.0 / (4096 - temp);
		double func = 1/(log(Res/11.09)/3500 + 1/273.15) - 273.15;
		return    func   ;
}
int display_fee(unsigned char* at)
{
	
	unsigned char error_info[1000];
	int error=0;
	error_info[0]='\0';
	strcat(error_info,"通信故障：");
	
	int PkgType=at[18]%16;

	int base = 8;
	double tmp1,tmp2,tmp3;
	
	
	//checksum
	unsigned  int sum=0;
	unsigned  int pktSum = at[266]*256 + at[267];
	for(int i=8;i<266;i++)
	{sum = sum + at[i];}
	sum = sum&0xFFFF;
	if(sum != pktSum)
			printf("CheckSum Error!!! sum %x, pktSum %x\n",sum, pktSum);  
	unsigned short int YC_NUM = at[4]*256+at[5];	
	unsigned int TIME_SEC = at[base+0]*16777216 + at[base+1]*65536 + at[base+2]*256 + at[base+3];
	unsigned short int TIME_MilSEC = at[base+4]*256 + at[base+5]; 
	unsigned short int STATUS = at[base+6]*256 + at[base+7];
	unsigned char COL_PAR = at[base+8];
	unsigned char RX_NUM = at[base+9];
	unsigned char ERR_NUM = (at[base+10]>>2)&0x3F;
	unsigned short int SCI_NUM = (at[base+10]&0x3)*256 +at[base+11];
	unsigned char CHIP1_CUR = at[base+12];
	unsigned char CHIP2_CUR = at[base+13];  
	unsigned char CHIP3_CUR = at[base+14];  
	unsigned char CHIP1_PAR = at[base+15];  
	unsigned char CHIP2_PAR = at[base+16];  
	unsigned char CHIP3_PAR = at[base+17];
	
	unsigned short int CeBr3 = at[base+18]*256 + at[base+19]; 
	unsigned short int PMT1 = at[base+20]*256 + at[base+21]; 
	unsigned short int PMT2 = at[base+22]*256 + at[base+23];
	unsigned short int PMT3 = at[base+24]*256 + at[base+25];     
	unsigned short int PMT4 = at[base+26]*256 + at[base+27];    
	unsigned short int PMT5 = at[base+28]*256 + at[base+28];     
	unsigned short int PMT6 = at[base+30]*256 + at[base+31];     
	unsigned short int PMT7 = at[base+32]*256 + at[base+33];     
	unsigned short int PMT8 = at[base+34]*256 + at[base+35];
	
	unsigned short int FEE1 = at[base+36]*256 + at[base+37];     
	unsigned short int FEE2 = at[base+38]*256 + at[base+39];     
	unsigned short int FEE3 = at[base+40]*256 + at[base+41];     
	unsigned short int FEE4 = at[base+42]*256 + at[base+43];  
	
	unsigned char ZZGMFHC = at[base+44];
	unsigned char GMCJYS = at[base+45];
	unsigned char ZZCJYS1 = at[base+46];
	unsigned char ZZCJYS2 = at[base+47];
	
	unsigned short int DUAL_DLY1 = at[base+48]*16+at[base+49]/16;
	unsigned short int DUAL_DLY2 = (at[base+49]&0x0f)*256+at[base+50];
	unsigned short int DUAL_DLY3 = at[base+51]*16+at[base+52]/16; 
	unsigned short int DUAL_DLY4 = (at[base+52]&0x0f)*256+at[base+53];
	
	unsigned char GMCFXZ = at[base+54];
	unsigned char ZZCFSN1 = at[base+55];
	unsigned char ZZCFSN2 = at[base+56];
	unsigned short int GMDSJS =at[base+57]*256+at[base+58];
	double Temp_Cebr3 = yc(CeBr3); 
	double Temp_PMT1 = yc(PMT1); 
	double Temp_PMT2 = yc(PMT2);
	double Temp_PMT3 = yc(PMT3);     
	double Temp_PMT4 = yc(PMT4);    
	double Temp_PMT5 = yc(PMT5);     
	double Temp_PMT6 = yc(PMT6);     
	double Temp_PMT7 = yc(PMT7);     
	double Temp_PMT8 = yc(PMT8);
	
	double Temp_FEE1 = yc(FEE1);     
	double Temp_FEE2 = yc(FEE2);     
	double Temp_FEE3 = yc(FEE3);     
	double Temp_FEE4 = yc(FEE4); 
	
	for(int i=0;i<128;i++)neutron[i]=neutron[i]+at[base+59+i];
	for(int i=0;i<64;i++)gamma[i]=gamma[i]+at[base+59+128+i];
	
	
	//时间码
	//SetCtrlVal(PnlHandle,PANEL_TIME_SEC,TIME_SEC);  
	//SetCtrlVal(PnlHandle,PANEL_TIME_MilSEC,TIME_MilSEC); 
	//SetTableCellVal (PnlHandle, PANEL_TABLE, MakePoint (2, 1), TIME_SEC);
	//SetTableCellVal (PnlHandle, PANEL_TABLE, MakePoint (4, 1), TIME_MilSEC); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 1), ATTR_CTRL_VAL, TIME_SEC);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 1), ATTR_CTRL_VAL, TIME_MilSEC);
	//遥测应答包计数
	//SetCtrlVal(PnlHandle,PANEL_YC_NUM,YC_NUM); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 3), ATTR_CTRL_VAL, YC_NUM);
	//工作状态
	//SetCtrlVal(PnlHandle,PANEL_STATUS,STATUS);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 1), ATTR_CTRL_VAL, STATUS); 
	//采集配置参数
	//SetCtrlVal(PnlHandle,PANEL_COL_PAR,COL_PAR);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 2), ATTR_CTRL_VAL, COL_PAR); 
	//接收指令计数
	//SetCtrlVal(PnlHandle,PANEL_RX_NUM,RX_NUM);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 2), ATTR_CTRL_VAL, RX_NUM); 
	//错误计数
	//SetCtrlVal(PnlHandle,PANEL_ERR_NUM,ERR_NUM); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 2), ATTR_CTRL_VAL, ERR_NUM); 
	//科学数据计数
	//SetCtrlVal(PnlHandle,PANEL_SCI_NUM,SCI_NUM);
	//SetTableCellVal (PnlHandle, PANEL_TABLE, MakePoint (2, 3), SCI_NUM); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 3), ATTR_CTRL_VAL, SCI_NUM);
	//VA1电流
	tmp1 =  CHIP1_CUR*1.0;
//	SetCtrlVal(PnlHandle,PANEL_CHIP1_CUR,tmp1);  
	//VA2电流
	tmp2 =  CHIP2_CUR*1.0;
//	SetCtrlVal(PnlHandle,PANEL_CHIP2_CUR,tmp2); 
	//VA3电流
	tmp3 =  CHIP3_CUR*1.0;
//	SetCtrlVal(PnlHandle,PANEL_CHIP3_CUR,tmp3); 
	//VA限流参数
	//SetCtrlVal(PnlHandle,PANEL_CHIP1_PAR,CHIP1_PAR);  
	//SetCtrlVal(PnlHandle,PANEL_CHIP2_PAR,CHIP2_PAR);  
	//SetCtrlVal(PnlHandle,PANEL_CHIP3_PAR,CHIP3_PAR);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 5), ATTR_CTRL_VAL, CHIP1_PAR); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 5), ATTR_CTRL_VAL, CHIP2_PAR); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 5), ATTR_CTRL_VAL, CHIP3_PAR); 
	//Cebr3温度
	//SetCtrlVal(PnlHandle,PANEL_Temp_Cebr3,Temp_Cebr3); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 6), ATTR_CTRL_VAL, Temp_Cebr3); 
	//塑闪温度
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT1,Temp_PMT1);    
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT2,Temp_PMT2); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT3,Temp_PMT3); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT4,Temp_PMT4); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT5,Temp_PMT5); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT6,Temp_PMT6); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT7,Temp_PMT7); 
	//SetCtrlVal(PnlHandle,PANEL_Temp_PMT8,Temp_PMT8); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 6), ATTR_CTRL_VAL, Temp_PMT1); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 6), ATTR_CTRL_VAL, Temp_PMT2); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 7), ATTR_CTRL_VAL, Temp_PMT3); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 7), ATTR_CTRL_VAL, Temp_PMT4); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 7), ATTR_CTRL_VAL, Temp_PMT5); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 8), ATTR_CTRL_VAL, Temp_PMT6); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 8), ATTR_CTRL_VAL, Temp_PMT7); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 8), ATTR_CTRL_VAL, Temp_PMT8); 
	//FEE温度
	//SetCtrlVal(PnlHandle,PANEL_Temp_FEE1,Temp_FEE1);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 3), ATTR_CTRL_VAL, Temp_FEE1); 
	//VA温度
	//SetCtrlVal(PnlHandle,PANEL_Temp_FEE2,Temp_FEE2);  
	//SetCtrlVal(PnlHandle,PANEL_Temp_FEE3,Temp_FEE3);  
	//SetCtrlVal(PnlHandle,PANEL_Temp_FEE4,Temp_FEE4); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 4), ATTR_CTRL_VAL, Temp_FEE2); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 4), ATTR_CTRL_VAL, Temp_FEE3); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 4), ATTR_CTRL_VAL, Temp_FEE4); 
	//中子伽马符合窗
	//SetCtrlVal(PnlHandle,PANEL_ZZGMFHC,ZZGMFHC);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 11), ATTR_CTRL_VAL, ZZGMFHC); 
	//伽马采集延时
	//SetCtrlVal(PnlHandle,PANEL_GMCJYS,GMCJYS);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 9), ATTR_CTRL_VAL, GMCJYS); 
	//中子采集延时
	//SetCtrlVal(PnlHandle,PANEL_ZZCJYS1,ZZCJYS1);
	//SetCtrlVal(PnlHandle,PANEL_ZZCJYS2,ZZCJYS2);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 9), ATTR_CTRL_VAL, ZZCJYS1); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 9), ATTR_CTRL_VAL, ZZCJYS2); 
	//双脉冲延时
	//SetCtrlVal(PnlHandle,PANEL_DUAL_DLY1,DUAL_DLY1);
	//SetCtrlVal(PnlHandle,PANEL_DUAL_DLY2,DUAL_DLY2);
	//SetCtrlVal(PnlHandle,PANEL_DUAL_DLY3,DUAL_DLY3);
	//SetCtrlVal(PnlHandle,PANEL_DUAL_DLY4,DUAL_DLY4);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 10), ATTR_CTRL_VAL, DUAL_DLY1); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 10), ATTR_CTRL_VAL, DUAL_DLY2); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 10), ATTR_CTRL_VAL, DUAL_DLY3); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 11), ATTR_CTRL_VAL, DUAL_DLY4); 
	//伽马触发选择
	//SetCtrlVal(PnlHandle,PANEL_GMCFXZ,GMCFXZ);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 12), ATTR_CTRL_VAL, GMCFXZ); 
	//中子触发使能
	//SetCtrlVal(PnlHandle,PANEL_ZZCFSN1,ZZCFSN1);
	//SetCtrlVal(PnlHandle,PANEL_ZZCFSN2,ZZCFSN2);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (2, 12), ATTR_CTRL_VAL, ZZCFSN1); 
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (4, 12), ATTR_CTRL_VAL, ZZCFSN2); 
	//伽马丢失计数
	//SetCtrlVal(PnlHandle,PANEL_GMDSJS,GMDSJS);
	SetTableCellAttribute (PnlHandle, PANEL_TABLE, MakePoint (6, 11), ATTR_CTRL_VAL, GMDSJS); 
	//draw current plot
	int limited_array=30;
	int cnt_va0=0; 
	int cnt_va1=0; 
	int cnt_va2=0; 

	DeleteGraphPlot (PnlHandle,PANEL_GRAPH_1,-1,VAL_IMMEDIATE_DRAW);

	if(index_s<limited_array) 
		{
			current0[index_s]=tmp1;
		 	cnt_va0=index_s+1;
			
			current1[index_s]=tmp2;
		 	cnt_va1=index_s+1;
			
			current2[index_s]=tmp3;
		 	cnt_va2=index_s+1;
		}
	else
		{
			for(int i=0;i<limited_array-1;i++) 
				current0[i]=current0[i+1];
			for(int i=0;i<limited_array-1;i++) 
				current1[i]=current1[i+1];
			for(int i=0;i<limited_array-1;i++) 
				current2[i]=current2[i+1];
			
			current0[limited_array-1]=tmp1;
			cnt_va0=limited_array;
			
			current1[limited_array-1]=tmp2;
			cnt_va1=limited_array;
			
			current2[limited_array-1]=tmp3;
			cnt_va2=limited_array;
		}
	PlotY (PnlHandle,PANEL_GRAPH_1,current0,cnt_va0,VAL_FLOAT,VAL_FAT_LINE,VAL_SIMPLE_DOT,VAL_SOLID,1,VAL_RED);

	//printf("%d, %d\n", cnt_va0, index_s);
	
	
	PlotY (PnlHandle,PANEL_GRAPH_1,current1,cnt_va1,VAL_FLOAT,VAL_FAT_LINE,VAL_SIMPLE_DOT,VAL_SOLID,1,VAL_GREEN);
	
	PlotY (PnlHandle,PANEL_GRAPH_1,current2,cnt_va2,VAL_FLOAT,VAL_FAT_LINE,VAL_SIMPLE_DOT,VAL_SOLID,1,VAL_GRAY);
	
	SetCtrlVal(PnlHandle,PANEL_VA_I_0,tmp1);   
	SetCtrlVal(PnlHandle,PANEL_VA_I_1,tmp2);   
	SetCtrlVal(PnlHandle,PANEL_VA_I_2,tmp3);   
	index_s++;
//////////////////////END DISPLAY VA CURRENT

/////////////////////DISPLAY NEUTRON

DeleteGraphPlot (PnlHandle,PANEL_NEUTRON,-1,VAL_IMMEDIATE_DRAW);
PlotY (PnlHandle, PANEL_NEUTRON, neutron, 128, VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_RED);

////////////////////DISPLAY GAMMA
DeleteGraphPlot (PnlHandle,PANEL_GAMMA,-1,VAL_IMMEDIATE_DRAW);	
PlotY (PnlHandle, PANEL_GAMMA, gamma, 64, VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_SIMPLE_DOT, VAL_SOLID, 1, VAL_RED);    	
/////////////////////RECORD ENGGINE PARAMETER

	display_date();
	display_time();
	fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
	fprintf(fp_blackbox2,"\nRX: ");for(int k=0;k<268;k++)fprintf(fp_blackbox2," %02X",at[k]);
	
	
	//EXCEL
	fprintf(fp_blackbox1,"%s,",sdate);
	fprintf(fp_blackbox1,"%s,",stime);
	fprintf(fp_blackbox1,"%X,",TIME_SEC);  
	fprintf(fp_blackbox1,"%X,",TIME_MilSEC);  
	fprintf(fp_blackbox1,"%X,",STATUS);  
	fprintf(fp_blackbox1,"%X,",COL_PAR);  
	fprintf(fp_blackbox1,"%d,",RX_NUM);  
	fprintf(fp_blackbox1,"%d,",ERR_NUM);  
	fprintf(fp_blackbox1,"%d,",SCI_NUM);
	fprintf(fp_blackbox1,"%d,",YC_NUM);
	fprintf(fp_blackbox1,"%f,",Temp_FEE1);   //FPGA
	fprintf(fp_blackbox1,"%f,",Temp_FEE2);  //VA1
	fprintf(fp_blackbox1,"%f,",Temp_FEE3); 	//VA2
	fprintf(fp_blackbox1,"%f,",Temp_FEE4);	//VA3
	
	fprintf(fp_blackbox1,"%X,",CHIP1_PAR);  
	fprintf(fp_blackbox1,"%X,",CHIP2_PAR); 
	fprintf(fp_blackbox1,"%X,",CHIP3_PAR);
	
	fprintf(fp_blackbox1,"%f,",Temp_Cebr3);
	fprintf(fp_blackbox1,"%f,",Temp_PMT1);  
	fprintf(fp_blackbox1,"%f,",Temp_PMT2); 
	fprintf(fp_blackbox1,"%f,",Temp_PMT3);  
	fprintf(fp_blackbox1,"%f,",Temp_PMT4); 
	fprintf(fp_blackbox1,"%f,",Temp_PMT5); 
	fprintf(fp_blackbox1,"%f,",Temp_PMT6);  
	fprintf(fp_blackbox1,"%f,",Temp_PMT7); 
	fprintf(fp_blackbox1,"%f,",Temp_PMT8); 
	
	fprintf(fp_blackbox1,"%d,",CHIP1_CUR);  
	fprintf(fp_blackbox1,"%d,",CHIP2_CUR);  
	fprintf(fp_blackbox1,"%d,",CHIP3_CUR);
	
	fprintf(fp_blackbox1,"%d,",GMCJYS);
	fprintf(fp_blackbox1,"%d,",ZZCJYS1); 
    fprintf(fp_blackbox1,"%d,",ZZCJYS2);
	
	fprintf(fp_blackbox1,"%d,",DUAL_DLY1);
	fprintf(fp_blackbox1,"%d,",DUAL_DLY2);
	fprintf(fp_blackbox1,"%d,",DUAL_DLY3);
	fprintf(fp_blackbox1,"%d,",DUAL_DLY4);
	
	fprintf(fp_blackbox1,"%X,",GMCFXZ);
	fprintf(fp_blackbox1,"%X,",GMDSJS);
	fprintf(fp_blackbox1,"%X,",ZZCFSN1);
	fprintf(fp_blackbox1,"%X,",ZZCFSN2);
	fprintf(fp_blackbox1,"%X,",ZZGMFHC);
	fprintf(fp_blackbox1,"\n");  

	return 0;
}

int main (int argc, char *argv[])
{
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;
	if ((PnlHandle = LoadPanel (0, "EPH5275_PNP.uir", PANEL)) < 0)
		return -1;
	timerid = NewAsyncTimer (TB, -1, 0, asynCB, asyncCBData);
	timerid2 = NewAsyncTimer (Obserinterval, -1, 0, ObserCB, ObserCBData);
	timerid3 = NewAsyncTimer (TB, -1, 0, SCICB, SCICBData);
	DisplayPanel (PnlHandle);
	st_load=LoadDLL();

	
	RunUserInterface (); 
	
	
	
	DiscardAsyncTimer (timerid);
	DiscardAsyncTimer (timerid2); 
	DiscardAsyncTimer (timerid3);
	DiscardPanel (PnlHandle);
	return 0;
}


int CVICALLBACK asyn_timerctrl (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int usbmode=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute (PnlHandle,PANEL_TOGGLEBUTTON_7 , ATTR_CTRL_VAL,&enable );
			//GetCtrlVal(panel,PANEL_TOGGLEBUTTON_7,&enable);
			
			GetCtrlAttribute(PnlHandle,PANEL_SAVEFILE_INTERVAL, ATTR_CTRL_VAL,&save_interval);
			GetCtrlAttribute(PnlHandle,PANEL_PF_ENABLE, ATTR_CTRL_VAL,&PF_FLAG);
			GetCtrlAttribute(PnlHandle,PANEL_HK_REQUEST, ATTR_CTRL_VAL,&HK_FLAG);
		   	///////////////////USB CONFIG////////////////////
			GetSystemDate(&month,&day,&year);
			GetSystemTime(&hour,&mini,&sec);
			s1[0]='\0';
			s2[0]='\0';Fmt(s2,"%s<%d",year);strcat(s1,s2);strcat(s1,"年");
			s2[0]='\0';Fmt(s2,"%s<%d",month);strcat(s1,s2);strcat(s1,"月");
			s2[0]='\0';Fmt(s2,"%s<%d",day);strcat(s1,s2);strcat(s1,"日");
			s2[0]='\0';Fmt(s2,"%s<%d",hour);strcat(s1,s2);strcat(s1,"点");
			s2[0]='\0';Fmt(s2,"%s<%d",mini);strcat(s1,s2);strcat(s1,"分");
			s2[0]='\0';Fmt(s2,"%s<%d",sec);strcat(s1,s2);strcat(s1,"秒");strcat(s1,"\n");
			SetCtrlAttribute (panel, PANEL_TEXTBOX, ATTR_CTRL_VAL,s1);
			
			//GetCtrlVal(panel,PANEL_RING_COM,&baud);
			baud=921600;
			GetCtrlAttribute(panel,PANEL_NUMERIC_COM,ATTR_CTRL_VAL,&COM_port0);
			com_state0=OpenComConfig(COM_port0,"",baud,1,8,2,64000,64000); 
			SetComTime(COM_port0,0.001); 
			GetCtrlAttribute(panel,PANEL_USB_MODE,ATTR_CTRL_VAL,&usbmode);
			if( (com_state0>=0)&&(enable==1) )
			{
				SetCtrlAttribute(panel,PANEL_LED_COM,ATTR_CTRL_VAL,1);
				SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX,ATTR_CTRL_VAL, "COM已连接！\n");
				
				//SetCtrlAttribute(PnlHandle,PANEL_RING_COM,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_NUMERIC_COM,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_COM_MINUS,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_COM_PULS,ATTR_DIMMED,1); 
				on_off=1;
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_INTERVAL, TB);
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 1);
			}
			///////////////////USB CONFIG////////////////////
			else
			{

					//SetCtrlAttribute(PnlHandle,PANEL_RING_COM,ATTR_DIMMED,0); 
					//SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 0);
					if(observe)CONFIC_FLAG=1;
					SetCtrlAttribute(PnlHandle,PANEL_NUMERIC_COM,ATTR_DIMMED,0);
					SetCtrlAttribute(PnlHandle,PANEL_COM_MINUS,ATTR_DIMMED,0); 
					SetCtrlAttribute(PnlHandle,PANEL_COM_PULS,ATTR_DIMMED,0);
					on_off=0;
			}

			if(enable==1)
			{
				SetCtrlAttribute(PnlHandle,PANEL_SAVEFILE_INTERVAL,ATTR_DIMMED,1);
				char hk_name[1000];
				hk_name[0]='\0';
				strcat(hk_name,"HK\\非周期性指令记录");
				display_date2();
				display_time2();
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".txt");
				fp_blackbox2=fopen(hk_name,"w");
				hk_name[0]='\0';
				strcat(hk_name,"HK\\工程参数");
				display_date2();
				display_time2();
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".csv");
				fp_blackbox1=fopen(hk_name,"w");
				wr_fpbox1 ();
				
				hk_name[0]='\0';
				strcat(hk_name,"HK\\Binary_log");
				display_date2();
				display_time2();
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".bin");
				fp_rec=fopen(hk_name,"wb");
				
				Tcnt=0;
			
				for(int i=0;i<8;i++)VA_I[i]=0.0;
				p_num=0;
				error_num=0;
				error_num2=0;
				
				index_s=0;
				rx_num=0;
				SetCtrlAttribute(PnlHandle,PANEL_USB_MODE,ATTR_DIMMED,1);
				config_usb(usbmode);
				
			} 
			else
			{   
				SetCtrlAttribute(PnlHandle,PANEL_SAVEFILE_INTERVAL,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_USB_MODE,ATTR_DIMMED,0);
				freed=1;closed=1;
				FT_STATUS st_close=Close();
				if(st_close == FT_OK) SetCtrlAttribute (panel, PANEL_TEXTBOX_USB,ATTR_CTRL_VAL,"\nClose SUCCESS!!");else printf("%d", st_close);
				SetCtrlAttribute (panel, PANEL_LED_USB,ATTR_CTRL_VAL,0);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK asynCB (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{
	
	int num=0;
	unsigned char buff_st[16384];
	int rx_num_T=0;
	int en;
	unsigned char a[268];
    switch (event)
    {
		case EVENT_TIMER_TICK:
			GetCtrlAttribute(PnlHandle,PANEL_TOGGLEBUTTON_7, ATTR_CTRL_VAL,&en);
			if( en==0 && Tcnt%10==2&&observe==0)  
			{		CloseCom (COM_port0);
					fclose(fp_blackbox1);
					fclose(fp_blackbox2);     
					fclose(fp_rec);
					//SetCtrlVal (PnlHandle, PANEL_LED_HK, 0);
					SetCtrlAttribute (PnlHandle, PANEL_LED_HK, ATTR_CTRL_VAL,0 );
					//SetCtrlVal(PnlHandle,PANEL_LED_COM,0);
					SetCtrlAttribute (PnlHandle, PANEL_LED_COM, ATTR_CTRL_VAL,0 ); 
					//SetCtrlVal (PnlHandle, PANEL_TEXTBOX, "COM已关闭！\n");
					SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX, ATTR_CTRL_VAL,"COM已关闭！\n" ); 
					SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED,0);
			}
			else  Tcnt++;
			if(com_state0>=0 && Tcnt%10==4&&PF_FLAG == 1) Pf();
			if(com_state0>=0 && Tcnt%10==5&&HK_FLAG == 1) Hk();
			if(com_state0>=0 && Tcnt%10==6)
			{
				if(SCMF_FLAG==1) sendcmdfile();
				if(ERR1_FLAG==1) ERROR1(); 
				if(ERR2_FLAG==1) ERROR2();
				if(TIME_FLAG==1) TimeCode();
				if(SHUT_FLAG==1) SD(); 
				if(OBSE_FLAG==1) observemode();
				if(CONFIC_FLAG==1) configmode();
				if(GT_FLAG ==1) GTRESET();
			}
			if(com_state0>=0 && Tcnt%10==6)
			{
				if(Muti_FLAG>0) mutisend();
			}
			
			
			if(com_state0>=0 && Tcnt%10==9)
			{
				num=ComRd(COM_port0,buff_st,1000);
				fwrite(buff_st,1,num,fp_rec);
				unsigned char tmp[1000];
				unsigned char tmp1[1000];
	 			tmp[0] = '\0';
				tmp1[0] = '\0';
				Fmt(tmp1,"%s<%d",num);
				display_date();display_time();
				strcat(tmp,sdate);
				strcat(tmp,stime);
				strcat(tmp,"  Receive");  
				strcat(tmp,tmp1);
				strcat(tmp,"Bytes");
				if(num > 0)
				{
					SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX,ATTR_CTRL_VAL,tmp);
		  	    	SetCtrlVal (PnlHandle, PANEL_TEXTBOX,"\n");
				}
				
				s1[0]='\0';
				if(num == 268)		  //head-8 data-268 check-2
				{
					for(int i=0;i<num;i++)
					a[i]=buff_st[i];
					display_fee(a);
					SetCtrlAttribute (PnlHandle, PANEL_LED_HK,ATTR_CTRL_VAL, 1);
				}
				else  SetCtrlAttribute (PnlHandle, PANEL_LED_HK,ATTR_CTRL_VAL, 0);

			}
			
			if(Tcnt%10==0) 
			{
				rx_num++;
				if(rx_num>=3600)
				{
					rx_num=0;
					fclose(fp_blackbox2);
					fclose(fp_rec);
					fclose(fp_blackbox1); 
					char hk_name[1000]; 
					hk_name[0]='\0';
					strcat(hk_name,"HK\\非周期性指令记录");
					display_date2();
					display_time2();
					strcat(hk_name,sdate);
					strcat(hk_name,stime);
					strcat(hk_name,".txt");
					fp_blackbox2=fopen(hk_name,"w");
					
					hk_name[0]='\0';
				strcat(hk_name,"HK\\工程参数");
				display_date2();
				display_time2();
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".csv");
				fp_blackbox1=fopen(hk_name,"w");
				wr_fpbox1();
				hk_name[0]='\0';
				strcat(hk_name,"HK\\Binary_log");
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".bin");
				fp_rec=fopen(hk_name,"wb");
				}
			}
				
			break;
	}
 return 0;
}

void wr_fpbox1 (void)
{
	if(fp_blackbox1!=NULL)
	{			
		fprintf(fp_blackbox1,"日期,时间,时间码-秒,时间码-毫秒,工作状态,采集配置参数,接收指令计数,错误指令计数,科学数据包计数,遥测包计数,FEE温度,VA1温度,VA2温度,VA3温度,VA1限流参数,VA2限流参数,VA3限流参数,内部温度测点CeBr3,内部温度测点PMT1,内部温度测点PMT2,内部温度测点PMT3,内部温度测点PMT4,内部温度测点PMT5,内部温度测点PMT6,内部温度测点PMT7,内部温度测点PMT8,VA1电流,VA2电流,VA3电流,伽马采集延时,中子采集延时1,中子采集延时2,双脉冲延迟1,双脉冲延迟2,双脉冲延迟3,双脉冲延迟4,伽马触发选择,伽马丢失计数,中子触发使能1,中子触发使能2,中子伽马符合窗,\n");
	}
	else  wr_fpbox1; 
}

int CVICALLBACK clr_box (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox (panel, PANEL_TEXTBOX, "");
			ResetTextBox (panel, PANEL_TEXTBOX_USB, "");
			ResetTextBox (panel, PANEL_TEXTBOX_2, "");
			ResetTextBox (panel, PANEL_TEXTBOX_4, ""); 
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
			if (closed==0 && g_ftHandle!=NULL) Close();
			
			int free_dll=FreeDLL();
			if(free_dll!=0) 
			{
				SetCtrlAttribute (panel, PANEL_TEXTBOX_USB,ATTR_CTRL_VAL,"\nFree DLL SUCCESS!!");
			}
			else 
				printf("\nFree DLL Failed!!");
			QuitUserInterface (0);
			break;
		}
	return 0;
}


int CVICALLBACK SelCmdFile(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	//注意关闭文件、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
	int ReFlag; 
	ssize_t size;  
	int m,n;
	unsigned char a[2][2];
	a[0][0]=1;
	a[0][1]=8;
	a[1][0]=3;
	a[1][1]=9;
	unsigned char (*b)[2];
	b = a;
	m=*(b[0]+1);
	//n=*b[1]+1;
	//*(b[1]+1) = 11;
	n =  *(b[1]+1)   ;
	unsigned short int tmp1;
	switch (event)
	{
		case EVENT_COMMIT:
			SendCmdFileEn = 0;
			 
		//	GetCtrlVal(panel, control, &ReFlag);
		//	if(ReFlag)
		//	{
			    //FileSelectPopup ("", "*.bin", "","",VAL_OK_BUTTON, 0, 0, 1, 0, SelCmdFilePathName);
			//	FileSelectPopupEx ("", "*.bin", "", "", VAL_OK_BUTTON, 0, 1, SelCmdFilePathName);

					
		//	}
		//	else
			//	SendCmdFileEn = 0;   
			break;
	}
	return 0;
}
int CVICALLBACK MutiSelCmdFile (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	ssize_t size; 
	unsigned char s1[100000]="";	
	switch (event)
	{
		case EVENT_COMMIT:
		int selectionStatus;
		int tempnum;
   	 	selectionStatus =MultiFileSelectPopupEx ("", "*.bin", "", "Select Config file(s)", 0, 1, &tempnum, &fileList);
		//PTRFILE = FILECMD;
    	if(selectionStatus == VAL_EXISTING_FILE_SELECTED)
    	{
       		temp = fileList;
			s1[0]='\0';
       		for(int i = numSelected ;i<numSelected + tempnum; i++) 
       		{
			   	GetFileInfo(*temp,&size);
			  	SelCmdFileHandle = OpenFile (*temp, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
				if(SelCmdFileHandle<0||size%18 != 0) 
				{
					MessagePopup("选择数据注入文件", "文件错误");
					if(SelCmdFileHandle>0)CloseFile(SelCmdFileHandle);
				}
				else
				{
					
					FileToArray(*temp, FILECMD+8000*i, VAL_CHAR, size, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_BINARY);
					MutiSendCmdNum[i] = size/18;
					MutiSendCmdEn[i] = 1;
					//SendCmdInfoNum = size/18;
					//SendCmdFileEn = 1;
					//Fmt(s,"%s<%d",num);
					strcat(s1,*temp);
					strcat(s1,"\n");
					CloseFile(SelCmdFileHandle); 
				}
            	free(*temp);
           		*temp=NULL;
            	temp++;
       		}
			numSelected = numSelected + tempnum;
			strcat(s,s1);
			SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_5,ATTR_CTRL_VAL,s1);
       		free(fileList);
    	}

			break;
	}
	return 0;
}

int CVICALLBACK MutiSendCmdFile (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{   
	unsigned char st[100000]=""; 
	switch (event)
	{
		case EVENT_COMMIT:
			Muti_FLAG = numSelected;
			Muti_Num = 0;
			if(Muti_FLAG){
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st,"\n");
			SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_2,ATTR_CTRL_VAL,st);
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,s);
			}
			break;
	}
	return 0;
}
void mutisend(void)
{
	Muti_FLAG = Muti_FLAG -1;
	int m;
	unsigned char payLoad = 4;
	unsigned char cmdType = 0x87;
	unsigned char len = 17;
	unsigned char sumHigh = 0;
	unsigned char sumLow = 0;
	unsigned int  sum = payLoad + cmdType + len;
	unsigned char cmd[25]={0};
	unsigned char buffer[1000] = {0};
	char aa[5];
	char bb[5000];
	cmd[0] = 0xEB;
	cmd[1] = 0x90;
	cmd[2] = payLoad;
	cmd[3] = cmdType;
	cmd[4] = len;
			if(MutiSendCmdEn[Muti_Num]==1)
			{
				//MessagePopup("数据指令注入", "发送完毕"); 
				for(m=0; m<MutiSendCmdNum[Muti_Num]; m++)
				{
						sum = payLoad + cmdType + len;  
						for(int i=0;i<18;i++)
						{
							cmd[5+i] = *(PTRFILE[Muti_Num]+m*18+i);
							sum = sum + *(PTRFILE[Muti_Num]+m*18+i);  
							
						}
						cmd[23] = (sum>>8) & 0xFF;
						cmd[24] = sum & 0xFF;
					for(int j=0;j<25;j++)	 buffer[j+25*m]= cmd[j];
			    }
				ComWrt(COM_port0,buffer,MutiSendCmdNum[Muti_Num]*25); 
				display_date();
				display_time();
				fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
				fprintf(fp_blackbox2,"\nTX: ");
				for(int i=0;i<5;i++) aa[i]='\0';
				for(int i=0;i<5000;i++) bb[i]='\0';
				
				for(int k=0;k<MutiSendCmdNum[Muti_Num]*25;k++) 
				{ 
					if(k%25 == 0 )strcat (bb,"\n");
				    aa[0]='\0';if(buffer[k]< 16)sprintf(aa," 0%X",buffer[k]);else sprintf(aa," %X",buffer[k]);
				    strcat(bb,aa);
				}
				fprintf(fp_blackbox2," %s",bb);
			}
			Muti_Num = Muti_Num+1;
			if(Muti_Num == numSelected) 
			{
				SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_2,ATTR_CTRL_VAL,"配置完成"); 
			}

}



int CVICALLBACK SendCmdFile(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char st[100000]=""; 
	switch (event)
	{
		case EVENT_COMMIT:
			SCMF_FLAG = 1;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st,"\n"); 
			SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_2,ATTR_CTRL_VAL,st);
			strcat(s,"\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,s);
			break;
	}
	return 0;
}

void sendcmdfile(void)
{
	SCMF_FLAG = -1;
	int m;
	unsigned char payLoad = 4;
	unsigned char cmdType = 0x87;
	unsigned char len = 17;
	unsigned char sumHigh = 0;
	unsigned char sumLow = 0;
	unsigned int  sum = payLoad + cmdType + len;
	unsigned char cmd[25]={0};
	unsigned char buffer[1000] = {0};
	char aa[5];
	char bb[5000];
	cmd[0] = 0xEB;
	cmd[1] = 0x90;
	cmd[2] = payLoad;
	cmd[3] = cmdType;
	cmd[4] = len;
	if(SendCmdFileEn==1)
			{
				//MessagePopup("数据指令注入", "发送完毕"); 
				for(m=0; m<SendCmdInfoNum; m++)
				{
						sum = payLoad + cmdType + len;  
						for(int i=0;i<18;i++)
						{
							cmd[5+i] = TmpData[m*18+i];
							sum = sum + TmpData[m*18+i];  
							
						}
						cmd[23] = (sum>>8) & 0xFF;
						cmd[24] = sum & 0xFF;
					for(int j=0;j<25;j++)	 buffer[j+25*m]= cmd[j];
			    }
				SendCmdFileEn = 0;  
				ComWrt(COM_port0,buffer,SendCmdInfoNum*25); 
				display_date();
				display_time();
				fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
				fprintf(fp_blackbox2,"\nTX: ");
				for(int i=0;i<5;i++) aa[i]='\0';
				for(int i=0;i<5000;i++) bb[i]='\0';
				
				for(int k=0;k<SendCmdInfoNum*25;k++) 
				{ 
					if(k%25 == 0 )strcat (bb,"\n");
				    aa[0]='\0';if(buffer[k]< 16)sprintf(aa," 0%X",buffer[k]);else sprintf(aa," %X",buffer[k]);
				    strcat(bb,aa);
				}
				fprintf(fp_blackbox2," %s",bb);
			}

}


int CVICALLBACK ShutDown (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
			unsigned char st[100000]="";
			unsigned char cmd[9];
			unsigned int tmp = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			 SHUT_FLAG = 1;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 载荷预关机\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st); 
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<9;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			break;
	}
	return 0;
}
void SD (void)
{
			SHUT_FLAG = -1; 
			unsigned char cmd[9];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,9); 

}

int CVICALLBACK ERROR_CMD1 (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
			unsigned char st[100000]="";
			unsigned char cmd[9];
			unsigned int tmp = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			ERR1_FLAG = 1;

			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 错误指令\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st); 
			display_date();
			display_time();
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<9;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			break;
	}
	return 0;
}
void ERROR1 (void)
{
 			ERR1_FLAG = -1;
			unsigned char cmd[9];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,8); 

}
int CVICALLBACK ERROR_CMD2 (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
			unsigned char st[100000]="";
			unsigned char cmd[9];
			unsigned int tmp = 0;
	switch (event)
	{
		case EVENT_COMMIT:
		   ERR2_FLAG = 1;

			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94 + 5;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 错误指令\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);
			display_date();
			display_time();
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<9;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			break;
	}
	return 0;
}
void ERROR2 (void)
{
			ERR2_FLAG = -1; 
			unsigned char cmd[9];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x94;
			cmd[4] = 1;
			cmd[5]= 0x94;
			cmd[6] = 0x94;
			tmp = 4 + 0x94 + 1 + 0x94 + 0x94 + 5;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,9); 
}
int CVICALLBACK TimeCode_Set (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
			unsigned char st[100000]="";
			unsigned char cmd[13];
			unsigned int tmp = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x13;
			cmd[4] = 5;
			cmd[5] = 0;
			cmd[6] = 0;
			cmd[7] = 0x80;
			cmd[8] = 0;
			cmd[9] = 2;
			cmd[10] = 0;
			tmp = 4 + 0x13 + 5 + cmd[5] + cmd[6] + cmd[7] + cmd[8] + cmd[9] + cmd[10];
			cmd[11] = (tmp>>8) & 0xFF;
			cmd[12] = tmp & 0xFF;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 授时指令\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<13;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			TIME_FLAG = 1;
			break;
	}
	return 0;
}
void TimeCode (void)
{
			TIME_FLAG = -1;
			unsigned char cmd[13];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x13;
			cmd[4] = 5;
			cmd[5] = 0;
			cmd[6] = 0;
			cmd[7] = 0x80;
			cmd[8] = 0;
			cmd[9] = 2;
			cmd[10] = 0;
			tmp = 4 + 0x13 + 5 + cmd[5] + cmd[6] + cmd[7] + cmd[8] + cmd[9] + cmd[10];
			cmd[11] = (tmp>>8) & 0xFF;
			cmd[12] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,13); 
}
int CVICALLBACK Hk_Request (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(PnlHandle,PANEL_HK_REQUEST,ATTR_CTRL_VAL,&HK_FLAG);
			break;
	}
	return 0;
}
void Hk (void)
{
			unsigned char st[100000]="";
			unsigned char cmd[9];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 4;
			cmd[3] = 0x25;
			cmd[4] = 1;
			cmd[5]= 0x5A;
			cmd[6] = 0x5A;
			tmp = 4 + 0x25 + 1 + 0x5A + 0x5A;
			cmd[7] = (tmp>>8) & 0xFF;
			cmd[8] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,9); 
			display_date();
			display_time();
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<9;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 轮询指令\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_4,st);
	
}
int CVICALLBACK pf_en (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(PnlHandle,PANEL_PF_ENABLE,ATTR_CTRL_VAL,&PF_FLAG);
			break;
	}
	return 0;
}
void Pf (void)
{
					unsigned char cmd1[53];
					unsigned int tmp = 0;
					unsigned char st[100000]="";
					cmd1[0] = 0xEB;
					cmd1[1] = 0x90;
					cmd1[2] = 4;
					cmd1[3] = 0x63;
					cmd1[4] = 45;
					for(int k=5;k<51;k++) cmd1[k]=platform;
					tmp = 4 + 0x63 + 45 + 46*platform;
					cmd1[51] = (tmp>>8) & 0xFF;
					cmd1[52] = tmp & 0xFF;
					ComWrt(COM_port0,cmd1,53); 
					display_date();
					display_time();
					fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
					fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<53;k++)fprintf(fp_blackbox2," %02X",cmd1[k]);
					strcat(st,sdate);
					strcat(st,stime);
					strcat(st," 平台信息\n");
					SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_4,ATTR_CTRL_VAL,st);
					platform++;	
	
}
int CVICALLBACK OBSERVE (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	unsigned char cmd[25];
	unsigned int tmp = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			data = 0;
			SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,1); 
			SetCtrlAttribute(PnlHandle,PANEL_TAP_CONFIG,ATTR_DIMMED,1);
			SetCtrlAttribute(PnlHandle,PANEL_GTRESET,ATTR_DIMMED,1);
			SetCtrlAttribute(PnlHandle,PANEL_TAP_CONFIG,ATTR_DIMMED,1);
			SetCtrlAttribute(PnlHandle,PANEL_TAP_CONFIG,ATTR_DIMMED,1);
			SetCtrlAttribute(PnlHandle,PANEL_TAP_CONFIG,ATTR_DIMMED,1);
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 0x04;
			cmd[3] = 0x87;
			cmd[4] = 0x11;
			cmd[5] = 0x70;
			cmd[6] = 0x00;
			cmd[7] = 0xFF;  
			cmd[8] = 0x00;  
			cmd[9] = 0x00;  
			cmd[10] = 0x00;  
			cmd[11] = 0x00;  
			cmd[12] = 0x00;
			cmd[13] = 0x00;  
			cmd[14] = 0x00;  
			cmd[15] = 0x00;  
			cmd[16] = 0x00;  
			cmd[17] = 0x00;
			cmd[18] = 0x00;  
			cmd[19] = 0x00;  
			cmd[20] = 0x00;  
			cmd[21] = 0x00;  
			cmd[22] = 0x00;
			tmp = 0x04 + 0x87 + 0x11 + 0x70 + 0xFF;
			cmd[23] = (tmp>>8) & 0xFF;
			cmd[24] = tmp & 0xFF;
			OBSE_FLAG =1;
			SCICNT = 0; 
			observe = 1;
			sci_rx_num=0; 
			sci_rx_num_temp=0;
			///////////////sci////////////
			SetCtrlAttribute(PnlHandle,PANEL_MODE1,ATTR_DIMMED,1);
			SetAsyncTimerAttribute (timerid3, ASYNC_ATTR_ENABLED, 1); 
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

			unsigned char st[100000]="";
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 观测模式\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);
			GetCtrlAttribute (PANEL, PANEL_OBSERTIME,ATTR_CTRL_VAL, &Obserinterval);
			SetCtrlAttribute (PnlHandle, PANEL_LED,ATTR_CTRL_VAL, 1);
			SetCtrlAttribute (PnlHandle, PANEL_LED, ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlAttribute(PnlHandle,PANEL_OBSERTIME,ATTR_DIMMED,1);
			SetCtrlAttribute(PnlHandle,PANEL_MODE2,ATTR_DIMMED,0); 
			if(Obserinterval > 0)
			{
				SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_INTERVAL, Obserinterval);
				SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 1);
			}
			else
			{
				SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 0);
			}
			display_date();
			display_time();
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<25;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
			break;
	}
	return 0;
}
void observemode(void)
{			
			OBSE_FLAG = -1;
 
			unsigned char cmd[25];
			unsigned int tmp = 0;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 0x04;
			cmd[3] = 0x87;
			cmd[4] = 0x11;
			cmd[5] = 0x70;
			cmd[6] = 0x00;
			cmd[7] = 0xFF;  
			cmd[8] = 0x00;  
			cmd[9] = 0x00;  
			cmd[10] = 0x00;  
			cmd[11] = 0x00;  
			cmd[12] = 0x00;
			cmd[13] = 0x00;  
			cmd[14] = 0x00;  
			cmd[15] = 0x00;  
			cmd[16] = 0x00;  
			cmd[17] = 0x00;
			cmd[18] = 0x00;  
			cmd[19] = 0x00;  
			cmd[20] = 0x00;  
			cmd[21] = 0x00;  
			cmd[22] = 0x00;
			tmp = 0x04 + 0x87 + 0x11 + 0x70 + 0xFF;
			cmd[23] = (tmp>>8) & 0xFF;
			cmd[24] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,25); 
}
int CVICALLBACK ObserCB (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{
		unsigned char st[100000]="";
		unsigned char cmd[25];
		unsigned int tmp = 0;
    switch (event)
    {
		case EVENT_TIMER_TICK:
		   CONFIC_FLAG = 1;
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 0x04;
			cmd[3] = 0x87;
			cmd[4] = 0x11;
			cmd[5] = 0x70;
			cmd[6] = 0x00;
			cmd[7] = 0x00;  
			cmd[8] = 0x00;  
			cmd[9] = 0x00;  
			cmd[10] = 0x00;  
			cmd[11] = 0x00;  
			cmd[12] = 0x00;
			cmd[13] = 0x00;  
			cmd[14] = 0x00;  
			cmd[15] = 0x00;  
			cmd[16] = 0x00;  
			cmd[17] = 0x00;
			cmd[18] = 0x00;  
			cmd[19] = 0x00;  
			cmd[20] = 0x00;  
			cmd[21] = 0x00;  
			cmd[22] = 0x00;
			tmp = 0x04 + 0x87 + 0x11 + 0x70;
			cmd[23] = (tmp>>8) & 0xFF;
			cmd[24] = tmp & 0xFF;
			observe = 0;
			rd_flag = 1;
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 配置模式\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);  
			SetCtrlAttribute(PnlHandle,PANEL_MODE2,ATTR_DIMMED,1);
			SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 0);

			display_date();
			display_time();
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<25;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
		   //MessagePopup("已进入配置模式", "观测完成");
		  break;
	}
 return 0;
}

int CVICALLBACK CONFIGURE (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			CONFIC_FLAG = 1;
			rd_flag = 1;
			break;
	}
	return 0;
}

void configmode(void)
{		   	
			unsigned char st[100000]="";  
			CONFIC_FLAG = -1;
			observe = 0;
			data = 0;
			unsigned char cmd[25];
			unsigned int tmp = 0;
			SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,0); 
			SetCtrlAttribute(PnlHandle,PANEL_TAP_CONFIG,ATTR_DIMMED,0);
			cmd[0] = 0xEB;
			cmd[1] = 0x90;
			cmd[2] = 0x04;
			cmd[3] = 0x87;
			cmd[4] = 0x11;
			cmd[5] = 0x70;
			cmd[6] = 0x00;
			cmd[7] = 0x00;  
			cmd[8] = 0x00;  
			cmd[9] = 0x00;  
			cmd[10] = 0x00;  
			cmd[11] = 0x00;  
			cmd[12] = 0x00;
			cmd[13] = 0x00;  
			cmd[14] = 0x00;  
			cmd[15] = 0x00;  
			cmd[16] = 0x00;  
			cmd[17] = 0x00;
			cmd[18] = 0x00;  
			cmd[19] = 0x00;  
			cmd[20] = 0x00;  
			cmd[21] = 0x00;  
			cmd[22] = 0x00;
			tmp = 0x04 + 0x87 + 0x11 + 0x70;
			cmd[23] = (tmp>>8) & 0xFF;
			cmd[24] = tmp & 0xFF;
			ComWrt(COM_port0,cmd,25); 
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 配置模式\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);  
			SetCtrlAttribute(PnlHandle,PANEL_MODE2,ATTR_DIMMED,1);
			SetCtrlAttribute (PnlHandle, PANEL_LED,ATTR_CTRL_VAL, 0);
			SetCtrlAttribute(PnlHandle,PANEL_MODE1,ATTR_DIMMED,0);
			SetCtrlAttribute(PnlHandle,PANEL_OBSERTIME,ATTR_DIMMED,0);
			SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 0);
			fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime);
			fprintf(fp_blackbox2,"\nTX: ");for(int k=0;k<25;k++)fprintf(fp_blackbox2," %02X",cmd[k]);
}



int CVICALLBACK SCICB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{

	unsigned char buff_sci[1000000];
	FT_STATUS st_read;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			t_start=clock();
			FT232_GetQueueStatus();
			while((dwRxBytes>0))
			{
					data = 0;
					st_read = Read(buff_sci,dwRxBytes,&realnum);
					if(st_read!= FT_OK){SetCtrlAttribute (PnlHandle, PANEL_TEXTBOX_USB, ATTR_CTRL_VAL,"\nFT232H READ ERROR!!\n"); break;}
					total_bytes=total_bytes+realnum;
					bytes10ms = bytes10ms +realnum;
					if(realnum>0) fwrite(buff_sci,1,realnum,fp_sci);
					FT232_GetQueueStatus();
					
			}
			if(dwRxBytes <=0) 
			{
				data++;
			}
			SCICNT++;
			if(data>=3&&sci_rx_num_temp >=save_interval)
			{
					data = 0;
					sci_rx_num_temp =0 ;
					fclose(fp_sci);
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
			}
			if(SCICNT%10==0)
			{
				if(total_bytes>0)
				{
					sci_rx_num++;
					sci_rx_num_temp++;
					SetCtrlAttribute (PnlHandle, PANEL_LED_SCI,ATTR_CTRL_VAL, 1);
					display_date();display_time();
					s3[0]='\0'; 
					strcat(s3,"\n");strcat(s3,sdate);
					strcat(s3,"  ");strcat(s3,stime);
					strcat(s3, "\nFT232H READ Successful: ");
					s2[0]='\0';Fmt(s2,"%s<%d",total_bytes);strcat(s3,s2);
					strcat(s3," Bytes");
					strcat(s3," sci_rx_num="); 
					s2[0]='\0';Fmt(s2,"%s<%d",sci_rx_num);strcat(s3,s2);
				
			    	t_stop=clock();
					s2[0]='\0';Fmt(s2,"%s<%d",t_stop -t_start);//strcat(s3,"\n interval time:");strcat(s3,s2);strcat(s3," ms");
					strcat(s3,"\n");
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX_USB, s3);
					
				}
				else  SetCtrlAttribute (PnlHandle, PANEL_LED_SCI,ATTR_CTRL_VAL, 0);
				if(observe == 0 &&total_bytes<=0&&rd_flag >0){
				rd_flag = -1; 				
				fclose(fp_sci);
				SetAsyncTimerAttribute (timerid3, ASYNC_ATTR_ENABLED, 0);
				}
				total_bytes=0; 
			}
			break;
	}
	return 0;
}

int CVICALLBACK clr_neutron (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			for(int i=0;i<128;i++)neutron[i]=0;
			break;
	}
	return 0;
}

int CVICALLBACK clr_gamma (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			for(int i=0;i<64;i++)gamma[i]=0;
			break;
	}
	return 0;
}



int CVICALLBACK RESET (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			  GT_FLAG = 1;
			break;
	}
	return 0;
}
void GTRESET (void)
{
		unsigned char st[100000]=""; 
		GT_FLAG = 0;
		unsigned char cmd[5];
		cmd[0]=0xAA;
		cmd[1]=0xBB;
		cmd[2]=0xCC;
		cmd[3]=0xDD;
		cmd[4]=0xEE;
		ComWrt(COM_port0,cmd,5);
			display_date();
			display_time();
			strcat(st,sdate);
			strcat(st,stime);
			strcat(st," 重置地检\n");
			SetCtrlVal (PnlHandle, PANEL_TEXTBOX_2,st);
}

int CVICALLBACK SETCOM_PULS (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
			
	
	switch (event)
	{
		case EVENT_COMMIT:
			 int com;
			 GetCtrlAttribute (panel, PANEL_NUMERIC_COM,ATTR_CTRL_VAL, &com);
			 com =com + 1;
			 SetCtrlAttribute (panel, PANEL_NUMERIC_COM, ATTR_CTRL_VAL,com);
			break;
	}
	return 0;
}

int CVICALLBACK SETCOM_MINUS (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 int com;
			 GetCtrlAttribute (panel, PANEL_NUMERIC_COM,ATTR_CTRL_VAL, &com);
			 com =com - 1 ;
			 SetCtrlAttribute (panel, PANEL_NUMERIC_COM,ATTR_CTRL_VAL, com);
			break;
	}
	return 0;
}

int CVICALLBACK READ_CONFIG (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK TAP_CONFIG (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK SETOBS_PULS (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 int OBS;
			 GetCtrlAttribute (panel, PANEL_OBSERTIME,ATTR_CTRL_VAL, &OBS);
			 OBS =OBS + 60 ;
			 SetCtrlAttribute (panel, PANEL_OBSERTIME,ATTR_CTRL_VAL, OBS);
			break;
	}
	return 0;
}

int CVICALLBACK SETOBS_MINUS (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	 int OBS;
			 GetCtrlAttribute (panel, PANEL_OBSERTIME,ATTR_CTRL_VAL, &OBS);
			 OBS =OBS - 60 ;
			 SetCtrlAttribute (panel, PANEL_OBSERTIME,ATTR_CTRL_VAL, OBS);
			break;
	}
	return 0;
}

int CVICALLBACK SETSAVE_PULS (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 int SAVE;
			 GetCtrlAttribute (panel, PANEL_SAVEFILE_INTERVAL,ATTR_CTRL_VAL, &SAVE);
			 SAVE =SAVE + 60 ;
			 SetCtrlAttribute (panel, PANEL_SAVEFILE_INTERVAL,ATTR_CTRL_VAL, SAVE);
			break;
	}
	return 0;
}

int CVICALLBACK SETSAVE_MINUS (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 int SAVE;
			 GetCtrlAttribute (panel, PANEL_SAVEFILE_INTERVAL,ATTR_CTRL_VAL, &SAVE);
			 SAVE =SAVE - 60 ;
			 SetCtrlAttribute (panel, PANEL_SAVEFILE_INTERVAL,ATTR_CTRL_VAL, SAVE);
			break;
	}
	return 0;
}

int CVICALLBACK AUTOMODE (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int MODE;
			GetCtrlAttribute (panel, PANEL_AUTO,ATTR_CTRL_VAL, &MODE);
			if(MODE){
				SetCtrlAttribute(PnlHandle,PANEL_GTRESET,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_TIMECODE,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_ERROR1,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_ERROR2,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_SHUTDOWN,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_HK_REQUEST,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_PF_ENABLE,ATTR_DIMMED,1);
				//SetCtrlAttribute(PnlHandle,PANEL_READ_CONFIG,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_MODE1,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_MODE2,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_SelFile,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_SendFile,ATTR_DIMMED,1);
				SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON_2,ATTR_DIMMED,1);
			}
			else{
				SetCtrlAttribute(PnlHandle,PANEL_GTRESET,ATTR_DIMMED,0); 
				SetCtrlAttribute(PnlHandle,PANEL_TIMECODE,ATTR_DIMMED,0); 
				SetCtrlAttribute(PnlHandle,PANEL_ERROR1,ATTR_DIMMED,0); 
				SetCtrlAttribute(PnlHandle,PANEL_ERROR2,ATTR_DIMMED,0); 
				SetCtrlAttribute(PnlHandle,PANEL_SHUTDOWN,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_HK_REQUEST,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_PF_ENABLE,ATTR_DIMMED,0);
				//SetCtrlAttribute(PnlHandle,PANEL_READ_CONFIG,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_MODE1,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_MODE2,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_SelFile,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_SendFile,ATTR_DIMMED,0);
				SetCtrlAttribute(PnlHandle,PANEL_COMMANDBUTTON_2,ATTR_DIMMED,0);
			}
		    
			break;
	}
	return 0;
}

int CVICALLBACK CLEAR (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			s[0]='\0';
			numSelected = 0;
			ResetTextBox (panel, PANEL_TEXTBOX_5, "");
			break;
	}
	return 0;
}
