#pragma once
#include "Node.h"
#include <vector>

class Network
{
public:
	Network(int layers_count);

	~Network();

	void AddNode(int layer, bool bind = true);
	bool AddNodes(int layer, int number, bool bind = true);

	NNData Handle(NNData input_data);

	std::vector<Node>& operator[](int index);

private:
	std::vector<std::vector<Node>>	m_layers;
};