/*Created by J.Wong 2017/2/8 11:06:27*/

#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>  

class CDrawTextPrivate
{
public:
	FT_UInt index;/*��������*/
	FT_Vector pos; /*����������ԭ��*/
	FT_Glyph image; /*����ͼ��*/		
};
