#pragma once
#include <vector>
#include <memory>
#include "../../glm/exponential.hpp"

namespace nn
{
	namespace math
	{
		inline double Sigmoid(double x)
		{
			return 1 / (1 + glm::exp(-x));
		}
	}

	class Node
	{
	public:
		Node();

		bool SetWeight(unsigned int node_index, float weight);

		struct Bond;

		Bond* GetBondToNext(Node* node);
		Bond* GetBondToPrev(Node* node);

		double Handle(const std::vector<double>& input) const;

		struct Bond
		{
			std::shared_ptr<Node> node;
			float weight;

			Bond(const std::shared_ptr<Node>& node, float weight)
				: node(node), weight(weight)
			{

			}

			static void Create(std::shared_ptr<Node>& first, std::shared_ptr<Node>& second, float weight = 1);
		};
	private:

		//relationships between layers
		std::vector<Bond> m_next;
		std::vector<Bond> m_previous;
	};
}
