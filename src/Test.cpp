#include "Network/Network.h"
#include <iostream>

using namespace nn;

int main()
{
	nn::Network network;
	network.AddLayer(2);
	network.AddLayer(1);

	std::vector<math::vec> testData({
		math::vec({0.1, 0.5}),
		math::vec({0.3, 0.5}),
		math::vec({0.4, 0.5}),
		math::vec({0.9, 0.1}),
		math::vec({0.1, 0.f}),
		math::vec({0.f, 0.f}),
		math::vec({0.1, 0.9}),
		math::vec({0.1, 0.6}),
		math::vec({0.1, 0.7})
		});

	std::vector<math::vec> testTarget({
		math::vec({0.6f}),
		math::vec({0.8f}),
		math::vec({0.9f}),
		math::vec({1.f}),
		math::vec({0.1f}),
		math::vec({0.f}),
		math::vec({1.f}),
		math::vec({0.7f}),
		math::vec({0.8f})
		});

	for (int i = 0; i < testData.size(); i++)
	{
		network.Tune(testData[i], testTarget[i]);
		//std::cout << network[0][0]->GetBondToNext(network[1][0].get())->weight;
		//std::cout << network[0][1]->GetBondToNext(network[1][1].get())->weight;
		//std::cout << network[0][0]->GetBondToNext(network[1][0].get())->weight << "\n";
	}

	std::cout << network.Handle({.3f, .4f}) << "\n";

}