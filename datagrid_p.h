/*Created by J.Wong 2017/05/02*/
#pragma once
#include "plot_p.h"
#include <map>
#include "common.h"
#include "cbitmap.h"
#include <map>
//#include <wstring>

class CDataGrid;

class CAbstractItem
{
	public:
		enum TouchType{
			ttTouchDown=0x02,
			ttTouchUp=0x04
		};
		
		enum ItemType{
			itNone,
			itCheckBox=0x01,
			itDigitBox=0x02,
			itImageBox=0x04,
			itStringBox=0x10,
			itDoubleBox=0x20
		};
		
		CAbstractItem(CDataGrid* grid=0);
		virtual ~CAbstractItem();
		void setParent(CDataGrid* grid=0);
		
		bool visible()const;
		void setVisible(bool visible);
		
		int padding()const;
		void setPadding(int padding);
		
		CRect rect()const;
		void setRect(const CRect& rect);
		
		WORD itemType()const;
		void setItemType(WORD type);
		inline void setRow(int row)
		{
			mRow=row;
		}
		inline void setColumn(int column)
		{
			mCol=column;
		}
		
		int row()const
		{
			return mRow;
		}
		
		int column()const
		{
			return mCol;
		}
		
		//翻译：
		void setTranslation(const std::string& str);
		CVector<uint32> translation()const;
		
		
		virtual  bool selectEvent(TouchType type, const CPoint& pos);
		virtual  void draw(CPainter* painter);
		
	private:
		CDataGrid* mGrid;
		CVector<uint32> mTranslationStr;
		bool mVisible;
		CRect mRect;
		int mPadding;
		int mRow;
		int mCol;
		
		WORD mItemType;
		
		friend class CItemCheckBox;
		friend class CItemImageBox;
		friend class CItemDigitBox;
		friend class CItemDoubleBox;
		friend class CItemStringBox;
		friend class CAbstractVariant;
		
};



class CItemCheckBox:public CAbstractItem
{
	public:
		explicit CItemCheckBox(CDataGrid* grid=0);
		~CItemCheckBox();
		
		virtual void draw(CPainter* painter);
		virtual bool selectEvent(TouchType type, const CPoint& pos);
		
		bool status()const;
		void setStatus(bool status);
		
		void setBmpPath(const std::string& selectedPath, const std::string& unSelectedPath);
		void setSelectedPath(const std::string& selectedPath);
		void setUnselectedPath(const std::string& unSelectedPath);
		
	private:
		bool touchInArea;
		bool mStatus;
		std::string mSelectedBmpPath;
		std::string mUnselectedBmpPath;
		CtmBitMap* mUnSelectedBitmap;
		CtmBitMap* mSelectedBitmap;
};

class CItemImageBox:public CAbstractItem
{
	public:
		CItemImageBox(CDataGrid* grid=0);
		~CItemImageBox();
		virtual void draw(CPainter* painter);
		void setImagePath(const std::string& imagePath);
		std::string imagePath()const;
	private:
		std::string mImagePath;
		CtmBitMap* mBitmap;
};


class CItemDigitBox:public CAbstractItem
{
	public:
		CItemDigitBox(CDataGrid* grid=0);
		~CItemDigitBox();
		
		void setFont(const PlotFont& font);
		PlotFont font()const;
		
		void setValue(int value);
		int value()const;
		
		virtual void draw(CPainter* painter);
		
		struct ValueData{
			std::string value;
			CRect bounds;
		};
	private:
	  int mValue;
	  PlotFont mFont;
	  ValueData calcBounds(int value);
	  void 	drawData(CPainter* painter,int x,int y,const ValueData& data);
};

class CItemDoubleBox:public CAbstractItem
{
	public:
		CItemDoubleBox(CDataGrid* grid=0);
		~CItemDoubleBox();
		
		void setFont(const PlotFont& font);
		PlotFont font()const;
		
		void setValue(double value);
		double value()const;
		
		virtual void draw(CPainter* painter);
		
		struct ValueData{
			std::string value;
			CRect bounds;
		};
		
		void setPrecision(int precision);
		int precision()const;
	private:
	  double mValue;
	  PlotFont mFont;
	  int mPrecision;
	  ValueData calcBounds(double value, int precision);
	  void 	drawData(CPainter* painter,int x,int y,const ValueData& data);
};

class CItemStringBox:public CAbstractItem
{
	public:
		CItemStringBox(CDataGrid* grid=0);
		~CItemStringBox();
		
		void setString(const std::string& str, bool translation=false);
		void setString(WORD* str);
		void setString(const CVector<uint32>& wStr);
		std::string string()const;
		
		void setFont(const PlotFont& font);
		PlotFont font()const;
		
		struct ValueData{
			CVector<uint32> wStr;
			std::string str;
			CRect bounds;
		};
		
		virtual void draw(CPainter* painter);
		
	private:
		bool mTranslation;
		std::string mStr;
		PlotFont mFont;
		ValueData calcBounds(std::string value);
		ValueData calcBounds(CVector<uint32> value);
		void drawData(CPainter* painter, int x, int y, const ValueData& data);
		
};

class CAbstractVariant
{
	public:
		CAbstractVariant()
		:mItemType(CAbstractItem::itNone)
		{
			
		}
		virtual ~CAbstractVariant()
		{
			
		}
		
		inline void setItemType(WORD type);
		inline WORD itemType()const{
			return mItemType;
		}
	
	protected:
		WORD mItemType;
		
};

inline void CAbstractVariant::setItemType(WORD type)
{
	 mItemType=type;
}

class CIntVariant:public CAbstractVariant
{
	
	public:
		CIntVariant(int value=0, int width=16, int height=16, int color=Gemini::black):
		mValue(value)
		{
			setItemType(CAbstractItem::itDigitBox);
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		~CIntVariant(){}
		
		inline void setFont(int width, int height, int color)
		{
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		inline void setFont(const PlotFont& font)
		{
			mFont=font;
		}
		PlotFont font()const
		{
			return mFont;
		}
		inline void setValue(int value);
		int  value()const{
			return mValue;
		}
	private:
		int mValue;
		PlotFont mFont;
};

inline void CIntVariant::setValue(int value)
{
	mValue=value;
}

class CDoubleVariant:public CAbstractVariant
{
	public:
		CDoubleVariant(double value=0,int precision=1, int width=16, int height=16, int color=Gemini::black)
		:mValue(value),
		mPrecision(precision)
		{
			setItemType(CAbstractItem::itDoubleBox);
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		~CDoubleVariant()
		{
			
		}
		
		inline void setFont(int width, int height, int color)
		{
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		inline void setFont(const PlotFont& font)
		{
			mFont=font;
		}
		
		PlotFont font()const
		{
			return mFont;
		}
		
		inline void setValue(double value);
		inline void setPrecision(int precision);
		
		double value()const
		{
			return mValue;
		}
		
		int  	precision()const
		{
			return mPrecision;
		}
		
	private:
		double mValue;
		int mPrecision;
		PlotFont mFont;
};

inline void CDoubleVariant::setValue(double value)
{
	mValue=value;
}

inline void CDoubleVariant::setPrecision(int precision)
{
	mPrecision=precision;
}


class CStringVariant:public CAbstractVariant
{
	public:
		CStringVariant()
		{
			setItemType(CAbstractItem::itStringBox);
			mFont.width=16;
			mFont.height=16;
			mFont.color=Gemini::black;
		}
		CStringVariant(const std::string& str, bool translation=false,int width=16, int height=16, int color=Gemini::black)
		:mStr(str),
		mTranslation(translation)
		{
			setItemType(CAbstractItem::itStringBox);
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		
		~CStringVariant()
		{
			
		}
		
		inline void setFont(int width, int height, int color)
		{
			mFont.width=width;
			mFont.height=height;
			mFont.color=color;
		}
		inline void setFont(const PlotFont& font)
		{
			mFont=font;
		}
		
		PlotFont font()const
		{
			return mFont;
		}
		
		inline void setString(const std::string& str, bool translation=false);
		inline void setString(WORD* str);
		inline void setString(CVector<uint32> str);
		std::string string()const
		{
			return mStr;
		}
		
		CVector<uint32> wString()const
		{
			return wStr;
		}
		
		inline bool translation()const
		{
			return mTranslation;
		}
		
	private:
		std::string mStr;
		CVector<uint32> wStr;
		bool mTranslation;
		PlotFont mFont;
};

inline void CStringVariant::setString(const std::string& str,bool translation)
{
	if(mStr!=str)
	{
		mStr=str;
		mTranslation=translation;
		wStr.clear();
	}
}

inline void CStringVariant::setString(WORD* str)
{
	if(!str) 	return;
	if(!*str) 	return;
	mStr.clear();
	while(*str)
	{
		wStr<<*str;
		++str;
	}
	wStr<<0;
	mTranslation=true;
}

inline void CStringVariant::setString(CVector<uint32> str)
{
	wStr=str;
	if(!wStr.isEmpty())
		mTranslation=true;
}

class CImageVariant:public CAbstractVariant
{
	public:
		CImageVariant()
		{
			setItemType(CAbstractItem::itImageBox);
		}
		
		CImageVariant(const std::string& imagePath)
		:mImagePath(imagePath)
		{
			setItemType(CAbstractItem::itImageBox);
		}
		
		~CImageVariant()
		{
			
		}
		
		inline void setImagePath(const std::string& imagePath);
		std::string imagePath()const
		{
			return mImagePath;
		}
	private:
		std::string mImagePath;
};

inline void CImageVariant::setImagePath(const std::string& imagePath)
{
	if(imagePath!=mImagePath)	mImagePath=imagePath;
}

class CCheckVariant:public CAbstractVariant
{
	public:
		CCheckVariant(bool status=false)
		:mStatus(status)
		{
			setItemType(CAbstractItem::itCheckBox);
		}
		
		CCheckVariant(const std::string& unSelectedImagePath, const std::string& selectedImagePath,
		bool status=false)
		:mUnselectedImagePath(unSelectedImagePath),
		mSelectedImagePath(selectedImagePath),
		mStatus(status)
		{
			setItemType(CAbstractItem::itCheckBox);
		}
		
		~CCheckVariant()
		{
			
		}
		
		inline void setSelectedImagePath(const std::string& imagePath);
		inline void setUnselectedImagePath(const std::string& imagePath);
		inline void setStatus(bool status);
		
		bool status()const
		{
			return  mStatus;
		}
		
		std::string selectedImagePath()const
		{
			return mSelectedImagePath;
		}
		
		std::string unSelectedImagePath()const
		{
			return mUnselectedImagePath;
		}
		
	private:
		std::string mSelectedImagePath;
		std::string mUnselectedImagePath;
		bool mStatus;
		
};

inline void CCheckVariant::setSelectedImagePath(const std::string& imagePath)
{
	if(imagePath!=mSelectedImagePath) mSelectedImagePath=imagePath;
}

inline void CCheckVariant::setUnselectedImagePath(const std::string& imagePath)
{
	if(imagePath!=mUnselectedImagePath) mUnselectedImagePath=imagePath;
}

inline void CCheckVariant::setStatus(bool status)
{
	mStatus=status;
}


struct CModelData
{
	CVector<CAbstractVariant*> mVariants;
	CAtomicInt ref;
};



struct CModelDataDataPointerDeleter;

class CAbstractDataModel
{
	public:
		CAbstractDataModel();
		CAbstractDataModel(const CAbstractDataModel& model);
		CAbstractDataModel& operator=(const CAbstractDataModel& model);
		
		virtual ~CAbstractDataModel();
		
		int elementCount()const
		{
			return m_data->mVariants.count();
		}
		
		inline void addData(const CIntVariant& variant);
		inline void addData(const CDoubleVariant& variant);
		inline void addData(const CCheckVariant& variant);
		inline void addData(const CStringVariant& variant);
		inline void addData(const CImageVariant& variant);
		
		inline void removeAt(int index);
		inline void clear();
		inline void modify(int index,const CIntVariant& variant);
		inline void modify(int index,const CDoubleVariant& variant);
		inline void modify(int index,const CCheckVariant& variant);
		inline void modify(int index,const CStringVariant& variant);
		inline void modify(int index,const CImageVariant& variant);
		
		inline void insert(int index, const CIntVariant& variant);
		inline void insert(int index, const CDoubleVariant& variant);
		inline void insert(int index, const CCheckVariant& variant);
		inline void insert(int index, const CStringVariant& variant);
		inline void insert(int index, const CImageVariant& variant);
		
		
		inline CIntVariant 		getIntVariant(int index, bool& find)const;
		inline CDoubleVariant 	getDoubleVariant(int index, bool& find)const;
		inline CStringVariant	getStringVariant(int index, bool& find)const;
		inline CImageVariant	getImageVariant(int index, bool& find)const;
		inline CCheckVariant	getCheckVariant(int index, bool& find)const;
		
		inline CAbstractVariant* elementAt(int index)const;
		
		
		inline CAbstractDataModel& operator<<(const CIntVariant& variant);
		inline CAbstractDataModel& operator<<(const CDoubleVariant& variant);
		inline CAbstractDataModel& operator<<(const CCheckVariant& variant);
		inline CAbstractDataModel& operator<<(const CStringVariant& variant);
		inline CAbstractDataModel& operator<<(const CImageVariant& variant);
		
		void cleanUp(CModelData* data);
		
	private:
		friend class CDataGrid;
		
		CScopedPointer<CModelData, CModelDataDataPointerDeleter> m_data;
		typedef CScopedPointer<CModelData, CModelDataDataPointerDeleter> DataPtr;
		
};

inline CIntVariant 		CAbstractDataModel::getIntVariant(int index, bool& find)const
{
	find=false;
	if(index<0||index>=m_data->mVariants.count()) return CIntVariant();
	if(m_data->mVariants.at(index)->itemType()!=CAbstractItem::itDigitBox) return CIntVariant();
	
	CIntVariant* pIntVariant=(CIntVariant* )m_data->mVariants.at(index);
	CIntVariant variant(pIntVariant->value());
	variant.setFont(pIntVariant->font());
	find=true;
	return variant;
}

inline CDoubleVariant 	CAbstractDataModel::getDoubleVariant(int index, bool& find)const
{
	find=false;
	if(index<0||index>=m_data->mVariants.count()) return CDoubleVariant();
	if(m_data->mVariants.at(index)->itemType()!=CAbstractItem::itDoubleBox) return CDoubleVariant();
	
	CDoubleVariant* pDoubleVariant=(CDoubleVariant*)m_data->mVariants.at(index);
	CDoubleVariant variant(pDoubleVariant->value(),pDoubleVariant->precision());
	variant.setFont(pDoubleVariant->font());
	find=true;
	return variant;
}

inline CStringVariant	CAbstractDataModel::getStringVariant(int index, bool& find)const
{
	find=false;
	if(index<0||index>=m_data->mVariants.count()) return CStringVariant();
	if(m_data->mVariants.at(index)->itemType()!=CAbstractItem::itStringBox) return CStringVariant();
	
	CStringVariant* pStringVariant=(CStringVariant*)m_data->mVariants.at(index);
	CStringVariant variant;
	if(pStringVariant->wString().isEmpty())
		variant.setString(pStringVariant->string(),pStringVariant->translation());
	else
		variant.setString(pStringVariant->wString());
	variant.setFont(pStringVariant->font());
	find=true;
	return variant;
}

inline CImageVariant	CAbstractDataModel::getImageVariant(int index, bool& find)const
{
	find=false;
	if(index<0||index>=m_data->mVariants.count()) return CImageVariant();
	if(m_data->mVariants.at(index)->itemType()!=CAbstractItem::itImageBox) return CImageVariant();
	
	CImageVariant* pImageVariant=(CImageVariant*)m_data->mVariants.at(index);
	CImageVariant variant(pImageVariant->imagePath());
	find=true;
	return variant;
}


inline CCheckVariant	CAbstractDataModel::getCheckVariant(int index, bool& find)const
{
	find=false;
	if(index<0||index>=m_data->mVariants.count()) return CCheckVariant();
	if(m_data->mVariants.at(index)->itemType()!=CAbstractItem::itCheckBox) return CCheckVariant();
	
	CCheckVariant* pCheckVariant=(CCheckVariant*)m_data->mVariants.at(index);
	CCheckVariant variant(pCheckVariant->selectedImagePath(), 
	pCheckVariant->unSelectedImagePath(),pCheckVariant->status());
	find=true;
	return variant;
}


inline void CAbstractDataModel::addData(const CIntVariant& variant)
{
	CIntVariant* pIntVariant=new CIntVariant;
	pIntVariant->setValue(variant.value());
	pIntVariant->setFont(variant.font());
	m_data->mVariants<<pIntVariant;
}

inline void CAbstractDataModel::addData(const CDoubleVariant& variant)
{
	CDoubleVariant* pDoubleVariant=new CDoubleVariant;
	pDoubleVariant->setValue(variant.value());
	pDoubleVariant->setPrecision(variant.precision());
	pDoubleVariant->setFont(variant.font());
	m_data->mVariants<<pDoubleVariant;
}

inline void CAbstractDataModel::addData(const CCheckVariant& variant)
{
	CCheckVariant* pCheckVariant=new CCheckVariant;
	pCheckVariant->setSelectedImagePath(variant.selectedImagePath());
	pCheckVariant->setUnselectedImagePath(variant.unSelectedImagePath());
	pCheckVariant->setStatus(variant.status());
	m_data->mVariants<<pCheckVariant;
	
}

inline void CAbstractDataModel::addData(const CStringVariant& variant)
{
	CStringVariant* pStringVariant=new CStringVariant;
	pStringVariant->setString(variant.string(),variant.translation());
	pStringVariant->setString(variant.wString());
	pStringVariant->setFont(variant.font());
	m_data->mVariants<<pStringVariant;
}

inline void CAbstractDataModel::addData(const CImageVariant& variant)
{
	CImageVariant* pImageVariant=new CImageVariant;
	pImageVariant->setImagePath(variant.imagePath());
	m_data->mVariants<<pImageVariant;
}


inline void CAbstractDataModel::removeAt(int index)
{
	CAbstractVariant* variant=m_data->mVariants.takeAt(index);
	m_data->mVariants.removeAt(index);
	if(variant) delete variant;
}

inline void CAbstractDataModel::clear()
{
	for(int i=0; i<m_data->mVariants.count(); ++i)
	{
		delete m_data->mVariants.at(i);
	}
	m_data->mVariants.clear();
}



inline void CAbstractDataModel::modify(int index,const CIntVariant& variant)
{
	if(index<0) return;
	if(index>=m_data->mVariants.size())
	{
		addData(variant);
	}
	else
	{
		((CIntVariant*)m_data->mVariants[index])->setValue(variant.value());
		((CIntVariant*)m_data->mVariants[index])->setFont(variant.font());
	}
}

inline void CAbstractDataModel::modify(int index,const CDoubleVariant& variant)
{
	if(index<0) return;
	if(index>=m_data->mVariants.size())
	{
		addData(variant);
	}
	else
	{
		((CDoubleVariant*)m_data->mVariants[index])->setValue(variant.value());
		((CDoubleVariant*)m_data->mVariants[index])->setPrecision(variant.precision());
		((CDoubleVariant*)m_data->mVariants[index])->setFont(variant.font());
	}
}

inline void CAbstractDataModel::modify(int index,const CCheckVariant& variant)
{
	if(index<0) return;
	if(index>=m_data->mVariants.size())
	{
		addData(variant);
	}
	else
	{
		((CCheckVariant*)m_data->mVariants[index])->setSelectedImagePath(variant.selectedImagePath());
		((CCheckVariant*)m_data->mVariants[index])->setUnselectedImagePath(variant.unSelectedImagePath());
		((CCheckVariant*)m_data->mVariants[index])->setStatus(variant.status());
	}
}

inline void CAbstractDataModel::modify(int index,const CStringVariant& variant)
{
	if(index<0) return;
	if(index>=m_data->mVariants.size())
	{
		addData(variant);
	}
	else
	{
		((CStringVariant*)m_data->mVariants[index])->setString(variant.string(),variant.translation());
		((CStringVariant*)m_data->mVariants[index])->setString(variant.wString());
		((CStringVariant*)m_data->mVariants[index])->setFont(variant.font());
	}
}

inline void CAbstractDataModel::modify(int index,const CImageVariant& variant)
{
	if(index<0) return;
	if(index>=m_data->mVariants.size())
	{
		addData(variant);
	}
	else
	{
		((CImageVariant*)m_data->mVariants[index])->setImagePath(variant.imagePath());
	}
}


inline void CAbstractDataModel::insert(int index, const CIntVariant& variant)
{
	CIntVariant* pIntVariant=new CIntVariant;
	pIntVariant->setValue(variant.value());
	pIntVariant->setFont(variant.font());
	m_data->mVariants.insert(index, pIntVariant);
}

inline void CAbstractDataModel::insert(int index, const CDoubleVariant& variant)
{
	CDoubleVariant* pDoubleVariant=new CDoubleVariant;
	pDoubleVariant->setValue(variant.value());
	pDoubleVariant->setPrecision(variant.precision());
	pDoubleVariant->setFont(variant.font());
	m_data->mVariants.insert(index, pDoubleVariant);
}

inline void CAbstractDataModel::insert(int index, const CCheckVariant& variant)
{
	CCheckVariant* pCheckVariant=new CCheckVariant;
	pCheckVariant->setSelectedImagePath(variant.selectedImagePath());
	pCheckVariant->setUnselectedImagePath(variant.unSelectedImagePath());
	pCheckVariant->setStatus(variant.status());
	m_data->mVariants.insert(index, pCheckVariant);
}

inline void CAbstractDataModel::insert(int index, const CStringVariant& variant)
{
	CStringVariant* pStringVariant=new CStringVariant;
	pStringVariant->setString(variant.string(),variant.translation());
	pStringVariant->setString(variant.wString());
	pStringVariant->setFont(variant.font());
	m_data->mVariants.insert(index, pStringVariant);
}

inline void CAbstractDataModel::insert(int index, const CImageVariant& variant)
{
	CImageVariant* pImageVariant=new CImageVariant;
	pImageVariant->setImagePath(variant.imagePath());
	m_data->mVariants.insert(index, pImageVariant);
}


CAbstractVariant* CAbstractDataModel::elementAt(int index)const
{
	if(index<0||index>=m_data->mVariants.size()) return 0;
	return m_data->mVariants.at(index);
}

inline CAbstractDataModel& CAbstractDataModel::operator<<(const CIntVariant& variant)
{
	addData(variant);
	return *this;
}
inline CAbstractDataModel& CAbstractDataModel::operator<<(const CDoubleVariant& variant)
{
	addData(variant);
	return *this;
}

inline CAbstractDataModel& CAbstractDataModel::operator<<(const CCheckVariant& variant)
{
	addData(variant);
	return *this;
}

inline CAbstractDataModel& CAbstractDataModel::operator<<(const CStringVariant& variant)
{
	addData(variant);
	return *this;
}

inline CAbstractDataModel& CAbstractDataModel::operator<<(const CImageVariant& variant)
{
	addData(variant);
	return *this;
}


class CDataGrid
{
	public:
		CDataGrid();
		~CDataGrid();
		
		void clear();
		
		void setRect(const CRect& rect);
		CRect rect()const;
		
		inline void setHeaderHeight(int height)
		{
			mHeaderHeight=height;
			setRowCount(mRows);
		}
		
		int  headerHeight()const
		{
			return mHeaderHeight;
		}
		
		inline void setPadding(int padding);
		int padding()const{
			return mPadding;
		}
		
		inline void setHorizontalGridVisible(bool visible)
		{
			mHorzGridVisible=visible;
		}
		
		inline void setGridPen(const CPen& pen)
		{
			if(pen!=mPen)
				mPen=pen;
		}
		
		CPen gridPen()const
		{
			return mPen;
		}
		
		
		
		inline void setVerticalGridVisible(bool visible)
		{
			mVertGridVisible=visible;
		}
		bool horizontalGridVisible()const
		{
			return mHorzGridVisible;
		}
		bool verticalGridVisible()const
		{
			return mVertGridVisible;
		}
		
		void setRowCount(int rows);
		void setColumnCount(int columns);
		
		int rowCount()const;
		int columnCount()const;
		
		void setRowHeight(int index,int height);
		void setColumnWidth(int index, int width);
		
		int  rowHeight(int index);
		int  columnWidth(int index);
		
		void rowStep(int step);
		void colStep(int step);
		void rowPageStep(int page);
		void colPageStep(int page);
		
		inline int row()const
		{
			return mRowStep;
		}
		inline int column()const
		{
			return mColStep;
		}
		void addModel(const CAbstractDataModel& model);
		int  dataCountInRow(int row)const;
		int  dataMaxColCount()const;
		int	 dataRowCount()const;
		int  dataCount()const;
		void removeModelAt(int index);
		
		void insertModel(int index, CAbstractDataModel& model);
		
		void setModels(const CList<CAbstractDataModel>& models);
		
		void modify(int row, int column, const CIntVariant& variant);
		void modify(int row, int column, const CDoubleVariant& variant);
		void modify(int row, int column, const CCheckVariant& variant);
		void modify(int row, int column, const CImageVariant& variant);
		void modify(int row, int column, const CStringVariant& variant);
		
		void insert(int row, int column, const CIntVariant& variant);
		void insert(int row, int column, const CDoubleVariant& variant);
		void insert(int row, int column, const CCheckVariant& variant);
		void insert(int row, int column, const CImageVariant& variant);
		void insert(int row, int column, const CStringVariant& variant);
		
		void addData(int row, const CIntVariant& variant);
		void addData(int row, const CDoubleVariant& variant);
		void addData(int row, const CImageVariant& variant);
		void addData(int row, const CCheckVariant& variant);
		void addData(int row, const CStringVariant& variant);
		
		CIntVariant 	getIntVariant(int row, int column, bool& find)const;
		CDoubleVariant 	getDoubleVariant(int row, int column, bool& find)const;
		CStringVariant 	getStringVariant(int row, int column, bool& find)const;
		CImageVariant 	getImageVariant(int row, int column, bool& find)const;
		CCheckVariant 	getCheckVariant(int row, int column, bool& find)const;
		
		void addHeader(const CStringVariant& variant);
		void insertHeader(int index, const CStringVariant& variant);
		void removeHeaderAt(int index);
		void modifyHeader(int index, const CStringVariant& variant);
		void setHeader(const CVector<CStringVariant>& headers);
		
		void removeAt(int row, int column);
		void updateValidAreaItems();
		
		void draw(CPainter* painter);
		
		void clearValidAreaItems();
		
		CAbstractItem* currentItem()const
		{
			return mCurrentItem;
		}
		inline void 	clearCurrentItem()
		{
			mCurrentItem=0;
		}
		
		bool selectEvent(CAbstractItem::TouchType type, const CPoint& pos);
		
	private:
		CVector<CAbstractItem*> validAreaItems;
		CList<CAbstractDataModel> models;
		
		CAbstractDataModel mHeaders;
		
		CAbstractItem* 	mCurrentItem;
		
		CVector<double> mHeights;
		CVector<double> mWidths;
		std::map<int, double>mSpecHeights;
		std::map<int, double>mSpecWidths;
		int mRows;
		int mCols;
		CRect mRect;
		int mPadding;
		
		int mRowStep;
		int mColStep;
		
		bool mHorzGridVisible;
		bool mVertGridVisible;
		
		int mHeaderHeight;
		
		CPen mPen;
		
		DISABLE_COPY(CDataGrid);
		CAbstractItem* createItem(const CAbstractVariant* variant);
		
		void drawGrid(CPainter* painter);
		
		friend class CGridDataWrapper;
		
		
};

inline void CDataGrid::setPadding(int padding)
{
	mPadding=padding;
}





class CScrollBar
{
	public:
	CScrollBar();
	~CScrollBar();
	enum BarStyle
	{
		bsSquare=0x00,
		bsRound=0x01
		
	};
	
	void draw(CPainter* painter);
	void drawTrack(CPainter* painter);
	void drawArrow(CPainter* painter);
	void drawFace(CPainter* painter);
	
	inline void setRect(const CRect& rect)
	{
		mRect=rect;
	}
	inline CRect rect()const
	{
			return mRect;
	}
	
	inline void setStep(int step)
	{
		mStep=mMax(0,mMin(step,mMaxLimit-1));
	}
	
	inline void setMaxLimit(int limit)
	{
		mMaxLimit=mMax(0,limit);
	}
	
	inline int step()const
	{
		return mStep;
	}
	
	inline int maxLimit()const
	{
		return mMaxLimit;
	}
	
	inline void setStyle(WORD style)
	{
		mStyle=style;
	}
	
	inline WORD style()const
	{
			return mStyle;
	}
	
	inline void setOrientation(WORD orientation)
	{
		mOrientation=orientation;
	}
	
	inline WORD orientation()const
	{
		return mOrientation;
	}
	void selectEvent(CAbstractItem::TouchType type, const CPoint& pos);
	
	inline void setVisible(BOOL visible)
	{
		mVisible=visible;
	}
	
	inline bool visible()const
	{
		return mVisible;
	}
	private:
	 CBrush mTrackBrush;
	 CBrush mArrowBrush;
	 CBrush mFaceBrush;
	 WORD mOrientation;
	 
	 int 	mStep;
	 int 	mMaxLimit;
	 CRect 	mRect;
	 WORD 	mStyle;
	 BOOL   mClicked[2];
	 void 	operatorScrollBarArea(const CPoint& pos);
	 bool   mVisible;
	 friend class CGridDataWrapper;
	 
};

class CtmDataGrid;

class CGridDataWrapper
{
	public:
		explicit CGridDataWrapper(CDataGrid* grid,const CtmDataGrid* parent=0);
		~CGridDataWrapper();
		
		void rowStep(int step);
		void colStep(int step);
		void rowPageStep(int page);
		void colPageStep(int page);
		

		
		void addModel(const CAbstractDataModel& model);
		int  dataCountInRow(int row);
		int  dataCount();
		void removeModelAt(int index);
		
		void insertModel(int index, CAbstractDataModel& model);
		
		void setModels(const CList<CAbstractDataModel>& models);
		
		void modify(int row, int column, const CIntVariant& variant);
		void modify(int row, int column, const CDoubleVariant& variant);
		void modify(int row, int column, const CCheckVariant& variant);
		void modify(int row, int column, const CImageVariant& variant);
		void modify(int row, int column, const CStringVariant& variant);
		
		void insert(int row, int column, const CIntVariant& variant);
		void insert(int row, int column, const CDoubleVariant& variant);
		void insert(int row, int column, const CCheckVariant& variant);
		void insert(int row, int column, const CImageVariant& variant);
		void insert(int row, int column, const CStringVariant& variant);
		
		void addData(int row, const CIntVariant& variant);
		void addData(int row, const CDoubleVariant& variant);
		void addData(int row, const CImageVariant& variant);
		void addData(int row, const CCheckVariant& variant);
		void addData(int row, const CStringVariant& variant);
		
		CIntVariant 	getIntVariant(int row, int column, bool& find)const;
		CDoubleVariant 	getDoubleVariant(int row, int column, bool& find)const;
		CStringVariant 	getStringVariant(int row, int column, bool& find)const;
		CImageVariant 	getImageVariant(int row, int column, bool& find)const;
		CCheckVariant 	getCheckVariant(int row, int column, bool& find)const;
		
		void addHeader(const CStringVariant& variant);
		void insertHeader(int index, const CStringVariant& variant);
		void removeHeaderAt(int index);
		void modifyHeader(int index, const CStringVariant& variant);
		void setHeader(const CVector<CStringVariant>& headers);
		
		
		void setRowHeight(int index,int height);
		void setColumnWidth(int index, int width);
		
		void removeAt(int row, int column);
		CAbstractItem* currentItem()const;
		void clear();
		
	private:
		CDataGrid*  mDataGrid;
		const CtmDataGrid* mGrid;
		void detach(){mDataGrid=0; mGrid=0;}
		
		
		friend class CtmDataGrid;
		
};



class CDataGridPrivate{
	public:
		CDataGridPrivate();
		~CDataGridPrivate();
		void init();
		void destroy();

		CScrollBar* horizontalBar;
		CScrollBar* verticalBar;
		
		CDataGrid*  dataGrid;
		
		CImage fmImage;
		CImage bgImage;
		CImage image;
		CImage scrollImage;
		
		CSize size;
		int marginLeft;
		int marginRight;
		int marginTop;
		int marginBottom;
		
		BOOL transparent;
		BOOL touched;
		
	
		bool focus;
		int x;
		int y;
		
		//32bit:每个bit位代表元件属性是否发生变化
	union
	{
		DWORD all;
		struct
		{
			DWORD dataGridBase:1;
			DWORD scrollBarBase:1;
			DWORD reserve:30;
		}changed;
	}properties;
	
	//滚动条是否可见
	BOOL  horzScrollBarVisible;
	BOOL  vertScrollBarVisible;
	
	//数据显示区网格大小
	int rowCount;
	int colCount;
	BOOL horzGridVisible;
	BOOL vertGridVisible;
	int  padding;
	
	WORD gridStyle;
	int  gridColor;
	int  gridLineWidth;
	
	int  headerHeight;
	
	int	oldBgc;
	bool clickDownInValidArea;
	

};

