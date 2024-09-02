/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2012. All Rights Reserved.          */
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
#define  PANEL_TCP_DISCONNECT             2       /* control type: command, callback function: disconnect */
#define  PANEL_TCP_CONNECT                3       /* control type: command, callback function: connect */
#define  PANEL_INIT_CARDS                 4       /* control type: command, callback function: Init_All_Cards */
#define  PANEL_QUITBUTTON                 5       /* control type: command, callback function: QuitCallback */
#define  PANEL_STRING_ADDRESS             6       /* control type: string, callback function: (none) */
#define  PANEL_TEXTBOX                    7       /* control type: textBox, callback function: (none) */
#define  PANEL_BINARYSWITCH_4             8       /* control type: binary, callback function: (none) */
#define  PANEL_BINARYSWITCH_3             9       /* control type: binary, callback function: (none) */
#define  PANEL_RTADDR                     10      /* control type: ring, callback function: (none) */
#define  PANEL_SUBADDR                    11      /* control type: ring, callback function: (none) */
#define  PANEL_SEND1553BCMD               12      /* control type: command, callback function: send1553bcmd */
#define  PANEL_SET1553BWORD               13      /* control type: command, callback function: set1553word */
#define  PANEL_RECORD                     14      /* control type: radioButton, callback function: record */
#define  PANEL_LED_TCP_3                  15      /* control type: LED, callback function: (none) */
#define  PANEL_LED_TCP_2                  16      /* control type: LED, callback function: (none) */
#define  PANEL_LED_TCP                    17      /* control type: LED, callback function: (none) */
#define  PANEL_BINARYSWITCH               18      /* control type: binary, callback function: (none) */
#define  PANEL_BINARYSWITCH_5             19      /* control type: binary, callback function: (none) */
#define  PANEL_BINARYSWITCH_2             20      /* control type: binary, callback function: (none) */
#define  PANEL_RING_2                     21      /* control type: ring, callback function: (none) */
#define  PANEL_RING                       22      /* control type: ring, callback function: (none) */
#define  PANEL_COMMANDBUTTON              23      /* control type: command, callback function: (none) */
#define  PANEL_TOGGLEBUTTON               24      /* control type: textButton, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK disconnect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Init_All_Cards(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK record(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK send1553bcmd(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK set1553word(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
