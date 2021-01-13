#include "Math.h"
#include <vector>
#include <iostream>

namespace math
{
	//vector
	vec::vec(int size, const float* data)
		:m_size(size)
	{
		m_data = new float[size];
		for (int i = 0; i < size && data + i != nullptr; i++)
		{
			m_data[i] = data[i];
		}
	}

	vec::~vec()
	{
		delete[] m_data;
	}

#define defOper(_op_) 	vec vec::operator _op_(const vec v) \
	{														\
		if (m_size != v.m_size)								\
		{													\
			return *this;									\
		}													\
															\
		vec res = *this;									\
															\
		for (int i = 0; i < m_size; i++) 					\
		{													\
			res[i] = res[i] _op_ v[i];						\
		}													\
															\
		return res;											\
	}	

	defOper(*);
	defOper(/);
	defOper(-);
	defOper(+);

	//matrix
	mat::mat(int width, int height, const float* data)
	{
		m_data.reserve(height);
		for (int i = 0; i < height; i++)
		{
			m_data.emplace_back(width, data + i * width);
		}
	}

	mat::~mat()
	{
	}

}
