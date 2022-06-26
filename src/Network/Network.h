#pragma once
#include "Layer.h"
#include <vector>
#include <string>




namespace nn
{

	class Network
	{
	public:
		Network(int layers_count = 0);

		~Network();

		void AddNode(int layer, bool bind = true);
		void AddNodes(int layer, int number, bool bind = true);

		void AddLayer(int nodes_number, bool bind_nodes = true);

		math::vec Handle(const math::vec& input);

	private:

		/// <summary>
		/// Consumes the input and returns the output of the layer with index {output_layer}
		/// </summary>
		math::vec Handle(const math::vec& input, int output_layer);
	public:

		void Tune(const math::vec& input, const math::vec& desired_output);
		void Tune(const math::vec& input, const math::vec& desired_output, int last_layer);

		//std::vector< std::shared_ptr<Node>>& operator[](int index);

		float learningRate = 1;

	private:
		//void HandlingInfo(const std::vector<double>& input, const std::vector<double>* outputs);
		math::vec PropagateBack(const std::vector<math::vec>& outputs, const math::vec& error, size_t last_layer);

		std::vector<std::shared_ptr<Layer>> m_layers;
	};

};