/*!
*	@file	vector.h
*	@brief	Simple n-dimensional vector class with most common operations
*	@author Bastian Rieck, IWR Heidelberg, June 2010
*/

#ifndef SIMPLE_VECTORS_H
#define SIMPLE_VECTORS_H

template<unsigned long n> class Vector
{
	public:
		Vector(void);

		Vector<n> operator+(const Vector a) const;
		Vector<n>& operator+=(const Vector a);

		Vector<n> operator-(const Vector a) const;
		Vector<n>& operator-=(const Vector a);

		Vector<n> operator*(double a) const;
		Vector<n>& operator*=(double a);

		Vector<n> operator/(double a) const;
		Vector<n>& operator/=(double a);

		Vector<n>& operator=(const Vector a);

		double& operator[](unsigned long i);
		double	operator* (const Vector a) const;

		Vector<n> normalize(void);
		double length(void);

	private:

		double v[n];
};

#endif
