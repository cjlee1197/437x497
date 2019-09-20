/*Created by J.Wong 2017/05/02*/
#include "drawtext.h"
#include "datagrid_p.h"
#include "datagrid.h"
#include "utils.h"
#include "ParseXMLRecord.h"

CAbstractItem::CAbstractItem(CDataGrid* grid):
mGrid(grid),
mVisible(true),
mRect(CPoint(0,0),CSize(60,20)),
mPadding(5),
mItemType(itNone),
mRow(-1),
mCol(-1)
{
	
}

CAbstractItem::~CAbstractItem()
{
	
}

void CAbstractItem::setParent(CDataGrid* grid)
{
	mGrid=grid;
}
		
bool CAbstractItem::visible()const
{
	return mVisible;
}

void CAbstractItem::setVisible(bool visible)
{
	mVisible=visible;
}
		
int CAbstractItem::padding()const
{
	return mPadding;
}

void CAbstractItem::setPadding(int padding)
{
	mPadding=padding;
}
		
CRect CAbstractItem::rect()const
{
	return mRect;
}

void CAbstractItem::setRect(const CRect& rect)
{
	mRect=rect;
}
		
void CAbstractItem::setTranslation(const std::string& str)
{
	mTranslationStr.clear();
	//process
	int	nIndex = 0;
	char* pszDest=NULL;
	bool bIsRecord=false;
	if(!str.empty())
	{
		pszDest=(char*)malloc(str.size()+1);
		memset(pszDest,0,str.size()+1);
		Trim(pszDest,(char*)str.c_str(),str.size());
	}
	
	nIndex = g_pParseXMLRecord->GetRecordIndex(pszDest);
	if(nIndex<0)
	{
		nIndex = g_pParseXMLViewStr->GetRecordIndex(pszDest);
		bIsRecord=FALSE;
	}
	
	if(pszDest!=NULL)
	{
		free(pszDest);	
	}
	
	if(nIndex >=0)
	{
		STRINGUNICODE	stringUnicode;
		if(bIsRecord)
		{
			stringUnicode = g_pParseXMLRecord->GetRecordStr(nIndex);
		}
		else
			stringUnicode = g_pParseXMLViewStr->GetRecordStr(nIndex);
		
		DWORD* pUnicode=stringUnicode.pdwStrUnicode;
		
		if(pUnicode)
		{
			while(*pUnicode)
			{
				mTranslationStr<<*pUnicode;
				++pUnicode;
			}
			mTranslationStr<<0;
		}
	}
	

}

CVector<uint32> CAbstractItem::translation()const
{
	return mTranslationStr;
}
		
		
bool CAbstractItem::selectEvent(TouchType type, const CPoint& pos)
{
	//process
	//std::cout<<"itemtype:"<<mItemType<<std::endl;
	return false;
}

void CAbstractItem::draw(CPainter* painter)
{
	//draw
}

WORD CAbstractItem::itemType()const
{
	return  mItemType;
}

void CAbstractItem::setItemType(WORD type)
{
	mItemType=type;
}



CItemCheckBox::CItemCheckBox(CDataGrid* grid):
CAbstractItem(grid),
mStatus(false),
touchInArea(false),
mUnSelectedBitmap(0),
mSelectedBitmap(0)
{
	setItemType(itCheckBox);
}

CItemCheckBox::~CItemCheckBox()
{
	if(mUnSelectedBitmap)
		delete mUnSelectedBitmap;
	if(mSelectedBitmap)
		delete mSelectedBitmap;
}
		
void CItemCheckBox::draw(CPainter* painter)
{
	if(painter)
	{
		int square=mMin(mRect.width(),mRect.height());
		CRect rect=CRect(mRect.topLeft(),CSize(15,15));
		rect=rect.translated(mRect.center()-rect.center());
		
		if(mStatus)
		{
			if(!mSelectedBitmap)
			{
				painter->setPen(CPen(0x80acacac));
				painter->drawRect(rect);
				painter->setPen(CPen(Gemini::NoPen));
				painter->setBrush(CBrush(Gemini::blue));
				painter->drawRect(rect-CMargin(2,2,1,1));
			}
			else
			{
				
				if(mSelectedBitmap->Data())
				{
					CImage image((const uint8*)mSelectedBitmap->Data(), (int)mSelectedBitmap->GetWidth(), 
								(int)mSelectedBitmap->GetHeight(),CImage::Format_RGB32);
									
					CImage mirror=image.mirrored();
					painter->drawImage(rect,mirror);
				}
			}
		}
		else
		{
			if(!mUnSelectedBitmap)
			{
				painter->setPen(CPen(0x80acacac));
				painter->drawRect(rect);
			}
			else
			{
				
				if(mUnSelectedBitmap->Data())
				{
					CImage image((const uint8*)mUnSelectedBitmap->Data(), (int)mUnSelectedBitmap->GetWidth(),
								(int)mUnSelectedBitmap->GetHeight(),CImage::Format_RGB32);
					CImage mirror=image.mirrored();
					painter->drawImage(rect,mirror);
				}
			}
		}
	}
}

void CItemCheckBox::setBmpPath(const std::string& selectedPath, const std::string& unSelectedPath)
{
	setSelectedPath(selectedPath);
	setUnselectedPath(unSelectedPath);
}

void CItemCheckBox::setSelectedPath(const std::string& selectedPath)
{
	if(mSelectedBmpPath!=selectedPath)
	{
		mSelectedBmpPath=selectedPath;
		if(mSelectedBitmap)
		{
			delete mSelectedBitmap;
		}
		mSelectedBitmap=new CtmBitMap;
		mSelectedBitmap->Create((char*)selectedPath.c_str());

	}
}

void CItemCheckBox::setUnselectedPath(const std::string& unSelectedPath)
{
	if(mUnselectedBmpPath!=unSelectedPath)
	{
		mUnselectedBmpPath=unSelectedPath;
		if(mUnSelectedBitmap)
		{
			delete mUnSelectedBitmap;
		}
		mUnSelectedBitmap=new CtmBitMap;
		mUnSelectedBitmap->Create((char*)unSelectedPath.c_str());
	}
}

bool CItemCheckBox::selectEvent(TouchType type, const CPoint& pos)
{
	bool result=false;
	if(type==ttTouchDown)
	{
		if(mRect.contains(pos))
		{
			
			touchInArea=true;
			result=true;
		}
	}
	else if(type==ttTouchUp)
	{
		if(mRect.contains(pos))
		{
			touchInArea=false;
			mStatus=!mStatus;
			result=true;
		}
		touchInArea=false;
	}
	return result;
}
		
bool CItemCheckBox::status()const
{
	return mStatus;
}

void CItemCheckBox::setStatus(bool status)
{
	mStatus=status;
}



CItemImageBox::CItemImageBox(CDataGrid* grid):
CAbstractItem(grid),
mBitmap(0)
{
	setItemType(itImageBox);
}

CItemImageBox::~CItemImageBox()
{
	if(mBitmap)
	{
		delete mBitmap;
	}
}

void CItemImageBox::draw(CPainter* painter)
{
	if(painter)
	{
		if(mBitmap)
		{
			if(mBitmap->Data())
			{
				CRect rect=mRect-CMargin(mPadding,mPadding,mPadding,mPadding);
				CImage image((const uint8*)mBitmap->Data(), (int)mBitmap->GetWidth(),
							(int)mBitmap->GetHeight(),CImage::Format_RGB32);
				CImage mirror=image.mirrored();
				painter->drawImage(rect,mirror);
			}
		}
	}
}
		
void CItemImageBox::setImagePath(const std::string& imagePath)
{
	if(mImagePath!=imagePath)
	{
		mImagePath=imagePath;
		if(mBitmap)
		{
			delete mBitmap;
		}
		mBitmap= new CtmBitMap;
		mBitmap->Create((char*)imagePath.c_str());
	}
}

std::string CItemImageBox::imagePath()const
{
	return mImagePath;
}


CItemDigitBox::CItemDigitBox(CDataGrid* grid):
CAbstractItem(grid),
mValue(0)
{
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::black;
	setItemType(itDigitBox);
}

CItemDigitBox::~CItemDigitBox()
{
	
}
		
void CItemDigitBox::setValue(int value)
{
	mValue=value;
}

int CItemDigitBox::value()const
{
	return mValue;
}

void CItemDigitBox::setFont(const PlotFont& font)
{
	mFont=font;
}

PlotFont CItemDigitBox::font()const
{
	return mFont;
}

CItemDigitBox::ValueData CItemDigitBox::calcBounds(int value)
{
	char strValue[64];
	sprintf(strValue,"%d",value);
	ValueData data;
	data.value=std::string(strValue);
	int textLength=0;
	if(g_pDrawText)
	{
		g_pDrawText->SetFontSize(mFont.width,mFont.height);
		textLength=g_pDrawText->GetTextLength((unsigned char*)data.value.data());
	}
	data.bounds=CRect(CPoint(0,0),CSize(textLength+2,mFont.height+4));
	return data;
}

void 	CItemDigitBox::drawData(CPainter* painter,int x,int y,const ValueData& data)
 {
	 if(g_pDrawText)
	 {
		if(!data.value.empty())
		{
			g_pDrawText->SetFontSize(mFont.width,mFont.height);
			g_pDrawText->SetFontColor(mFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)data.value.c_str(),data.bounds,data.value.size());
		}	
	 }
 }
  
void CItemDigitBox::draw(CPainter* painter)
{
	if(painter)
	{
		CRect rect=mRect-CMargin(mPadding, mPadding, mPadding, mPadding);
		ValueData data=calcBounds(mValue);
		CImage dataImage=((CImage*)painter->device())->copy(rect);
		CPainter dataPainter(&dataImage);
		CPoint offset=CRect(CPoint(0,0),rect.size()).center()-data.bounds.center();
		data.bounds.translate(offset);
		drawData(&dataPainter,0,0,data);
		painter->translate(rect.topLeft());
		painter->drawImage(CPoint(0,0),dataImage);
	}
}


CItemDoubleBox::CItemDoubleBox(CDataGrid* grid):
CAbstractItem(grid),
mValue(0),
mPrecision(1)
{
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::black;
	setItemType(itDoubleBox);
}

CItemDoubleBox::~CItemDoubleBox()
{
	
}
		
void CItemDoubleBox::setFont(const PlotFont& font)
{
	mFont=font;
}

PlotFont CItemDoubleBox::font()const
{
	return mFont;
}


void CItemDoubleBox::setValue(double value)
{
	mValue=value;
}

double CItemDoubleBox::value()const
{
	return mValue;
}

void CItemDoubleBox::setPrecision(int precision)
{
	mPrecision=precision;
}

int CItemDoubleBox::precision()const
{
	return mPrecision;
}

CItemDoubleBox::ValueData CItemDoubleBox::calcBounds(double value, int precision)
{
	char strValue[64];
	sprintf(strValue,"%.*lf",precision,value);
	ValueData data;
	data.value=std::string(strValue);
	int textLength=0;
	if(g_pDrawText)
	{
		g_pDrawText->SetFontSize(mFont.width,mFont.height);
		textLength=g_pDrawText->GetTextLength((unsigned char*)data.value.data());
	}
	data.bounds=CRect(CPoint(0,0),CSize(textLength+2,mFont.height+4));
	return data;
}

void 	CItemDoubleBox::drawData(CPainter* painter,int x,int y,const ValueData& data)
{
	 if(g_pDrawText)
	 {
		if(!data.value.empty())
		{
			g_pDrawText->SetFontSize(mFont.width,mFont.height);
			g_pDrawText->SetFontColor(mFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)data.value.c_str(),data.bounds,data.value.size());
		}	
	 }
}
		
		
void CItemDoubleBox::draw(CPainter* painter)
{
	if(painter)
	{
		CRect rect=mRect-CMargin(mPadding, mPadding, mPadding, mPadding);
		ValueData data=calcBounds(mValue, mPrecision);
		CImage dataImage=((CImage*)painter->device())->copy(rect);
		
		CPoint offset=CRect(CPoint(0,0),rect.size()).center()-data.bounds.center();
		data.bounds.translate(offset);
		
		CPainter dataPainter(&dataImage);
		drawData(&dataPainter,0,0,data);
		painter->translate(rect.topLeft());
		painter->drawImage(CPoint(0,0),dataImage);
	}
}


CItemStringBox::CItemStringBox(CDataGrid* grid):
CAbstractItem(grid)
{
	
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::black;
	setItemType(itStringBox);
}

CItemStringBox::~CItemStringBox()
{
	
}
		
void CItemStringBox::setString(const std::string& str, bool translation)
{
	mTranslation=translation;
	if(translation)	
		setTranslation(str);
	else
		mStr=str;
}

void CItemStringBox::setString(WORD* str)
{
	if(!str) return ;
	if(!*str)  return;
	mTranslationStr.clear();
	while(*str)
	{
		mTranslationStr<<*str;
		++str;
	}
	mTranslationStr<<0;
	mTranslation=true;
}

void CItemStringBox::setString(const CVector<uint32>& wStr)
{
	mTranslationStr=wStr;
	if(!wStr.isEmpty())
		mTranslation=true;
}

std::string CItemStringBox::string()const
{
	return mStr;
}
		
void CItemStringBox::setFont(const PlotFont& font)
{
	mFont=font;
}

PlotFont CItemStringBox::font()const
{
	return mFont;
}

void CItemStringBox::draw(CPainter* painter)
{
	if(painter)
	{
		CRect rect=mRect-CMargin(mPadding, mPadding, mPadding, mPadding);
		
		ValueData data;
		if(!mTranslation)
			data=calcBounds(mStr);
		else
			data=calcBounds(mTranslationStr);
		
		CPoint offset=CRect(CPoint(0,0),rect.size()).center()-data.bounds.center();
		data.bounds.translate(offset);
		CImage dataImage=((CImage*)painter->device())->copy(rect);
		CPainter dataPainter(&dataImage);
		drawData(&dataPainter,0,0,data);
	
		painter->translate(rect.topLeft());
		painter->drawImage(CPoint(0,0),dataImage);
	}
}

CItemStringBox::ValueData CItemStringBox::calcBounds(std::string value)
{
	/* ValueData data;
	data.str=value;
	data.bounds=CRect(CPoint(0,0),CSize((mFont.width+1)*(data.str.size()+1)>>1,mFont.height)); */
	ValueData data;
	data.str=value;
	int textLength=0;
	if(g_pDrawText)
	{
		g_pDrawText->SetFontSize(mFont.width,mFont.height);
		textLength=g_pDrawText->GetTextLength((unsigned char*)value.data());
	}
	data.bounds=CRect(CPoint(0,0),CSize(textLength+2,mFont.height+4));
	return data;
}

CItemStringBox::ValueData CItemStringBox::calcBounds(CVector<uint32> value)
{
	ValueData data;
	data.wStr=mTranslationStr;
	int textLength=0;
	if(g_pDrawText)
	{
		g_pDrawText->SetFontSize(mFont.width,mFont.height);
		textLength=g_pDrawText->GetTextLength(value.data());
	}
	data.bounds=CRect(CPoint(0,0),CSize(textLength,mFont.height));
	return data;
}

void CItemStringBox::drawData(CPainter* painter, int x, int y, const ValueData& data)
{
	if(g_pDrawText)
	 {
		if(mTranslation)
		{
			g_pDrawText->SetFontSize(mFont.width,mFont.height);
			g_pDrawText->SetFontColor(mFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned int*)data.wStr.data(),data.bounds,data.wStr.size());
		}
		else
		{
			if(!data.str.empty())
			{
				g_pDrawText->SetFontSize(mFont.width,mFont.height);
				g_pDrawText->SetFontColor(mFont.color);
				g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)data.str.c_str(),data.bounds,data.str.size());
			}
		}
	 }
}

		
struct CModelDataDataPointerDeleter
{
    static inline void deleteData(CModelData *d)
    {
		for(int i=0; i<d->mVariants.count(); ++i)
		{
			delete d->mVariants[i];
		}
		d->mVariants.clear();
		delete d;
    }

    static inline void cleanup(CModelData *d)
    {
        if (d && !d->ref.deref()) {
            deleteData(d);
        }
    }
};

class CNullModelData
{
public:
    CModelData *model;
    CNullModelData() : model(new CModelData)
    {
        model->ref.store(1);
    }
	
    ~CNullModelData()
    {
        if (!model->ref.deref())
            delete model;
        model = 0;
    }
};



CAbstractDataModel::CAbstractDataModel():m_data(new CModelData)
{
	assert(m_data);
    m_data->ref.ref();
}

CAbstractDataModel::CAbstractDataModel(const CAbstractDataModel& model):
m_data(model.m_data.data())
{
	m_data->ref.ref();
}

CAbstractDataModel&  CAbstractDataModel::operator=(const CAbstractDataModel& model)
{
	if (m_data == model.m_data)
        return *this;

    model.m_data->ref.ref();
    m_data.reset(model.m_data.data());
    return *this;
} 

CAbstractDataModel::~CAbstractDataModel()
{
	
}


void CAbstractDataModel::cleanUp(CModelData* data)
{
	CModelDataDataPointerDeleter::deleteData(data);
}

		
		
CDataGrid::CDataGrid():
mRect(CPoint(0,0),CSize(520,240)),
mRows(6),
mCols(4),
mRowStep(0),
mColStep(0),
mPadding(5),
mHorzGridVisible(true),
mVertGridVisible(false),
mCurrentItem(0),
mHeaderHeight(40),
mPen(0xffacacac,2,Gemini::SolidLine)
{
	mHeights=CVector<double>(6,40);
	mWidths=CVector<double>(4,120);
}

CDataGrid::~CDataGrid()
{
	clear();
}

void CDataGrid::clear()
{
	clearValidAreaItems();
	models.clear();
}

void CDataGrid::clearValidAreaItems()
{
	for(int i=0; i<validAreaItems.count(); ++i)
	{
		delete validAreaItems.at(i);
	}
	validAreaItems.clear();
}


static inline void nCopy(CVector<double>& vector,std::map<int, double>&spec, double value, int n)
{
	int i=0;
	while(i<n)
	{
		if(spec[i])
			vector<<spec[i];
		else
			vector<<value;
		i++;
	}
}

void CDataGrid::setRect(const CRect& rect)
{
	double height=0;
	double width=0;
	if(rect.width()!=mRect.width()||rect.height()!=mRect.height())
	{
		mRect = rect;
		setRowCount(mRows);
		setColumnCount(mCols);
	}
}



CRect CDataGrid::rect()const
{
	return mRect;
}


void CDataGrid::setRowCount(int rows)
{
	if(rows<=0) rows=1;
	double itemHeight=(double)(mRect.height()-mHeaderHeight)/rows;
	mHeights.clear();
	nCopy(mHeights, mSpecHeights,itemHeight, rows);
	mRows = rows;
}

void CDataGrid::setColumnCount(int columns)
{
	if(columns<=0) columns=1;
	double itemWidth=(double)(mRect.width())/columns;
	mWidths.clear();
	nCopy(mWidths,mSpecWidths, itemWidth, columns);
	mCols = columns;
}


void CDataGrid::setRowHeight(int index,int height)
{
	if(index<0||index>=mRows)
	{
		mSpecHeights[index]=height;
		return ;
	}
	mHeights[index]=height;
	mSpecHeights[index]=height;
	
}

void CDataGrid::setColumnWidth(int index, int width)
{
	if(index<0||index>=mCols)
	{
		mSpecWidths[index]=width;
		return ;
	}
	mWidths[index]=width;
	mSpecWidths[index]=width;
}

int  CDataGrid::rowHeight(int index)
{
	if(index<0||index>=mRows) return 0;
	if(mSpecHeights[index])
		return mSpecHeights[index];
	return mHeights.at(index);
}

int  CDataGrid::columnWidth(int index)
{
	if(index<0||index>=mCols) return 0;
	if(mSpecWidths[index])
		return mSpecWidths[index];
	return mWidths.at(index);
}
		
int CDataGrid::rowCount()const
{
	return mRows;
}

int CDataGrid::columnCount()const
{
	return mCols;
}


int  CDataGrid::dataCountInRow(int row)const
{
	if(row<0||row>=models.count()) return 0;
	return models.at(row).elementCount();
}

int  CDataGrid::dataCount()const
{
	int total=0;
	for(int i=0; i<models.count();++i)
	{
		total+=models.at(i).elementCount();
	}
	return total;
}

int CDataGrid::dataRowCount()const
{
	return models.count();
}

int  CDataGrid::dataMaxColCount()const
{
	int max=0;
	for(int i=0; i<models.count();++i)
	{
		max=mMax(max, models.at(i).elementCount());
	}
	return max;
}

void CDataGrid::removeModelAt(int index)
{
	models.removeAt(index);
}
		
void CDataGrid::insertModel(int index, CAbstractDataModel& model)
{
	if(models.isEmpty())
		models<<model;
	else
		models.insert(index,model);
}

void CDataGrid::addModel(const CAbstractDataModel& model)
{
	models<<model;
}

		
void CDataGrid::setModels(const CList<CAbstractDataModel>& models)
{
	this->models=models;
}

	
void CDataGrid::modify(int row, int column, const CIntVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].modify(column, variant);
}
	
void CDataGrid::modify(int row, int column, const CDoubleVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].modify(column, variant);
}	
void CDataGrid::modify(int row, int column, const CCheckVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].modify(column, variant);
}

void CDataGrid::modify(int row, int column, const CImageVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].modify(column, variant);
}

void CDataGrid::modify(int row, int column, const CStringVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].modify(column, variant);
}

void CDataGrid::insert(int row, int column, const CIntVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].insert(column, variant);
}

void CDataGrid::insert(int row, int column, const CDoubleVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].insert(column, variant);
}

void CDataGrid::insert(int row, int column, const CCheckVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].insert(column, variant);
}

void CDataGrid::insert(int row, int column, const CImageVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].insert(column, variant);
}


void CDataGrid::insert(int row, int column, const CStringVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].insert(column, variant);
}



void CDataGrid::addData(int row, const CIntVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].addData(variant);
}

void CDataGrid::addData(int row, const CDoubleVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].addData(variant);
}

void CDataGrid::addData(int row, const CCheckVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].addData(variant);
}

void CDataGrid::addData(int row, const CImageVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].addData(variant);
}

void CDataGrid::addData(int row, const CStringVariant& variant)
{
	if(row<0||row>=models.count()) return;
	models[row].addData(variant);
}



CIntVariant 	CDataGrid::getIntVariant(int row, int column, bool& find)const
{
	find=false;
	if(row<0||row>=models.count()) return CIntVariant();
	return models.at(row).getIntVariant(column,find);
}

CDoubleVariant 	CDataGrid::getDoubleVariant(int row, int column, bool& find)const
{
	find=false;
	if(row<0||row>=models.count()) return CDoubleVariant();
	return models.at(row).getDoubleVariant(column,find);
}

CStringVariant 	CDataGrid::getStringVariant(int row, int column, bool& find)const
{
	find=false;
	if(row<0||row>=models.count()) return CStringVariant();
	return models.at(row).getStringVariant(column,find);
}

CImageVariant 	CDataGrid::getImageVariant(int row, int column, bool& find)const
{
	find=false;
	if(row<0||row>=models.count()) return CImageVariant();
	return models.at(row).getImageVariant(column,find);
}

CCheckVariant 	CDataGrid::getCheckVariant(int row, int column, bool& find)const
{
	find=false;
	if(row<0||row>=models.count()) return CCheckVariant();
	return models.at(row).getCheckVariant(column,find);
}

void CDataGrid::removeAt(int row, int column)
{
	if(row<0||row>=models.count()) return;
	models[row].removeAt(column);
}

void CDataGrid::rowStep(int step)
{
	mRowStep=step;
	mRowStep=mMax(0,mMin(mRowStep,models.count()-1));
}

void CDataGrid::colStep(int step)
{
	mColStep=step;
	int maxCount=0;
	for(int i=0;i<models.count();++i)
	{
		maxCount=mMax(maxCount,models.at(i).elementCount());
	}
	mColStep=mMax(0,mMin(mColStep,maxCount-1));
}

void CDataGrid::rowPageStep(int page)
{
	int step=page*mRows;
	mRowStep=0;
	rowStep(step);
}

void CDataGrid::colPageStep(int page)
{
	int step=page*mCols;
	mColStep=0;
	colStep(step);
}

void CDataGrid::updateValidAreaItems()
{
	if(models.isEmpty()) return;
	clearValidAreaItems();
	double y=0;
	double x=0;
	for(int i=0; i<mCols; ++i)
	{
		if(mHeaders.elementCount()<=(mColStep+i)) break;
		CAbstractVariant* variant=mHeaders.elementAt(mColStep+i);
		CAbstractItem* item=createItem(variant);
		CRect rect=CRect(CPoint(ceil(x),ceil(y)),CSize(floor(mWidths.at(i)),floor(mHeaderHeight)));
		rect.translate(mRect.topLeft());
		rect&=mRect;
		item->setRect(rect);
		validAreaItems<<item;
		x+=mWidths.at(i);
		x=mMin(x,mRect.width());
	}
	
	y=mHeaderHeight;
	for(int i=0; i<mRows;++i)
	{
		//行数据超出，直接结束检查
		if(models.count()<=(mRowStep+i)) return;
		CAbstractDataModel model=models.at(mRowStep+i);
		double x=0;
		for(int j=0; j<mCols; ++j)
		{
			//该列超出,检查下一列数据
			if(model.elementCount()<=(mColStep+j)) continue;
			CAbstractVariant* variant=model.elementAt(mColStep+j);
			CAbstractItem* item=createItem(variant);
			CRect rect=CRect(CPoint(ceil(x),ceil(y)),CSize(floor(mWidths.at(j)),floor(mHeights.at(i))));
			rect.translate(mRect.topLeft());
			rect&=mRect;
			item->setRect(rect);
			item->setRow(mRowStep+i);
			item->setColumn(mColStep+j);
			validAreaItems<<item;
			x+=mWidths.at(j);
			x=mMin(x, (double)mRect.width());
		}
		y+=mHeights.at(i);
		y=mMin(y,(double)mRect.height());
	}
	
}

CAbstractItem* CDataGrid::createItem(const CAbstractVariant* variant)
{
	switch(variant->itemType())
	{
		case CAbstractItem::itDigitBox:
			{
				int value=((CIntVariant*)variant)->value();
				PlotFont font=((CIntVariant*)variant)->font();
				CItemDigitBox* digitBox=new CItemDigitBox;
				digitBox->setValue(value);
				digitBox->setPadding(mPadding);
				digitBox->setFont(font);
				return digitBox;
			}
			
		case CAbstractItem::itDoubleBox:
			{
				double value=((CDoubleVariant*)variant)->value();
				int    precision=((CDoubleVariant*)variant)->precision();
				PlotFont font=((CDoubleVariant*)variant)->font();
				CItemDoubleBox* doubleBox=new CItemDoubleBox;
				doubleBox->setValue(value);
				doubleBox->setPrecision(precision);
				doubleBox->setPadding(mPadding);
				doubleBox->setFont(font);
				return doubleBox;
			}
			
		case CAbstractItem::itCheckBox:
			{
				std::string selectedPath=((CCheckVariant*)variant)->selectedImagePath();
				std::string unSelectedPath=((CCheckVariant*)variant)->unSelectedImagePath();
				bool status=((CCheckVariant*)variant)->status();
				
				CItemCheckBox* checkBox=new CItemCheckBox;
				checkBox->setStatus(status);
				checkBox->setBmpPath(selectedPath, unSelectedPath);
				checkBox->setPadding(mPadding);
				return checkBox;
			}
		
		case CAbstractItem::itImageBox:
			{
				std::string imagePath=((CImageVariant*)variant)->imagePath();
				CItemImageBox* imageBox=new CItemImageBox;
				imageBox->setImagePath(imagePath);
				imageBox->setPadding(mPadding);
				return imageBox;
			}
		case CAbstractItem::itStringBox:
			{
				CItemStringBox* stringBox= new CItemStringBox;
				std::string str=((CStringVariant*)variant)->string();
				PlotFont font=((CStringVariant*)variant)->font();
				CVector<uint32> wStr=((CStringVariant*)variant)->wString();
				bool translation=((CStringVariant*)variant)->translation();
				if(wStr.isEmpty())
				{
					stringBox->setString(str, translation);
				}
				else
				{
					stringBox->setString(wStr);
				}
				stringBox->setPadding(mPadding);
				stringBox->setFont(font);
				return stringBox;
			}
		default:
			return new CAbstractItem();
	}
}

void CDataGrid::draw(CPainter* painter)
{
	if(painter)
	{
		//draw border style
		drawGrid(painter);
		//draw items
		updateValidAreaItems();
		for(int i=0; i<validAreaItems.count();++i)
		{
			painter->save();
			CAbstractItem* item=validAreaItems.at(i);
			item->draw(painter);
			painter->restore();
		}
	}
}

void CDataGrid::drawGrid(CPainter* painter)
{
	double x=mRect.left(); 
	double y=mRect.top()+mHeaderHeight;
	double x2=mRect.right();
	double y2=mRect.bottom();
	painter->save();
	painter->setPen(mPen);
	
	if(mHorzGridVisible)
	{
		for(int i=0; i<mHeights.count(); ++i)
		{
			painter->drawLine(x, ceil(y),x2, ceil(y));
			y+=mHeights[i];
			y=mMin(y,y2);
		}
		painter->drawLine(x, ceil(y),x2, ceil(y));
	}
	if(mVertGridVisible)
	{
		y=mRect.top()+mHeaderHeight;
		y=mMin(y,y2);
		for(int i=0; i<mWidths.count();++i)
		{
			painter->drawLine(ceil(x),y,ceil(x),y2);
			x+=mWidths[i];
			x=mMin(x,x2);
				
		}
		painter->drawLine(ceil(x),y,ceil(x),y2);
	}
	painter->restore();
}


void CDataGrid::addHeader(const CStringVariant& variant)
{
	mHeaders<<variant;
}

void CDataGrid::insertHeader(int index, const CStringVariant& variant)
{
	mHeaders.insert(index,variant);
}

void CDataGrid::removeHeaderAt(int index)
{
	mHeaders.removeAt(index);
}

void CDataGrid::modifyHeader(int index, const CStringVariant& variant)
{
	mHeaders.modify(index, variant);
}

void CDataGrid::setHeader(const CVector<CStringVariant>& headers)
{
	for(int i=0; i<headers.count(); ++i)
	{
		mHeaders<<headers[i];
	}
}

bool CDataGrid::selectEvent(CAbstractItem::TouchType type, const CPoint& pos)
{
	bool result=false;
	mCurrentItem=0;
	for(int i=0; i<validAreaItems.count(); ++i)
	{
		if(validAreaItems.at(i)->rect().contains(pos))
		{
			mCurrentItem=validAreaItems.at(i);
			result=mCurrentItem->selectEvent(type, pos);
			if(type==CAbstractItem::ttTouchUp)
			{
				//将事件处理结果反馈到相应的变量对象中
				switch(mCurrentItem->itemType())
				{
					case CAbstractItem::itCheckBox:
					{
						
						int row=mCurrentItem->row();
						int col=mCurrentItem->column();
						CCheckVariant variant;
						bool find;
						variant=getCheckVariant(row,col,find);
						if(find)
						{
							variant.setStatus(((CItemCheckBox*)mCurrentItem)->status());
							modify(row, col, variant);
						}
					}
					break;
				}
			}
			break;
		}
	}

	return result;
}


CScrollBar::CScrollBar():
mTrackBrush(0x50acacac),
mArrowBrush(0xffa0a0a0),
mFaceBrush(0x80acacac),
mOrientation(Gemini::Vertical),
mStyle(bsSquare),
mMaxLimit(0),
mStep(0),
mVisible(true)
{
	mClicked[0]=FALSE;
	mClicked[1]=FALSE;
}

CScrollBar::~CScrollBar()
{
	
}

void CScrollBar::draw(CPainter* painter)
{
	if(mVisible)
	{
		if(painter)
		{
			if(mRect.isValid())
			{
				drawTrack(painter);
				drawArrow(painter);
				drawFace(painter);
			}
		}
	}
}

void CScrollBar::drawTrack(CPainter* painter)
{
	painter->save();
	painter->setPen(CPen(Gemini::NoPen));
	painter->setBrush(mTrackBrush);
	CRect rect=mRect-CMargin(4,4,4,4);
	painter->drawRect(rect);
	painter->restore();
}

void CScrollBar::drawArrow(CPainter* painter)
{
	painter->save();
	painter->setPen(CPen(mArrowBrush.color()));
	painter->setBrush(mArrowBrush);
	painter->setRenderHint(CPainter::Antialiasing,true);
	CRect rect=mRect-CMargin(4,4,4,4);
	if(mOrientation==Gemini::Vertical)
	{
		int height=rect.height();
		if(height>=40)
		{
			//up
			CLineEnding lineEnding;
			lineEnding.setStyle(CLineEnding::SpikeArrow);
			CRect upEndRect=CRect(rect.topLeft(),CSize(rect.width(),18));
			CPoint center=upEndRect.center();
			CLineF baseLine=CLineF(CPointF(center.x()+0.5f, rect.y()+lineEnding.length()),CPointF(center.x()+0.5f,rect.y()));
			CVector2D baseLineVector(baseLine.dx(), baseLine.dy());
			//draw
			if(mClicked[0])
			{
				painter->drawRect(upEndRect);
				painter->setPen(Gemini::white);
			}
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(-1),baseLineVector);
			//down
			CRect downEndRect=CRect(rect.bottomLeft()-CPoint(0,18),CSize(rect.width(),18));
			center=downEndRect.center();
			baseLine=CLineF(CPointF(center.x()+0.5f, rect.bottom()-20+lineEnding.length()),CPointF(center.x()+0.5f,rect.bottom()-20));
			CVector2D baseLine2Vector(baseLine.dx(), baseLine.dy());
			painter->setPen(CPen(mArrowBrush.color()));
			if(mClicked[1])
			{
				painter->drawRect(downEndRect);
				painter->setPen(Gemini::white);
			}
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength(),-baseLine2Vector);
		}
	}
	else
	{
		int width=rect.width();
		if(width>=40)
		{
			//left
			CLineEnding lineEnding;
			lineEnding.setStyle(CLineEnding::SpikeArrow);
			CRect upEndRect=CRect(rect.topLeft(),CSize(18,rect.height()));
			CPoint center=upEndRect.center();
			CLineF baseLine=CLineF(CPointF(rect.x()+lineEnding.length()+0.5f,center.y()+0.5f),CPointF(rect.x()+0.5f,center.y()+0.5f));
			CVector2D baseLineVector(baseLine.dx(),baseLine.dy());
			//draw
			if(mClicked[0])
			{
				painter->drawRect(upEndRect);
				painter->setPen(Gemini::white);
			}
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(-1),baseLineVector);
			//right
			CRect downEndRect=CRect(rect.topRight()-CPoint(18,0),CSize(18,rect.height()));
			center=downEndRect.center();
			baseLine=CLineF(CPointF(rect.right()-20+lineEnding.length(),center.y()+0.5f),CPointF(rect.right()-20,center.y()+0.5f));
			CVector2D baseLine2Vector(baseLine.dx(),baseLine.dy());
			painter->setPen(CPen(mArrowBrush.color()));
			if(mClicked[1])
			{
				painter->drawRect(downEndRect);
				painter->setPen(Gemini::white);
			}
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength(),-baseLine2Vector);
		}
	}
	
	painter->restore();
}

void CScrollBar::drawFace(CPainter* painter)
{
	painter->save();
	painter->setPen(CPen(Gemini::NoPen));
	painter->setBrush(CBrush(mFaceBrush));
	CRect rect=mRect-CMargin(4,4,4,4);
	if(mOrientation==Gemini::Vertical)
	{
		if(rect.height()>=44)
		{
			double height=0;
			if(mMaxLimit)
				height=(double)(rect.height()-44)/mMaxLimit;
			else
				height=rect.height()-44;
			double drawHeight=height<4?4:height;
			CRect faceRect=CRect(rect.topLeft()+CPoint(0,20),CSize(rect.width(),floor(drawHeight)));
			faceRect.translate(0,floor(mStep*height));
			painter->drawRect(faceRect);
		}
	}
	else
	{
		if(rect.width()>=44)
		{
			double width=0;
			if(mMaxLimit)
				width=(double)(rect.width()-44)/mMaxLimit;
			else
				width=rect.width()-44;
			double drawWidth=width<4?4:width;
			CRect faceRect=CRect(rect.topLeft()+CPoint(20,0),CSize(floor(drawWidth),rect.height()));
			faceRect.translate(floor(mStep*width),0);
			painter->drawRect(faceRect);
		}
	}
	painter->restore();
}


void CScrollBar::selectEvent(CAbstractItem::TouchType type, const CPoint& pos)
{
	switch(type)
	{
		case CAbstractItem::ttTouchDown:
		{
			operatorScrollBarArea(pos);
		}
		break;
		default:
		mClicked[0]=FALSE;
		mClicked[1]=FALSE;
		break;
	}
}

void CScrollBar::operatorScrollBarArea(const CPoint& pos)
{
	CRect rect=mRect-CMargin(4,4,4,4);
	if(mOrientation==Gemini::Vertical)
	{
		double height=0;
		if(mMaxLimit)
			height=(double)(rect.height()-44)/mMaxLimit;
		else
			height=rect.height()-44;
		double drawHeight=height<4?4:height;
		CRect faceRect=CRect(rect.topLeft()+CPoint(0,20),CSize(rect.width(),floor(drawHeight)));
		
		faceRect.translate(0,floor(mStep*height));
			
		CRect upRect=CRect(rect.topLeft(),faceRect.topRight());
		CRect downRect=CRect(faceRect.bottomLeft(),rect.bottomRight());
		if(upRect.contains(pos))
		{
			setStep(mStep-1);
		}
		else if(downRect.contains(pos))
		{
			setStep(mStep+1);
		}
		
		CRect upArrowRect=CRect(rect.topLeft(),CSize(rect.width(),18));
		CRect downArrowRect=CRect(rect.bottomLeft()-CPoint(0,18),CSize(rect.width(),18));
		if(upArrowRect.contains(pos))
		{
			mClicked[0]=TRUE;
		}
		else if(downArrowRect.contains(pos))
		{
			mClicked[1]=TRUE;
		}
		
	}
	else
	{
		
		double width=0;
		if(mMaxLimit)
			width=(double)(rect.width()-44)/mMaxLimit;
		else
			width=rect.width()-44;
		double drawWidth=width<4?4:width;
		CRect faceRect=CRect(rect.topLeft()+CPoint(20,0),CSize(floor(drawWidth),rect.height()));
		faceRect.translate(floor(mStep*width),0);	
		CRect leftRect=CRect(rect.topLeft(),faceRect.bottomLeft());
		CRect rightRect=CRect(faceRect.topRight(),rect.bottomRight());
		if(leftRect.contains(pos))
		{
			setStep(mStep-1);
		}
		else if(rightRect.contains(pos))
		{
			setStep(mStep+1);
		}
		CRect leftArrowRect=CRect(rect.topLeft(),CSize(18,rect.height()));
		CRect rightArrowRect=CRect(rect.topRight()-CPoint(18,0),CSize(18,rect.height()));
		
		if(leftArrowRect.contains(pos))
		{
			mClicked[0]=TRUE;
		}
		else if(rightArrowRect.contains(pos))
		{
			mClicked[1]=TRUE;
		}
	}
}




CGridDataWrapper::CGridDataWrapper(CDataGrid* grid, const CtmDataGrid* parent):
mDataGrid(grid),
mGrid(parent)
{
	
}

CGridDataWrapper::~CGridDataWrapper()
{
	detach();
}

void CGridDataWrapper::rowStep(int step)
{
	if(mDataGrid)
	{
		if(step!=mDataGrid->mRowStep)
		{
			mDataGrid->rowStep(step);
			if(mGrid)
			{
				mGrid->d_func()->properties.changed.dataGridBase=1;
				mGrid->d_func()->verticalBar->setStep(mDataGrid->row());
				mGrid->d_func()->properties.changed.scrollBarBase=1;
			}
		}
	}
}

void CGridDataWrapper::colStep(int step)
{
	if(mDataGrid)
	{
		if(step!=mDataGrid->mColStep)
		{
			mDataGrid->colStep(step);
			if(mGrid)
			{
				mGrid->d_func()->properties.changed.dataGridBase=1;
				mGrid->d_func()->horizontalBar->setStep(mDataGrid->column());
				mGrid->d_func()->properties.changed.scrollBarBase=1;
			}
		}
	}
}

void CGridDataWrapper::rowPageStep(int page)
{
	if(mDataGrid)
	{
		if(page*mDataGrid->rowCount()!=mDataGrid->mRowStep)
		{
			mDataGrid->rowPageStep(page);
			if(mGrid)
			{
				mGrid->d_func()->properties.changed.dataGridBase=1;
				mGrid->d_func()->verticalBar->setStep(mDataGrid->row());
				mGrid->d_func()->properties.changed.scrollBarBase=1;
			}
		}
	}
}

void CGridDataWrapper::colPageStep(int page)
{
	if(mDataGrid)
	{
		if(page*mDataGrid->columnCount()!=mDataGrid->mColStep)
		{
			mDataGrid->colPageStep(page);
			if(mGrid)
			{
				mGrid->d_func()->properties.changed.dataGridBase=1;
				mGrid->d_func()->horizontalBar->setStep(mDataGrid->column());
				mGrid->d_func()->properties.changed.scrollBarBase=1;
			}
		}
	}
}
		
void CGridDataWrapper::addModel(const CAbstractDataModel& model)
{
	if(mDataGrid)
	{
		mDataGrid->addModel(model);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

int  CGridDataWrapper::dataCountInRow(int row)
{
	if(mDataGrid)
		return mDataGrid->dataCountInRow(row);
	return 0;
}

int  CGridDataWrapper::dataCount()
{
	if(mDataGrid)
		return mDataGrid->dataCount();
	return 0;
}

void CGridDataWrapper::removeModelAt(int index)
{
	if(mDataGrid)
	{
		mDataGrid->removeModelAt(index);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
		
void CGridDataWrapper::insertModel(int index, CAbstractDataModel& model)
{
	if(mDataGrid)
	{
		mDataGrid->insertModel(index,model);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
	
void CGridDataWrapper::setModels(const CList<CAbstractDataModel>& models)
{
	if(mDataGrid)
	{
		mDataGrid->setModels(models);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
	
void CGridDataWrapper::modify(int row, int column, const CIntVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modify(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::modify(int row, int column, const CDoubleVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modify(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::modify(int row, int column, const CCheckVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modify(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::modify(int row, int column, const CImageVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modify(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::modify(int row, int column, const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modify(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
	
void CGridDataWrapper::insert(int row, int column, const CIntVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insert(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::insert(int row, int column, const CDoubleVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insert(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::insert(int row, int column, const CCheckVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insert(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::insert(int row, int column, const CImageVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insert(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::insert(int row, int column, const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insert(row, column, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
		
void CGridDataWrapper::addData(int row, const CIntVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addData(row, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::addData(int row, const CDoubleVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addData(row, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::addData(int row, const CImageVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addData(row, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::addData(int row, const CCheckVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addData(row, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::addData(int row, const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addData(row, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}
		
CIntVariant 	CGridDataWrapper::getIntVariant(int row, int column, bool& find)const
{
	if(mDataGrid)
	{
		return getIntVariant(row, column, find);
	}
	find=false;
	return CIntVariant();
}

CDoubleVariant 	CGridDataWrapper::getDoubleVariant(int row, int column, bool& find)const
{
	if(mDataGrid)
	{
		return getDoubleVariant(row, column, find);
	}
	find=false;
	return CDoubleVariant();
}

CStringVariant 	CGridDataWrapper::getStringVariant(int row, int column, bool& find)const
{
	if(mDataGrid)
	{
		return getStringVariant(row, column, find);
	}
	find=false;
	return CStringVariant();
}

CImageVariant 	CGridDataWrapper::getImageVariant(int row, int column, bool& find)const
{
	if(mDataGrid)
	{
		return getImageVariant(row, column, find);
	}
	find=false;
	return CImageVariant();
}

CCheckVariant 	CGridDataWrapper::getCheckVariant(int row, int column, bool& find)const
{
	if(mDataGrid)
	{
		return getCheckVariant(row, column, find);
	}
	find=false;
	return CCheckVariant();
}
		
void CGridDataWrapper::addHeader(const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->addHeader(variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::insertHeader(int index, const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->insertHeader(index,variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::removeHeaderAt(int index)
{
	if(mDataGrid)
	{
		mDataGrid->removeHeaderAt(index);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::modifyHeader(int index, const CStringVariant& variant)
{
	if(mDataGrid)
	{
		mDataGrid->modifyHeader(index, variant);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

void CGridDataWrapper::setHeader(const CVector<CStringVariant>& headers)
{
	if(mDataGrid)
	{
		mDataGrid->setHeader(headers);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}


void CGridDataWrapper::setRowHeight(int index,int height)
{
	if(mDataGrid)
	{
		if(mDataGrid->rowHeight(index)!=height)
		{
			mDataGrid->setRowHeight(index, height);
			if(mGrid)
				mGrid->d_func()->properties.changed.dataGridBase=1;
		}
	}
}

void CGridDataWrapper::setColumnWidth(int index, int width)
{
	if(mDataGrid)
	{
		if(mDataGrid->columnWidth(index)!=width)
		{
			mDataGrid->setColumnWidth(index,width);
			if(mGrid)
				mGrid->d_func()->properties.changed.dataGridBase=1;
		}
	}
}
		
void CGridDataWrapper::removeAt(int row, int column)
{
	if(mDataGrid)
	{
		mDataGrid->removeAt(row, column);
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}

CAbstractItem* CGridDataWrapper::currentItem()const
{
	if(mDataGrid)
	{
		return mDataGrid->currentItem();
	}
	return 0;
}

void CGridDataWrapper::clear()
{
	if(mDataGrid)
	{
		mDataGrid->clear();
		if(mGrid)
			mGrid->d_func()->properties.changed.dataGridBase=1;
	}
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

CDataGridPrivate::CDataGridPrivate()
{
	init();
}

CDataGridPrivate::~CDataGridPrivate()
{
	destroy();
}

void CDataGridPrivate::init()
{
	horizontalBar=new CScrollBar;
	verticalBar=new CScrollBar;
	horizontalBar->setOrientation(Gemini::Horizontal);
	
	dataGrid=new CDataGrid;
	
	size=CSize(520,240);
	marginLeft=10;
	marginRight=10;
	marginTop=10;
	marginBottom=10;
	focus=false;
	x=0;
	y=0;
	transparent=false;
	touched=false;
	properties.all=0;
	horzScrollBarVisible=false;
	vertScrollBarVisible=true;
	rowCount=6;
	colCount=4;
	horzGridVisible=true;
	vertGridVisible=false;
	gridStyle=Gemini::SolidLine;
	gridColor=0xffacacac;
	gridLineWidth=2;
	headerHeight=40;
	oldBgc=0;
	clickDownInValidArea=false;
	
	
}

void CDataGridPrivate::destroy()
{
	delete horizontalBar;
	delete verticalBar;
	delete dataGrid;
}




IMPLEMENT_DYNCREATE(CtmDataGrid, CtmWnd)


CtmDataGrid::CtmDataGrid()
{
	strcpy(TYPEName, "CtmDataGrid");
	d=new CDataGridPrivate;
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);	
	}
	
	marginLeft=10;
	marginRight=10;
	marginBottom=10;
	marginTop=10;
	transparent=false;
}

CtmDataGrid::~CtmDataGrid()
{
	delete d;
}

CDataGridPrivate* CtmDataGrid::d_func()const
{
	return d;
}


static inline void drawScroll(CDataGridPrivate* d)
{
	CPainter painter(&d->image);
	CRect rect=CRect(CPoint(0,0),d->size)-CMargin(d->marginLeft,d->marginTop,d->marginRight,d->marginBottom);
	if(d->horzScrollBarVisible)
		rect-=CMargin(0,0,0,30);
	if(d->vertScrollBarVisible)
		rect-=CMargin(0,0,30,0);
	
	if(d->horzScrollBarVisible)
	{
		CRect scrollRect=CRect(rect.bottomLeft(),CSize(rect.width(),24));
		d->horizontalBar->setRect(scrollRect);
		d->horizontalBar->draw(&painter);
	}
	if(d->vertScrollBarVisible)
	{
		CRect scrollRect=CRect(rect.topRight()+CPoint(0,d->headerHeight),CSize(24,rect.height()-d->headerHeight));
		d->verticalBar->setRect(scrollRect);
		d->verticalBar->draw(&painter);
	}
}

static inline void drawGrid(CDataGridPrivate* d)
{
	CPainter painter(&d->image);
	CRect rect=CRect(CPoint(0,0),d->size)-CMargin(d->marginLeft,d->marginTop,d->marginRight,d->marginBottom);
	if(d->horzScrollBarVisible)
		rect-=CMargin(0,0,0,30);
	if(d->vertScrollBarVisible)
		rect-=CMargin(0,0,30,0);
	d->dataGrid->setRect(rect);
	d->dataGrid->draw(&painter);
}

void    CtmDataGrid::Show()
{
	if(d)
	{
		if(IsTransParentChanged(d)||IsNeedResize(d))
		{
			if(IsNeedResize(d))
				Resize(d);
			
			d->oldBgc=m_bgc;
			
			if(d->transparent)
			{
				if(!d->bgImage.isNull())
					d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
			}
			else
			{
				d->fmImage.fill((uint)m_bgc);
			}
			d->properties.all=0x00FF;
		}
		if(m_bVisible)
		{
			CheckPropertiesChanged();
			
			if(m_bgc!=d->oldBgc)
			{
				d->oldBgc=m_bgc;
				d->fmImage.fill((uint)m_bgc); 
				d->properties.all=0x00FF;
			}
			
			if(d->properties.all!=0)
			{
				Clear();
				drawScroll(d); 
				drawGrid(d);
				_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
				_PutImage(0,0,d->image);
					
			}
			ClearPropertiesBit();
		}
		
	}
}

void    CtmDataGrid::Update()
{
	Show();
}

void    CtmDataGrid::Clear()
{
	if(d)
	{
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
	}
}

BOOL 	CtmDataGrid::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
	return true;
}

void 	CtmDataGrid::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo((char*)"margin-left",tmINT, 	&marginLeft);
	SetPropInfo((char*)"margin-right",tmINT,	&marginRight);
	SetPropInfo((char*)"margin-top",tmINT,		&marginTop);
	SetPropInfo((char*)"margin-bottom",tmINT,	&marginBottom);
	SetPropInfo((char*)"transparent",tmBOOL, &transparent);
	SetPropInfo((char*)"touched", tmBOOL, &d->touched);
	SetPropInfo((char*)"padding", tmINT, &d->padding);
	
	SetPropInfo((char*)"row-count",tmINT,	&d->rowCount);
	SetPropInfo((char*)"column-count",tmINT, &d->colCount);
	
	SetPropInfo((char*)"header-height",tmINT, &d->headerHeight);
	SetPropInfo((char*)"grid-style",tmWORD,&d->gridStyle);
	SetPropInfo((char*)"grid-color",tmINT, &d->gridColor);
	SetPropInfo((char*)"gridline-width",tmINT,&d->gridLineWidth);
	SetPropInfo((char*)"horzgrid-visible",tmBOOL,&d->horzGridVisible);
	SetPropInfo((char*)"vertgrid-visible",tmBOOL,&d->vertGridVisible);
	SetPropInfo((char*)"horzscroll-visible",tmBOOL,&d->horzScrollBarVisible);
	SetPropInfo((char*)"vertscroll-visible",tmBOOL,&d->vertScrollBarVisible);
	
}

WORD    CtmDataGrid::OnKey(WORD wKey)
{
	return wKey;
}
		
void   	CtmDataGrid::OnGetFocus()
{
	if(d)
	{
		d->focus = true;
		CtmWnd::DrawGetFocus();
	}
}

void    CtmDataGrid::OnLoseFocus()
{
	if(d)
	{
		d->focus=false;
		CtmWnd::DrawLoseFocus();
	}
}

inline bool CheckTouchInValidArea(CDataGridPrivate* d,const CPoint& pos)
{
	CRect rect=CRect(CPoint(0,0),d->size);
	if(rect.contains(pos-CPoint(d->x,d->y)))
		return true;
	return false;
}
		
void	CtmDataGrid::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if(!d->touched) return ;
	int x=wParam>>16;
	int y=wParam&0x0000FFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			{
				if(d->clickDownInValidArea)
				{
					d->clickDownInValidArea=false;
					d->dataGrid->selectEvent(CAbstractItem::ttTouchUp,CPoint(x,y)-CPoint(d->x,d->y));
					d->horizontalBar->selectEvent(CAbstractItem::ttTouchUp,CPoint(x,y)-CPoint(d->x,d->y));
					d->verticalBar->selectEvent(CAbstractItem::ttTouchUp,CPoint(x,y)-CPoint(d->x,d->y));
					d->properties.changed.scrollBarBase=1;
					d->properties.changed.dataGridBase=1;
					d->dataGrid->rowStep(d->verticalBar->step());
					d->dataGrid->colStep(d->horizontalBar->step());
					Update();
				}
			}
			break;
		case MSG_MOUSELDOWN:
			{
				d->clickDownInValidArea=CheckTouchInValidArea(d, CPoint(x,y));
				d->horizontalBar->selectEvent(CAbstractItem::ttTouchDown,CPoint(x,y)-CPoint(d->x,d->y));
				d->verticalBar->selectEvent(CAbstractItem::ttTouchDown,CPoint(x,y)-CPoint(d->x,d->y));
				d->properties.changed.scrollBarBase=1;
				d->properties.changed.dataGridBase=1;
				Update();

			}
			break;
		default:
			break;
	}
}

void   CtmDataGrid::CheckPropertiesChanged()
{
	CheckPenChanged(d);
	CheckHeaderChanged(d);
	CheckScrollVisibleChanged(d);
	CheckGridVisibleChanged(d);
	CheckGridCountChanged(d);
	CheckMarginChanged(d);
	CheckScrollMaxLimitChanged(d);
}

void   CtmDataGrid::ClearPropertiesBit()
{
	//清除所有属性检查位
	if(d)
	{
		d->properties.all=0;
	}
}
		
void 	CtmDataGrid::Resize(CDataGridPrivate* d)
{
	if(d)
	{
		d->fmImage=CImage(CSize(GetClientWidth(),GetClientHeight()),CImage::Format_RGB32);
		d->size=CSize(GetClientWidth(),GetClientHeight());
		d->x=m_rectClient.left;
		d->y=m_rectClient.top;
		_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());
		d->bgImage=d->fmImage.copy(CRect(CPoint(0,0),d->size));
		//获取新位置处的背景色
		if(!d->bgImage.isNull())
			_GetImage(0,0,d->bgImage);
	}
}

bool	CtmDataGrid::IsNeedResize(CDataGridPrivate* d)
{
	if(d)
	{
		return (CRect(CPoint(m_rectClient.left,m_rectClient.top),CSize(GetClientWidth(),GetClientHeight()))
			!=CRect(CPoint(d->x,d->y),d->size));
	}
	return false;
}

bool 	CtmDataGrid::IsTransParentChanged(CDataGridPrivate* d)
{
	if(d)
	{
		if(d->transparent!=transparent)
		{
			d->transparent=transparent;
			return true;
		}
	}
	return false;
}

void 	CtmDataGrid::CheckPenChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CheckPenStyleChanged(d);
		CheckPenWidthChanged(d);
		CheckPenColorChanged(d);
	}
}


void 	CtmDataGrid::CheckPenStyleChanged(CDataGridPrivate* d)
{
	if(d->dataGrid->gridPen().style()!=d->gridStyle)
	{
		CPen pen=d->dataGrid->gridPen();
		pen.setStyle(Gemini::PenStyle(d->gridStyle));
		d->dataGrid->setGridPen(pen);
		d->properties.changed.dataGridBase=1;
	}
}

void 	CtmDataGrid::CheckPenWidthChanged(CDataGridPrivate* d)
{
	if(d->dataGrid->gridPen().style()!=Gemini::NoPen)
	{
		CPen pen=d->dataGrid->gridPen();
		if(pen.width()!=d->gridLineWidth)
		{
			pen.setWidth(d->gridLineWidth);
			d->dataGrid->setGridPen(pen);
			d->properties.changed.dataGridBase=1;
		}
	}
}

void 	CtmDataGrid::CheckPenColorChanged(CDataGridPrivate* d)
{
	if(d->dataGrid->gridPen().style()!=Gemini::NoPen)
	{
		CPen pen=d->dataGrid->gridPen();
		if(pen.color()!=d->gridColor)
		{
			pen.setColor(d->gridColor);
			d->dataGrid->setGridPen(pen);
			d->properties.changed.dataGridBase=1;
		}
	}
}

void 	CtmDataGrid::CheckMarginChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CMargin margin(marginLeft,marginTop,marginRight,marginBottom);
		CMargin marginD(d->marginLeft,d->marginTop,d->marginRight,d->marginBottom);
		if(margin!=marginD)
		{
			d->marginLeft=marginLeft;
			d->marginRight=marginRight;
			d->marginTop=marginTop;
			d->marginBottom=marginBottom;
			d->properties.changed.dataGridBase=1;
			d->properties.changed.scrollBarBase=1;
		}
	}
}

void CtmDataGrid::CheckGridCountChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CheckRowCountChanged(d);
		CheckColCountChanged(d);
	}
}

void CtmDataGrid::CheckRowCountChanged(CDataGridPrivate* d)
{
	if(d->rowCount!=d->dataGrid->rowCount())
	{
		d->dataGrid->setRowCount(d->rowCount);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckColCountChanged(CDataGridPrivate* d)
{
	if(d->colCount!=d->dataGrid->columnCount())
	{
		d->dataGrid->setColumnCount(d->colCount);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckGridVisibleChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CheckHorzGridVisibleChanged(d);
		CheckVerzGridVisibleChanged(d);
	}
}

void CtmDataGrid::CheckHorzGridVisibleChanged(CDataGridPrivate* d)
{
	if(d->horzGridVisible!=d->dataGrid->horizontalGridVisible())
	{
		d->dataGrid->setHorizontalGridVisible(d->horzGridVisible);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckVerzGridVisibleChanged(CDataGridPrivate* d)
{
	if(d->vertGridVisible!=d->dataGrid->verticalGridVisible())
	{
		d->dataGrid->setVerticalGridVisible(d->vertGridVisible);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckScrollVisibleChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CheckVertScrollVisibleChanged(d);
		CheckHorzScrollVisibleChanged(d);
	}
}

void CtmDataGrid::CheckVertScrollVisibleChanged(CDataGridPrivate* d)
{
	if(d->vertScrollBarVisible!=d->verticalBar->visible())
	{
		d->verticalBar->setVisible(d->vertScrollBarVisible);
		d->properties.changed.scrollBarBase=1;
	}
}

void CtmDataGrid::CheckHorzScrollVisibleChanged(CDataGridPrivate* d)
{
	if(d->horzScrollBarVisible!=d->horizontalBar->visible())
	{
		d->horizontalBar->setVisible(d->horzScrollBarVisible);
		d->properties.changed.scrollBarBase=1;
	}
}

void CtmDataGrid::CheckHeaderChanged(CDataGridPrivate* d)
{
	if(d->headerHeight!=d->dataGrid->headerHeight())
	{
		d->dataGrid->setHeaderHeight(d->headerHeight);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckPaddingChanged(CDataGridPrivate* d)
{
	if(d->padding!=d->dataGrid->padding())
	{
		d->dataGrid->setPadding(d->padding);
		d->properties.changed.dataGridBase=1;
	}
}

void CtmDataGrid::CheckVertScrollMaxLimitChanged(CDataGridPrivate* d)
{
	if(d->dataGrid->dataRowCount()!=d->verticalBar->maxLimit())
	{
		d->verticalBar->setMaxLimit(d->dataGrid->dataRowCount());
		d->horizontalBar->setStep(d->dataGrid->row());
		d->properties.changed.scrollBarBase=1;
	}
}

void CtmDataGrid::CheckHorzScrollMaxLimitChanged(CDataGridPrivate* d)
{
	if(d->dataGrid->dataMaxColCount()!=d->horizontalBar->maxLimit())
	{
		d->horizontalBar->setMaxLimit(d->dataGrid->dataMaxColCount());
		d->horizontalBar->setStep(d->dataGrid->column());
		d->properties.changed.scrollBarBase=1;
	}
}

void 	CtmDataGrid::CheckScrollMaxLimitChanged(CDataGridPrivate* d)
{
	if(d)
	{
		CheckVertScrollMaxLimitChanged(d);
		CheckHorzScrollMaxLimitChanged(d);
	}
}

CGridDataWrapper CtmDataGrid::GetDataGrid()const
{
	return CGridDataWrapper(d->dataGrid,this);
}







