#pragma once

#include <iostream>

class Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	~Vector2D();

	friend Vector2D operator+(const Vector2D &v1, const Vector2D &v2);
	friend Vector2D operator-(const Vector2D &v1, const Vector2D &v2);
	friend Vector2D operator*(const Vector2D &v, const float s);
	friend Vector2D operator*(const Vector2D &v1, const Vector2D &v2);
	friend Vector2D operator/(const Vector2D &v, const float s);
	friend Vector2D operator/(const Vector2D &v1, const Vector2D &v2);
	Vector2D operator++();
	Vector2D operator++(int);
	Vector2D operator--();
	Vector2D operator--(int);
	friend bool operator<(const Vector2D &v1, const Vector2D &v2);
	friend bool operator>(const Vector2D &v1, const Vector2D &v2);
	friend std::ostream& operator<<(std::ostream &os, const Vector2D &v);

	float x, y;

	double GetLength();
	void Normalize();
	void print();
};

