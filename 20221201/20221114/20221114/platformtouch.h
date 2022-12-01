#pragma once

#include "framework.h"
#include <vector>

typedef struct tagplatform
{

	RECT pf;

}PLATFORM;
			
std::vector<tagplatform> pf_vector;

BOOL platformtouch(LPRECT rect, CONST RECT * rect1, CONST PLATFORM * rect2)
{
	if (rect1->top <= rect2->pf.bottom &&
		rect1->right >= rect2->pf.left &&
		rect1->left <= rect2->pf.right &&
		rect1->bottom >= rect2->pf.top)
	{
		//top
		if (rect1->top > rect2->pf.top)
		{
			rect->top = rect2->pf.top;
		}
		else if (rect1->top < rect2->pf.top)
		{
			rect->top = rect1->top;
		}

		//bottom
		if (rect1->bottom < rect2->pf.bottom)
		{
			rect->bottom = rect2->pf.bottom;
		}
		else if (rect1->bottom > rect2->pf.bottom)
		{
			rect->bottom = rect1->bottom;
		}

		//right
		if (rect1->right < rect2->pf.right)
		{
			rect->right = rect2->pf.right;
		}
		else if (rect1->right > rect2->pf.right)
		{
			rect->right = rect1->right;
		}

		//left
		if (rect1->left > rect2->pf.left)
		{
			rect->left = rect2->pf.left;
		}
		else if (rect1->left < rect2->pf.left)
		{
			rect->left = rect1->left;
		}

		return true;
	}

	else
		return false;
}

class platform;