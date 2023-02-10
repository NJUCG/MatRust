#pragma once
#include"FilmHeader.h"
#include<glm/glm.hpp>
using namespace glm;

class ColorHelper
{
public:
	static vec4 colorOf(FilmComposition compose) {
		switch (compose)
		{
		case None:
			break;
		case CuI_O:
			return vec4(0, 0, 0, 255);//vec4(144, 38, 37, 255);
		case CuII_O:
			return vec4(0, 0, 0, 255); //vec4(144, 38, 37, 255);
		case CuSO4:
			return vec4(10, 77, 60, 255);
		case CSA:
			return vec4(4, 121, 214, 255);
		default:
			break;
		}
		return vec4(0, 0, 0, 0);
	}
};

