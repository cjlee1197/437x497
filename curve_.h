/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 22/08/2005                                                     |
|  Revision : netdigger v2.0  2006/4/17                                      |
+===========================================================================*/
#ifndef	D_DCURVE
#define	D_DCURVE

#include	"wnd.h"
#include	"wndx2.h"
#include	"gui_ex.h"
 
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#define			CURVE_LINE_MAX			10		// �̦h�����u�ƥ�
#define			CURVE_BUFFERLINE_MAX	100
#define			CURVE_BEELINE			0x00000001		// ���u
#define			CURVE_CURVELINE			0x00000002		// ���u

/*
#define			CURVE_SHOWXNAME			0x00000001		// ���X�b�W��
#define			CURVE_SHOWYNAME			0x00000002		// ���Y�b�W��
*/
#define			CURVE_XCONTRARY			0x00000001		// X�b�q�j��p

#define			CURVE_CUTLINE_RIGHT		0x00000010		// �b�k����ܹϨ�
#define			CURVE_CUTLINE_TOP		0x00000020		// �b�W����ܹϨ�
#define			CURVE_CUTLINE_LEFT		0x00000040		// �b������ܹϨ�
#define			CURVE_CUTLINE_BOTTOM	0x00000080		// �b�U����ܹϨ�

#define			CURVE_NOTDRAW			0x00000100		// ���e���u

//	
#define			CURVE_COOR_BROKENLINE	0x1000			//	the coordinate is real line
#define			CURVE_RULE_REALLINE		0x2000			//	the RULE is broken line

#define			CURVE_LINETYPE			0x000000FF		// ���u����

const	int		CURVE_XXPOS_TOP			= 0x00000010;	//	top
const	int		CURVE_XXPOS_BOTTOM		= 0x00000020;	//	bottom
const	int		CURVE_XXPOS_LEFT		= 0x00000040;	//	left
const	int		CURVE_XXPOS_RIGHT		= 0x00000080;	//	right

const	int		CURVE_VALUE_NOTHING	 	= 0x00000000;		//	show nothing
const	int		CURVE_VALUE_ALL	 		= 0x00000001;
const	int		CURVE_VALUE_LIMIT	 	= 0x00000002;		//	show the min  and max 
const	int		CURVE_VALUE_MINONLY 	= 0x00000003;		//	only show the min vlaue
const	int		CURVE_VALUE_MAXONLY 	= 0x00000004;		//	only show the max value
                                          
//const	int		CURVE_XAXES_POS_NOTHING = 0x00000000;		//	don't show the x axes
//const	int		CURVE_XAXES_POS_TOP	 	= 0x00000001;		//	show the X axes on the top of the curve
//const	int		CURVE_XAXES_POS_BOTTOM	= 0x00000002;		//	show the X axes on the bottom of the curve
//                                          
//const	int		CURVE_YAXES_POS_NOTHING	= 0x00000000;
//const	int		CURVE_YAXES_POS_LEFT	= 0x00000003;		//	show the y axes on the left of the curve
//const	int		CURVE_YAXES_POS_RIGHT	= 0x00000004;		//	show the y axes on the right of the curve

//�վ�Ҧ�
#define			CURVE_OPERATION_NONE				0
#define			CURVE_OPERATION_MOVE_COORDINATE		1
#define			CURVE_OPERATION_STRETCH				2

#define			DRAW_CURVE_IGNORE_POINT				1
#define			DRAW_CURVE_RELOCATION				2
#define			DRAW_CURVE_LINK_POINTS				4
#define			DRAW_CURVE_NEW_POINT				8

const	int		CURVE_ARROW_LNE			= 8;
const	int		CURVE_ARROW_INCLINE		= 2;


//�Ф�����
#define			RULE_NONE				0x0000			//�L�Ф�
#define			RULE_VERTICAL			0x0001			//�����Ф�
#define			RULE_HORIZONTAL			0x0002			//�����Ф�
#define			RULE_NO_SHOW			0x0004			//����ܼФ�

const	int 	MAXPOINT = 600;//�@�ӽu�үह�\���̤j�I��	fans add
const	int 	MAXPOINTEX = 2000;/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef	struct tagCURVEPOINT
{
	double	dX;
	double 	dY;
}	CURVEPOINT;

typedef	struct tagLINE
{
	DWORD	dwColor;		// ���u���C��
	int		nFlag;			// ���u����
	char	*pszName;		// ���u�W��
	int		nRefernce;		//	2008/4/2 10:51�W��	���u�����Шt	fans add
	union
	{
		struct
		{
			CURVEPOINT	PTStart;		// ���u���_�l�I
			CURVEPOINT	PTEnd;			// ���u�����I
		};
		struct
		{
			int 		nCount;			// ���u�I���ƥ�
			CURVEPOINT 	*pValue;		// ���u�I���Ʋժ����w
		};
	};
}	LINE;

typedef struct tagLineBuffer
{
	DWORD	dwColor;
	int		nRefernce;		//	2008/4/2 10:51�W��	���u�����Шt	fans add
	struct
	{
		int 		nLineCount;			//	�аOlinebuffer�����h�ֱ���
		int			nIndex;				//	��ݭn���ɭԡA�аO�۬O��Buffer����������?�i��F�ާ@
	};
	struct
	{
		int			nCount[CURVE_BUFFERLINE_MAX];	//	�C����?�������I��,���`�ƬOMAXPOINT�O�@�ӱ`��
		CURVEPOINT* pValue[CURVE_BUFFERLINE_MAX];	//	���骺����
	};
}	LINEBUFFER;

typedef	struct tagCURVE
{
	int			nFlag;					// ���u����
	int			nXDigits;				// X�b�p�Ʀ��
	int			nYDigits;				// Y�b�p�Ʀ��
	int			nCol;					// ��ܦC��
	int			nRow;					// ��ܦ��

	char		*pszName;				// �Ϫ�W��
	char		*pszXName;				// X�b�W��
	char		*pszYName;				// Y�b�W��
	
	LINE		*pLine[CURVE_LINE_MAX];
}	CURVE;

typedef	struct tagCURVEEx
{
	int			nFlag;					// ���u����
	int			nXDigits;				// X�b�p�Ʀ��
	int			nYDigits;				// Y�b�p�Ʀ��
	int			nYExDigits;
	int			nCol;					// ��ܦC��
	int			nRow;					// ��ܦ��

	char		*pszName;				// �Ϫ�W��
	char		*pszXName;				// X�b�W��
	char		*pszYName;				// Y�b�W��
	char		*pszYNameEx;
	LINE		*pLine[CURVE_LINE_MAX];
}	CURVEEX;

//	fans add
typedef struct tagtmPoint
{
	int ix;
	int iy;
}tmPoint;


typedef	struct	tagtmRectRelation
{
	RECT* 				pRect;
	tagtmRectRelation*	pPre;
	tagtmRectRelation*	pNext;
	//int					nPreRelationType;	//CURVE_XXPOS_TOP			= 0x00000010;	//	top     
	//int					nNextRelationType;  //CURVE_XXPOS_BOTTOM		= 0x00000020;	//	bottom  
	//                                        //CURVE_XXPOS_LEFT		= 0x00000040;	//	left    
	//                                        //CURVE_XXPOS_RIGHT		= 0x00000080;	//	right   
	
}	tmRectRelation;


/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
typedef	int	(* pFuncShowRuleStr)(double, char*, double*);
/*===========================================================================+
|           Class declaration - CtmCurve                                     |
+===========================================================================*/
class	CCurve	:	public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CCurve)
	public:
		CCurve(tmWND* pwnd, CURVE* pCurve);
		~CCurve();
		
		CCurve();	//	fans
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		double				m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X�b�MY�b��̤ܳj�ȳ̤p��
		double				m_dXStart, m_dXEnd, m_dYStart, m_dYEnd;	// X�b�_�l���ЩM�פ��
		double				m_dXUnit, m_dYUnit; 				// �C���I�N���ȡC
		RECT				m_rectCoordinate;					// �۹�󤸥�rectClient����m	
		CURVE				*m_pCurve;
		BOOL				m_bTableLine;						//Line Table visible/disable James add 2011/7/13 �U�� 05:19:16
		int					m_nXRulerLineColor;						//Line x color
		int					m_nYRulerLineColor;						//Line y color
		//LINE				**m_pLine;	fans mark
		LINE				m_pLine[CURVE_LINE_MAX];//	fans add
		ImageBackground 	m_Image;
		//	fans add
		CURVE				m_Curve;
		char*				pszCurveNameID;
		char*				pszXNameID;
		char*				pxzYNameID;
		char*				m_pszLineNameID[CURVE_LINE_MAX];	
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		
		LINEBUFFER*			m_BufferLine[CURVE_LINE_MAX];	//	save point buffer
		int					m_nALineIndex[CURVE_LINE_MAX];		//	save current index in the point buffer
		int					m_nALineNum[CURVE_LINE_MAX];		//	save current num in the 
		
		//	�ثe�ݨӬO���ݭn�Ҽ{���A�ҥH�U�����L�Ϊ�
		int					m_iLineCout;			//	���u���ƥ�
		//char*				m_pszXPointID[CURVE_LINE_MAX][MAXPOINT];	//	�C���I�ҹ�������Ʈw��ID�]x�b�W���^
		//char*				m_pszYPointID[CURVE_LINE_MAX][MAXPOINT];	//	�C���I�ҹ�������Ʈw��ID�]Y�b�W���^
		
		//	2009-2-10 10:01:31 Fans Add
		double				m_dXMinSet, m_dXMaxSet, m_dYMinSet, m_dYMaxSet;
		int					m_nSettingState;	//	0x0001 x min setting; 0x0002 x max setting; 0x0004 y min setting; 0x0008 y max setting
		int					m_nMaxPoint;		// James add 2015/1/19 16:16:30
		
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		void	Show();
		void	Update();
		
		void 	SetPropertys();	//	fans add
		void 	SetPropertys_Nub();	
	    BOOL 	CreateA();		//	fans add
		void	SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount);	//	fans add
		void	SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount);	//	fans add
		
		//	set point buffer for the line that name is pszLineName
		void	SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount);	//	fans add 
		//	set point buffer for the line that index is iLineIndex
		void	SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount);	//	fans add
		
		BOOL	SetLineNum(char* pszLineName, int iNum);
		BOOL	SetLineNum(int	iLineIndex, int iNum);
		
		//	����ĴX���I������
		tmPoint	GetSpecialPoint(int iLineIndex, int iPointIndex);				//	fans add
		//	�ھڵ��w��X�AY�ȡA�ӭp��X���I������
		tmPoint	GetSpecilaPonitA(double dX, double dY);							//	fans add
		
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
	//	int		AddLine(LINE *pLine);
	//	int		DeleteLine(int nIndex);
	
	
		//	2009-2-10 11:44:40 fans add
		bool SetPropValueT(char* pszPropName, double Value);
	private:
		int		GetRange();			// ��X,Y�b�̤j�ȳ̤p��
		int		DrawCutLine();		// �e�Ϩ�
		int		DrawCoordinate();
		int		DrawLines();
		int		DrawBeeLine(LINE *pLine);
		int		DrawCurveLine(LINE *pLine);
		int		DrawBufferLine();
};


class	CtmCurveEx: public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmCurveEx)
	public:
		CtmCurveEx();
		~CtmCurveEx();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//===========
		//	the operations inherit from CtmWnd
		//===========
		BOOL 	CreateA();
		void 	SetPropertys();	
		void 	SetPropertys_Nub();	
		void	Show();
		void	Update();
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		void    OnGetFocus();			
	  void    OnLoseFocus();			
		WORD    OnKey(WORD wKey);
		//===========
		//	the owner's operations
		//===========
		void	SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPointT(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount, int nReference = 0);	
		BOOL	SetLineNum(char* pszLineName, int iNum);
		BOOL	SetLineNum(int	iLineIndex, int iNum);
		int		GetIndexBaseOnX(CURVEPOINT 	*pValue, int iCount, double dX);
		void	SetShowFunc(pFuncShowRuleStr pFunc);
		int		GetRuleType() { return m_RuleType; }
		int		GetPointIndex() { return m_nPointIndex; }
		BOOL	OnFocus() { return m_bOnFocus;}
	protected:
		RECT	GetTitleRect();					//	Title���d��           
		RECT	GetCutRect();					//	��o�ϨҪ��d��        
		RECT	GetXNameRect();					//	X�b�W�٪��d��
		RECT	GetXAxesScaleRect();			//	���X�b���y�ЭȪ��d�� 
		RECT	GetYNameRect();					//	Y�b�W�٪��d��         
		RECT	GetYAxesScaleRect();			//	���Y�b���y�ЭȪ��d�� 
		RECT	GetLineRect();					//	����u�����e�Ͻd��    
	
		void	SeperateArea();					//	���εe�Ϫ��ϰ�        
		
		void	DrawTitle();
		void	DrawAxesGrid();
		void	DrawAxes();
		void	DrawArrow();
		
		void	DrawName();	
		void	DrawXName();
		void	DrawYName();
			
		void	DrawSclae();
		void	DrawXScale();
		void	DrawYScale();
		void	DrawCoordinate();
		
		int		GetRangeXY();					// ��X,Y�b�̤j�ȳ̤p��
		int		DrawCutLine();					// �e�Ϩ�
		void	DrawLines();
		void	DrawCurveLine(LINE *pLine);
		void	DrawCurveLine(LINE *pLine, int nLineIndex);

		void	PropareShow();
		void	ShowTitle();
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		short	ShowExtentEx(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		int		ComposeYRuleStrDef();
		int		ComposeXRuleStrDef();
		int		ShowYRuleStr();
		int		ShowXRuleStr();
		void	DrawRuleMark();
		void	DrawRuleMark(int nLineIndex);
		void	DrawRuleLine();	
		void	GetXPosValue();	
		int		GetPointLocation(tmPoint *point, int nLineIndex = -1, int nPointIndex = -1);
		void	DrawHRuleLine();
		void	DrawVRuleLine();
		int		FindNextPoint(int nBaseIndex, int nOffset);
	private:
        int		DrawBufferLine();
        void    InitData();
		BOOL	FindRelationFirst(tmRectRelation* pSource, tmRectRelation** pDest);
		BOOL	FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest);
	
		BOOL	AddTopRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddDownRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddLeftRelation(tmRectRelation* pSource, int nWidth);
		BOOL	AddRightRelation(tmRectRelation* pSource, int nWidth);
		RECT	FindLast(tmRectRelation* pSource);
		int		GetLastLeft();
		int		GetLastRight();
		int		GetLastTop();
		int		GetLastBottom();
		
		public:
		void	SetOpMode(int nMode);
		int		GetOpMode();
		void	SwitchOpMode();
		BOOL	MouseUp(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16
		BOOL	MouseMove(short x_pos,short y_pos,int key_flag);
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);	// fans 2008/6/16 11:29�W��
		
		private:
		//void	AdjustPoint(double dXDelta, double dYDelta);
		//void	MovePoint(double dXDelta, double dYDelta);
		//void	MoveCurve(double dXDelta, double dYDelta);
		//void	Operate(double dXDelta, double dYDelta);
		void	MoveCoordinate(double dXDelta, double dYDelta);
		void	ResetCoordinate();
		void	StretchCurve(double dRatio);
		//void	AutoSetPoint();
		int		GetIndex(char *pszDBID, int nOffset);
		BOOL	CheckValid(int nLineIndex, int nPointIndex);
		
		//void	SetDirection(BOOL bAdd);
		tmPoint*	GetPointPos(tmPoint *pPoint, int nLineIndex, double dX, double dY);
		tmPoint*	GetPointPos(tmPoint *pPoint, int nLineIndex, int nPointIndex);
		int 	GenerateEndPoints(int nLineIndex, CURVEPOINT *p1, CURVEPOINT *p2, 
												tmPoint *pPoints[], int pNum);
		void	ConvertPos(short x_pos, short y_pos);
		CURVEPOINT	*GetPointValue(CURVEPOINT *pValue, tmPoint *pPoint);
		void	CalcPointValueDelta();

	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		ImageBackground 	m_Image;
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		double	m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X�b�MY�b��̤ܳj�ȳ̤p��
		double	m_dXStart, m_dXEnd, m_dYStart, m_dYEnd;	// X�b�_�l���ЩM�פ��
		double	m_dXUnit, m_dYUnit; 				// �C���I�N���ȡC
		
		double	m_dYMin1, m_dYMax1, m_dYMin2, m_dYMax2;
		
		BOOL	m_bDoubleBuffer;				//	double buffer's on-off
		BOOL	m_bXName;						//	the on-off for shoowing XName
		BOOL	m_bYName;						//	the on-off for showing YName
		BOOL	m_bXArrow;						//	the on-off for the x arrow
		BOOL	m_bYArrow;						//	the on-off for the y arrow
		
		BOOL	m_bShowGrid;
		
		int					m_nHRulePos;			//�����Ф�Y�b���Ц�m(�۹理��)
		int					m_nVRulePos;			//�����Ф�X�b���Ц�m
		int					m_RuleType;				//�Ф�����(����/����)
		int					m_nPointIndex;
		int					m_nLineIndex;
		
		
		int		m_nShowXAxesType;				//	the type for XAxes
		int		m_nShowYAxesType;
		int		m_nShowXScaleType;				//	the type for x value
		int		m_nShowYScaleType;				//	the type for y value
		int		m_nShowTitleType;				//	the type for the curve's title
		int		m_nShowCutLineType;				//	the type for the cut curve
		
		char*	m_pszCurveNameID;				//	the name's id of the curve
		char*	m_pszXNameID;					//	the name's id of the x
		char*	m_pszYNameID;					//	the name's id of the y
		//char*	m_pszCurveName;					//	the name of the curve
		//char*	m_pszXName;						//	the name of the x
		//char*	m_pszYName;						//	the name of the y
		
		int		m_nXCoordinateColor;
		int		m_nYCoordinateColor;
		int		m_nXAxesColor;
		int		m_nYAxesColor;
		
		int		m_nXRange;		//�T�w���ШtX�b�d��p��覡�A0�N���q�{�Ҧ��A��L�ȥN��ϥΦ����u���d��
		int		m_nInterval;
		
		CURVE				m_Curve;
		LINE				m_pLine[CURVE_LINE_MAX];		
		LINEBUFFER*			m_BufferLine[CURVE_LINE_MAX];		//	save point buffer
		int					m_nALineIndex[CURVE_LINE_MAX];		//	save current index in the point buffer
		int					m_nALineNum[CURVE_LINE_MAX];		//	save current num in the 
		char*				m_pszLineNameID[CURVE_LINE_MAX];	
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		int					m_XRuleColor;			//	property
		int					m_YRuleColor;			//	property
		char				m_szYRule[256];			//	the show string of the y rule
		char				m_szXRule[256];			//	the show string of the x rule
		pFuncShowRuleStr	m_pFuncYStr;			//	the function pointer of the formlib, compose the show str
		pFuncShowRuleStr	m_pFuncXStr;			//	the function pointer of the formlib, compose the show str
		int					m_xPos;					//	the X pos of the y rule line, onle need it, beacuase the y can't changed
		int					m_yPos;					//	the y pos of the x rule line, onle need it, beacuase the x can't changed 
		double				m_dXPosValue;			//	
		int					m_xPerMove;				//	on pressing key(left & right), that move the piexs. it's a property
		int					m_yPerMove;				//	on pressing key(Up & Down), that move the piexs. it's a property
		int					m_yRuleStrXPos;			//	the x pos of the y rule show string
		int					m_yRuleStrYPos;			//	the y pos of the y rule show string
		int					m_xRuleStrXPos;			//	the x pos of the x rule show string
		int					m_xRuleStrYPos;			//	the y pos of the x rule show string
		double				m_dfPercent;			//	
		char*				m_pszPrompt;					
 		char*				m_pszPromptID;
	
		BOOL				m_bTouchSupport;
		int					m_nMaxPoint;		// James add 2015/1/19 16:16:30
	private:
		RECT	m_rectTitle;
		RECT	m_rectCutLine;
		RECT	m_rectXName;	
		RECT	m_rectXAxesScale;				//
		RECT	m_rectYName;		
		RECT	m_rectYAxesScale;				//	
		RECT	m_rectLine;
		RECT	m_rectCoordinate;					// �۹�󤸥�rectClient����m	
		
		tmRectRelation	m_relationTitle;        
		tmRectRelation	m_relationCutLine;      
		tmRectRelation	m_relationXName;	    
		tmRectRelation	m_relationXAxesScale;	
		tmRectRelation	m_relationYName;		
		tmRectRelation	m_relationYAxesScale;	
		tmRectRelation	m_relationLine;         
		
		
		
		
		
		BOOL	m_bTitle;						//	the on-off for the title
		BOOL	m_bCutLine;
		//RECT	m_rectCoordinate;				//	
		
		tmRectRelation	m_relationLeft;
		tmRectRelation	m_relationRight;
		tmRectRelation	m_relationUp;
		tmRectRelation 	m_relationDown;
		
		int	m_nReferenceFrame;
		
	private:	
		double			m_dOldXMin, m_dOldXMax;
		double			m_dOldYMin, m_dOldYMax, m_dOldYMin2, m_dOldYMax2;
		tmPoint			m_CurPos;
		int				m_nOpMode;			//�վ�Ҧ�
		BOOL			m_bOut;
		int				m_nXPosDelta, m_nYPosDelta;
		double			m_dXValueDelta, m_dYValueDelta;
		
};


class	CtmCurveEx2: public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmCurveEx2)
	public:
		CtmCurveEx2();
		~CtmCurveEx2();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//===========
		//	the operations inherit from CtmWnd
		//===========
		BOOL 	CreateA();
		void 	SetPropertys();	
		void 	SetPropertys_Nub();	
		void	Show();
		void	Update();
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		void    OnGetFocus();			
	  	void    OnLoseFocus();			
		WORD    OnKey(WORD wKey);
		//===========
		//	the owner's operations
		//===========
		void	SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPointT(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount, int nReference = 0);	
		BOOL	SetLineNum(char* pszLineName, int iNum);
		BOOL	SetLineNum(int	iLineIndex, int iNum);
		int		GetIndexBaseOnX(CURVEPOINT 	*pValue, int iCount, double dX);
		void	SetShowFunc(pFuncShowRuleStr pFunc);
		int		GetRuleType() { return m_RuleType; }
		int		GetPointIndex() { return m_nPointIndex; }
		BOOL	OnFocus() { return m_bOnFocus;}
	protected:
		RECT	GetTitleRect();					//	Title���d��           
		RECT	GetCutRect();					//	��o�ϨҪ��d��        
		RECT	GetXNameRect();					//	X�b�W�٪��d��
		RECT	GetXAxesScaleRect();			//	���X�b���y�ЭȪ��d�� 
		RECT	GetYNameRect();					//	Y�b�W�٪��d��         
		RECT	GetYAxesScaleRect();			//	���Y�b���y�ЭȪ��d�� 
		RECT	GetLineRect();					//	����u�����e�Ͻd��    
	
		void	SeperateArea();					//	���εe�Ϫ��ϰ�        
		
		void	DrawTitle();
		void	DrawAxesGrid();
		void	DrawAxes();
		void	DrawArrow();
		
		void	DrawName();	
		void	DrawXName();
		void	DrawYName();
			
		void	DrawSclae();
		void	DrawXScale();
		void	DrawYScale();
		void	DrawCoordinate();
		
		int		GetRangeXY();					// ��X,Y�b�̤j�ȳ̤p��
		int		DrawCutLine();					// �e�Ϩ�
		void	DrawLines();
		void	DrawCurveLine(LINE *pLine);
		void	DrawCurveLine(LINE *pLine, int nLineIndex);

		void	PropareShow();
		void	ShowTitle();
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		short	ShowExtentEx(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		int		ComposeYRuleStrDef();
		int		ComposeXRuleStrDef();
		int		ShowYRuleStr();
		int		ShowXRuleStr();
		void	DrawRuleMark();
		void	DrawRuleMark(int nLineIndex);
		void	DrawRuleLine();	
		void	GetXPosValue();	
		int		GetPointLocation(tmPoint *point, int nLineIndex = -1, int nPointIndex = -1);
		void	DrawHRuleLine();
		void	DrawVRuleLine();
		int		FindNextPoint(int nBaseIndex, int nOffset);
	private:
        int		DrawBufferLine();
        void    InitData();
		BOOL	FindRelationFirst(tmRectRelation* pSource, tmRectRelation** pDest);
		BOOL	FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest);
	
		BOOL	AddTopRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddDownRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddLeftRelation(tmRectRelation* pSource, int nWidth);
		BOOL	AddRightRelation(tmRectRelation* pSource, int nWidth);
		RECT	FindLast(tmRectRelation* pSource);
		int		GetLastLeft();
		int		GetLastRight();
		int		GetLastTop();
		int		GetLastBottom();
		
		public:
		void	SetOpMode(int nMode);
		int		GetOpMode();
		void	SwitchOpMode();
		BOOL	MouseUp(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16
		BOOL	MouseMove(short x_pos,short y_pos,int key_flag);
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);	// fans 2008/6/16 11:29�W��
		
		private:
		
		void	MoveCoordinate(double dXDelta, double dYDelta);
		void	ResetCoordinate();
		void	StretchCurve(double dRatio);
		
		int		GetIndex(char *pszDBID, int nOffset);
		BOOL	CheckValid(int nLineIndex, int nPointIndex);
		
		
		tmPoint*	GetPointPos(tmPoint *pPoint, int nLineIndex, double dX, double dY);
		tmPoint*	GetPointPos(tmPoint *pPoint, int nLineIndex, int nPointIndex);
		int 	GenerateEndPoints(int nLineIndex, CURVEPOINT *p1, CURVEPOINT *p2, 
												tmPoint *pPoints[], int pNum);
		void	ConvertPos(short x_pos, short y_pos);
		CURVEPOINT	*GetPointValue(CURVEPOINT *pValue, tmPoint *pPoint, int nLineIndex);
		void	CalcPointValueDelta();

	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		ImageBackground 	m_Image;
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		double	m_dXMin, m_dXMax, m_dYMin, m_dYMax, m_dYExMin, m_dYExMax;	// X�b�MY�b��̤ܳj�ȳ̤p��
		double	m_dXStart, m_dXEnd, m_dYStart, m_dYEnd;	// X�b�_�l���ЩM�פ��
        double	m_dXUnit, m_dYUnit,m_dYExUnit; 				// �C���I�N���ȡC
		
		BOOL	m_bDoubleBuffer;				//	double buffer's on-off
		BOOL	m_bXName;						//	the on-off for shoowing XName
		BOOL	m_bYName;						//	the on-off for showing YName
		BOOL	m_bYNameEx;
		
		BOOL	m_bShowGrid;
		
		int					m_nHRulePos;			//�����Ф�Y�b���Ц�m(�۹理��)
		int					m_nVRulePos;			//�����Ф�X�b���Ц�m
		int					m_RuleType;				//�Ф�����(����/����)
		int					m_nPointIndex;
		int					m_nLineIndex;
		
		
		int		m_nShowXAxesType;				//	the type for XAxes
		int		m_nShowYAxesType;
		int		m_nShowXScaleType;				//	the type for x value
		int		m_nShowYScaleType;				//	the type for y value
		int		m_nShowTitleType;				//	the type for the curve's title
		int		m_nShowCutLineType;				//	the type for the cut curve
		
		char*	m_pszCurveNameID;				//	the name's id of the curve
		char*	m_pszXNameID;					//	the name's id of the x
		char*	m_pszYNameID;					//	the name's id of the y
		char* 	m_pszYNameExID;
		
		int		m_nXCoordinateColor;
		int		m_nYCoordinateColor;
		int		m_nXAxesColor;
		int		m_nYAxesColor;
		
		int		m_nXRange;		//�T�w���ШtX�b�d��p��覡�A0�N���q�{�Ҧ��A��L�ȥN��ϥΦ����u���d��
		int		m_nInterval;
		
		CURVEEX				m_Curve;
		LINE				m_pLine[CURVE_LINE_MAX];		
		LINEBUFFER*			m_BufferLine[CURVE_LINE_MAX];		//	save point buffer
		int					m_nALineIndex[CURVE_LINE_MAX];		//	save current index in the point buffer
		int					m_nALineNum[CURVE_LINE_MAX];		//	save current num in the 
		char*				m_pszLineNameID[CURVE_LINE_MAX];	
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52�W��
		//=======================================	
		int					m_XRuleColor;			//	property
		int					m_YRuleColor;			//	property
		char				m_szYRule[256];			//	the show string of the y rule
		char				m_szXRule[256];			//	the show string of the x rule
		pFuncShowRuleStr	m_pFuncYStr;			//	the function pointer of the formlib, compose the show str
		pFuncShowRuleStr	m_pFuncXStr;			//	the function pointer of the formlib, compose the show str
		int					m_xPos;					//	the X pos of the y rule line, onle need it, beacuase the y can't changed
		int					m_yPos;					//	the y pos of the x rule line, onle need it, beacuase the x can't changed 
		double				m_dXPosValue;			//	
		int					m_xPerMove;				//	on pressing key(left & right), that move the piexs. it's a property
		int					m_yPerMove;				//	on pressing key(Up & Down), that move the piexs. it's a property
		int					m_xMoveInterval;
		int					m_yMoveInterval;
		int					m_yRuleStrXPos;			//	the x pos of the y rule show string
		int					m_yRuleStrYPos;			//	the y pos of the y rule show string
		int					m_xRuleStrXPos;			//	the x pos of the x rule show string
		int					m_xRuleStrYPos;			//	the y pos of the x rule show string
		double				m_dfPercent;			//	
		char*				m_pszPrompt;					
 		char*				m_pszPromptID;
	
		BOOL				m_bTouchSupport;
		int					m_nMaxPoint;		// James add 2015/1/19 16:16:30
	private:
		RECT	m_rectTitle;
		RECT	m_rectCutLine;
		RECT	m_rectXName;	
		RECT	m_rectXAxesScale;				//
		RECT	m_rectYName;		
		RECT	m_rectYNameEx;
		RECT	m_rectYAxesScale;				//	
		RECT	m_rectYAxesScaleEx;	
		RECT	m_rectLine;
		RECT	m_rectCoordinate;					// �۹�󤸥�rectClient����m	
		
		tmRectRelation	m_relationTitle;        
		tmRectRelation	m_relationCutLine;      
		tmRectRelation	m_relationXName;	    
		tmRectRelation	m_relationXAxesScale;	
		tmRectRelation	m_relationYName;
		tmRectRelation	m_relationYNameEx;		
		tmRectRelation	m_relationYAxesScale;	
		tmRectRelation	m_relationYAxesScaleEx;
		tmRectRelation	m_relationLine;         
		
		
		
		
		BOOL	m_bTitle;						//	the on-off for the title
		BOOL	m_bCutLine;
		
		tmRectRelation	m_relationLeft;
		tmRectRelation	m_relationRight;
		tmRectRelation	m_relationUp;
		tmRectRelation 	m_relationDown;
		
		
	private:	
		double			m_dOldXMin, m_dOldXMax;
		double			m_dOldYMin, m_dOldYMax, m_dOldYMin2, m_dOldYMax2;
		double		    m_dOldYExMin , m_dOldYExMax ;
		tmPoint			m_CurPos;
		int				m_nOpMode;			//�վ�Ҧ�
		BOOL			m_bOut;
		int				m_nXPosDelta, m_nYPosDelta;
		double			m_dXValueDelta, m_dYValueDelta;
		BOOL			m_bRuleMark;
		
};
#endif
