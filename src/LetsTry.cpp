#include "Network/Network.h"
#include <iostream>


int main()
{
	Network network(2);
	network.AddNodes(0, 3);
	network.AddNodes(1, 3);
	//network.AddNodes(2, 3);


	double ar[]{ 2.0, 0.3, 3.1};
	NNData input(ar, 3);
	network.Handle(input);
	//std::cout << v[2];
}