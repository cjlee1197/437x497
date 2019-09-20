/*
*	created by J.Wong 2016/5/3 14:21:33
*/

#ifndef  _PARSEPDF_H
#define  _PARSEPDF_H


#include <cstdlib>
#include <iostream>
#include <memory>
#include <error.h>

/*===========================================================================+
|           Class declaration - ParsePDF                                     |
+===========================================================================*/

typedef struct tmPAGE
{
	int height;
	int width;	
}TMPAGE;

class	CtmParsePDF{
	
	public:
		CtmParsePDF();
		~CtmParsePDF();
		void	openFile(char* pszFile);
		int		selectPage(int nPage=0);
		char*	image();
		int		height();
		int		width();
		int		pageHeight();
		int		pageWidth();
		void	setPageHeight(int height);
		void	setPageWidth(int width);	
		int		setHeight(int height);
		int		setWidth(int width);
		void	step(int hstep,int vstep);
		int		bytes_per_row();
		int		pages();
		int		curpage();
		void	setZoom(int zoom);
		
	private:
		class           CtmPrivateData;
		CtmPrivateData* m_data;
		int		        stepInPage(int nPage=0);
		inline          TMPAGE	origin();
};

#endif