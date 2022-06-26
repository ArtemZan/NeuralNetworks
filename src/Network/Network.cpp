#include "Network.h"
//#include "gtc/matrix_transform.hpp"
//#include "time.h"
#include <iostream>
#include "src/Logging.h"

namespace nn
{

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
			logging::Error(logging::NNerror::DEFUNCT_LAYER);

		m_layers[layer]->AddNode(bind);

		/*m_layers[layer].push_back(std::make_shared<Node>());
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
		}*/
	}

	void Network::AddNodes(int layer, int number, bool bind)
	{
		if (layer >= m_layers.size())
		{
			m_layers.resize(layer + 1);
			logging::Error(logging::NNerror::DEFUNCT_LAYER);
		}

		//m_layers[layer].reserve(m_layers[layer].size() + number);
		m_layers[layer]->AddNodes(number, bind);
	}

	void Network::AddLayer(int nodes_number, bool bind_nodes)
	{
		if (m_layers.size())
		{
			m_layers.push_back(std::make_shared<Layer>(m_layers.front(), nodes_number, bind_nodes));
			return;
		}

		m_layers.push_back(std::make_shared<Layer>(std::shared_ptr<Layer>(), nodes_number, bind_nodes));
	}


	math::vec Network::Handle(const math::vec& input)
	{
		return Handle(input, m_layers.size() - 1);
	}

	math::vec Network::Handle(const math::vec& input, int output_layer)
	{
		if (input.getSize() != m_layers[0]->getSize())
		{
			logging::Error(logging::NNerror::SIZE_MISMATCH);
			return math::vec(0);
		}

		math::vec last_output = input;

		for (int l = 1; l < m_layers.size() && l <= output_layer; l++)
		{
			const auto& layer = m_layers[l];

			last_output = layer->Handle(last_output);
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

	void Network::Tune(const math::vec& input, const math::vec& desired_output)
	{
		Tune(input, desired_output, m_layers.size() - 1);
	}

	void Network::Tune(const math::vec& input, const math::vec& desired_output, int last_layer)
	{
		if (last_layer <= 0)
		{
			return;
		}

		if (input.getSize() != m_layers[0]->getSize())
		{
			logging::Error(logging::NNerror::SIZE_MISMATCH);
			return;
		}

		math::vec last_output = input;

		std::vector<math::vec> outputs;

		outputs.reserve(m_layers.size());
		outputs.push_back(input);

		for (int l = 1; l < m_layers.size() && l <= last_layer; l++)
		{
			const auto& layer = m_layers[l];

			last_output = layer->Handle(last_output);

			outputs.push_back(last_output);
		}



		/*std::vector<std::shared_ptr<Node>> layer = m_layers[last_layer];
		std::vector<std::shared_ptr<Node>> prev_layer = m_layers[last_layer - 1];

		if (desired_output.size() != layer.size())
		{
			logging::Error(logging::NNerror::SIZE_MISMATCH);
			return;
		}*/

		math::vec error = desired_output - last_output;

		for (int l = m_layers.size() - 1; l > 0; l--)
		{
			error = PropagateBack(outputs, error, l);
		}

		// To do: check for the first layer (base case)
		//std::vector<double> prev_layer_error(prev_layer.size(), 0);

		//for (int i = 0; i < desired_output.size(); i++)
		//{
		//	double dif = desired_output[i] - output[i];

		//	std::shared_ptr<Node>& node = layer[i];

		//	double total_weight = 0;

		//	for (std::shared_ptr<Node>& prev : prev_layer)
		//	{
		//		total_weight += abs(prev->GetBondToNext(node.get())->weight);
		//	}

		//	for (int prev = 0; prev < prev_layer.size(); prev++)
		//	{
		//		Node::Bond* bond1 = prev_layer[prev]->GetBondToNext(node.get());
		//		Node::Bond* bond2 = node->GetBondToPrev(prev_layer[prev].get());

		//		const double weight = bond1->weight;

		//		const double bond_error = dif * weight / total_weight;

		//		const double bond_change = -0.01 * bond_error * output[i] * (1 - output[i]) * prev_output[prev];

		//		//prev_layer_error[prev] += bond_error;

		//		if (bond1) bond1->weight += bond_change;
		//		if (bond2) bond2->weight += bond_change;
		//	}
		//}

		/*std::vector<double> desired_prev_output = Handle(input, last_layer - 1);

		for (int node = 0; node < prev_layer.size(); node++) {
			desired_prev_output[node] -= prev_layer_error[node] / 10;
		}*/

		//Tune(input, desired_prev_output, last_layer - 1);
	}

	math::vec Network::PropagateBack(const std::vector<math::vec>& outputs, const math::vec& error, size_t last_layer)
	{
		const math::vec lastOutput = outputs[last_layer];

		const math::vec prevOutput = outputs[last_layer - 1];

		// The error for the previous layer
		const math::vec distributedError = m_layers[last_layer]->prevWeights.transpose() * error;


		// Update the weights

		const math::vec actFuncDerivative = lastOutput * (lastOutput.map([](float v)->float {return 1 - v; }));

		math::mat dWeight = (error * actFuncDerivative * learningRate).asMatrix() * prevOutput.asMatrix().transpose();

		m_layers[last_layer]->prevWeights += dWeight;

		return distributedError;
	}
}

