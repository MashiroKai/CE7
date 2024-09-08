/*-------------------------------------------------
  Copyright (C), 2004-2012, Enpht Tech. Co., Ltd.
  File name     : Eph5275_32.h
  Author        : Zengm
  Version       : V1.00
  Date          : 2012.4
  Description   : cPCI/PXI LVDSͨѶ�ӿڰ�
  Function List :    
  History       : V100      
-------------------------------------------------*/
#include "visatype.h"

#ifndef _EPH5275_H_
#define _EPH5275_H_

/*************************************************
*  EP-H5275����ģʽ���ð汾����
*************************************************/
#define TRIG_SOFT_MODE              0x00000001 // �����������
#define TRIG_EXT_RAISE_EDGE_OUT     0x00000002 //�ⴥ��LVDS�����������
#define TRIG_EXT_FALL_EDGE_OUT      0x00000004 //�ⴥ��LVDS�����������
#define TRIG_EXT_RAISE_EDGE_IN      0x00000008 //�ⴥ��LVDS������������
#define TRIG_EXT_FALL_EDGE_IN       0x00000010 //�ⴥ��LVDS���븺������
#define TRIG_SYS_RAISE_EDGE         0x00000020 //ϵͳ������������
#define TRIG_SYS_FALL_EDGE          0x00000040 //ϵͳ������������
#define TRIG_STAR_RAISE_EDGE        0x00000080 //������������
#define TRIG_STAR_FALL_EDGE         0x00000100 //���͸�������

/*************************************************
*  EP-H5275 ��������ֵ
*************************************************/
#define API_SUCCESS              0     // �������óɹ�
#define API_FAILED               -1    // ��������ʧ��
#define API_EPHDEV_NOTINITED    202    // ģ��δ��ʼ��

#define  LVDS_FRAME_LEN         0xff   //LVDS֡�����ð汾�̶�255

//LVDS֡�ṹ��������֡��ʽ���� 
typedef struct lvds_frame_inf
{
	ViUInt8 FrameHeader[4];	    //֡ͷ(32Bit)Ĭ��:0x1ACFFC1D 32Bit
	ViUInt8 VCDU_Head_Ver;	    //UCDU����ͷ �汾��	2Bit
	ViUInt8 VCDU_Head_FLAG_0;   //VCDU��ʶ�� ��������ʶ�� 8Bit
	ViUInt8 VCDU_Head_FLAG_1;   //VCDU��ʶ�� �����ŵ���ʶ�� 6Bit
	ViUInt8 VCDU_Head_COUNT[3];	//UCDU����ͷ VCDU֡���� 24Bit
	ViUInt8 VCDU_Head_MSG;		//UCDU����ͷ VCDU��Ϣ�� 8Bit
	ViUInt8 VCDU_Data[438];	    //UCDU������ 3504Bit
	ViUInt8 RS_Parity[64];		//RSУ����� 512Bit

}LVDS_FRAME_INF;


//ȫ�ֱ��������а����ж���Ϣ��
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
* Description:���ӵ�ϵͳ�ҵ��ĵ�һ��EP-H5275ģ�飬���ϵͳ��ֻ����һ��
			EP-H5275ģ�飬�Ƽ�ʹ�ô˺���
* Parameters: 
*			cardnum[out]:����ģ��ɹ��󷵻ص�ģ����������������ʹ��
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_AutoConnectToFirst(
	ViUInt32	*cardnum);

/**********************************************************
* Function: EphLvds_AutoConnectToBusSlot
* Description:���ӵ�ָ�����ߵ�ַ�Ͳۺŵ�EP-H5275ģ�飬���ߵ�ַ�Ͳۺſ�ͨ��
			������豸��������ѯ��ϵͳ�д��ڶ���EP-H5275ģ��ʱ����Ҫʹ��
			�˺���
* Parameters: 
*			BusNo[in]: ģ�����ߵ�ַ
*			SlotNo[in]:ģ��ۺ�
*			cardnum[out]:����ģ��ɹ��󷵻ص�ģ����������������ʹ��
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_AutoConnectToBusSlot(
	ViUInt16	BusNo,  
	ViUInt16	SlotNo, 
	ViUInt32	*cardnum);

/**********************************************************
* Function: EphLvds_Close
* Description:	�رյ�ǰ���ӵ�EP-H5275ģ�飬���ͷ�ϵͳ��Դ
* Parameters: 
*			cardnum[in]:ģ����
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_Close(
	ViUInt32	cardnum);


/**********************************************************
* Function: EphLvds_Reset
* Description:	��ʼ��EP-H5275ģ��
* Parameters: 
*			cardnum[in]:ģ����
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_Reset(
	ViUInt32	cardnum);


/**********************************************************
* Function: EphLvds_GetVersion
* Description:	��ȡģ��汾��
* Parameters: 
*			cardnum[in]:ģ����
*			version[out]:�汾�ţ�0x100��ʾVer 1.00
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetVersion(
	ViUInt32	cardnum,
	ViUInt16	*version);


/**********************************************************
* Function: EphLvds_GetManuId
* Description:	��ȡ����ID
* Parameters: 
*			cardnum[in]:ģ����
*			ManuId[out]:����ID��������Ĭ��ID:0x41f8
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetManuId(
	ViUInt32	cardnum,
	ViUInt16	*ManuId);


/**********************************************************
* Function: EphLvds_GetDevId
* Description:	��ȡģ��ID
* Parameters: 
*			cardnum[in]:ģ����
*			DevId[out]:ģ��ID��EP-H5275ģ��ID:0x5275
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_GetDevId(
	ViUInt32	cardnum,
	ViUInt16	*DevId);

/**********************************************************
* Function: EphLvds_SetIntLen
* Description:	���ò����жϵĳ��ȣ�������������ֵ�����ݺ�Ӳ������һ���ж�
* Parameters: 
*			cardnum[in]:ģ����
*			Len[in]:�жϲ������ȣ�0��ʾ����1��֧������0---4M/֡��-1(16Bit)
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_SetIntLen(
	ViUInt32	cardnum,
	ViUInt32	Len);




/**********************************************************
* Function: EphLvds_SetChIntEN
* Description:	���ý���ͨ���ж�����
* Parameters: 
*			cardnum[in]:ģ����
*			ChEn[in]:�ж�����ͨ����0��ʾͨ��0�����жϣ�1��ʾͨ��1�����ж�
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_SetChIntEN(
	ViUInt32 cardnum,
	ViUInt16 ChEn);




/**********************************************************
* Function: EphLvds_RX_SetFrameLen
* Description:���ý���LVDS���ݵ�֡����0��ʾ֡��1���ð汾�̶�Ϊ256��,��0xff
* Parameters: 
*			cardnum[in]:ģ����
*			FrameLen[in]:LVDS���ݵ�֡����0��ʾ֡��1���ð汾�̶�Ϊ256��,��0xff
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetFrameLen(
	ViUInt32	cardnum,
	ViUInt32	FrameLen);


/**********************************************************
* Function: EphLvds_RX_SetFrameHead
* Description:���ý���LVDS���ݵ�֡ͷ
* Parameters: 
*			cardnum[in]:ģ����
*			FrameHead[in]:LVDS���ݵ�֡ͷ��Ĭ��Ϊ0x1ACFFC1D
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetFrameHead(
	ViUInt32	cardnum,
	ViUInt32	FrameHead);

/**********************************************************
* Function: EphLvds_RX_SetMode
* Description:���ý���LVDS����ģʽ
* Parameters: 
*			cardnum[in]:ģ����
*			ClkMode[in]:����ʱ����ѡ��0������ ��1�����أ�  Ĭ��Ϊ0
*			ParityHead[in]:֡ͷУ�������־��1������У�飻0����������飬Ĭ��Ϊ1
*			CHMode[in]:����ģʽ�����£�		
						--0 �����������ݣ�Ĭ�ϣ�
						--1 ֻ���ջط��ŵ�����
						--2 ֻ��������ŵ�����
						--3 ֻ���ջط��ŵ�������ŵ������ݣ�����������������Ϣ�����ݣ�
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetMode(
	ViUInt32	cardnum,
	ViUInt16	ClkMode,
	ViUInt16	ParityHead,
	ViUInt16	CHMode);

/**********************************************************
* Function: EphLvds_RX_StartStop
* Description:����/ֹͣLVDS��������
* Parameters: 
*			cardnum[in]:ģ����
*			flag[in]:1��ʾ�������գ�0��ʾֹͣ���գ�����������֮ǰ��
			Ӧ������֡���ȡ�֡ͷ�ͽ���ģʽ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_StartStop(
	ViUInt32	cardnum,
	ViUInt16	flag);


/**********************************************************
* Function: EphLvds_RX_GetEndID
* Description:��ȡ����ͨ������֡ID������ǰ��֡����
* Parameters: 
*			cardnum[in]:ģ����
*			FrameID[out]:��ǰ����ͨ����֡ID
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetEndID(
	ViUInt32	cardnum,
	ViUInt16	Ch,
	ViUInt32	*FrameID);


/**********************************************************
* Function: EphLvds_RX_GetEndAddr
* Description:��ȡ����ͨ���Ľ�����ַ
* Parameters: 
*			cardnum[in]:ģ����
*			EndAddr[out]:��ǰ����ͨ���Ľ�����ַ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetEndAddr(
	ViUInt32	cardnum,
	ViUInt16	Ch,
	ViUInt32	*EndAddr);


/**********************************************************
* Function: EphLvds_TX_SetFrameLen
* Description:����LVDSģ�ⷢ��֡��
* Parameters: 
*			cardnum[in]:ģ����
*			FrameLen[in]:LVDSģ�ⷢ��֡�����ð汾�̶�256��
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetFrameLen(
	ViUInt32	cardnum,
	ViUInt32	FrameLen);


/**********************************************************
* Function: EphLvds_TX_SetFrameHead
* Description:����LVDSģ�ⷢ��֡ͷ
* Parameters: 
*			cardnum[in]:ģ����
*			FrameLen[in]:LVDSģ�ⷢ��֡ͷ��Ĭ��Ϊ0x1ACFFC1D
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetFrameHead(
	ViUInt32	cardnum,
	ViUInt32	FrameHead);


/**********************************************************
* Function: EphLvds_TX_SetMode
* Description:����LVDSģ�ⷢ��ģʽ
* Parameters: 
*			cardnum[in]:ģ����
*			ClkMode[in]:����ʱ��ģʽ�����£�
						--1: ���ط�������(Ĭ��)
						--2: ���ط�������
*			DataMode[in]:����ģʽ������
						--0: �̶�����һ��ֵ
						--1: ���õ���ʼ���ݵ�������(Ĭ��)
						--2: ���õ���ʼ���ݵݼ�����
*			CHMode[in]:�����ŵ�ģʽ������
						--0: ��������ŵ���ط��ŵ������淢��ģʽ(Ĭ��)
						--1: ֻ���ͻط��ŵ�����
						--2: ֻ��������ŵ�����
						
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetMode(
	ViUInt32	cardnum,
	ViUInt16	ClkMode,
	ViUInt16	DataMode,
	ViUInt16	CHMode);



/**********************************************************
* Function: EphLvds_TX_StartStop
* Description:����/ֹͣLVDSģ�����ݷ���
* Parameters: 
*			cardnum[in]:ģ����
*			flag[in]:1��ʾ�������ͣ�0��ʾֹͣ���ͣ�����������֮ǰ��Ӧ�����÷���
			֡����֡ͷ�ͷ���ģʽ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_StartStop(
	ViUInt32	cardnum,
	ViUInt16	flag);

/**********************************************************
* Function: EphLvds_TX_SetStartData
* Description:LVDSģ�����ݷ��͵���ʼ����
* Parameters: 
*			cardnum[in]:ģ����
*			Data[in]:LVDSģ�����ݷ��͵���ʼ����
			֡����֡ͷ�ͷ���ģʽ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_TX_SetStartData(
	ViUInt32	cardnum,
	ViUInt32	Data);


/**********************************************************
* Function: EphLvds_RX_ReadFrameByEndAddr
* Description:ָ��������ַ��ȡLVDS֡����
* Parameters: 
*			cardnum[in]:ģ����
*			Ch[in]:����ͨ���ţ�0��1
*			EndAddr[in]:ͨ���Ľ�����ַ
*			FrameCount[in]:��Ҫ���ص�֡�������ð汾֡��Ϊ0xff
*			FrameData[out]:���ص����ݣ�ÿһԪ�ض�Ӧһ�����ݣ�����0-255��ʾ��һ֡��256-512��ʾ�ڶ�֡
*			ReadFrameCount[out]:ʵ�ʶ��ص�֡����
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
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
* Description:ָ��������ַ��ȡLVDS֡����
* Parameters: 
*			cardnum[in]:ģ����
*			Ch[in]:����ͨ���ţ�0��1
*			EndID[in]:ͨ���Ľ���֡ID
*			FrameCount[in]:��Ҫ���ص�֡�������ð汾֡��Ϊ0xff
*			FrameData[out]:���ص����ݣ�ÿһԪ�ض�Ӧһ�����ݣ�����0-255��ʾ��һ֡��256-512��ʾ�ڶ�֡
*			ReadFrameCount[out]:ʵ�ʶ��ص�֡����
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
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
* Description:ԭʼ֡���ݵ�����ṹ��ת��
* Parameters: 
*			cardnum[in]:ģ����
*			FrameData[in]:һ֡ԭʼ����
*			FrameInf[out]:ת����Ĵ���ṹ֡��Ϣ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_FrameDecompose(
	ViUInt16 *FrameData,
	LVDS_FRAME_INF *FrameInf);

/**********************************************************
* Function: EphLvds_RegisterFunction
* Description:ע���ж�
* Parameters: 
*			cardnum[in]:ģ����
*			sIntFIFO1[in]:ָ��API_INT_FIFO�ṹ���ж���Ϣ
*			flag[in]:0��ʾע���жϣ�1��ʾע���ж�
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RegisterFunction(
   ViUInt32		cardnum, 
   API_INT_FIFO *sIntFIFO1,
   ViUInt32		flag);
/**********************************************************
* Function: EphLvds_SetByteMode
* Description:���ô洢���ݴ�С��ģʽ
* Parameters: 
*			cardnum[in]:ģ����
*			ByteMode[in]:0��ʾ���ģʽ��1��ʾС��ģʽ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
**********************************************************/
ViStatus _VI_FUNC EphLvds_RX_SetByteMode(
	ViUInt32 cardnum,
	ViUInt16 ByteMode);

/**********************************************************
* Function: EphLvds_GetByteMode
* Description:��ȡ�洢���ݴ�С��ģʽ
* Parameters: 
*			cardnum[in]:ģ����
*			ByteMode[out]:0��ʾ���ģʽ��1��ʾС��ģʽ
* Return: 0: ��ʾ�ɹ� ��0: ��ʾ����
*********************************************************/
ViStatus _VI_FUNC EphLvds_RX_GetByteMode(
	ViUInt32 cardnum,
	ViUInt16 *ByteMode);
#ifdef __cplusplus
}
#endif

#endif  //end _EPH5275_H_

