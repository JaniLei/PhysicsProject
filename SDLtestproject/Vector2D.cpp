#include "Vector2D.h"


Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2D::~Vector2D()
{
}

Vector2D operator+(const Vector2D &v1, const Vector2D &v2)
{
	Vector2D result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}

Vector2D operator-(const Vector2D &v1, const Vector2D &v2)
{
	Vector2D result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vector2D operator*(const Vector2D &v, const float s)
{
	Vector2D result;
	result.x = v.x * s;
	result.y = v.y * s;
	return result;
}

Vector2D operator*(const Vector2D &v1, const Vector2D &v2)
{
	Vector2D result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	return result;
}

Vector2D operator/(const Vector2D &v, const float s)
{
	Vector2D result;
	result.x = v.x / s;
	result.y = v.y / s;
	return result;
}

Vector2D operator/(const Vector2D &v1, const Vector2D &v2)
{
	Vector2D result;
	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	return result;
}

Vector2D Vector2D::operator++()
{
	x++;
	y++;
	return (*this);
}

Vector2D Vector2D::operator++(int)
{
	Vector2D ret = (*this);
	++(*this);
	return ret;
}

Vector2D Vector2D::operator--()
{
	x--;
	y--;
	return (*this);
}

Vector2D Vector2D::operator--(int)
{
	Vector2D ret = (*this);
	--(*this);
	return ret;
}

bool operator<(const Vector2D &v1, const Vector2D &v2)
{
	return (v1.x < v2.x && v1.y < v2.y);
}

bool operator>(const Vector2D &v1, const Vector2D &v2)
{
	return (v1.x > v2.x && v1.y > v2.y);
}

std::ostream& operator<<(std::ostream &os, const Vector2D &v)
{
	os << "x = " << v.x << ", y = " << v.y;
	return os;
}

double Vector2D::GetLength()
{
	return abs(sqrt(powf(x, 2) + powf(y, 2)));
}

void Vector2D::Normalize()
{
	x /= GetLength();
	y /= GetLength();
}

void Vector2D::print()
{
	std::cout << "x = " << x << ", y = " << y << std::endl;
}
