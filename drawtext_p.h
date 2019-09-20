/*Created by J.Wong 2017/2/8 11:06:27*/

#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>  

class CDrawTextPrivate
{
public:
	FT_UInt index;/*字形索引*/
	FT_Vector pos; /*基线上字形原点*/
	FT_Glyph image; /*字形图像*/		
};
