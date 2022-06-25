#include "Network/Network.h"
#include <iostream>


int main()
{
	nn::Network network(2);
	network.AddNodes(0, 2);
	network.AddNodes(1, 1);


	std::vector<double> input1({ 0.1, 0.5 });
	std::vector<double> input2({ 0.5, 0.1 });
	std::vector<double> output;

	network.Handle(input1);
	network.Handle(input2);

	for (int i = 0; i < 100000; i++)
	{
		network.Tune(input1, { 0 });
		network.Tune(input2, { 1 });
		//std::cout << network[0][0]->GetBondToNext(network[1][0].get())->weight;
		//std::cout << network[0][1]->GetBondToNext(network[1][1].get())->weight;
		//std::cout << network[0][0]->GetBondToNext(network[1][0].get())->weight << "\n";
	}

	output = network.Handle(input1);


}