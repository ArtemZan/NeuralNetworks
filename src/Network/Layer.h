#pragma once
#include "src/Math.h"
#include "src/Logging.h"
#include <memory>

namespace nn
{
	class Layer
	{
	public:
		Layer(const std::shared_ptr<Layer>& prev, size_t nodes_number = 0, bool bind = true);

		size_t getSize() const
		{
			if (prevWeights.data.size() != 0 && biases.getSize() != prevWeights.data[0].getSize())
			{
				logging::Error(logging::NNerror::SIZE_MISMATCH);
				return 0;
			}

			return biases.getSize();
		}

		math::vec Handle(const math::vec& input)
		{
			return (prevWeights * input + biases).map(math::Sigmoid);
		}

		Layer& AddNode(bool bind)
		{
			return AddNodes(1, bind);
		}

		Layer& AddNodes(size_t nodes_number, bool bind = true)
		{
			biases.addElements(nodes_number, 0);

			if (!bind)
			{
				return *this;
			}

			for (math::vec& col : prevWeights.data)
			{
				col.addElements(nodes_number, 1);
			}

			if (!next.expired())
			{
				const auto nextPtr = next.lock();

				nextPtr->prevWeights.data.resize(nodes_number, math::vec(nextPtr->getSize(), 1));
			}

			return *this;
		}


		math::vec biases;
		math::mat prevWeights; // One column for each node of the previous layer; one row for each node of this layer

		std::weak_ptr<Layer> prev;
		std::weak_ptr<Layer> next;
	};
}
