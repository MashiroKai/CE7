/*-------------------------------------------------
  Copyright (C), 2004-2012, Enpht Tech. Co., Ltd.
  File name     : Eph5275_32.h
  Author        : Zengm
  Version       : V1.00
  Date          : 2012.4
  Description   : cPCI/PXI LVDS通讯接口板
  Function List :    
  History       : V100      
-------------------------------------------------*/
#include "visatype.h"

#ifndef _EPH5275_H_
#define _EPH5275_H_

/*************************************************
*  EP-H5275触发模式，该版本保留
*************************************************/
#define TRIG_SOFT_MODE              0x00000001 // 软件触发允许
#define TRIG_EXT_RAISE_EDGE_OUT     0x00000002 //外触发LVDS输出正沿允许
#define TRIG_EXT_FALL_EDGE_OUT      0x00000004 //外触发LVDS输出负沿允许
#define TRIG_EXT_RAISE_EDGE_IN      0x00000008 //外触发LVDS输入正沿允许
#define TRIG_EXT_FALL_EDGE_IN       0x00000010 //外触发LVDS输入负沿允许
#define TRIG_SYS_RAISE_EDGE         0x00000020 //系统触发正沿允许
#define TRIG_SYS_FALL_EDGE          0x00000040 //系统触发负沿允许
#define TRIG_STAR_RAISE_EDGE        0x00000080 //星型正沿允许
#define TRIG_STAR_FALL_EDGE         0x00000100 //星型负沿允许

/*************************************************
*  EP-H5275 函数返回值
*************************************************/
#define API_SUCCESS              0     // 函数调用成功
#define API_FAILED               -1    // 函数调用失败
#define API_EPHDEV_NOTINITED    202    // 模块未初始化

#define  LVDS_FRAME_LEN         0xff   //LVDS帧长，该版本固定255

//LVDS帧结构，按传输帧格式解码 
typedef struct lvds_frame_inf
{
	ViUInt8 FrameHeader[4];	    //帧头(32Bit)默认:0x1ACFFC1D 32Bit
	ViUInt8 VCDU_Head_Ver;	    //UCDU主导头 版本号	2Bit
	ViUInt8 VCDU_Head_FLAG_0;   //VCDU标识符 航天器标识符 8Bit
	ViUInt8 VCDU_Head_FLAG_1;   //VCDU标识符 虚拟信道标识符 6Bit
	ViUInt8 VCDU_Head_COUNT[3];	//UCDU主导头 VCDU帧计数 24Bit
	ViUInt8 VCDU_Head_MSG;		//UCDU主导头 VCDU信息域 8Bit
	ViUInt8 VCDU_Data[438];	    //UCDU数据域 3504Bit
	ViUInt8 RS_Parity[64];		//RS校验符号 512Bit

}LVDS_FRAME_INF;


//全局变量，其中包含中断信息等
typedef struct api_int_fifo
{
	ViInt32 (_stdcall *function)(ViUInt32 cardnum, struct api_int_fifo *pFIFO);
	
	ViUInt32 cardnum; // Card number associated with this thread. (RO) $
	int iPriority; // reserved
	ViUInt32 dwMilliseconds; // reserved
	ViInt32 iNotification; // reserved

	int nUser[8]; // Spare variables for use by the user.
	void *pUser[8]; // Spare variables for use by the user.
}
API_INT_FIFO;

#ifdef __cplusplus
extern "C"{
#endif

/**********************************************************
* Function: EphLvds_AutoConnectToFirst
* Description:连接到系统找到的第一张EP-H5275模块，如果系统中只存在一张
			EP-H5275模块，推荐使用此函数
* Parameters: 
*			cardnum[out]:连接模块成功后返回的模块句柄，供其他函数使用
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_AutoConnectToFirst(
	ViUInt32	*cardnum);

/**********************************************************
* Function: EphLvds_AutoConnectToBusSlot
* Description:连接到指定总线地址和槽号的EP-H5275模块，总线地址和槽号可通过
			计算机设备管理器查询，系统中存在多张EP-H5275模块时，需要使用
			此函数
* Parameters: 
*			BusNo[in]: 模块总线地址
*			SlotNo[in]:模块槽号
*			cardnum[out]:连接模块成功后返回的模块句柄，供其他函数使用
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_AutoConnectToBusSlot(
	ViUInt16	BusNo,  
	ViUInt16	SlotNo, 
	ViUInt32	*cardnum);

/**********************************************************
* Function: EphLvds_Close
* Description:	关闭当前连接的EP-H5275模块，并释放系统资源
* Parameters: 
*			cardnum[in]:模块句柄
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_Close(
	ViUInt32	cardnum);


/**********************************************************
* Function: EphLvds_Reset
* Description:	初始化EP-H5275模块
* Parameters: 
*			cardnum[in]:模块句柄
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_Reset(
	ViUInt32	cardnum);


/**********************************************************
* Function: EphLvds_GetVersion
* Description:	获取模块版本号
* Parameters: 
*			cardnum[in]:模块句柄
*			version[out]:版本号，0x100表示Ver 1.00
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetVersion(
	ViUInt32	cardnum,
	ViUInt16	*version);


/**********************************************************
* Function: EphLvds_GetManuId
* Description:	获取厂商ID
* Parameters: 
*			cardnum[in]:模块句柄
*			ManuId[out]:厂商ID，恩菲特默认ID:0x41f8
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetManuId(
	ViUInt32	cardnum,
	ViUInt16	*ManuId);


/**********************************************************
* Function: EphLvds_GetDevId
* Description:	获取模块ID
* Parameters: 
*			cardnum[in]:模块句柄
*			DevId[out]:模块ID，EP-H5275模块ID:0x5275
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetDevId(
	ViUInt32	cardnum,
	ViUInt16	*DevId);

/**********************************************************
* Function: EphLvds_SetIntLen
* Description:	设置产生中断的长度，即当接收设置值个数据后，硬件产生一个中断
* Parameters: 
*			cardnum[in]:模块句柄
*			Len[in]:中断产生长度，0表示长度1，支持设置0---4M/帧长-1(16Bit)
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_SetIntLen(
	ViUInt32	cardnum,
	ViUInt32	Len);




/**********************************************************
* Function: EphLvds_SetChIntEN
* Description:	设置接收通道中断允许
* Parameters: 
*			cardnum[in]:模块句柄
*			ChEn[in]:中断允许通道，0表示通道0允许中断，1表示通道1允许中断
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_SetChIntEN(
	ViUInt32 cardnum,
	ViUInt16 ChEn);




/**********************************************************
* Function: EphLvds_RX_SetFrameLen
* Description:设置接收LVDS数据的帧长，0表示帧长1，该版本固定为256字,即0xff
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameLen[in]:LVDS数据的帧长，0表示帧长1，该版本固定为256字,即0xff
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetFrameLen(
	ViUInt32	cardnum,
	ViUInt32	FrameLen);


/**********************************************************
* Function: EphLvds_RX_SetFrameHead
* Description:设置接收LVDS数据的帧头
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameHead[in]:LVDS数据的帧头，默认为0x1ACFFC1D
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetFrameHead(
	ViUInt32	cardnum,
	ViUInt32	FrameHead);

/**********************************************************
* Function: EphLvds_RX_SetMode
* Description:设置接收LVDS数据模式
* Parameters: 
*			cardnum[in]:模块句柄
*			ClkMode[in]:接收时钟沿选择，0：负沿 ；1：正沿；  默认为0
*			ParityHead[in]:帧头校验允许标志，1：允许校验；0：不允许检验，默认为1
*			CHMode[in]:接收模式，如下：		
						--0 接收所有数据（默认）
						--1 只接收回放信道数据
						--2 只接收填充信道数据
						--3 只接收回放信道与填充信道的数据（即丢弃非这两种信息的数据）
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetMode(
	ViUInt32	cardnum,
	ViUInt16	ClkMode,
	ViUInt16	ParityHead,
	ViUInt16	CHMode);

/**********************************************************
* Function: EphLvds_RX_StartStop
* Description:启动/停止LVDS接收数据
* Parameters: 
*			cardnum[in]:模块句柄
*			flag[in]:1表示启动接收，0表示停止接收，在启动接收之前，
			应先设置帧长度、帧头和接收模式
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_StartStop(
	ViUInt32	cardnum,
	ViUInt16	flag);


/**********************************************************
* Function: EphLvds_RX_GetEndID
* Description:获取接收通道结束帧ID，即当前的帧个数
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameID[out]:当前接收通道的帧ID
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetEndID(
	ViUInt32	cardnum,
	ViUInt16	Ch,
	ViUInt32	*FrameID);


/**********************************************************
* Function: EphLvds_RX_GetEndAddr
* Description:获取接收通道的结束地址
* Parameters: 
*			cardnum[in]:模块句柄
*			EndAddr[out]:当前接收通道的结束地址
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetEndAddr(
	ViUInt32	cardnum,
	ViUInt16	Ch,
	ViUInt32	*EndAddr);


/**********************************************************
* Function: EphLvds_TX_SetFrameLen
* Description:设置LVDS模拟发送帧长
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameLen[in]:LVDS模拟发送帧长，该版本固定256字
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetFrameLen(
	ViUInt32	cardnum,
	ViUInt32	FrameLen);


/**********************************************************
* Function: EphLvds_TX_SetFrameHead
* Description:设置LVDS模拟发送帧头
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameLen[in]:LVDS模拟发送帧头，默认为0x1ACFFC1D
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetFrameHead(
	ViUInt32	cardnum,
	ViUInt32	FrameHead);


/**********************************************************
* Function: EphLvds_TX_SetMode
* Description:设置LVDS模拟发送模式
* Parameters: 
*			cardnum[in]:模块句柄
*			ClkMode[in]:发送时钟模式，如下：
						--1: 正沿发送数据(默认)
						--2: 负沿发送数据
*			DataMode[in]:数据模式，如下
						--0: 固定发送一数值
						--1: 设置的起始数据递增数据(默认)
						--2: 设置的起始数据递减数据
*			CHMode[in]:发送信道模式，如下
						--0: 发送填充信道与回放信道，交替发送模式(默认)
						--1: 只发送回放信道数据
						--2: 只发送填充信道数据
						
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetMode(
	ViUInt32	cardnum,
	ViUInt16	ClkMode,
	ViUInt16	DataMode,
	ViUInt16	CHMode);



/**********************************************************
* Function: EphLvds_TX_StartStop
* Description:启动/停止LVDS模拟数据发送
* Parameters: 
*			cardnum[in]:模块句柄
*			flag[in]:1表示启动发送，0表示停止发送，在启动发送之前，应先设置发送
			帧长、帧头和发送模式
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_StartStop(
	ViUInt32	cardnum,
	ViUInt16	flag);

/**********************************************************
* Function: EphLvds_TX_SetStartData
* Description:LVDS模拟数据发送的起始数据
* Parameters: 
*			cardnum[in]:模块句柄
*			Data[in]:LVDS模拟数据发送的起始数据
			帧长、帧头和发送模式
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetStartData(
	ViUInt32	cardnum,
	ViUInt32	Data);


/**********************************************************
* Function: EphLvds_RX_ReadFrameByEndAddr
* Description:指定结束地址读取LVDS帧数据
* Parameters: 
*			cardnum[in]:模块句柄
*			Ch[in]:接收通道号，0、1
*			EndAddr[in]:通道的结束地址
*			FrameCount[in]:需要读回的帧个数，该版本帧长为0xff
*			FrameData[out]:读回的数据，每一元素对应一个数据，例如0-255表示第一帧，256-512表示第二帧
*			ReadFrameCount[out]:实际读回的帧个数
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_ReadFrameByEndAddr(
	ViUInt32	cardnum,
	ViUInt16	Ch,
	ViUInt32	EndAddr,
	ViUInt16	FrameCount,
	ViUInt16	*FrameData,
	ViUInt16	*ReadFrameCount);


/**********************************************************
* Function: EphLvds_RX_ReadFrameByEndID
* Description:指定结束地址读取LVDS帧数据
* Parameters: 
*			cardnum[in]:模块句柄
*			Ch[in]:接收通道号，0、1
*			EndID[in]:通道的结束帧ID
*			FrameCount[in]:需要读回的帧个数，该版本帧长为0xff
*			FrameData[out]:读回的数据，每一元素对应一个数据，例如0-255表示第一帧，256-512表示第二帧
*			ReadFrameCount[out]:实际读回的帧个数
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_ReadFrameByEndID(
	ViUInt32 cardnum,
	ViUInt16 Ch,	
	ViUInt32 EndID,
	ViUInt16 FrameCount,
	ViUInt16 *FrameData,
	ViUInt16 *ReadFrameCount);

/**********************************************************
* Function: EphLvds_FrameDecompose
* Description:原始帧数据到传输结构的转换
* Parameters: 
*			cardnum[in]:模块句柄
*			FrameData[in]:一帧原始数据
*			FrameInf[out]:转换后的传输结构帧信息
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_FrameDecompose(
	ViUInt16 *FrameData,
	LVDS_FRAME_INF *FrameInf);

/**********************************************************
* Function: EphLvds_RegisterFunction
* Description:注册中断
* Parameters: 
*			cardnum[in]:模块句柄
*			sIntFIFO1[in]:指向API_INT_FIFO结构的中断信息
*			flag[in]:0表示注销中断，1表示注册中断
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RegisterFunction(
   ViUInt32		cardnum, 
   API_INT_FIFO *sIntFIFO1,
   ViUInt32		flag);
/**********************************************************
* Function: EphLvds_SetByteMode
* Description:设置存储数据大小端模式
* Parameters: 
*			cardnum[in]:模块句柄
*			ByteMode[in]:0表示大端模式，1表示小端模式
* Return: 0: 表示成功 非0: 表示错误
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetByteMode(
	ViUInt32 cardnum,
	ViUInt16 ByteMode);

/**********************************************************
* Function: EphLvds_GetByteMode
* Description:获取存储数据大小端模式
* Parameters: 
*			cardnum[in]:模块句柄
*			ByteMode[out]:0表示大端模式，1表示小端模式
* Return: 0: 表示成功 非0: 表示错误
*********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetByteMode(
	ViUInt32 cardnum,
	ViUInt16 *ByteMode);
#ifdef __cplusplus
}
#endif

#endif  //end _EPH5275_H_

