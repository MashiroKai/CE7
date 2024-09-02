/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-2010.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       toolbox.h                                                     */
/* Purpose:     provides commonly useful functions                            */
/*                                                                            */
/*============================================================================*/

#ifndef HAVE_CVI_RTE
#define HAVE_CVI_RTE    1
#endif


#ifndef TOOLBOX_HEADER
#define TOOLBOX_HEADER

#include <cvidef.h>
#include <userint.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if HAVE_CVI_RTE
#include <utility.h>
#include <mbsupp.h>
#elif defined(_NI_linux_) && _NI_linux_
#include <mbsupp.h>
#else
#include <mbstring.h>
#include <mbctype.h>
/* Special return value for getmbcp to signal that a single byte   */
/* code page is currently in use.                                  */
#define _MB_CP_SBCS             0
extern int  __CVI_SystemCodePage;   /* same as _getmbcp() */
#define OnMBSystem()    (__CVI_SystemCodePage != _MB_CP_SBCS)
#endif

#if _NI_linux_
#ifndef LLONG_MAX
#define LLONG_MAX   0x7fffffffffffffffLL
#endif
#ifndef LLONG_MIN
#define LLONG_MIN   (-LLONG_MAX-1)
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX  0xffffffffffffffffULL
#endif
#endif /* _NI_linux */

#ifdef __cplusplus
    extern "C" {
#endif                                  
        
int CVIFUNC ToolboxInit(void);

    /********************/
    /* MB STUFF         */
    /********************/

typedef unsigned short int  CmbChar;

#ifndef CVISTART

/* CmbIsSingleC checks if the given 16-bit character fits into a single byte. */
#define CmbIsSingleC(c)             (!((c) & 0xff00))

/* CmbGetNumBytesInChar returns the number of bytes c; */
/* returns 1 if c is a single byte character, 2 if it is a double byte character */
#define CmbGetNumBytesInChar(c)     (CmbIsSingleC(c)    ? 1 : 2)

/* CmbIsLeadByte checks if the given byte is a valid lead byte.      */
/* You should call this only if you know that the byte starts on     */
/* a character boundary (i.e., that it cannot be a trailing byte)    */
#define CmbIsLeadByte(c)            (OnMBSystem() && _ismbblead(c))

/* CmbCharCodeLeadByte returns the lead byte of a double byte character */
#define CmbCharCodeLeadByte(c)      (((c) & 0xFF00) >> 8)

/* CmbCharCodeTrailByte returns the trail byte of a double byte character */
#define CmbCharCodeTrailByte(c)     ((c) & 0x00FF)

/* Byte masks for lead, trail and single byte characters */
#define CmbLeadByteMask             0x0000FF00
#define CmbSingleByteMask           0x000000FF
#define CmbTrailByteMask            CmbSingleByteMask


/* values returned by CmbStrByteType */
#define CMB_SINGLE_BYTE     _MBC_SINGLE
#define CMB_LEAD_BYTE       _MBC_LEAD
#define CMB_TRAIL_BYTE      _MBC_TRAIL
#define CMB_ILLEGAL_BYTE    _MBC_ILLEGAL

/* CmbStrByteType returns the type of the byte at a given offset within */
/* a string, taking into account the bytes before the given byte.       */
/* If you know that p[offset] is not in the middle of a multibyte       */
/* character, you can call CmbIsLeadByte instead.                       */

#define CmbStrByteType(p,offset)  \
                (OnMBSystem() ? _mbsbtype((const unsigned char *)(p),(offset)) : CMB_SINGLE_BYTE)

/* internal auxiliary macros - don't use these directly */
#define __MBBytePtr(s)                   ((unsigned char*)(s))
#define __MBPrevPrevByte(s)             *((unsigned char*)(s)-2)
#define __MBPrevByte(s)                 *((unsigned char*)(s)-1)
#define __MBCurrByte(s)                 *((unsigned char*)(s))
#define __MBNextByte(s)                 *((unsigned char*)(s)+1)

/* Moving character pointers */
/* In these macros, p == pointer to beginning of string           */
/*                  s == character pointer you are trying to move */

/* CmbStrDec changes s by moving it back by one character.                  */
/* This is analogous to --s.  They differ from --s in that                  */
/* they set s to NULL if you mve past the beginning of the string.          */
/* They are implemented identically because you don't get much of an        */
/* optimization in the single byte case once you implement the check for    */
/* moving past the beginning of the string.                                 */
#define CmbStrDec(p, s)         ((s)=CmbStrPrev((p),(s)))

/* CmbStrInc changes s by moving it forward by one character.               */
/* This is analogous to ++s                                                 */
#if _NI_linux_
#define CmbStrInc(s)                ((s)=CmbStrNext(s))
#else
#define CmbStrInc(s)                (OnMBSystem()                   \
                                        ? ((s)=_mbsinc((unsigned char *)s))   \
                                        : ((s)=__MBBytePtr(s)+1) )
//                                      : ++(__MBBytePtr((s))))
#endif

/* CmbStrPrev returns a pointer to the previous character in                */
/* the string. This is analogous to s-1.  They differ from s-1 in that      */
/* they return NULL if you move past the beginning of the string.           */
/* They are implemented identically because you don't get much of an        */
/* optimization in the single byte case once you implement the check for    */
/* moving past the beginning of the string.                                 */
#if _NI_linux_
#define CmbStrPrev(p, s)            (__MBBytePtr(s)-1)
#else
#define CmbStrPrev(p, s)            (_mbsdec((const unsigned char *)(p),(unsigned char *)(s)))
#endif

/* CmbStrNext returns a pointer to the next character in                    */
/* the string.  This is analogous to s+1                                    */
#if _NI_linux_
#define CmbStrNext(s)           (__MBBytePtr(s)+1)
#else
#define CmbStrNext(s)           (OnMBSystem()         \
                                        ? _mbsinc(s)  \
                                        : __MBBytePtr(s)+1)
#endif

/* Getting and setting characters */

/* CmbGetC retrieves the character at the given pointer.            */
/* It is analogous to  *s                                           */
#define CmbGetC(s)          (OnMBSystem()                                               \
                                ? (CmbChar)(CmbIsLeadByte(__MBCurrByte(s))              \
                                        ? ((__MBCurrByte(s) << 8) | __MBNextByte(s))    \
                                        : __MBCurrByte(s))                              \
                                : *(__MBBytePtr(s)))

/* CmbSetC sets the character at the given pointer.                       */
/* They just overwrites any values at the given location, and they do not */
/* properly insert a multibyte character into the middle of a string.     */
/* It is analogous to  *s = c                                             */
#define CmbSetC(s,c)            (OnMBSystem()                                                   \
                                ? (CmbChar)((CmbIsSingleC(c)                                    \
                                        ? (__MBCurrByte(s) = (unsigned char)(c))                \
                                        : ((__MBCurrByte(s) = (unsigned char)((c) >> 8)),       \
                                           (__MBNextByte(s) = (unsigned char)((c) & 0x00ff)))), \
                                     (c))                                                       \
                                : (*(__MBBytePtr(s)) = (unsigned char)(c)))

/* CmbGetCInc first retrieves the character at the given               */
/* position and then advances the pointer to the next character.       */
/* They are analogous to  *s++                                         */
/* CmbGetCNdxInc first retrieves the character at the                  */
/* given position and then advances the index to the next character.   */
/* It is analogous to  s[i++]                                          */
#if _NI_linux_
#define CmbGetCInc(s)       (*(((s)=CmbStrNext(s))-1))
#else
#define CmbGetCInc(s)       (OnMBSystem()                                                   \
                                ? (CmbChar)(CmbIsLeadByte(__MBCurrByte(s))                  \
                                        ? ((__MBBytePtr(s) += 2),                           \
                                           (__MBPrevPrevByte(s) << 8 | __MBPrevByte(s)))    \
                                        : ((__MBBytePtr(s) += 1),                           \
                                           (__MBPrevByte(s))))                              \
                                : *(__MBBytePtr(s))++)
#endif
#define CmbGetCNdxInc(s,i)  (OnMBSystem()                                                           \
                                ? (CmbChar)(CmbIsLeadByte(__MBCurrByte((s)+(i)))                    \
                                        ? ((i) += 2,                                                \
                                           (__MBPrevPrevByte((s)+(i)) << 8 | __MBPrevByte((s)+(i))))\
                                        : ((i) += 1,                                                \
                                           __MBPrevByte((s)+(i))))                                  \
                                : *(__MBBytePtr((s)+(i)++)))

/* CmbSetCInc first sets the character at the given position and               */
/* then advances the pointer to the next character. The newly-set character is */
/* returned.                                                                   */
/* It is analogous to  *s++ = c                                                */
/* CmbSetCNdxInc first sets the character at the given                         */
/* position and then advances the index to the next character. The newly-set   */
/* character is returned.                                                      */
/* It is analogous to  s[i++] = c                                              */
#if _NI_linux_
#define CmbSetCInc(s,c)     (*(((s)=CmbStrNext(s))-1)=(unsigned char *)(c))
#else
#define CmbSetCInc(s,c)     (OnMBSystem()                                                       \
                                ? (CmbChar)((CmbIsSingleC(c)                                    \
                                        ? ((__MBCurrByte(s) = (unsigned char)(c)),              \
                                           (__MBBytePtr(s) += 1))                               \
                                        : ((__MBCurrByte(s) = (unsigned char)((c) >> 8)),       \
                                           (__MBNextByte(s) = (unsigned char)((c) & 0x00ff)),   \
                                           (__MBBytePtr(s) += 2))),                             \
                                     (c))                                                       \
                                : (*(__MBBytePtr(s)) = (unsigned char)(c)),(__MBBytePtr(s) += 1))
#endif
#define CmbSetCNdxInc(s,i,c)    (OnMBSystem()                                                       \
                                ? (CmbChar)((CmbIsSingleC(c)                                        \
                                        ? ((__MBCurrByte((s)+(i)) = (unsigned char)(c)),            \
                                           (__MBBytePtr((s)+(i)++)))                                \
                                        : ((__MBCurrByte((s)+(i)) = (unsigned char)((c) >> 8)),     \
                                           (__MBNextByte((s)+(i)) = (unsigned char)((c) & 0x00ff)), \
                                           (__MBBytePtr((s)+((i)+=2))))),                           \
                                        (c))                                                        \
                                : (*(__MBBytePtr((s)+(i)++)) = (unsigned char)(c)))

/* CmbIncGetC first advances the given pointer to the next               */
/* character and then returns that character.                            */
/* It is analogous to *++s                                               */
#if _NI_linux_
#define CmbIncGetC(s)           (*++(__MBBytePtr(s)))
#else
#define CmbIncGetC(s)        (OnMBSystem()                                              \
                                ? (CmbChar)((s)=_mbsinc(s),                             \
                                        (CmbChar)(CmbIsLeadByte(__MBCurrByte(s))        \
                                        ? ((__MBCurrByte(s) << 8) | __MBNextByte(s))    \
                                        : __MBCurrByte(s)))                             \
                                : *++(__MBBytePtr(s)))
#endif

/* CmbIncSetC first advances the given pointer to the next                 */
/* character and then sets and returns the new character.                  */
/* It is analogous to *++s = c                                             */
#if _NI_linux_
#define CmbIncSetC(s ,c)        ((*++(__MBBytePtr(s)) = (c)))
#else
#define CmbIncSetC(s, c)     (OnMBSystem()                                                      \
                                ? (CmbChar)((s)=_mbsinc(s),(CmbChar)((CmbIsSingleC(c)           \
                                        ? (__MBCurrByte(s) = (unsigned char)(c))                \
                                        : ((__MBCurrByte(s) = (unsigned char)((c) >> 8)),       \
                                           (__MBNextByte(s) = (unsigned char)((c) & 0x00ff)))), \
                                     (c)))                                                      \
                                : (*++(__MBBytePtr(s)) = (c)))
#endif

/* Converting multibyte strings to unicode - requires windows.h */

/* CmbStrToWideStr is a simple wrapper for MultiByteToWideChar in the SDK.  */
/* !!! See docs for MultibyteToWideChar before using this.                  */
#define CmbStrToWideStr(s1, n1, s2, n2) \
    MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (s1), (n1), (s2), (n2))

/* Converting unicode strings to multibyte - requires windows.h */

/* WideStrToCmbStr is a simple wrapper for WideCharToMultiByte in the SDK. */
/* !!! See docs for WideCharToMultiByte                                    */
#define WideStrToCmbStr(s1, n1, s2, n2) \
    WideCharToMultiByte(CP_ACP, 0, (s1), (n1), (s2), (n2), NULL, 0)


long CmbFirstByteOfChar (long c);

/********************************************************
*   You should not typically have to use this. Instead  *
*   use strlen to determine number of bytes.            *
********************************************************/
long CVIFUNC CmbNumChars(const char *s);

long CVIFUNC CmbStrEq(const unsigned char *s1, const unsigned char *s2);
long CVIFUNC CmbStrEqI(const unsigned char *s1, const unsigned char *s2);
long CVIFUNC CmbStrEqN(const unsigned char *s1, const unsigned char *s2, size_t n);
long CVIFUNC CmbStrEqNI(const unsigned char *s1, const unsigned char *s2, size_t n);

/********************************************************
*   Use these to determine lexical order of strings.    *
********************************************************/
#define CmbStrCmp    _mbscmp
#define CmbStrNCmp   _mbsnbcmp
#define CmbStrICmp   _mbsicmp
#define CmbStrNICmp  _mbsnbicmp

#define CmbStrCat     strcat
#define CmbStrNCat   _mbsnbcat
#define CmbStrCpy     strcpy
#define CmbStrNCpy   _mbsnbcpy
#define CmbStrSpn    _mbsspn
#define CmbStrCSpn   _mbscspn
#define CmbStrChr    _mbschr
#define CmbStrRChr   _mbsrchr
#define CmbStrTok    _mbstok
#define CmbStrPBrk   _mbspbrk
#define CmbStrStr    _mbsstr
unsigned char * CVIFUNC CmbStrUpr(unsigned char *s);
unsigned char * CVIFUNC CmbStrLwr(unsigned char *s);

long CVIFUNC CmbStrByteIs(const unsigned char *buf, size_t offset, long ch);
unsigned char * CVIFUNC CmbStrLastChar (const unsigned char *str);
unsigned char * CVIFUNC StringEnd(const unsigned char *string);

#else /* CVISTART */

/* CmbIsLeadByte checks if the given byte is a valid lead byte.      */
/* You should call this only if you know that the byte starts on     */
/* a character boundary (i.e., that it cannot be a trailing byte)    */
#define CmbIsLeadByte(c)             (IsDBCSLeadByte(c))

#endif /* CVISTART */

    /********************/
    /* MACROS:          */
    /********************/

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef Max
#define Max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef Min
#define Min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef ABS_VAL
#define ABS_VAL(a) ((a) < 0 ? (-(a)) : (a))
#endif

#ifndef Assert
#define Assert(passed) DoAssert((int)(passed), __FILE__, __LINE__, 0)
#endif

#ifndef AssertMsg
#define AssertMsg(passed, msg) DoAssert((int)(passed), __FILE__, __LINE__, msg)
#endif

#ifndef UNUSED
#ifdef _CVI_
#define UNUSED(a)
#else
#define UNUSED(a) switch((size_t)&a){default:break;}   /* bypass bogus warnings on external compilers */
#endif
#endif

#ifndef CAT4CHARS
#define CAT4CHARS(c1,c2,c3,c4)  (((int)(unsigned char)(c1)<<24)|((int)(unsigned char)(c2)<<16)|((int)(unsigned char)(c3)<<8)|((int)(unsigned char)(c4)))
#endif


/*  The errChk and nullChk macros are useful for implementing a consistent error
    handling system.  These can macros can be place around function calls to
    force an  automatic jump to a function's error handling code when an error
    occurs.  This is analogous to exception handling, and is even easier to use.

    These macros make the following assumptions:
        1)  The following local declaration of an error code variable is
            made in every function in which they are used:

            int error = 0;

        2)  Every function in which they are used contains a goto label
            named Error which precedes the error handling code for the function.

        3)  Every function call or error code enclosed in a errChk() macro
            is assumed to return an integer which, if negative, is the code for
            the error which occured.  If the value is zero or positive then the
            error checking macros have no effect.

            Every function call or value enclosed in a nullChk() macro is
            assummed to return a non-zero value if no error occurred, or a
            zero value if an "Out Of Memory" error occurred (nullChk() is
            useful for malloc, calloc, and similar resource allocation functions).
*/

#ifndef errChk
#define errChk(fCall) if (error = (fCall), error < 0) \
{goto Error;} else
#endif

#ifndef nullChk
#define nullChk(fCall) if ((fCall) == 0) \
{error = UIEOutOfMemory; goto Error;} else
#endif



#define EVENT_MOUSE_MOVE        14000       /* see EnableExtendedMouseEvents() */
#define EVENT_RIGHT_MOUSE_UP    14001
#define EVENT_LEFT_MOUSE_UP     14002

    /*****************/
    /* enums:        */
    /*****************/

typedef enum /* Error codes from -5000 to -5299, and from -6000 to -6199 */
    {
    // Toolbox.c error codes
    ToolErr_CouldNotOpenFileForReading      = -5001,
    ToolErr_ErrorReadingFile                = -5002,
    ToolErr_CouldNotFindUnusedTempFileName  = -5003,
    ToolErr_CouldNotCreateTempFile          = -5004,
    ToolErr_IncorrectFileFormat             = -5005,
    ToolErr_UnexpectedEndOfFile             = -5006,
    ToolErr_64BitNotImplemented             = -5007,
    ToolErr_InvalidIntNumber                = -5021,
    ToolErr_InvalidUIntNumber               = -5022,
    ToolErr_InvalidDoubleNumber             = -5023,
    ToolErr_InvalidBooleanValue             = -5024,
    ToolErr_MissingSection                  = -5025,
    ToolErr_MissingItem                     = -5026,
    ToolErr_UnRecognizedValue               = -5027,
    ToolErr_OperationInProgress             = -5050,
    ToolErr_TooManyItems                    = -5051,
    ToolErr_DuplicateItemOrValue            = -5052,
    ToolErr_PanelNotAChildOfCorrectPanel    = -5060,
    ToolErr_CantOpenKey                     = -5065,
    ToolErr_CantSetKeyValue                 = -5066,
    ToolErr_CantAccessKeyValue              = -5067,
    ToolErr_MissingKeyValue                 = -5068,
    ToolErr_WrongKeyValueType               = -5069,
    ToolErr_MissingKey                      = -5070,
    ToolErr_OutOfMemory                     = -5071,
    ToolErr_CantRemoveKey                   = -5072,
    ToolErr_CantRemoveKeyWithSubkeys        = -5073,
    ToolErr_CantRemoveValue                 = -5074,
    ToolErr_InvalidDisplayElement           = -5075,
    ToolErr_MsgCallbackNotInstalled         = -5080,
    ToolErr_MsgCallbackAlreadyInstalled     = -5081,
    ToolErr_PanelNotParent                  = -5082,
    ToolErr_NoPanelCallback                 = -5083,
    ToolErr_DragDropNotEnabled              = -5084,
    ToolErr_InvalidMsgCallbackMode          = -5085,
    ToolErr_NoMenuForTrayIcon               = -5087,
    ToolErr_InvalidTrayIconMenuItem         = -5088,
    ToolErr_InvalidTrayIcon                 = -5089,
    ToolErr_ErrorLoadingIconFileForTray     = -5090,
    ToolErr_ErrorLoadingDLL                 = -5093,
    ToolErr_FuncNotFound                    = -5094,
    ToolErr_UnknownSystemError              = -5095,
    ToolErr_FileAssociationNotAvailable     = -5096,
    ToolErr_InvalidHashTable                = -5200, // begin HashTable errors
    ToolErr_InvalidKeyType                  = -5201,
    ToolErr_InvalidKeySize                  = -5202,
    ToolErr_InvalidValueSize                = -5203,
    ToolErr_InvalidKeyArgument              = -5204,
    ToolErr_InvalidValueArgument            = -5205,
    ToolErr_NullArgument                    = -5206,
    ToolErr_BufferTooSmall                  = -5207,
    ToolErr_InvalidAttribute                = -5208,
    ToolErr_InvalidAttributeValue           = -5209,
    ToolErr_InvalidIterator                 = -5210,
    ToolErr_UnallocatedIterator             = -5211,
    ToolErr_CannotAllocateIterator          = -5212,
    ToolErr_IteratorAtEnd                   = -5213,
    ToolErr_ExceedsMaximumTableSize         = -5214,
    ToolErr_ExceedsMaximumChainLength       = -5215,
    ToolErr_FailedToResizeTable             = -5216,

    // Other error codes (from -6000 to -6199)
    // LocalUI error codes
    LocalUI_InvalidOrCorruptLangFile        = -6000
    } ToolError;


    /********************/
    /* Functions:       */
    /********************/


    /*** Miscellaneous ***/
/*** Commands to pass to ShowHtmlHelp()   ***/
#if !defined(HH_DISPLAY_TOPIC)  /* prevent warnings when HtmlHelp files are used */
#define HH_DISPLAY_TOPIC        0x0000
#define HH_HELP_FINDER          0x0000
#define HH_DISPLAY_TOC          0x0001
#define HH_DISPLAY_INDEX        0x0002
#define HH_HELP_CONTEXT         0x000F
#define HH_CLOSE_ALL            0x0012
#endif /* if !defined(HH_DISPLAY_TOPIC) */

double          CVIFUNC Pin(double value, double low, double high);
void            CVIFUNC DoAssert(int passed, const char *fileName, int line, const char *msg);
void            CVIFUNC SwapBlock(void *block1, void *block2, size_t numBytes);
double          CVIFUNC Random(double minimum, double maximum);
void            CVIFUNC SetRandomSeed(unsigned long seed);
#ifndef _NI_mswin64_
void            CVIFUNC StartPCSound(unsigned int frequency);
void            CVIFUNC StopPCSound(void);
#endif // _NI_mswin64_
void            CVIFUNC TransposeData(void *data, int dataType, size_t numPoints, int numChannels);
void            CVIFUNC ConvertArrayType(const void *sourceArray, int sourceDataType, void *targetArray,
                                        int targetDataType, size_t numberOfPoints);
int             CVIFUNC SetBOLE(int on);
char            * CVIFUNC GetToolboxErrorString(int errorCode);        /* no function panel yet */
char            * CVIFUNC GetGeneralErrorString(int errorCode);
int             CVIFUNC InternationalTime(char timeString[], size_t bufferSize);
int             CVIFUNC InternationalDate(char dateString[], size_t bufferSize);
int             CVIFUNC InternationalFileTime(const char filePath[], char timeString[], size_t bufferSize);
int             CVIFUNC InternationalFileDate(const char filePath[], char dateString[], size_t bufferSize);
int             CVIFUNC GetFileCLibTime(const char filePath[], time_t *time);
int             CVIFUNC RegisterExistence(const void *objectPtr);       /* no function panel yet */
int             CVIFUNC ObjectExists(const void *objectPtr);            /* no function panel yet */
void            CVIFUNC DeRegisterExistence(void *objectPtr);           /* no function panel yet */
int             CVIFUNC FindClosestColorInTable (int color, const int *table, size_t tableSize);
int             CVIFUNC RoundToNextMultiple(int number, int multiple);  /* no function panel yet */
char            * CVIFUNC FindFileDotPosition (const char *pathString); /* no function panel yet */
char            * CVIFUNC FindFileExtension(const char *pathString);    /* no function panel yet */
char            * CVIFUNC FindFileName(const char *pathString);         /* no function panel yet */
int             CVIFUNC ShowHtmlHelp(const char szFile[], unsigned int uCommand, void *dwData);
int             CVIFUNC OpenDocumentInDefaultViewer(const char *documentPathName, int windowState);

#if (_NI_mswin_ || _NI_linux_)
int             CVIFUNC NetworkVariablePopup(char selectedItem[MAX_PATHNAME_LEN]);
#endif

void            CVIFUNC DelayWithEventProcessing(double numberOfSeconds);

    /*** Special Double Precision Floating Point Numbers ***/
double          CVIFUNC PositiveInfinity(void);
double          CVIFUNC NegativeInfinity(void);
double          CVIFUNC NotANumber(void);
int             CVIFUNC IsNotANumber(double number);
int             CVIFUNC IsInfinity(double number);
int             CVIFUNC IsFinite(double number);

    /*** Floating Point Comparisons With Tolerance ***/

#define FP_CompareEpsilon       0.00000001  /* a small amount: used for floating point comparisons */

int             CVIFUNC FP_Compare(double a, double b);
int             CVIFUNC FP_EQ(double a, double b);      /* no function panel yet */
int             CVIFUNC FP_GT(double a, double b);      /* no function panel yet */
int             CVIFUNC FP_GE(double a, double b);      /* no function panel yet */
int             CVIFUNC FP_LT(double a, double b);      /* no function panel yet */
int             CVIFUNC FP_LE(double a, double b);      /* no function panel yet */
int             CVIFUNC ConvertDataToText(const unsigned char *dataPtr, size_t dataSize, unsigned char **textPtr);  /* no function panel yet */
int             CVIFUNC ConvertTextToData(const unsigned char *textPtr, size_t *dataSize, unsigned char **dataPtr); /* no function panel yet */

    /*** String Functions ***/

int             CVIFUNC AppendString(char **string, const char *stringToAdd, int lengthToAdd);
int             CVIFUNC AddStringPrefix(char **string, const char *stringPrefix, int lengthOfPrefix);
int             CVIFUNC AddStringQuotes(char **string);
char            * CVIFUNC StrDup(const char *stringToDuplicate);
char            * CVIFUNC StrDupWithoutSurrWhiteSpace (const char *string);
char            * CVIFUNC SkipWhiteSpace (const char *string);
char            * CVIFUNC SkipNonWhiteSpace (const char *string);
void            CVIFUNC StringCopyMax(char *dest, const char *source, size_t destBufferSize);
void            CVIFUNC RemoveSurroundingWhiteSpace(char string[]);
int             CVIFUNC HasNonWhiteSpace (const char *s);
int             CVIFUNC StrICmp (const char *s1, const char *s2);
int             CVIFUNC StrNICmp (const char *s1, const char *s2, int n);
int             CVIFUNC StrICmpWithoutSurrWhiteSpace (const char *s1, const char *s2);
#ifdef StrToInt
#undef StrToInt
#endif
int             CVIFUNC StrToInt (const char *str, int *n);
int             CVIFUNC StrToUInt (const char *str, unsigned int *n);
int             CVIFUNC StrToLongLong (const char *str, long long *n);
int             CVIFUNC StrToULongLong (const char *str, unsigned long long *n);
unsigned int    CVIFUNC BinStrToUInt(const char *str, int *overflow);
char            * CVIFUNC UIntToBinStr(unsigned int value, char buffer[]);
unsigned long long  CVIFUNC BinStrToULongLong(const char *str, int *overflow);
char            * CVIFUNC ULongLongToBinStr(unsigned long long value, char buffer[]);
int             CVIFUNC SpliceCommaSeparatedList(const char *list1, const char *list2, char **splicedList);     /* no function panel */
char            * CVIFUNC StringInsert(char *destinationString, const char *stringToInsert, size_t destinationBufferSize); /* no function panel */
int             CVIFUNC AcceleratorEscapeCodeLocation(const char *text);  /* no function panel */
void            CVIFUNC RemoveAcceleratorEscapeCode(char *text);    /* no function panel */


    /*** File Functions ***/

#if   defined(_NI_mswin_) && _NI_mswin_
    #define FILE_SEPARATOR  "\\"
    #define LINE_TERMINATOR "\r\n"
    #define LINE_TERMINATOR_LEN 2
#elif defined(_NI_unix_) && _NI_unix_
    #define FILE_SEPARATOR  "/"
    #define LINE_TERMINATOR "\n"
    #define LINE_TERMINATOR_LEN 1
#elif defined(_NI_mac_) && _NI_mac_
    #define FILE_SEPARATOR  ":"
    #define LINE_TERMINATOR "\r"
    #define LINE_TERMINATOR_LEN 1
#endif

int             CVIFUNC GetFileWritability (const char pathName[], int *isWritable);
int             CVIFUNC CreateAndOpenTemporaryFile (const char *pathName, const char *prefix, const char *extension,
                    const char *openModeString, char tempPathName[], FILE **tempFile);
void            CVIFUNC RemoveFileIfExists (const char pathName[]);
int             CVIFUNC DeleteAndRename (const char srcPathName[], const char destPathName[]);
int             CVIFUNC FileExists(const char *pathName, ssize_t *fileSize);
int             CVIFUNC WriteStringToFile (FILE *stream, const char *string);
void            CVIFUNC ChangeFileNameExtension(const char *fileName, char *newFileName, const char *extension);    /* no function panel yet */
void            CVIFUNC ChangeBaseFileName(const char *pathString, char *newPathString, const char *baseName);      /* no function panel yet */
int             CVIFUNC IsAbsolutePath(const char *pathString);       /* no function panel yet */
char            * CVIFUNC SkipDriveName(const char *pathString);      /* no function panel yet */
int             CVIFUNC FindBytesInFile(FILE *file, void *bytesToFind, int numBytesToFind, ssize_t *offset, ssize_t *position); /* no function panel yet */
int             CVIFUNC WriteByteToFile(FILE *file, char byte, int numTimes);     /* no function panel yet */
int             CVIFUNC ReadStringFromBinaryFile(FILE *file, char **string, int maxCharactersToRead, int *endOfFile, int *maxCharactersRead);   /* no function panel yet */
int             CVIFUNC ReadLineFromBinaryFile(FILE *file, char **string, int maxCharactersToRead, int *endOfFile, int *maxCharactersRead); /* no function panel yet */
int             CVIFUNC BinaryFileSize(FILE *file, ssize_t *size);  /* no function panel yet */
int             CVIFUNC SaveBitmapToFile (const char PathName[], int BmpId);
int             CVIFUNC SaveCtrlDisplayToFile (int panelHandle, int ctrlId, int include_label,
                                        int height, int width, const char PathName[]);
int             CVIFUNC SavePanelDisplayToFile (int panelHandle, int scope, Rect area,
                                        int height, int width, const char PathName[]);

    /*** Searching, Sorting, Comparison ***/

typedef int     (CVICALLBACK * CompareFunction)(void *item, void *itemOrArray); /* Typedef for ansi compatible comparison function  */
int             CVICALLBACK ShortCompare(void *a, void *b);
int             CVICALLBACK IntCompare(void *a, void *b);
int             CVICALLBACK FloatCompare(void *a, void *b);
int             CVICALLBACK DoubleCompare(void *a, void *b);
int             CVICALLBACK CStringCompare(void *a, void *b);
int             CVICALLBACK CStringNoCaseCompare(void *a, void *b);





ssize_t         CVIFUNC BinSearch(const void *array, size_t numElements, size_t elementSize, const void *itemPtr, CompareFunction compareFunction);
void            CVIFUNC HeapSort(void *array, size_t numElements, size_t elementSize, CompareFunction compareFunction);
void            CVIFUNC InsertionSort(void *array, size_t numElements, size_t elementSize, CompareFunction compareFunction);



    /*** Memory Block Handles ***/

typedef void **Handle;

Handle    CVIFUNC NewHandle(size_t numBytes);
void      CVIFUNC DisposeHandle(Handle handle);
size_t    CVIFUNC GetHandleSize(Handle handle);
int       CVIFUNC SetHandleSize(Handle handle, size_t newSize);



    /*** Lists ***/

    /*  Note:   FRONT_OF_LIST and END_OF_LIST only substitute for item positions in the
        arguments to a list function, they are not returned as item positions and should
        not be compared to item positions.
    */
#define     FRONT_OF_LIST    -1      /* Handy Constants that substitute for item positions */
#define     END_OF_LIST      0       /* END_OF_LIST means one past current length of list when
                                        inserting. Otherwise it refers the last item in the list. */

typedef struct ListStructTag *ListType;        /* The list abstract data type */
typedef int (CVICALLBACK * ListApplicationFunc)(int index, void *ptrToItem, void *callbackData);
typedef int (CVICALLBACK * ListApplicationFuncEx)(size_t index, void *ptrToItem, void *callbackData);

    /* Basic List Operations */
ListType    CVIFUNC ListCreate(size_t elementSize);
void        CVIFUNC ListDispose(ListType list);
void        CVIFUNC ListDisposePtrList(ListType list);
size_t      CVIFUNC ListNumItems(ListType list);
int         CVIFUNC ListInsertItem(ListType list, const void *ptrToItem, ssize_t itemPosition);
void        CVIFUNC ListGetItem(ListType list, void *itemDestination, ssize_t itemPosition);
void        CVIFUNC ListReplaceItem(ListType list, const void *ptrToItem, ssize_t itemPosition);
void        CVIFUNC ListRemoveItem(ListType list, void *itemDestination, ssize_t itemPosition);

int         CVIFUNC ListInsertItems(ListType list, const void *ptrToItems, ssize_t firstItemPosition, ssize_t numItemsToInsert);
void        CVIFUNC ListGetItems(ListType list, void *itemsDestination, ssize_t firstItemPosition, ssize_t numItemsToGet);
void        CVIFUNC ListReplaceItems(ListType list, const void *ptrToItems, ssize_t firstItemPosition, ssize_t numItemsToReplace);
void        CVIFUNC ListRemoveItems(ListType list, void *itemsDestination, ssize_t firstItemPosition, ssize_t numItemsToRemove);

ListType    CVIFUNC ListCopy(ListType originalList);
int         CVIFUNC ListAppend(ListType list1, ListType list2);
void        CVIFUNC ListClear(ListType list);
int         CVIFUNC ListEqual(ListType list1, ListType list2);
int         CVIFUNC ListInsertInOrder(ListType list, const void *ptrToItem, CompareFunction compareFunction);
void        * CVIFUNC ListGetPtrToItem(ListType list, ssize_t itemPosition);
void        * CVIFUNC ListGetDataPtr(ListType list);
int         CVIFUNC ListApplyToEach(ListType list, int ascending, ListApplicationFunc funcToApply, void *callbackData);
int         CVIFUNC ListApplyToEachEx(ListType list, int ascending, ListApplicationFuncEx funcToApply, void *callbackData);

    /* List Searching and Sorting */
size_t      CVIFUNC ListFindItem(ListType list, const void *ptrToItem, ssize_t startingPosition, CompareFunction compareFunction);
void        CVIFUNC ListRemoveDuplicates(ListType list, CompareFunction compareFunction);
ssize_t     CVIFUNC ListBinSearch(ListType list, const void *itemPtr, CompareFunction compareFunction);
void        CVIFUNC ListQuickSort(ListType list, CompareFunction compareFunction);
void        CVIFUNC ListHeapSort(ListType list, CompareFunction compareFunction);
void        CVIFUNC ListInsertionSort(ListType list, CompareFunction compareFunction);
int         CVIFUNC ListIsSorted(ListType list, CompareFunction compareFunction);

    /* List Output to Files and Standard I/O */
typedef int (CVICALLBACK * ItemOutputFunction)(FILE *stream, void *ptrToItem);

int         CVICALLBACK OutputStringItem(FILE *stream, void *ptrToItem);
int         CVICALLBACK OutputShortItem(FILE *stream, void *ptrToItem);
int         CVICALLBACK OutputDoubleItem(FILE *stream, void *ptrToItem);
int         CVICALLBACK OutputIntegerItem(FILE *stream, void *ptrToItem);
size_t      CVIFUNC     OutputList(ListType list, FILE *stream, const char *listPrefix, const char *listSuffix, const char *itemPrefix, const char *itemSuffix, ItemOutputFunction itemOutputFunction);

    /*  Advanced List Functions */
void        CVIFUNC ListSetAllocationPolicy(ListType list, int minItemsPerAlloc, int percentIncreasePerAlloc);
void        CVIFUNC ListCompact(ListType list);
int         CVIFUNC ListPreAllocate(ListType list, size_t numItems);
int         CVIFUNC ListGetItemSize(ListType list);
int         CVIFUNC GetIntListFromParmInfo(va_list parmInfo, size_t numIntegers, ListType *integerList);    /* no function panel, too obscure */
ListType    CVIFUNC_C BuildIntegerList(size_t numIntegers, ...);             /* no function panel yet */
int         CVIFUNC_C AppendToIntegerList(ListType listToAppendTo, size_t numIntegers, ...);     /* no function panel yet */
int         CVIFUNC ListInsertAfterItem(ListType list, const void *ptrToItem, const void *ptrToItemToInsertAfter, CompareFunction compareFunction);    /* no function panel */
int         CVIFUNC ListInsertBeforeItem(ListType list, const void *ptrToItem, const void *ptrToItemToInsertBefore, CompareFunction compareFunction);  /* no function panel */

#if defined(_CVI_DEBUG_) && !defined(_LINK_CVI_LVRT_)
#pragma soft_reference (ListNumItems);
#pragma soft_reference (ListGetItemSize);
#pragma soft_reference (ListGetPtrToItem);
#endif

    /*** Hash Tables ***/

typedef struct _HashTable   *HashTableType;
typedef intptr_t            HashTableIterator;
typedef unsigned long       KeyHash;        /* Hash values are generated as 32-bit integers */

typedef KeyHash (CVICALLBACK * HashBytesFunc)(const void *keyBytes, size_t numBytes); /* Prototype for the key hashing function */

typedef enum
    { 
    FIXED_SIZE_KEY,
    C_STRING_KEY
    } HashTableKeyType;

typedef enum
    {
    ATTR_HT_SIZE,
    ATTR_HT_NUM_BUCKETS,
    ATTR_HT_LOAD,
    ATTR_HT_GROW_LOAD,
    ATTR_HT_SHRINK_LOAD,
    ATTR_HT_HASH_FUNCTION,
    ATTR_HT_HASH_DISTRIBUTION
    } HashTableAttribute;

#define VAL_DO_NOT_GROW     -1.0
#define VAL_DO_NOT_SHRINK   -1.0

#define HASH_TABLE_END      1

#define DEFAULT_HASH_FUNC   Lookup3_hashlittle
KeyHash CVICALLBACK Lookup3_hashlittle       (const void *keyBytes, size_t numBytes);

    /* Hash Map Operations */
int     CVIFUNC     HashTableCreate          (unsigned int initialSize, HashTableKeyType keyType, size_t keySize, size_t valueSize, HashTableType *table);
int     CVIFUNC     HashTableDispose         (HashTableType table);
int     CVIFUNC     HashTableInsertItem      (HashTableType table, const void *key, const void *value);
int     CVIFUNC     HashTableGetItem         (HashTableType table, const void *key, void *value, size_t valueBufferSize);
int     CVIFUNC     HashTableFindItem        (HashTableType table, const void *key, int *found);
int     CVIFUNC     HashTableRemoveItem      (HashTableType table, const void *key, void *value, size_t valueBufferSize);
int     CVIFUNC     HashTableClear           (HashTableType table);
int     CVIFUNC     HashTableIteratorCreate  (HashTableType table, HashTableIterator *iterator);
int     CVIFUNC     HashTableIteratorGetItem (HashTableType table, HashTableIterator iterator, void *key, size_t keyBufferSize, void *value, size_t valueBufferSize);
int     CVIFUNC     HashTableIteratorAdvance (HashTableType table, HashTableIterator iterator);
int     CVIFUNC     HashTableIteratorDispose (HashTableType table, HashTableIterator iterator);
int     CVIFUNC     HashTableGetAttribute    (HashTableType table, int attribute, void *value);
int     CVIFUNC_C   HashTableSetAttribute    (HashTableType table, int attribute, ...);



    /*** User Interface Utility Functions ***/

    /*  Constants helpful for precise control positioning */
#define LIST_BOX_SCROLL_BAR_WIDTH     16
#define TEXT_BOX_FRAME_WIDTH           3
#define LIST_BOX_FRAME_WIDTH           3
#define LIST_BOX_RIGHT_MARGIN          2
#define LIST_BOX_LEFT_MARGIN           4
#define LIST_BOX_HORIZ_OVERHEAD       28  /* right position: 16 for scroll bar, 3+3 for frame, 4+2 for margins */
#define LIST_BOX_CHECKMARK_AREA_WIDTH 16
#define LIST_BOX_LEFT_PIXEL_POS        5  /* based in empirical observation */
#define RING_HORIZ_OVERHEAD           24  /* 16 + 8 extra for margin */
#define INC_DEC_WIDTH                  9

    /*  Values for the direction parameter of DistributeCtrls() */
#define VAL_TB_HORIZONTAL_DISTRIBUTION  0
#define VAL_TB_VERTICAL_DISTRIBUTION    1

    /*  Values for the spacing parameter of DistributeCtrls() */
#define VAL_TB_AREA_SPACING             0
#define VAL_TB_FIXED_GAP_SPACING        1

    /*  Values for the whichExtreme parameter of SetAllCtrlsToIntAttributeExtreme() */
#define VAL_TB_MAX                      0
#define VAL_TB_MIN                      1

typedef enum  /* these values may be passed to the AttrMinMax... functions */
    {
    ATTR_TB_BOUNDS_TOP = 6000,
    ATTR_TB_BOUNDS_LEFT,
    ATTR_TB_BOUNDS_HEIGHT,      /* gettable only */
    ATTR_TB_BOUNDS_WIDTH        /* gettable only */
    } ExtraToolBoxAttributes;

typedef enum
    {
    CANCELED        = 0,
    CHANGE_MADE     = 1,
    NO_CHANGE_MADE  = 2
    } ResponseCode;     /* useful for generic dialog box return codes and other places too */

typedef enum    /* for PlaceCtrlRelative */
    {
    kPositionCtrlTopLeft,
    kPositionCtrlTopCenter,
    kPositionCtrlTopRight,
    kPositionCtrlCenterLeft,
    kPositionCtrlCenterCenter,
    kPositionCtrlCenterRight,
    kPositionCtrlBottomLeft,
    kPositionCtrlBottomCenter,
    kPositionCtrlBottomRight
    } PositionCtrlSide;

typedef enum    /* for PlaceCtrlRelative */
    {
    kPositionCtrlTopOrLeftJustification,
    kPositionCtrlCenterJustification,
    kPositionCtrlBottomOrRightJustification
    } PositionCtrlJustification;

// preserve earlier names of these values, for backwards compatibility
#define kPositionCtrlTopOrLeftJustication       kPositionCtrlTopOrLeftJustification
#define kPositionCtrlBottomOrRightJustication   kPositionCtrlBottomOrRightJustification
 

int     CVIFUNC PutLabelOnLeft(int panel, int ctrl, int labelToCtrlGap);
int     CVIFUNC SetCommonDialogShortcutKeys(int panel, int enterCtrl, int escCtrl, int closeCtrl);
int     CVIFUNC GetDataTypeSize(int dataType);     /* no function panel yet */

int     CVIFUNC_C SetAttributeForCtrls(int panel, int attribute, int value, int numCtrls, ...);
int     CVIFUNC_C AttributeMaxAndMin(int panel, int attribute, int numCtrls, int *max, int *ctrlWithMax, int *min, int *ctrlWithMin, ...);
int     CVIFUNC_C SetCtrlsToAttributeExtreme(int panel, int attribute, int whichExtreme, int numCtrls, ...);
int     CVIFUNC_C DistributeCtrls(int panel, int direction, int spacing, int *position, int gap, int numCtrls, ...);
int     CVIFUNC_C PutLabelsOnLeft(int panel, int labelToCtrlGap, int numCtrls, ...);

int     CVIFUNC SetAttributeForList(int panel, ListType ctrlList, int attribute, int value);
int     CVIFUNC AttrMaxAndMinForList(int panel, ListType ctrlList, int attribute, int *max, int *ctrlWithMax, int *min, int *ctrlWithMin);
int     CVIFUNC SetAttributeToExtremeForList(int panel, ListType ctrlList, int attribute, int whichExtreme);
int     CVIFUNC DistributeCtrlsInList(int panel, ListType ctrlList, int direction, int spacing, int *position, int gap);
int     CVIFUNC PutLabelsOnLeftForList(int panel, ListType ctrlList, int labelToCtrlGap);
int     CVIFUNC MoveInFront(int panel, int ctrl, int ctrlToMove);
int     CVIFUNC MoveBehind(int panel, int ctrl, int ctrlToMove);
int     CVIFUNC PlaceAfter(int panel, int ctrl, int ctrlToMove);                /* no function panel yet */
int     CVIFUNC PlaceBefore(int panel, int ctrl, int ctrlToMove);               /* no function panel yet */
int     CVIFUNC PositionCtrlRelativeToCtrl(int panel, int ctrl, int considerLabel,
                        int ctrlToMove, int considerCtrlToMoveLabel, int gap,
                        PositionCtrlSide whichSide, PositionCtrlJustification justification);
int     CVIFUNC GetNumericAttributeAsType(int panel, int ctrl, int attribute, int destType, void *value); /* no function panel, too obscure */
int     CVIFUNC CenterCtrl(int panel, int ctrl, int top, int left, int height, int width);
int     CVIFUNC SizeRingCtrlToText(int panel, int ring);
int     CVIFUNC ConformCtrlBitmapToPanel (int panel, int control, int index, int bitmap3dColor, int new3dColor, int bitmapTextColor, int newTextColor);
void    CVIFUNC SetCtrlRightPosition(int panel, int control, int right);        /* no function panel yet */
int     CVIFUNC GetCtrlBoundRectAttribute(int panel, int ctrl, int attribute, int *value);   /* no function panel */
int     CVIFUNC SetCtrlBoundRectAttribute(int panel, int ctrl, int attribute, int value);    /* no function panel */
int     CVIFUNC PreviousCtrl(int panel, int ctrl, int wrap);                    /* no function panel yet */
int     CVIFUNC CtrlCanBeTabStop(int panel, int ctrl);                          /* no function panel yet */
int     CVIFUNC NextCtrlInTabOrder(int panel, int ctrl, int forward, int wrap); /* no function panel yet */
int     CVIFUNC ActivateFirstCtrl(int panel);                                   /* no function panel yet */
int     CVIFUNC RecessCtrlFrame(int panel, int control);
int     CVIFUNC RecessAllCtrlFrames(int panel);
void    CVIFUNC DiscardCtrlNoBOLE(int panel, int ctrl, int noDiscardEvent);     /* no function panel yet */
void    CVIFUNC DiscardPanelNoBOLE(int panel, int noDiscardEvent);              /* no function panel yet */
int     CVIFUNC AllocateCtrlValString(int panel, int ctrl, char **string);
int     CVIFUNC GetCtrlValStringLength(int panel, int ctrl, int *length);
int     CVIFUNC_C SetSelectedRadioButtons(int panel, int activeButton, int numCtrls, ...);
int     CVIFUNC_C GetSelectedRadioButtons(int panel, int *activeButton, int numCtrls, ...);
int     CVIFUNC GetMetaFontInfo(const char metaFont[], char typeface[], int *bold, int *underline, int *strikeout,  int *italic, int *size);
int     CVIFUNC GetMetaFontInfoEx(const char metaFont[], char typeface[], int *bold, int *underline, int *strikeout,  int *italic, int *size, int *angle);
int     CVIFUNC GetMetaFontInfoWithCharacterSet(const char metaFont[], char typeface[], int *bold, int *underline, 
                        int *strikeout,  int *italic, int *size, int *angle, int *characterSet);
int     CVIFUNC IsMetaFont(const char fontname[256]);
int     CVIFUNC GetCtrlList(int panel, ListType *ctrlList);                     /* no function panel yet */
void    CVIFUNC SortCtrlListByTabOrder(int panel, ListType ctrlList);           /* no function panel yet */
char    * CVIFUNC UILEventString(int event);
void    CVIFUNC LocalizeNumberString (char *str);
void    CVIFUNC DelocalizeNumberString (char *str);

#define CTRL_TOOLTIP_ATTR_TEXT      25000
#define CTRL_TOOLTIP_ATTR_ENABLE    25001

int     CVIFUNC_C SetCtrlToolTipAttribute(int panel, int ctrl, int attribute, ...);
int     CVIFUNC GetCtrlToolTipAttribute(int panel, int ctrl, int attribute, void *value);

int     CVIFUNC ChainCtrlCallback(int panel, int ctrl, CtrlCallbackPtr newCallback, void *newCallbackData, const char *type);
int     CVIFUNC GetChainedCallbackData(int panel, int ctrl, const char *type, void **callbackData);
int     CVIFUNC CtrlHasChainedCallback(int panel, int ctrl, const char *type);
int     CVIFUNC SetChainedCallbackData(int panel, int ctrl, const char *type, void *callbackData);
int     CVIFUNC CallCtrlCallback(int panel, int ctrl, int event, int eventData1, int eventData2, int *returnValue);
int     CVIFUNC UnchainCtrlCallback(int panel, int ctrl, const char *type);

int     CVIFUNC ChainPanelCallback(int panel, PanelCallbackPtr newCallback, void *newCallbackData, const char *type);
int     CVIFUNC GetChainedPanelCallbackData(int panel, const char *type, void **callbackData);
int     CVIFUNC SetChainedPanelCallbackData(int panel, const char *type, void *callbackData);
int     CVIFUNC CallPanelCallback(int panel, int event, int eventData1, int eventData2, int *returnValue);
int     CVIFUNC UnchainPanelCallback(int panel, const char *type);

int     CVIFUNC EnableExtendedMouseEvents(int panel, int ctrl, double minPeriod);
int     CVIFUNC DisableExtendedMouseEvents(int panel, int ctrl);
int     CVIFUNC ConvertMouseCoordinates(int panel, int ctrl, int relativeToCtrl, int clipToControl, int *x, int *y);

    /*** Popup Dialogs ***/

typedef void (CVICALLBACK * ShowColorChangeFunction)(ListType colorList, void *colorChangeCallbackData, int colorListIndex);

typedef struct
    {
    char    *name;
    int     defaultColor;
    int     color;
    } ColorChangeEntry;

int     CVIFUNC ColorChangePopup(const char title[], ListType colorList, int allowDefault, ShowColorChangeFunction showColorChangeFunc, void *colorChangeCallbackData);
int     CVIFUNC CreateProgressDialog(const char title[], const char indicatorLabel[], int modal, int indicatorMarkerStyle, const char cancelButtonLabel[]);
int     CVIFUNC UpdateProgressDialog(int panel, int percentDone, int processEvents);
void    CVIFUNC DiscardProgressDialog(int panel);



    /*** Constants and Conversions ***/

    /*  Note: A 8-byte double has 15 digits of precision */
#define PI                          3.1415926535897932384626433832795028841971
#define TWO_PI                      6.28318530717958646
#define HALF_PI                     1.57079632679489661
#define THREE_HALVES_PI             4.71238898038468990
#define RECIPROCAL_OF_PI            0.31830988618379067153
#define LN_OF_PI                    1.14472988584940017414
#define LOG_10_OF_PI                0.49714987269413385435
#define EULER                       2.71828182845904523536
#define RECIPROCAL_OF_EULER         0.36787944117144232159
#define LOG_10_OF_EULER             0.4342944819032518276511289189166050822944
#define LN_OF_10                    2.3025850929940456840179914546843642076011
#define LN_OF_2                     0.6931471805599453094172321214581765680755

#define PLANCK_CONSTANT             6.626176e-34        /* 1/Hz, uncertainty 5.4 ppm */
#define ELEMENTARY_CHARGE           1.6021892e-19       /* Coulombs, uncertainty 2.9 ppm */
#define SPEED_OF_LIGHT              299792458.0         /* Meters/Second, uncertainty 0.004 ppm */
#define GRAVITATIONAL_CONSTANT      6.6720e-11          /* N*M^2/K^2, uncertainty 615 ppm */
#define AVOGADRO_CONSTANT           6.022045e23         /* 1/mol, uncertainty 5.1 ppm */
#define RYDBERG_CONSTANT            10973731.77         /* 1/m, uncertainty 0.075 ppm */
#define MOLAR_GAS_CONSTANT          8.31441             /* 1/(m * K), uncertainty 31 ppm */

#define RAD_TO_DEG(r)               ((r)*(360.0/TWO_PI))
#define DEG_TO_RAD(d)               ((d)*(TWO_PI/360.0))
#define CELSIUS_TO_KELVIN(c)        ((c) + 273.15)
#define KELVIN_TO_CELSIUS(k)        ((k) - 273.15)
#define CELSIUS_TO_FAHRENHEIT(c)    (((c)*(9.0/5.0))+32.0)
#define FAHRENHEIT_TO_CELSIUS(f)    (((f)-32.0) * (5.0/9.0))
#define METERS_TO_FEET(m)           ((m)/0.3048)
#define FEET_TO_METERS(f)           ((f)*0.3048)
#define KILOMETERS_TO_MILES(k)      ((k)/1.609344)
#define MILES_TO_KILOMETERS(m)      ((m)*1.609344)
#define KILOGRAMS_TO_POUNDS(k)      ((k)/0.453592)      /* avdp.*/
#define POUNDS_TO_KILOGRAMS(p)      ((p)*0.453592)
#define LITERS_TO_GALLONS(l)        ((l)/3.7854)        /* U.S. Liquid */
#define GALLONS_TO_LITERS(g)        ((g)*3.7854)        /* U.S. Liquid */

double CVIFUNC Pi(void);
double CVIFUNC TwoPi(void);
double CVIFUNC HalfPi(void);
double CVIFUNC ThreeHalvesPi(void);
double CVIFUNC ReciprocalOfPi(void);
double CVIFUNC LnOfPi(void);
double CVIFUNC Log10OfPi(void);
double CVIFUNC Euler(void);
double CVIFUNC ReciprocalOfEuler(void);
double CVIFUNC Log10OfEuler(void);
double CVIFUNC Ln10(void);
double CVIFUNC Ln2(void);

double CVIFUNC PlanckConstant(void);
double CVIFUNC ElementaryCharge(void);
double CVIFUNC SpeedOfLight(void);
double CVIFUNC GravitationalConstant(void);
double CVIFUNC AvogadroConstant(void);
double CVIFUNC RydbergConstant(void);
double CVIFUNC MolarGasConstant(void);

double CVIFUNC DegToRad(double degrees);
double CVIFUNC RadToDeg(double radians);
double CVIFUNC CelsiusToKelvin(double degreesCelsius);
double CVIFUNC KelvinToCelsius(double degreesKelvin);
double CVIFUNC CelsiusToFahrenheit(double degreesCelsius);
double CVIFUNC FahrenheitToCelsius(double degreesFahrenheit);
double CVIFUNC MetersToFeet(double meters);
double CVIFUNC FeetToMeters(double feet);
double CVIFUNC KilometersToMiles(double kilometers);
double CVIFUNC MilesToKilometers(double miles);
double CVIFUNC KilogramsToPounds(double kilograms);
double CVIFUNC PoundsToKilograms(double pounds);
double CVIFUNC LitersToGallons(double liters);
double CVIFUNC GallonsToLiters(double gallons);



    /*** Windows Registry ***/

#if defined(_NI_mswin_) && _NI_mswin_

#define REGKEY_HKCU 0x80000001 /* HKEY_CURRENT_USER   */
#define REGKEY_HKCR 0x80000000 /* HKEY_CLASSES_ROOT   */
#define REGKEY_HKU  0x80000003 /* HKEY_USERS          */
#define REGKEY_HKLM 0x80000002 /* HKEY_LOCAL_MACHINE  */
#define REGKEY_HKCC 0x80000005 /* HKEY_CURRENT_CONFIG */
#define REGKEY_HKDD 0x80000006 /* HKEY_DYN_DATA       */

int CVIFUNC RegWriteBinary(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, const unsigned char* userData, size_t userDataSize);
int CVIFUNC RegReadBinary(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, unsigned char* userBuffer, size_t userBufSize, unsigned int* realDataSize);
int CVIFUNC RegWriteULong(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, unsigned long userUL, int bigEndian);
int CVIFUNC RegReadULong(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, unsigned long* userLong, int bigEndian);
int CVIFUNC RegWriteString(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, const unsigned char* userString);
int CVIFUNC RegWriteStringArray (unsigned int userRootKey, const char* userSubKeyName, size_t userArrayItems, const char* userValNames[], const char* userStrings[]);
int CVIFUNC RegReadString(unsigned int userRootKey, const char* userSubKeyName, const char* userValName, unsigned char* userString, size_t userStringSize, unsigned int* realStringSize);
int CVIFUNC RegRemoveKey(unsigned int userRootKey, const char* userSubKeyName);
int CVIFUNC RegRemoveValue(unsigned int userRootKey, const char* userSubKeyName, const char* userValName);
int CVIFUNC RegQueryInfoOnKey (unsigned int userRootKey, const char *userSubKeyName, unsigned int *pKeys, unsigned int *pValues, unsigned int *pMaxSubKeyLen, unsigned int *pMaxValueNameLen, unsigned int *pMaxValueLen);
int CVIFUNC RegEnumerateKey(unsigned int userRootKey, const char *userSubKeyName, unsigned int userIndex, char *pKeyName, unsigned int *pKeyNameLen);
int CVIFUNC RegEnumerateValue(unsigned int userRootKey, const char *userSubKeyName, unsigned int userIndex, char *pValueName, unsigned int *pValNameLen, void *value, unsigned int *valueLen, int *pValueType);

// Registry data types
#define _REG_NONE                       ( 0 )   // No value type
#define _REG_SZ                         ( 1 )   // Unicode nul terminated string
#define _REG_EXPAND_SZ                  ( 2 )   // Unicode nul terminated string
                                                // (with environment variable references)
#define _REG_BINARY                     ( 3 )   // Free form binary
#define _REG_DWORD                      ( 4 )   // 32-bit number
#define _REG_DWORD_LITTLE_ENDIAN        ( _REG_DWORD )  // 32-bit number (same as _REG_DWORD)
#define _REG_DWORD_BIG_ENDIAN           ( 5 )   // 32-bit number
#define _REG_LINK                       ( 6 )   // Symbolic Link (unicode)
#define _REG_MULTI_SZ                   ( 7 )   // Multiple Unicode strings
#define _REG_RESOURCE_LIST              ( 8 )   // Resource list in the resource map
#define _REG_FULL_RESOURCE_DESCRIPTOR   ( 9 )   // Resource list in the hardware description
#define _REG_RESOURCE_REQUIREMENTS_LIST ( 10 )


#endif /* defined(_NI_mswin_) && _NI_mswin_ */



    /*** Windows System/User Information ***/

#define MAX_COMPNAME_LEN                16

#if defined(_NI_mswin_) && _NI_mswin_

typedef struct {
    unsigned long int loBytes;
    unsigned long int hiBytes;
} UInt64Type;


#define KYBD_IBM_PCXT                   1
#define KYBD_OLIVETTI_ICO               2
#define KYBD_IBM_PCAT                   3
#define KYBD_IBM_Enh                    4
#define KYBD_NOKIA_1050                 5
#define KYBD_NOKIA_9140                 6
#define KYBD_JAPANESE                   7

#define PLATFORM_WIN32s                 0
#define PLATFORM_WIN95                  1
#define PLATFORM_WINNT                  2

#define SYS_SCROLLBAR                   0
#define SYS_BACKGROUND                  1
#define SYS_ACTIVECAPTION               2
#define SYS_INACTIVECAPTION             3
#define SYS_MENU                        4
#define SYS_WINDOW                      5
#define SYS_WINDOWFRAME                 6
#define SYS_MENUTEXT                    7
#define SYS_WINDOWTEXT                  8
#define SYS_CAPTIONTEXT                 9
#define SYS_ACTIVEBORDER                10
#define SYS_INACTIVEBORDER              11
#define SYS_APPWORKSPACE                12
#define SYS_HIGHLIGHT                   13
#define SYS_HIGHLIGHTTEXT               14
#define SYS_BTNFACE                     15
#define SYS_BTNSHADOW                   16
#define SYS_GRAYTEXT                    17
#define SYS_BTNTEXT                     18
#define SYS_INACTIVECAPTIONTEXT         19
#define SYS_BTNHIGHLIGHT                20

int CVIFUNC GetWindowsColor(int displayElement, unsigned int* pcolor);
int CVIFUNC GetWindowsDirs(char* winPathBuf, char* sysPathBuf);
int CVIFUNC GetWinOSVersion(unsigned long* pmajorVersion, unsigned long* pminorVersion, unsigned long* pbuild, unsigned long* pplatform);
int CVIFUNC GetKeyboardPreferences(int* pkeyboardType, int* pnumFuncKeys, unsigned long* prepeatDelay, unsigned long* pspeed);
int CVIFUNC SetKeyboardPreferences(unsigned long repeatDelay, unsigned long speed);
int CVIFUNC GetScreenSaver(int* enabled, int* timeToActivate);
int CVIFUNC SetScreenSaver(int enabled, int timeToActivate, const char* ssFile);
int CVIFUNC GetDiskSpace(const char* directory, UInt64Type* ptotalBytes, UInt64Type* pbytesFree);
int CVIFUNC UInt64TypeCompareUInt(UInt64Type UInt64Item, unsigned int UInt32Item);
int CVIFUNC GetMemoryInfo(unsigned long* ppercentLoad, size_t* ptotalPhysical, size_t* ptotalPage, size_t* ptotalVirtual, size_t* pavailPhysical, size_t *pavailPage, size_t* pavailVirtual);
int CVIFUNC SetWallpaper(int tiled, const char* bitmapPath);
int CVIFUNC GetWallpaper(int* tiled, char bitmapPath[]);

#endif /* defined(_NI_mswin_) && _NI_mswin_ */

int CVIFUNC GetCompName(char* nameBuf);
int CVIFUNC GetCurrentUser(char nameBuf[], size_t bufSize, unsigned int* realNameSize);

    /*** Windows Messaging ***/

#if defined(_NI_mswin_) && _NI_mswin_

typedef struct
    {
    unsigned int  uMsg;
    intptr_t      wParam;
    intptr_t      lParam;
    long          result;
    } InterceptedWindowsMsg;

#define VAL_MODE_INTERCEPT     1000
#define VAL_MODE_IN_QUEUE      1001

#define ATTR_CALLBACK_MODE     1002
#define ATTR_CALL_DEF_WIN_PROC 1008
#define ATTR_WIN_PROC_RET_VAL  1009

#define EVENT_NEWHANDLE        1004
#define EVENT_FILESDROPPED     1005

#define EVENT_WINDOWS_MSG      23956 /* eventData1 is a pointer to InterceptedWindowsMsg */

typedef int (CVICALLBACK *WinMsgCallback) (int panelHandle, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData);

int CVIFUNC   InstallWinMsgCallback(int panelHandle, int msgNum, WinMsgCallback pcallback, int callWhen, void* pcallbackData, intptr_t* phwnd);
int CVIFUNC_C SetMsgCallbackAttribute(int panelHandle, int msgNum, int attribute, ...);
int CVIFUNC   GetMsgCallbackAttribute(int panelHandle, int msgNum, int attribute, void* pval);
int CVIFUNC   RemoveWinMsgCallback(int panelHandle, int msgNum);
int CVIFUNC   EnableDragAndDrop(int panelHandle);
int CVIFUNC   DisableDragAndDrop(int panelHandle);
int CVIFUNC   GetDragAndDropData(char ***fileNames, Point *mousePoint);
int CVIFUNC   RouteWinMsgToPanelCallback(int panel, unsigned int uMsg);
int CVIFUNC   GetRoutedWinMsgParams(int panel, InterceptedWindowsMsg *msg);
int CVIFUNC   SetRoutedWinMsgResult(int panel, int result);
int CVIFUNC   UnrouteWinMsgToPanelCallback(int panel, unsigned int uMsg);

#endif /* defined(_NI_mswin_) && _NI_mswin_ */



    /*** Windows Taskbar System Tray Icons ***/

#if defined(_NI_mswin_) && _NI_mswin_

#define ATTR_POPUP_DEFAULT_ITEM      5002

#define ATTR_TRAY_ICOFILE            5004
#define ATTR_TRAY_TOOLTIPTEXT        5005
#define ATTR_TRAY_TOOLTIPTEXT_LENGTH 5006

#define EVENT_MENU_ITEM              1012

typedef int (CVICALLBACK *TrayIconCallback) (int iconHandle, int event, int eventData);

int CVIFUNC InstallSysTrayIcon(const char* icoFile, const char* toolTipText, TrayIconCallback taskIconProc, int* htrayIcon);
int CVIFUNC SetTrayIconAttr(int hicon, int attribute, const char* val);
int CVIFUNC GetTrayIconAttr(int huser, int attribute, void* val);
int CVIFUNC AttachTrayIconMenu(int iconHandle);
int CVIFUNC SetTrayIconMenuAttr(int hicon, int attribute, int val);
int CVIFUNC GetTrayIconMenuAttr(int huser, int attribute, int* pval);
int CVIFUNC InsertTrayIconMenuItem(int iconHandle, const char* itemName, int* pindex);
int CVIFUNC SetTrayIconMenuItemAttr(int iconHandle, int itemHandle, int attribute, int val);
int CVIFUNC GetTrayIconMenuItemAttr(int huser, int itemNum, int attribute, int* pval);
int CVIFUNC DetachTrayIconMenu(int hicon);
int CVIFUNC RemoveSysTrayIcon(int hicon);

#endif /* defined(_NI_mswin_) && _NI_mswin_ */

    /*** Callback Posting ***/
#define POST_CALL_WAIT_TIMEOUT_INFINITE -1
typedef void (CVICALLBACK * DelayedCallbackFunc)(void *callbackData);

int CVIFUNC PostDelayedCall(DelayedCallbackFunc callback, void *callbackData, double delay);
int CVIFUNC PostDeferredCallToThreadAndWait (DeferredCallbackPtr funcPtr, void *callbackData,
                                             unsigned int targetThreadId, int timeout);

    /*** CVI Time Conversion Functions ***/
#if HAVE_CVI_RTE
int CVIFUNC CVIAbsoluteTimeToCVIANSITime(CVIAbsoluteTime absTime, time_t *ansiTime);
int CVIFUNC CVIAbsoluteTimeFromCVIANSITime(time_t ansiTime, CVIAbsoluteTime *absTime);

int CVIFUNC CVIAbsoluteTimeToCVIUILTime(CVIAbsoluteTime absTime, double *uilTime);
int CVIFUNC CVIAbsoluteTimeFromCVIUILTime(double uilTime, CVIAbsoluteTime *absTime);

int CVIFUNC CVIAbsoluteTimeToCNVTime(CVIAbsoluteTime absTime, unsigned __int64 *cnvTime);
int CVIFUNC CVIAbsoluteTimeFromCNVTime(unsigned __int64 cnvTime, CVIAbsoluteTime *absTime);

#if defined(_NI_mswin_) && _NI_mswin_
/* The following two functions require windows.h */
#ifndef _FILETIME_ 
#pragma pack(push)
#pragma pack(4)
typedef struct _FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME;
#pragma pack(pop)
#define _FILETIME_
#endif
int CVIFUNC CVIAbsoluteTimeToFILETIME(CVIAbsoluteTime absTime, FILETIME *fileTime);
int CVIFUNC CVIAbsoluteTimeFromFILETIME(FILETIME fileTime, CVIAbsoluteTime *absTime);
#endif /* defined(_NI_mswin_) && _NI_mswin_ */

#endif /* HAVE_CVI_RTE */

int CVIFUNC HostIsBigEndian(void);
int CVIFUNC HostIsLittleEndian(void);
unsigned short CVIFUNC ToBigEndian16(unsigned short n);
unsigned short CVIFUNC ToLittleEndian16(unsigned short n);
unsigned short CVIFUNC ToOtherEndian16(unsigned short n);
unsigned int CVIFUNC ToBigEndian32(unsigned int n);
unsigned int CVIFUNC ToLittleEndian32(unsigned int n);
unsigned int CVIFUNC ToOtherEndian32(unsigned int n);
unsigned __int64 CVIFUNC ToBigEndian64(unsigned __int64 n);
unsigned __int64 CVIFUNC ToLittleEndian64(unsigned __int64 n);
unsigned __int64 CVIFUNC ToOtherEndian64(unsigned __int64 n);

#ifdef __cplusplus
    }
#endif

#endif /* TOOLBOX_HEADER */
