#include "Network/Network.h"
#include <iostream>


int main()
{
	nn::Network network(2);
	network.AddNodes(0, 3);
	network.AddNodes(1, 3);
	network.AddNodes(2, 1);


	double ar[]{ 2.0, 0.3, 3.1};
	double ar1[]{ 0.4, 1.3, 1.5};
	nn::NNData input(ar, 3);
	nn::NNData output;
	NN_CALL(network.Handle(input, &output));
	double res[3];
	res[0] = output.m_data[0];
	res[1] = output.m_data[1];
	res[2] = output.m_data[2];
}