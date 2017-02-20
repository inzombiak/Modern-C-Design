#ifndef FAMILIAR_FACTORY_H
#define FAMILIAT_FACTORY_H

#include "Typelists.h"

class Familiar;
class FamiliarFactory
{
public:
	template<class TList>
	Familiar* CreateFamiliar(TList typlist);


};

#endif