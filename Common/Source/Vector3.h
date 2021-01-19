#pragma once

#include <iostream>
#include "MyMath.h"

#pragma warning(push)
#pragma warning(disable: 4290) //For throw(DivideByZero)

struct Vector3{
	Vector3();
	Vector3(float xyz);
	Vector3(float x, float y, float z);
	Vector3(const Vector3&);
	~Vector3() = default;

	void Set(float, float, float); //Set all data
	void SetZero(); //Set all data to zero
	bool IsZero() const; //Check if data is zero
	bool IsEqual(float, float) const;

	Vector3 operator+(const Vector3&) const; //Vector addition
	Vector3& operator+=(const Vector3&);
	Vector3 operator-(const Vector3&) const; //Vector subtraction
	Vector3& operator-=(const Vector3&);
	Vector3 operator-() const; //Unary negation
	Vector3 operator*(float) const; //Scalar multiplication
	Vector3& operator*=(float);
	bool operator==(const Vector3&) const; //Equality check
	bool operator!=(const Vector3&) const; //Inequality check
	Vector3& operator=(const Vector3&);

	float Length() const;
	float LengthSquared() const;

	float Dot(const Vector3&) const;
	Vector3 Cross(const Vector3&) const;

	Vector3 Normalized() const throw(DivideByZero); //Return a copy of this vector normalized, throw a divide by zero exception if normalizing a zero/... vector
	Vector3& Normalize() throw(DivideByZero); //Normalize this vector and return a reference to it, ...

	friend std::ostream& operator<<(std::ostream&, Vector3&); //Print to ostream
	friend Vector3 operator*(float, const Vector3&);

	float x;
	float y;
	float z;
};