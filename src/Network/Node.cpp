#include "Node.h"

//namespace nn
//{
//	Node::Node()
//	{
//
//	}
//
//	bool Node::SetWeight(unsigned int node_index, float weight)
//	{
//		if (node_index >= m_next.size())
//		{
//			return false;
//		}
//		m_next[node_index].weight = weight;
//		m_next[node_index].node->GetBondToPrev(this)->weight = weight;
//	}
//
//	Node::Bond* Node::GetBondToNext(Node* node)
//	{
//		for (Bond& bond : m_next)
//		{
//			if (bond.node.get() == node)
//			{
//				return &bond;
//			}
//		}
//
//		return nullptr;
//	}
//
//	Node::Bond* Node::GetBondToPrev(Node* node)
//	{
//		for (Bond& bond : m_previous)
//		{
//			if (bond.node.get() == node)
//			{
//				return &bond;
//			}
//		}
//
//		return nullptr;
//	}
//
//	double Node::Handle(const std::vector<double>& previous_output) const
//	{
//		double s = 0;
//		for (int i = 0; i < previous_output.size(); i++)
//		{
//			s += previous_output[i] * m_previous[i].weight;
//		}
//		return math::Sigmoid(s);// 1.0 / (1.0 + pow(10, -s));
//	}
//
//	void Node::Bond::Create(std::shared_ptr<Node>& first, std::shared_ptr<Node>& second, float weight)
//	{
//		first->m_next.emplace_back(second, weight);
//		second->m_previous.emplace_back(first, weight);
//	}
//
//}