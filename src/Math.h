#pragma once
#include <vector>

namespace math
{
	class vec
	{
	public:
		vec(int size, const float* data);

		~vec();

		inline float& operator[](int index) const { return m_data[index]; }
		vec operator*(const vec v);
		vec operator/(const vec v);
		vec operator+(const vec v);
		vec operator-(const vec v);

	private:
		float m_size;
		float* m_data;
	};

	class mat
	{
	public:
		mat(int width, int height, const float* data);

		~mat();
	private:
		std::vector<vec> m_data;
	};
}