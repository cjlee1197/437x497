/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/

/*===========================================================================+
|  Class    : Selvat Manager                                                 |
|  Task     : Selvat Manager header file                                     |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision : 09/19/2003                                                     |
+===========================================================================*/

#ifndef     D_SELVAT
#define     D_SELVAT

#include    "common.h"
/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define     SELVAT_CREATE_SUCCESS           0x0001      // ���O�y���Ы�(���\)
#define     SELVAT_CREATE_FAILURE           0x0000      // ���O�y���Ы�(����)

#define     SELVAT_DATASOURCE_FILE          0x0001      // ���O�y��ƨӷ������
#define     SELVAT_DATASOURCE_CONST         0x0002      // ���O�y��ƨӷ����R�A�w�q���ܶq
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct	tagSELVATTABLE
{
	WORD		    wTonValue;          // �o������
	WORD		    wTableSize;         // ���z�y��j�p
	int*		    pnRealTonTable;     // �ҪO��m�Ʋի��w
	int*		    pnOilTonTable;      // �o���ƾڼƲի��w
} SELVATTABLE;

typedef struct tagSELVATINFO
{
    char*           pcSelvatFileName;   // ���O�y�~���Ƥ���
    char*           pcOilVatSelectCode; // ���O�y���ƿ�ܦr�q
    char**          pcVatTransID;       // ���O�y�ѼƦC����w
    SELVATTABLE*    pSelvatTable;       // ���O�y�D�~���ƫ��w
    WORD            wDataSource;        // ���O�y��ƨӷ�
} SELVATINFO;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*===========================================================================+
|           Class declaration - CSelvat                                      |
+===========================================================================*/
class CSelvat
{
public:
	CSelvat();
	~CSelvat();

private:
    SELVATINFO      m_SelvatInfo;
    WORD            m_wState;

    int*            m_apnRealTonTable;      // ��e���ƹ������ҪO��m�Ʋ�(�Ӧۤ���)
    int*            m_apnOilTonTable;       // ��e���ƹ������o���ƾڼƲ�(�Ӧۤ���)
    WORD            m_wCurTonValue;         // ��e����
    WORD            m_wPreTonValue;         // �W������
    SELVATTABLE*    m_poutSelvatTable;      // �~���ƾ�
    int*            m_poutRealOilPosi;      // ���ɤ��U�ձ��O�y��m

    int*            m_pTonRealTable;        // ��e���ƹ������ҪO��m�Ʋի��w
    int*            m_pTonOilTable;         // ��e���ƹ������o���ƾڼƲի��w
    int             m_Oilhigh;              // ��e���ƹ������ҪO��m�Ʋժ���
    int             m_Realhigh;             // ��e���ƹ������o���ƾڼƲժ���

public:
	WORD		Create(SELVATINFO* pSelvatInfo);
	// ���o��e���ƹ��������z�y��
    BOOL       	GetOilTable();
    // �ഫ�ҪO��m���o����m
    int         GetOilVat(int x);
    // �ഫ�o����m���ҪO��m
    int         OilToPosi(int y);
    // �ˬd���w�ѼƬO�_�ݩ���z�y�Ѽ�
    BOOL       	CheckOilVatID(char* pcIDOilVat);
    // �ഫ�Ҧ������z�y�ѼƬ��o����m
    void        TransferOilVat(BYTE* panBuffer);
    // �ǰe�Ҧ������z�y�Ѽƨ�D��
    void        SendOilVat();
    // �ഫ�Ҧ������z�y�ѼƬ��ҪO��m
    void        SetVatDefPosi();
private:
    // �[�����O�y��SELVATTABLE���c���
	BOOL        LoadSelvatTableFile();
	// �[�����O�y���w���ƪ��ƾ�
	BOOL        LoadSelvatData(WORD wTon);
	// �R�A�ƾڼg�J����
	BOOL        WriteStaticDataToFile(char* pfilename, SELVATTABLE* pselvattable);
};

/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      CSelvat*        g_pSelvat;

extern      SELVATTABLE		g_aSelvatTable[];
extern      char*           g_apcVatTransID[];
#endif
