#include "stdafx.h"
#include <math.h>
#include "mbVector3D.h"


mbVector3D& mbVector3D::rotateAboutX(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

	float ny = c * y - s * z;
	float nz = c * z + s * y;

	y = ny;
	z = nz;

	return (*this);
}

mbVector3D& mbVector3D::rotateAboutY(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

	float nx = c * x + s * z;
	float nz = c * z - s * x;

	x = nx;
	z = nz;

	return (*this);
}

mbVector3D& mbVector3D::rotateAboutZ(float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

	float nx = c * x - s * y;
	float ny = c * y + s * x;

	x = nx;
	y = ny;

	return (*this);
}

mbVector3D& mbVector3D::rotateAboutAxis(float angle, const mbVector3D& axis)
{
	float s = sinf(angle);
	float c = cosf(angle);
	float k = 1.0F - c;

	float nx = x * (c + k * axis.x * axis.x) + y * (k * axis.x * axis.y - s * axis.z)
		+ z * (k * axis.x * axis.z + s * axis.y);
	float ny = x * (k * axis.x * axis.y + s * axis.z) + y * (c + k * axis.y * axis.y)
		+ z * (k * axis.y * axis.z - s * axis.x);
	float nz = x * (k * axis.x * axis.z - s * axis.y) + y * (k * axis.y * axis.z + s * axis.x)
		+ z * (c + k * axis.z * axis.z);

	x = nx;
	y = ny;
	z = nz;

	return (*this);
}