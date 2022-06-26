#pragma once
#include <vector>
#include <iostream>
#include "glm/exponential.hpp"

namespace nn
{
	namespace math
	{
		class mat;

		//To do: make iterable
		class vec
		{
		public:
			vec(int size, const float* data = nullptr);
			vec(std::initializer_list<float> data);
			vec(int size, float scale);
			vec(const std::vector<float>& data);

			vec& addElement(float value);
			vec& addElements(size_t count, float value);

			inline float& operator[](int index) { return data[index]; }
			inline const float& operator[](int index) const { return data[index]; }
			inline size_t getSize() const { return data.size(); }

			inline vec operator*(const vec& v) const { return vec(*this) *= v; }
			inline vec operator*(float k) const { return vec(*this) *= k; }
			inline vec operator/(const vec& v) const { return vec(*this) /= v; }
			inline vec operator/(float k) const { return *this * (1 / k); }
			inline vec operator+(const vec& v) const { return vec(*this) += v; }
			inline vec operator-(const vec& v) const { return vec(*this) -= v; };

			vec& operator*=(const vec& v);
			vec& operator*=(float k);
			vec& operator*=(const mat& mat);
			vec& operator/=(const vec& v);
			inline vec& operator/=(float k) { return *this *= (1 / k); }
			vec& operator+=(const vec& v);
			vec& operator-=(const vec& v);

			vec map(float(*mapper)(float)) const;

			mat asMatrix() const;

			std::vector<float> data;
		};


		std::ostream& operator<<(std::ostream& stream, const vec& vec);


		class mat
		{
		public:
			mat(int width, int height, const float* data = nullptr);
			mat(const std::vector<vec> data);

			vec operator*(const vec& vec) const;
			mat operator*(const mat& mat) const;

			mat& operator+=(const mat& mat);

			mat transpose() const;

			inline size_t getWidth() const
			{
				return data.size();
			}

			inline size_t getHeight() const
			{
				return data.front().getSize();
			}

			std::vector<vec> data;
		};

		template<typename T = float>
		inline T Sigmoid(T x)
		{
			return 1 / (1 + glm::exp(-x));
		}
	}
}