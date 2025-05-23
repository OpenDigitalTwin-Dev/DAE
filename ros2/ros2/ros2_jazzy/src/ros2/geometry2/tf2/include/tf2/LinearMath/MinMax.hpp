/*
Copyright (c) 2003-2006 Gino van den Bergen / Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/



#ifndef TF2__LINEARMATH__MINMAX_HPP_
#define TF2__LINEARMATH__MINMAX_HPP_

#include "Scalar.hpp"

template <class T>
TF2SIMD_FORCE_INLINE const T& tf2Min(const T& a, const T& b) 
{
  return a < b ? a : b ;
}

template <class T>
TF2SIMD_FORCE_INLINE const T& tf2Max(const T& a, const T& b) 
{
  return  a > b ? a : b;
}

template <class T>
TF2SIMD_FORCE_INLINE const T& GEN_clamped(const T& a, const T& lb, const T& ub) 
{
	return a < lb ? lb : (ub < a ? ub : a); 
}

template <class T>
TF2SIMD_FORCE_INLINE void tf2SetMin(T& a, const T& b) 
{
    if (b < a) 
	{
		a = b;
	}
}

template <class T>
TF2SIMD_FORCE_INLINE void tf2SetMax(T& a, const T& b) 
{
    if (a < b) 
	{
		a = b;
	}
}

template <class T>
TF2SIMD_FORCE_INLINE void GEN_clamp(T& a, const T& lb, const T& ub) 
{
	if (a < lb) 
	{
		a = lb; 
	}
	else if (ub < a) 
	{
		a = ub;
	}
}

#endif  // TF2__LINEARMATH__MINMAX_HPP_
