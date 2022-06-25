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
		m_layers.resize(layers_count);
		srand(time(0));
	}

	Network::~Network()
	{

	}

	void Network::AddNode(int layer, bool bind)
	{
		if (m_layers.size() <= layer)
			NN_WARNING(DEFUNCT_LAYER);

		m_layers[layer].push_back(std::make_shared<Node>());
		std::shared_ptr<Node>& n = m_layers[layer].back();

		if (bind)
		{
			if (layer)
			{
				for (std::shared_ptr<Node>& prev : m_layers[layer - 1])
				{
					Node::Bond::Create(prev, n);
				}
			}

			if (layer < m_layers.size() - 1)
			{
				for (std::shared_ptr<Node>& next : m_layers[layer + 1])
				{
					Node::Bond::Create(n, next);
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

		m_layers[layer].reserve(m_layers[layer].size() + number);
		while (number--) 
			AddNode(layer, bind);
	}


	std::vector<double> Network::Handle(const std::vector<double>& input)
	{
		return Handle(input, m_layers.size() - 1);
	}

	std::vector<double> Network::Handle(const std::vector<double>& input, int output_layer)
	{
		if (input.size() != m_layers[0].size())
		{
			NN_ERROR(SIZE_MISMATCH);
			return std::vector<double>();
		}

		std::vector<double> last_output = input;

		for (int l = 1; l < m_layers.size() && l <= output_layer; l++)
		{
			const std::vector<std::shared_ptr<Node>>& layer = m_layers[l];

			std::vector<double> out(layer.size());

			for (int n = 0; n < layer.size(); n++)
			{
				out[n] = layer[n]->Handle(last_output);
			}

			last_output = out;
		}

		return last_output;
	}

	/*void Network::HandlingInfo(NNData input, NNData* outputs)
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
	}*/

	void Network::Tune(const std::vector<double>& input, const std::vector<double>& desired_output)
	{
		Tune(input, desired_output, m_layers.size() - 1);
	}

	void Network::Tune(const std::vector<double>& input, const std::vector<double>& desired_output, int last_layer)
	{
		if (last_layer <= 0)
		{
			return;
		}

		const std::vector<double> output = Handle(input, last_layer);
		const std::vector<double> prev_output = Handle(input, last_layer - 1);

		std::vector<std::shared_ptr<Node>> layer = m_layers[last_layer];
		std::vector<std::shared_ptr<Node>> prev_layer = m_layers[last_layer - 1];

		if (desired_output.size() != layer.size())
		{
			NN_ERROR(SIZE_MISMATCH);
			return;
		}

		// To do: check for the first layer (base case)
		//std::vector<double> prev_layer_error(prev_layer.size(), 0);

		for (int i = 0; i < desired_output.size(); i++)
		{
			double dif = desired_output[i] - output[i];

			std::shared_ptr<Node>& node = layer[i];

			double total_weight = 0;

			for (std::shared_ptr<Node>& prev : prev_layer)
			{
				total_weight += abs(prev->GetBondToNext(node.get())->weight);
			}

			for (int prev = 0; prev < prev_layer.size(); prev++)
			{
				Node::Bond* bond1 = prev_layer[prev]->GetBondToNext(node.get());
				Node::Bond* bond2 = node->GetBondToPrev(prev_layer[prev].get());

				const double weight = bond1->weight;

				const double bond_error = dif * weight / total_weight;

				const double bond_change = -0.01 * bond_error * output[i] * (1 - output[i])* prev_output[prev];

				//prev_layer_error[prev] += bond_error;

				if(bond1) bond1->weight += bond_change;
				if(bond2) bond2->weight += bond_change;
			}
		}

		/*std::vector<double> desired_prev_output = Handle(input, last_layer - 1);

		for (int node = 0; node < prev_layer.size(); node++) {
			desired_prev_output[node] -= prev_layer_error[node] / 10;
		}*/

		//Tune(input, desired_prev_output, last_layer - 1);
	}

	std::vector<std::shared_ptr<Node>>& Network::operator[](int index)
	{
		return m_layers[index];
	}
}

