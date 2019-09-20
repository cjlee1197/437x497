#include "arabictables.h"

bool	CheckInFrontSet(unsigned long unicode)
{
	for(int i=0; i<sizeof(arabic_frontset)/sizeof(unsigned long);i++)
	{
		if(unicode==arabic_frontset[i])	return true;	
	}
	
	return false;		
}

bool	CheckInNextSet(unsigned long unicode)
{
	for(int i=0; i<sizeof(arabic_nextset)/sizeof(unsigned long);i++)
	{
		if(unicode==arabic_nextset[i])		return true;
	}	
	return false;
}

 bool 	CheckInTransform(unsigned long unicode)
{
	if(unicode>=0x621&&unicode<=0x6CC)  return true; 
	return false;	
}

bool		CheckIsArabicSet(unsigned long unicode)
 {
 	if(unicode>=0x621&&unicode<=0x6FF||unicode>=0xFB00&&unicode<=0xFDFF||
 	   unicode>=0xFE70&&unicode<=0xFEFF||unicode==0x20||unicode==0x200C)  return true; 
	return false;		
 }