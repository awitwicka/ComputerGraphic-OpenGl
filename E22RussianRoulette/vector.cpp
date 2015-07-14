/*!
*	@file	vector.cpp
*	@brief	Simple n-dimensional vector class with most common operations
*	@author Bastian Rieck, IWR Heidelberg, June 2010
*/

#include <cmath>
#include <stdexcept>

#include "vector.h"

/*!
*	Default constructor; creates the zero vector in n dimensions.
*/
template<unsigned long n> Vector<n>::Vector(void)
{
	for(unsigned long i = 0; i < n; i++)
		v[i] = 0;
}

/*!
*	Adds vector a to current vector and returns the result.
*/
template<unsigned long n> Vector<n> Vector<n>::operator+(const Vector a) const
{
	Vector<n> res;
	for(unsigned long i = 0; i < n; i++)
		res.v[i] = v[i] + a.v[i];

	return(res);
}

/*!
*	Adds vector a to current vector, thereby changing it.
*/
template<unsigned long n> Vector<n>& Vector<n>::operator+=(const Vector<n> a)
{
	for(unsigned long i = 0; i < n; i++)
		v[i] += a.v[i];

	return(*this);
}

/*!
*	Subtracts vector a from current vector and returns the result.
*/
template<unsigned long n> Vector<n> Vector<n>::operator-(const Vector<n> a) const
{
	Vector<n> res;
	for(unsigned long i = 0; i < n; i++)
		res.v[i] = v[i] - a.v[i];

	return(res);
}

/*!
*	Subtracts vector a from current vector, thereby changing it.
*/
template<unsigned long n> Vector<n>&  Vector<n>::operator-=(const Vector<n> a)
{
	for(unsigned long i = 0; i < n; i++)
		v[i] -= a.v[i];

	return(*this);
}

/*
*	Returns result of multiplication of current vector with scalar value a.
*/
template<unsigned long n> Vector<n> Vector<n>::operator*(double a) const
{
	Vector<n> res;
	for(unsigned long i = 0; i < n; i++)
		res.v[i] = v[i] * a;

	return(res);
}

/*!
*	Multiplies current vector by scalar value a. This function _overwrites_
*	the current vector.
*/
template<unsigned long n> Vector<n>& Vector<n>::operator*=(double a)
{
	for(unsigned long i = 0; i < n; i++)
		v[i] *= a;

	return(*this);
}

/*!
*	Returns result of division of the current vector by scalar value a.
*	Might throw a range_error exception.
*/
template<unsigned long n> Vector<n> Vector<n>::operator/(double a) const
{
	if(a == 0.0)
		throw(std::range_error("Vector<n>::operator/: Attempted division by zero"));
	else
		return(operator*(1/a));
}

/*!
*	Divides current vector by scalar a. This function _overwrites_ the
*	current vector.
*/
template<unsigned long n> Vector<n>& Vector<n>::operator/=(double a)
{
	if(a == 0.0)
		throw(std::range_error("Vector<n>::operator/=: Attempted division by zero"));
	else
		return(operator*=(1/a));
}

/*!
*	Copy constructor. Assigns vector a to the current vector, thereby
*	overwriting it.
*/
template<unsigned long n> Vector<n>& Vector<n>::operator=(const Vector a)
{
	for(unsigned long i = 0; i < n; i++)
		v[i] = a.v[i];

	return(*this);
}

/*!
*	Calculates the scalar (dot) product of the current vector and
*	vector a.
*/
template<unsigned long n> double Vector<n>::operator*(const Vector a) const
{
	double res = 0.0;
	for(unsigned long i = 0; i < n; i++)
		res += v[i]*a.v[i];

	return(res);
}

/*!
*	Enables element-wise access to the vector's components. Throws an
*	out_of_range exception.
*/
template<unsigned long n> double& Vector<n>::operator[](unsigned long i)
{
	if(i < n)
		return(v[i]);
	else
		throw(std::out_of_range("Vector<n>::operator[]: Index out ouf range"));
}

/*!
*	Normalizes the current vector and returns the result.
*/
template<unsigned long n> Vector<n> Vector<n>::normalize(void)
{
	if( length() == 0 )
		return(*this);
	else
		return(operator/(length()));
}

/*!
*	Computes the Euclidean length of the current vector, i.e., the square
*	root of the sum of the squares of the vector's components.
*/
template<unsigned long n> double Vector<n>::length(void)
{
	double len = 0.0;
	for(unsigned long i = 0; i < n; i++)
		 length += v[i]*v[i];

	return(sqrt(len));
}
