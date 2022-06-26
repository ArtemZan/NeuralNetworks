#include "Math.h"
#include "Logging.h"
#include <vector>
#include <iostream>

namespace nn
{
	namespace math
	{
		//vector
		vec::vec(int size, const float* data)
		{
			if (!data)
			{
				this->data.resize(size, 0);
				return;
			}

			this->data.insert(this->data.begin(), std::initializer_list<float>(data, data + size));
		}

		vec::vec(std::initializer_list<float> data)
			:data(data)
		{

		}

		vec::vec(int size, float scale)
		{
			this->data.resize(size, scale);
		}

		vec::vec(const std::vector<float>& data)
		{
			this->data = data;
		}

		vec& vec::addElement(float value)
		{
			data.push_back(value);
			return *this;
		}

		vec& vec::addElements(size_t count, float value)
		{
			data.resize(data.size() + count, value);
			return *this;
		}

		vec& elementwise(vec& vec1, const vec& vec2, float(*oper)(float a, float b))
		{
			if (vec1.getSize() != vec2.getSize())
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return vec1;
			}

			const size_t size = vec1.getSize();

			for (int i = 0; i < size; i++)
			{
				vec1[i] = oper(vec1[i], vec2[i]);
			}

			return vec1;
		}


		vec& vec::operator+=(const vec& v) {
			return elementwise(*this, v, [](float a, float b) -> float {return a + b; });
		}

		vec& vec::operator-=(const vec& v) {
			return elementwise(*this, v, [](float a, float b) -> float {return a - b; });
		}

		vec& vec::operator*=(const vec& v) {
			return elementwise(*this, v, [](float a, float b) -> float {return a * b; });
		}

		vec& vec::operator*=(float k)
		{
			for (float& el : data)
			{
				el *= k;
			}

			return *this;
		}

		vec& vec::operator*=(const mat& mat)
		{
			return *this = mat * *this;
		}

		vec& vec::operator/=(const vec& v) {
			return elementwise(*this, v, [](float a, float b) -> float {return a / b; });
		}

		std::ostream& operator<<(std::ostream& stream, const vec& vec)
		{
			stream << "v" + vec.getSize() << "[";
			bool first = true;

			for (float el : vec.data)
			{
				if (!first)
				{
					stream << " ";
				}

				stream << el;
			}

			stream << "]";

			return stream;
		}


		vec nn::math::vec::map(float(*mapper)(float)) const
		{
			vec res = *this;

			for (float& el : res.data)
			{
				el = mapper(el);
			}

			return res;
		}

		mat vec::asMatrix() const
		{
			return mat(1, getSize(), data.data());
		}


		//matrix

		mat::mat(int width, int height, const float* data)
		{
			if (!data)
			{
				this->data.resize(width, vec(height, 0.f));
				return;
			}

			this->data.reserve(width);

			for (int c = 0; c < width; c++) //c for column
			{
				this->data.emplace_back(height, data + c * height);
			}
		}

		mat::mat(const std::vector<vec> data)
			:data(data)
		{

		}

		vec mat::operator*(const vec& v) const
		{

			if (v.getSize() != data.size())
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return v;
			}

			const size_t columnsCount = data.size();

			if (columnsCount == 0)
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return v;
			}

			vec res(data[0].getSize());

			for (int c = 0; c < data.size(); c++)
			{
				res += data[c] * v.data[c];
			}

			return res;
		}

		mat mat::operator*(const mat& m) const
		{
			if (getWidth() != m.getHeight())
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return *this;
			}

			mat res(m.getWidth(), getHeight());

			for (int c = 0; c < m.getWidth(); c++)
			{
				res.data[c] = *this * m.data[c];
			}

			return res;
		}

		mat& mat::operator+=(const mat& mat)
		{
			if (getWidth() != mat.getWidth() || getHeight() != mat.getHeight())
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return *this;
			}

			for (int c = 0; c < getWidth(); c++)
			{
				data[c] += mat.data[c];
			}

			return *this;
		}

		mat mat::transpose() const
		{
			mat res(getHeight(), getWidth());

			for (int sc = 0; sc < getWidth(); sc++) // sc for 'source column'
			{
				for (int sr = 0; sr < getHeight(); sr++)
				{
					res.data[sr].data[sc] = data[sc].data[sr];
				}
			}

			return res;
		}

	}
}