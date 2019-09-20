#ifndef		D_LIBXMLCOMM_H
#define		D_LIBXMLCOMM_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

typedef	struct tagtmControl{
	xmlChar*	id;
	xmlChar*	buddy;
	xmlChar*	name;
	xmlChar*	detail;
	xmlChar*	img;	
}tmControl,*tmControlPtr;

#endif