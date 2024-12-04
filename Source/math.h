#pragma once
#include <math.h>
#include "unreal.h"

namespace pinguu
{
	namespace math
	{
		inline float deg_2_rad(float degrees)
		{
			float radians;
			radians = degrees * (3.14159265358979323846 / 180);
			return radians;
		}

		inline void angle_vectors(const fvector& angles, fvector* forward)
		{
			float    sp, sy, cp, cy;
			sy = sin(deg_2_rad(angles.y));
			cy = cos(deg_2_rad(angles.y));
			sp = sin(deg_2_rad(angles.x));
			cp = cos(deg_2_rad(angles.x));
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
	}
}