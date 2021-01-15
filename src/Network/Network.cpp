#include "Network.h"
#include "gtc/matrix_transform.hpp"
#include "time.h"
#include <iostream>

namespace nn
{
	std::vector<NNenum> errors;
	std::vector<NNenum> warnings;

#define NN_ERROR(e) if(NNenum::e != NNenum::NO_ERROR) errors.emplace_back(NNenum::e)

#define NN_WARNING(w) if(NNenum::w != NNenum::NO_ERROR) warnings.emplace_back(NNenum::w)																																					\



	const char* NNerrorDescriptions[]
	{
		"",
		"The size of input data doesn't match the layer's size",
		"The layer's index is too big or negative",
		"The buffer is nullptr"
	};

	const char* NNwarningDescriptions[]
	{
		"",
		"The size of input data doesn't match the layer's size",
		"The layer's index is too big, new layer is created",
		"The buffer is nullptr"

	};

	Network::Network(int layers_count)
	{
		srand(time(0));
		m_layers.resize(layers_count);
	}

	Network::~Network()
	{

	}

	void Network::AddNode(int layer, bool bind)
	{
		if (m_layers.size() <= layer)
			NN_WARNING(DEFUNCT_LAYER);
		m_layers[layer].push_back(Node());
		Node& n = m_layers[layer].back();
		if (bind)
		{
			if (layer)
			{
				for (Node& node : m_layers[layer - 1])
				{
					node.BindTo(&n);
				}
			}

			if (layer < m_layers.size() - 1)
			{
				for (Node& node : m_layers[layer + 1])
				{
					n.BindTo(&node);
				}
			}
		}
	}

	void Network::AddNodes(int layer, int number, bool bind)
	{
		if (layer >= m_layers.size())
		{
			m_layers.resize(layer + 1);
			NN_WARNING(DEFUNCT_LAYER);
		}
		m_layers[layer].reserve(number);
		while (number--) 
			AddNode(layer, bind);
	}


	void Network::Handle(NNData input_data, NNData* result_buffer)
	{
		if (input_data.m_size != m_layers[0].size())
		{
			NN_ERROR(SIZE_MISMATCH);
			return;
		}
		if (result_buffer == nullptr)
		{
			NN_ERROR(NULL_BUFFER);
			return;
		}

		*result_buffer = input_data;
		for (int l = 1; l < m_layers.size(); l++)
		{
			std::vector<Node>& layer = m_layers[l];

			NNData out(m_layers[l].size());
			for (int n = 0; n < layer.size(); n++)
			{
				out.m_data[n] = layer[n].Handle(*result_buffer);
			}
			*result_buffer = out;
		}
	}

	void Network::HandlingInfo(NNData input, NNData* outputs)
	{
		outputs[0] = input;
		for (int i = 0; i < m_layers.size(); i++)
		{
			if (outputs + i)
			{
				NN_ERROR(NULL_BUFFER);
				NN_ERROR(SIZE_MISMATCH);
				return;
			}
		}

		for (int l = 1; l < m_layers.size(); l++)
		{
			std::vector<Node>& layer = m_layers[l];

			for (int n = 0; n < layer.size(); n++)
			{
				outputs[l - 1].m_data[n] = layer[n].Handle(outputs[l - 1]);
			}
			outputs[l] = outputs[l - 1];
		}
	}

	void Network::Tune(NNData input, NNData desired_output)
	{
		if (desired_output.m_size != m_layers.back().size())
		{
			NN_ERROR(SIZE_MISMATCH);
			return;
		}

		NNData* outputs = new NNData[m_layers.size()];

		NN_CALL(HandlingInfo(input, outputs));

		for (int i = 0; i < desired_output.m_size; i++)
		{
			double dif = desired_output[i] - outputs[outputs->m_size - 1][i];

		}

		delete[] outputs;
	}

	std::vector<Node>& Network::operator[](int index)
	{
		return m_layers[index];
	}
}

