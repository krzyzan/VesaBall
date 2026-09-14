#ifndef VEC2_H
#define VEC2_H

#include <cmath>

//! Portable 2D vector, replacing DirectX 8's D3DXVECTOR2.
/*!
	Same shape and semantics as D3DXVECTOR2 (member x/y, the usual
	arithmetic operators, and the handful of free functions the game
	actually used), so call sites needed only a mechanical rename.
*/
struct Vec2
{
	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2 & operator+=(const Vec2 & v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vec2 & operator-=(const Vec2 & v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vec2 & operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	Vec2 & operator/=(float f)
	{
		x /= f;
		y /= f;
		return *this;
	}

	Vec2 operator+() const { return *this; }
	Vec2 operator-() const { return Vec2(-x, -y); }

	Vec2 operator+(const Vec2 & v) const { return Vec2(x + v.x, y + v.y); }
	Vec2 operator-(const Vec2 & v) const { return Vec2(x - v.x, y - v.y); }
	Vec2 operator*(float f) const { return Vec2(x * f, y * f); }
	Vec2 operator/(float f) const { return Vec2(x / f, y / f); }

	bool operator==(const Vec2 & v) const { return x == v.x && y == v.y; }
	bool operator!=(const Vec2 & v) const { return x != v.x || y != v.y; }

	float x, y;
};

inline Vec2 operator*(float f, const Vec2 & v)
{
	return Vec2(f * v.x, f * v.y);
}

inline float Vec2Length(const Vec2* v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

inline Vec2* Vec2Normalize(Vec2* out, const Vec2* in)
{
	float len = Vec2Length(in);
	if (len > 0.0f)
	{
		out->x = in->x / len;
		out->y = in->y / len;
	}
	else
	{
		out->x = 0.0f;
		out->y = 0.0f;
	}
	return out;
}

inline Vec2* Vec2Maximize(Vec2* out, const Vec2* a, const Vec2* b)
{
	out->x = (a->x > b->x) ? a->x : b->x;
	out->y = (a->y > b->y) ? a->y : b->y;
	return out;
}

inline Vec2* Vec2Minimize(Vec2* out, const Vec2* a, const Vec2* b)
{
	out->x = (a->x < b->x) ? a->x : b->x;
	out->y = (a->y < b->y) ? a->y : b->y;
	return out;
}

//! Rotates v by 'radians' (counter-clockwise).
/*!
	Replaces D3DXMatrixRotationZ() + D3DXVec2TransformCoord(), which the
	original code only ever used together, for the EightBall bonus.
*/
inline Vec2 Vec2Rotate(const Vec2 & v, float radians)
{
	float s = sinf(radians);
	float c = cosf(radians);
	return Vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}

const float PI = 3.14159265358979323846f;

#endif
