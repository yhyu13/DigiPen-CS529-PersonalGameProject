#pragma once
#include <string>
#include <map>
#include <memory>

namespace My
{
	enum class COMPONENT_TYPE
	{
		SPRITE=0,
		SCENECOMPONENT,

		TRANSFORM,
		BODY = TRANSFORM,

		PLAYERCONTROLLER,
		AICONTROLLER,
		PACKCONTROLLER,

		HEALTHPOINT,
		WEAPONARYSYSTEM,

		NUM
	};
}