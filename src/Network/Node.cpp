#include "Node.h"

namespace nn
{
	NNData::NNData(int size)
		:m_data(new double[size]), m_size(size)
	{

	}

	NNData::NNData(double* data, int size)
		: m_data(new double[size]), m_size(size)
	{
		memcpy(m_data, data, size * sizeof(double));
	}

	const NNData& NNData::operator=(const NNData& data)
	{
		m_size = data.m_size;
		m_data = new double[m_size];
		memcpy(m_data, data.m_data, m_size * sizeof(double));
		return *this;
	}

	NNData::NNData(const NNData& data)
		:m_data(new double[data.m_size]), m_size(data.m_size)
	{
		memcpy(m_data, data.m_data, m_size * sizeof(double));
	}

	NNData::~NNData()
	{
		delete[] m_data;
	}


	Node::Node()
	{

	}

	void Node::BindTo(Node* node, float weight)
	{
		if (weight <= 0)
			weight = (float)(rand() % 100) / 100;
		srand(rand());
		m_next.emplace_back(node, weight);
		node->m_previous.emplace_back(this, weight);
	}

	bool Node::SetWeight(unsigned int node_index, float weight)
	{
		if (node_index >= m_next.size())
		{
			return false;
		}
		m_next[node_index].weight = weight;
		m_next[node_index].node->GetBondToPrev(this).weight = weight;
	}

	Node::Bond& Node::GetBondToNext(Node* node)
	{
		for (Bond& bond : m_next)
		{
			if (bond.node == node)
			{
				return bond;
			}
		}
		Bond b(nullptr, 0);
		return b;
	}

	Node::Bond& Node::GetBondToPrev(Node* node)
	{
		for (Bond& bond : m_previous)
		{
			if (bond.node == node)
			{
				return bond;
			}
		}
		Bond b(nullptr, 0);
		return b;
	}

	double Node::Handle(const NNData& previous_output) const
	{
		double s = 0;
		for (int i = 0; i < previous_output.m_size; i++)
		{
			s += previous_output.m_data[i] * m_previous[i].weight;
		}
		return s;// 1.0 / (1.0 + pow(10, -s));
	}

	void Node::IncreaseOutput()
	{

	}

	void Node::DecreaseOutput()
	{

	}
}