#pragma once
#include "../Common/Math/culling2d.RectF.h"
#include "culling2d.Object.h"
#include <vector>
#include <array>

namespace culling2d
{
	class Grid
	{
		int resolutionLevel;
		RectF gridRange;
		int index;
		std::set<Object*> objects;
	public:
		Grid(int resolutionLevel,RectF gridRange);
		~Grid();
		int GetResolutionLevel();
		RectF GetGridRange();

		static std::array<int,4> GetChildrenIndices(int index);
		
		bool AddObject(Object* object);
		bool RemoveObject(Object* object);
	};
}