#include "Network.h"
#include "gtc/matrix_transform.hpp"

NNData::NNData(int size)
	:m_data(new double[size]), m_size(size)
{

}

NNData::NNData(double* data, int size)
	:m_data(new double[size]), m_size(size)
{
	memcpy(m_data, data, size * sizeof(double));
}

NNData NNData::operator=(const NNData& data)
{
	return data;
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



Network::Network(int layers_count)
{
	//m_layers.reserve(layers_count);
	m_layers.resize(layers_count);
}

Network::~Network()
{
	
}

void Network::AddNode(int layer, bool bind)
{
	Node n;
	m_layers[layer].push_back(n); 
	if (bind)
	{
		if (layer)
		{
			for (Node& node : m_layers[layer - 1])
			{
				node.BindTo(&m_layers[layer].back());
			}
		}

		if (layer < m_layers.size() - 1)
		{
			for (Node& node : m_layers[layer + 1])
			{
				m_layers[layer].back().BindTo(&node);
			}
		}
	}
}

bool Network::AddNodes(int layer, int number, bool bind)
{
	if (layer >= m_layers.size())
		return false;
	m_layers[layer].reserve(number);
	while (number--) AddNode(layer, bind); 
	return true;
}


NNData Network::Handle(NNData input_data)
{
	NNData output = input_data;
	for (int l = 1; l < m_layers.size(); l++)
	{
		std::vector<Node>& layer = m_layers[l]; // just to be easier

		NNData out(m_layers[l].size());
		for (int n = 0; n < layer.size(); n++)
		{
			out.m_data[n] = layer[n].Handle(output);
		}
		output = out;
	}
	return output;
}


std::vector<Node>& Network::operator[](int index)
{
	return m_layers[index];
}
