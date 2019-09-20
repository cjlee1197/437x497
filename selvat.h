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
#define     SELVAT_CREATE_SUCCESS           0x0001      // 推力座類創建(成功)
#define     SELVAT_CREATE_FAILURE           0x0000      // 推力座類創建(失敗)

#define     SELVAT_DATASOURCE_FILE          0x0001      // 推力座資料來源於文檔
#define     SELVAT_DATASOURCE_CONST         0x0002      // 推力座資料來源於靜態定義的變量
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct	tagSELVATTABLE
{
	WORD		    wTonValue;          // 油壓噸數
	WORD		    wTableSize;         // 推理座表大小
	int*		    pnRealTonTable;     // 模板位置數組指針
	int*		    pnOilTonTable;      // 油壓數據數組指針
} SELVATTABLE;

typedef struct tagSELVATINFO
{
    char*           pcSelvatFileName;   // 推力座外放資料文檔
    char*           pcOilVatSelectCode; // 推力座噸數選擇字段
    char**          pcVatTransID;       // 推力座參數列表指針
    SELVATTABLE*    pSelvatTable;       // 推力座非外放資料指針
    WORD            wDataSource;        // 推力座資料來源
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

    int*            m_apnRealTonTable;      // 當前噸數對應的模板位置數組(來自文檔)
    int*            m_apnOilTonTable;       // 當前噸數對應的油壓數據數組(來自文檔)
    WORD            m_wCurTonValue;         // 當前噸數
    WORD            m_wPreTonValue;         // 上次噸數
    SELVATTABLE*    m_poutSelvatTable;      // 外部數據
    int*            m_poutRealOilPosi;      // 文檔中各組推力座位置

    int*            m_pTonRealTable;        // 當前噸數對應的模板位置數組指針
    int*            m_pTonOilTable;         // 當前噸數對應的油壓數據數組指針
    int             m_Oilhigh;              // 當前噸數對應的模板位置數組長度
    int             m_Realhigh;             // 當前噸數對應的油壓數據數組長度

public:
	WORD		Create(SELVATINFO* pSelvatInfo);
	// 取得當前噸數對應的推理座表
    BOOL       	GetOilTable();
    // 轉換模板位置為油缸位置
    int         GetOilVat(int x);
    // 轉換油缸位置為模板位置
    int         OilToPosi(int y);
    // 檢查指定參數是否屬於推理座參數
    BOOL       	CheckOilVatID(char* pcIDOilVat);
    // 轉換所有的推理座參數為油缸位置
    void        TransferOilVat(BYTE* panBuffer);
    // 傳送所有的推理座參數到主機
    void        SendOilVat();
    // 轉換所有的推理座參數為模板位置
    void        SetVatDefPosi();
private:
    // 加載推力座的SELVATTABLE結構資料
	BOOL        LoadSelvatTableFile();
	// 加載推力座指定噸數的數據
	BOOL        LoadSelvatData(WORD wTon);
	// 靜態數據寫入文檔
	BOOL        WriteStaticDataToFile(char* pfilename, SELVATTABLE* pselvattable);
};

/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      CSelvat*        g_pSelvat;

extern      SELVATTABLE		g_aSelvatTable[];
extern      char*           g_apcVatTransID[];
#endif
