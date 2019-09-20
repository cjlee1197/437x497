#ifndef D_FONTCACHE_H
#define D_FONTCACHE_H
#include "glyph.h"
#include "pthread.h"
#include <map>
#include <stdio.h>

typedef struct tagSize
{
	int width;
	int height;
}SIZE;
class FontCache
{
	public:
		FontCache();
		~FontCache();
	public:
		Glyph* GetFontCache(unsigned int  dwCode, SIZE size) ;
		void  AddFontCache(unsigned int dwCode, SIZE size,  Glyph&);
	private:
		pthread_mutex_t		m_Lock;
		struct FontCacheKey
		{
			unsigned int dwCode;
			SIZE 		size;	
		};
		typedef unsigned long long uint64;
		typedef unsigned int uint;
		struct cmp_key 
		{
			bool operator()(const FontCacheKey a, const FontCacheKey b) {
				//uint64 key1=((uint64)a.dwCode)<<32+a.size.width<<16+a.size.height;
				//uint64 key2=((uint64)b.dwCode)<<32+b.size.width<<16+b.size.height;
				uint key1=a.dwCode<<16|a.size.width<<8|a.size.height;
				uint key2=b.dwCode<<16|b.size.width<<8|b.size.height;
				return (key1<key2)?true:false;
//
//				if (a.dwCode == b.dwCode) /*return a.size < b.size;*/
//				{
//					if(a.size.width==b.size.width) return a.size.height<b.size.height;
//					return a.size.width<b.size.width;	
//				}
//				return a.dwCode < b.dwCode;
			}
		};
		
		std::map<FontCacheKey, Glyph*, cmp_key> m_cache;
			
};

#endif /*D_FONTCACHE_H*/