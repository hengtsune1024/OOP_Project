#pragma once
#include "constants.h"
#include "Entity.h"

class Tool :public Entity
{
	ToolType type;

public:
	Tool();
	~Tool();

};

