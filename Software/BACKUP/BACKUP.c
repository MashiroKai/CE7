#include <rs232.h>
#include "asynctmr.h"
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "BACKUP.h"

static int PnlHandle;
static int timerid; 
static int timerid1;
static int timerid2;
static void *asyncCBData = "这是一个异步定时器示例！5555555555";
static void *asyncCBData1 = "这是一个异步定时器示例！5555555555"; 
static void *asyncCBData2 = "这是一个异步定时器示例！5555555555";    
#define TB 1 
#define TB1 0.1
int COM_port0;
long baud; 

int year,month,day;
int hour,mini,sec;
int flag;
int Tcnt=0; 
int num=0; 

unsigned char sdate[100];
unsigned char stime[100];

unsigned char s1[100000]="";
unsigned char s2[100000]="";

int com_state0=-1; 
int CVICALLBACK asynCB (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2); 
int CVICALLBACK asynCB1 (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2); 
int CVICALLBACK asynCB2 (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int Writefile(unsigned char* at) ;


FILE  *fp_blackbox2;

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
		return -1;	/* out of memory */
	if ((PnlHandle = LoadPanel (0, "BACKUP.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (PnlHandle);
	timerid = NewAsyncTimer (TB, -1, 0, asynCB, asyncCBData);
	timerid1 = NewAsyncTimer (TB, -1, 0, asynCB1, asyncCBData1);
	timerid2 = NewAsyncTimer (TB1, -1, 0, asynCB2, asyncCBData2);
	RunUserInterface (); 
	DiscardAsyncTimer (timerid);
	DiscardAsyncTimer (timerid1); 
	DiscardAsyncTimer (timerid2); 
	DiscardPanel (PnlHandle);
	return 0;
}

int CVICALLBACK CONFIG_UART_USB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	char hk_name[1000];
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetSystemDate(&month,&day,&year);
			GetSystemTime(&hour,&mini,&sec);
			s1[0]='\0';
			s2[0]='\0';Fmt(s2,"%s<%d",year);strcat(s1,s2);strcat(s1,"年");
			s2[0]='\0';Fmt(s2,"%s<%d",month);strcat(s1,s2);strcat(s1,"月");
			s2[0]='\0';Fmt(s2,"%s<%d",day);strcat(s1,s2);strcat(s1,"日");
			s2[0]='\0';Fmt(s2,"%s<%d",hour);strcat(s1,s2);strcat(s1,"点");
			s2[0]='\0';Fmt(s2,"%s<%d",mini);strcat(s1,s2);strcat(s1,"分");
			s2[0]='\0';Fmt(s2,"%s<%d",sec);strcat(s1,s2);strcat(s1,"秒");strcat(s1,"\n");
			SetCtrlVal (panel, PANEL_TEXTBOX,s1);
			
			//GetCtrlVal(panel,PANEL_RING_COM,&baud);
			baud=115200;
			GetCtrlVal(panel,PANEL_NUMERIC_13,&COM_port0);
			com_state0=OpenComConfig(COM_port0,"",baud,1,8,2,64000,64000); 
			
			GetCtrlVal(panel,PANEL_TOGGLEBUTTON,&flag);
			
			if( (com_state0>=0)&&(flag==1) )
			{
				SetCtrlVal(panel,PANEL_LED,1);
				SetCtrlVal (PnlHandle, PANEL_TEXTBOX, "COM已连接！\n");
				SetComTime(COM_port0,0.001);
				//SetCtrlAttribute(PnlHandle,PANEL_RING_COM,ATTR_DIMMED,1); 
				SetCtrlAttribute(PnlHandle,PANEL_NUMERIC_13,ATTR_DIMMED,1);
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 1); 
				SetAsyncTimerAttribute (timerid1, ASYNC_ATTR_ENABLED, 0); 
				SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 1); 
				
				
				hk_name[0]='\0';
				strcat(hk_name,"HK\\指令记录");
				display_date2();
				display_time2();
				strcat(hk_name,sdate);
				strcat(hk_name,stime);
				strcat(hk_name,".txt");
				fp_blackbox2=fopen(hk_name,"w");
			}
			else
			{
				CloseCom (COM_port0);
				SetCtrlVal(panel,PANEL_LED,0);
				SetCtrlVal (PnlHandle, PANEL_TEXTBOX, "COM已关闭！\n");
				//SetCtrlAttribute(PnlHandle,PANEL_RING_COM,ATTR_DIMMED,0); 
				SetCtrlAttribute(PnlHandle,PANEL_NUMERIC_13,ATTR_DIMMED,0);
				SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 0);
				SetAsyncTimerAttribute (timerid1, ASYNC_ATTR_ENABLED, 0); 
				SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 0);
				
				fclose(fp_blackbox2); 
			}
			break;
	}
	return 0;
}

int CVICALLBACK QUIT (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
		switch (event)
		{
		case EVENT_COMMIT:
			//if(fp_sci!=NULL) fclose(fp_sci);
			//if(fp_rec!=NULL) fclose(fp_rec);
			//if(fp_blackbox!=NULL) fclose(fp_blackbox);
			QuitUserInterface (0);
			break;
		}
	return 0;
}

int CVICALLBACK asynCB (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
		case EVENT_TIMER_TICK:
			unsigned char cmd[1];
			cmd[0] = 0xF0;
			ComWrt(COM_port0,cmd,1);
			SetAsyncTimerAttribute (timerid2, ASYNC_ATTR_ENABLED, 1);
			SetAsyncTimerAttribute (timerid, ASYNC_ATTR_ENABLED, 0); 
			break;
	}
 return 0;
}


int CVICALLBACK asynCB1 (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
		case EVENT_TIMER_TICK:
			
			break;
	}
 return 0;
}


int CVICALLBACK asynCB2 (int reserved, int timerId, int event, 
  void *callbackData, int eventData1, int eventData2)
{
	unsigned char buff_st[16384];
	unsigned char cmd[1]; 
	unsigned char tmp[1000];
	unsigned char tmp1[1000];
    switch (event)
    {
		case EVENT_TIMER_TICK:
			Tcnt++;	
			
			if(com_state0>=0 && Tcnt%10==0)  
			{
			cmd[0] = 0xFF;
			ComWrt(COM_port0,cmd,1);
			}

			if(com_state0>=0 && Tcnt%10==9)
			{
				num=ComRd(COM_port0,buff_st,3000);
				Writefile(buff_st);
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
					SetCtrlVal (PnlHandle, PANEL_TEXTBOX,tmp);
		  	    	SetCtrlVal (PnlHandle, PANEL_TEXTBOX,"\n");
				}

			}

			break;
	}
 return 0;
}

int Writefile(unsigned char* at) 
{  
	display_date();
	display_time();
	fprintf(fp_blackbox2,"\n\n%s %s",sdate,stime); 
	for(int i=0;i<num;i++) {
		if((at[i]==0xEB)&&(at[i+1]==0x90))
		{
			if(at[i+2]==0x04)
			{
			
			switch(at[i+3])
			{
				case 0x63:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  平台信息  ");
				fprintf(fp_blackbox2," %02X",at[i]); break;	
				case 0x87:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  数据注入  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;
				case 0x94:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  预关机  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;
				case 0x13:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  时间码  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;
				case 0x25:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  状态轮询  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;
				case 0xf0:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  正确应答  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;	
				case 0xff:
				fprintf(fp_blackbox2,"\nCMP: ");
				fprintf(fp_blackbox2,"  错误应答  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;	
				case 0x55:
				fprintf(fp_blackbox2,"\nFPGA: ");
				fprintf(fp_blackbox2,"  指令监测  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				SetCtrlVal (PnlHandle, PANEL_STP, at[i+4]);
				SetCtrlVal (PnlHandle, PANEL_CHE, at[i+5]);
				SetCtrlVal (PnlHandle, PANEL_ECE, at[i+6]);
				SetCtrlVal (PnlHandle, PANEL_RIE, at[i+7]);
				SetCtrlVal (PnlHandle, PANEL_IIE, at[i+8]);
				SetCtrlVal (PnlHandle, PANEL_BIE, at[i+9]);
				SetCtrlVal (PnlHandle, PANEL_LCE, at[i+10]);
				SetCtrlVal (PnlHandle, PANEL_CSE, at[i+11]);
				SetCtrlVal (PnlHandle, PANEL_ITE, at[i+12]);
				SetCtrlVal (PnlHandle, PANEL_PIE, at[i+13]);
				SetCtrlVal (PnlHandle, PANEL_HCE, at[i+14]);
				break; 
				default : 
				fprintf(fp_blackbox2,"  未知指令  ");
				fprintf(fp_blackbox2," %02X",at[i]);
				break;
			}
			
			}
			else if(at[i+2]==0x00)
			{
			fprintf(fp_blackbox2,"\nFEE: ");
			fprintf(fp_blackbox2,"  状态应答  "); 
			fprintf(fp_blackbox2," %02X",at[i]);
			}
		}
		else
		{
			fprintf(fp_blackbox2," %02X",at[i]);	
		}
	}

return 0; 				
	
}

int CVICALLBACK CLEAR (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
 			ResetTextBox (panel, PANEL_TEXTBOX, ""); 
			break;
	}
	return 0;
}
