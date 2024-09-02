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
#define  PANEL_LED                        2       /* control type: LED, callback function: (none) */
#define  PANEL_TEXTBOX                    3       /* control type: textBox, callback function: (none) */
#define  PANEL_LCE                        4       /* control type: numeric, callback function: (none) */
#define  PANEL_STP                        5       /* control type: numeric, callback function: (none) */
#define  PANEL_PIE                        6       /* control type: numeric, callback function: (none) */
#define  PANEL_ITE                        7       /* control type: numeric, callback function: (none) */
#define  PANEL_BIE                        8       /* control type: numeric, callback function: (none) */
#define  PANEL_ECE                        9       /* control type: numeric, callback function: (none) */
#define  PANEL_RIE                        10      /* control type: numeric, callback function: (none) */
#define  PANEL_IIE                        11      /* control type: numeric, callback function: (none) */
#define  PANEL_CSE                        12      /* control type: numeric, callback function: (none) */
#define  PANEL_HCE                        13      /* control type: numeric, callback function: (none) */
#define  PANEL_CHE                        14      /* control type: numeric, callback function: (none) */
#define  PANEL_CLEAR                      15      /* control type: command, callback function: CLEAR */
#define  PANEL_QUIT                       16      /* control type: command, callback function: QUIT */
#define  PANEL_TOGGLEBUTTON               17      /* control type: textButton, callback function: CONFIG_UART_USB */
#define  PANEL_NUMERIC_13                 18      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CLEAR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CONFIG_UART_USB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUIT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
