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
#define  PANEL_TEXTBOX                    2       /* control type: textBox, callback function: (none) */
#define  PANEL_TOGGLEBUTTON_7             3       /* control type: textButton, callback function: asyn_timerctrl */
#define  PANEL_TOGGLEBUTTON_5             4       /* control type: textButton, callback function: config_usb */
#define  PANEL_LED                        5       /* control type: LED, callback function: (none) */
#define  PANEL_CLR_2                      6       /* control type: command, callback function: check */
#define  PANEL_QUIT                       7       /* control type: command, callback function: QuitSoft */
#define  PANEL_CLR                        8       /* control type: command, callback function: clr_box */
#define  PANEL_ERRORCMD_FEE_8             9       /* control type: numeric, callback function: (none) */
#define  PANEL_CMD_RX_FEE_8               10      /* control type: numeric, callback function: (none) */
#define  PANEL_SYNNUM_FEE_8               11      /* control type: numeric, callback function: (none) */
#define  PANEL_SDID_FEE_8                 12      /* control type: numeric, callback function: (none) */
#define  PANEL_RSTNUM_FEE_8               13      /* control type: numeric, callback function: (none) */
#define  PANEL_ST_FEE_8                   14      /* control type: numeric, callback function: (none) */
#define  PANEL_MODE_FEE_8                 15      /* control type: numeric, callback function: (none) */
#define  PANEL_OPEN_NUM_FEE_8             16      /* control type: numeric, callback function: (none) */
#define  PANEL_CLOSE_NUM_FEE_8            17      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_1                    18      /* control type: graph, callback function: (none) */
#define  PANEL_DECORATION_8               19      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION                 20      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_10                 21      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK asyn_timerctrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK check(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK clr_box(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK config_usb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitSoft(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
