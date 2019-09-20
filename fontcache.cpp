#include "fontcache.h"
#include <stdio.h>
FontCache::FontCache()
{
	pthread_mutex_init(&m_Lock,NULL);
}

FontCache::~FontCache()
{
	pthread_mutex_lock(&m_Lock);
	std::map<FontCacheKey, Glyph*>::iterator it = m_cache.begin();
	for (; it != m_cache.end(); ++it) {
			delete it->second;
	}
	m_cache.clear();
	pthread_mutex_unlock(&m_Lock);
	pthread_mutex_destroy(&m_Lock);
}


 Glyph* FontCache::GetFontCache(unsigned int  dwCode, SIZE size) 
{
	pthread_mutex_lock(&m_Lock);
	FontCacheKey key = {dwCode, size};
	std::map<FontCacheKey, Glyph*>::iterator it = m_cache.find(key);
	if (it != m_cache.end())
	{
		pthread_mutex_unlock(&m_Lock);
		return it->second;
	}
	
	pthread_mutex_unlock(&m_Lock);
	return NULL;
}
void  FontCache::AddFontCache(unsigned int dwCode, SIZE size,  Glyph& glyph)
{
	pthread_mutex_lock(&m_Lock);
	Glyph* FontItem = new Glyph(glyph);
	FontCacheKey key = {dwCode, size};
	m_cache[key] = FontItem;
	pthread_mutex_unlock(&m_Lock);
}


