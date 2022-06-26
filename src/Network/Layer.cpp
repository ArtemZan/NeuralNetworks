#include "Layer.h"

namespace nn
{
	Layer::Layer(const std::shared_ptr<Layer>& prev, size_t nodes_number, bool bind)
		:biases(0), prevWeights(0, 0), prev(std::weak_ptr<Layer>(prev))
	{
		if (prev)
		{
			prevWeights.data.resize(prev->getSize(), math::vec(0));
		}

		AddNodes(nodes_number, bind);
	}

}