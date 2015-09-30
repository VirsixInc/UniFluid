#pragma once
class mbVector3D
{
public:

	float   x;
	float   y;
	float   z;

	mbVector3D() {}

	mbVector3D(float xInit, float yInit, float zInit)
	{
		x = xInit;
		y = yInit;
		z = zInit;
	}

	mbVector3D& Set(float xInit, float yInit, float zInit)
	{
		x = xInit;
		y = yInit;
		z = zInit;
		return (*this);
	}

	float& operator [](long k)
	{
		return ((&x)[k]);
	}

	const float& operator [](long k) const
	{
		return ((&x)[k]);
	}


	mbVector3D& operator +=(const mbVector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}


	mbVector3D& operator -=(const mbVector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	mbVector3D& operator *=(float t)
	{
		x *= t;
		y *= t;
		z *= t;
		return (*this);
	}

	mbVector3D& operator /=(float t)
	{
		float f = 1.0F / t;
		x *= f;
		y *= f;
		z *= f;
		return (*this);
	}

	mbVector3D& operator %=(const mbVector3D& v)
	{
		float       r, s;

		r = y * v.z - z * v.y;
		s = z * v.x - x * v.z;
		z = x * v.y - y * v.x;
		x = r;
		y = s;

		return (*this);
	}

	mbVector3D& operator &=(const mbVector3D& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return (*this);
	}

	mbVector3D operator -(void) const
	{
		return (mbVector3D(-x, -y, -z));
	}

	mbVector3D operator +(const mbVector3D& v) const
	{
		return (mbVector3D(x + v.x, y + v.y, z + v.z));
	}

	mbVector3D operator -(const mbVector3D& v) const
	{
		return (mbVector3D(x - v.x, y - v.y, z - v.z));
	}


	mbVector3D operator *(float t) const
	{
		return (mbVector3D(x * t, y * t, z * t));
	}

	mbVector3D operator /(float t) const
	{
		float f = 1.0F / t;
		return (mbVector3D(x * f, y * f, z * f));
	}

	float operator *(const mbVector3D& v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	mbVector3D operator %(const mbVector3D& v) const
	{
		return (mbVector3D(y * v.z - z * v.y, z * v.x - x * v.z,
			x * v.y - y * v.x));
	}

	mbVector3D operator &(const mbVector3D& v) const
	{
		return (mbVector3D(x * v.x, y * v.y, z * v.z));
	}

	bool operator ==(const mbVector3D& v) const
	{
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	bool operator !=(const mbVector3D& v) const
	{
		return ((x != v.x) || (y != v.y) || (z != v.z));
	}

	mbVector3D& Normalize(void)
	{
		return (*this /= sqrtf(x * x + y * y + z * z));
	}

	mbVector3D& rotateAboutX(float angle);
	mbVector3D& rotateAboutY(float angle);
	mbVector3D& rotateAboutZ(float angle);
	mbVector3D& rotateAboutAxis(float angle, const mbVector3D& axis);
};


inline mbVector3D operator *(float t, const mbVector3D& v)
{
	return (mbVector3D(t * v.x, t * v.y, t * v.z));
}


inline float mbDot(const mbVector3D& v1, const mbVector3D& v2)
{
	return (v1 * v2);
}

inline mbVector3D mbCross(const mbVector3D& v1, const mbVector3D& v2)
{
	return (v1 % v2);
}

inline float mbMagnitude(const mbVector3D& v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float mbInverseMag(const mbVector3D& v)
{
	return (1.0F / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float mbSquaredMag(const mbVector3D& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}
