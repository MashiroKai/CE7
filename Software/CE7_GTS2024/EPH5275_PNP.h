/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_CLR_GAMMA                  2       /* control type: command, callback function: clr_gamma */
#define  PANEL_CLR_NEUTRON                3       /* control type: command, callback function: clr_neutron */
#define  PANEL_CLR                        4       /* control type: command, callback function: clr_box */
#define  PANEL_QUIT                       5       /* control type: command, callback function: QuitSoft */
#define  PANEL_TEXTBOX_USB                6       /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX_4                  7       /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX_5                  8       /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX_2                  9       /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX                    10      /* control type: textBox, callback function: (none) */
#define  PANEL_LED                        11      /* control type: LED, callback function: (none) */
#define  PANEL_LED_USB                    12      /* control type: LED, callback function: (none) */
#define  PANEL_LED_SCI                    13      /* control type: LED, callback function: (none) */
#define  PANEL_LED_HK                     14      /* control type: LED, callback function: (none) */
#define  PANEL_LED_COM                    15      /* control type: LED, callback function: (none) */
#define  PANEL_VA_I_2                     16      /* control type: numeric, callback function: (none) */
#define  PANEL_POWER_COM                  17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_COM                18      /* control type: numeric, callback function: (none) */
#define  PANEL_TEXTMSG_42                 19      /* control type: textMsg, callback function: (none) */
#define  PANEL_GAMMA                      20      /* control type: graph, callback function: (none) */
#define  PANEL_NEUTRON                    21      /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH_1                    22      /* control type: graph, callback function: (none) */
#define  PANEL_VA_I_1                     23      /* control type: numeric, callback function: (none) */
#define  PANEL_UART_SET                   24      /* control type: command, callback function: UART_CONFIG */
#define  PANEL_SendFile                   25      /* control type: command, callback function: MutiSendCmdFile */
#define  PANEL_SelFile                    26      /* control type: command, callback function: MutiSelCmdFile */
#define  PANEL_VA_I_0                     27      /* control type: numeric, callback function: (none) */
#define  PANEL_ERROR2                     28      /* control type: command, callback function: ERROR_CMD2 */
#define  PANEL_MODE2                      29      /* control type: command, callback function: CONFIGURE */
#define  PANEL_MODE1                      30      /* control type: command, callback function: OBSERVE */
#define  PANEL_GTRESET                    31      /* control type: command, callback function: RESET */
#define  PANEL_CLEAR_CONFIG               32      /* control type: command, callback function: CLEAR */
#define  PANEL_GETPWCOM                   33      /* control type: command, callback function: GETPWCOMNUM */
#define  PANEL_GETCOM                     34      /* control type: command, callback function: GETCOMNUM */
#define  PANEL_ERROR1                     35      /* control type: command, callback function: ERROR_CMD1 */
#define  PANEL_SHUTDOWN                   36      /* control type: command, callback function: ShutDown */
#define  PANEL_SAVEFILE_INTERVAL          37      /* control type: numeric, callback function: (none) */
#define  PANEL_OBSERTIME                  38      /* control type: numeric, callback function: (none) */
#define  PANEL_PF_ENABLE                  39      /* control type: textButton, callback function: pf_en */
#define  PANEL_POWER_ST                   40      /* control type: textButton, callback function: Powerctrl */
#define  PANEL_USB_MODE                   41      /* control type: textButton, callback function: (none) */
#define  PANEL_TOGGLEBUTTON_7             42      /* control type: textButton, callback function: asyn_timerctrl */
#define  PANEL_TEXTMSG_48                 43      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  44      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_45                 45      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_56                 46      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_55                 47      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_54                 48      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_53                 49      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_52                 50      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_47                 51      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_46                 52      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_44                 53      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    54      /* control type: textMsg, callback function: (none) */
#define  PANEL_PICTURE                    55      /* control type: picture, callback function: (none) */
#define  PANEL_DECORATION_6               56      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_4               57      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_43                 58      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_5               59      /* control type: deco, callback function: (none) */
#define  PANEL_TABLE                      60      /* control type: table, callback function: (none) */
#define  PANEL_TIMECODE                   61      /* control type: textButton, callback function: TimeCode_Set */
#define  PANEL_HK_REQUEST                 62      /* control type: textButton, callback function: Hk_Request */
#define  PANEL_FEEIOUT                    63      /* control type: string, callback function: (none) */
#define  PANEL_GTIOUT                     64      /* control type: string, callback function: (none) */
#define  PANEL_SAVE_MINUS                 65      /* control type: command, callback function: SETSAVE_MINUS */
#define  PANEL_SAVE_PULS                  66      /* control type: command, callback function: SETSAVE_PULS */
#define  PANEL_POWER_COM_MINUS            67      /* control type: command, callback function: SETPOWERCOM_MINUS */
#define  PANEL_POWER_COM_PULS             68      /* control type: command, callback function: SETPOWERCOM_PULS */
#define  PANEL_OBSERBE_MINUS              69      /* control type: command, callback function: SETOBS_MINUS */
#define  PANEL_OBSERVE_PULS               70      /* control type: command, callback function: SETOBS_PULS */
#define  PANEL_COM_MINUS                  71      /* control type: command, callback function: SETCOM_MINUS */
#define  PANEL_COM_PULS                   72      /* control type: command, callback function: SETCOM_PULS */
#define  PANEL_AUTO                       73      /* control type: textButton, callback function: AUTOMODE */
#define  PANEL_EX_NAME                    74      /* control type: string, callback function: (none) */
#define  PANEL_STOPBIT                    75      /* control type: ring, callback function: (none) */
#define  PANEL_CHECKBIT                   76      /* control type: ring, callback function: (none) */
#define  PANEL_BYTEINTERVAL               77      /* control type: numeric, callback function: (none) */
#define  PANEL_BAUD                       78      /* control type: numeric, callback function: (none) */
#define  PANEL_GTIMAX                     79      /* control type: string, callback function: (none) */
#define  PANEL_GTVMAX                     80      /* control type: string, callback function: (none) */
#define  PANEL_FEEVMAX                    81      /* control type: string, callback function: (none) */
#define  PANEL_FEEIMAX                    82      /* control type: string, callback function: (none) */
#define  PANEL_hkOnce                     83      /* control type: command, callback function: hkOnce */


     /* Control Arrays: */

#define  CTRLARRAY                        1
#define  CTRLARRAY_2                      2
#define  CTRLARRAY_3                      3

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK asyn_timerctrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AUTOMODE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CLEAR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK clr_box(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK clr_gamma(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK clr_neutron(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CONFIGURE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ERROR_CMD1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ERROR_CMD2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GETCOMNUM(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GETPWCOMNUM(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Hk_Request(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK hkOnce(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MutiSelCmdFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MutiSendCmdFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OBSERVE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pf_en(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Powerctrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitSoft(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RESET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETCOM_MINUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETCOM_PULS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETOBS_MINUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETOBS_PULS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETPOWERCOM_MINUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETPOWERCOM_PULS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETSAVE_MINUS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETSAVE_PULS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ShutDown(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimeCode_Set(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UART_CONFIG(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
