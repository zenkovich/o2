#include "property.h"

OPEN_O2_NAMESPACE

void cPropertyBase::initielizeProps( void* tclass, ... )
{
	va_list vlist;
	va_start(vlist, tclass);

	do 
	{
		void* prop = va_arg(vlist, void*);
		if (!prop)
			break;

		void* target = va_arg(vlist, void*);
		void* setter = va_arg(vlist, void*);
		void* getter = va_arg(vlist, void*);

		((cPropertyBase*)(prop))->xinit(target, tclass, setter, getter);
	} 
	while (true);

	va_end(vlist);
}

CLOSE_O2_NAMESPACE