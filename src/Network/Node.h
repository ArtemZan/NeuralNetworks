#pragma once
#include <vector>

struct NNData
{
	int m_size;
	double* m_data;
	NNData(int size = 0);
	NNData(double* data, int size);
	NNData(const NNData& data);
	NNData operator= (const NNData& data);
	~NNData();
};

class Node
{
public:
	Node();

	void BindTo(Node* node, float weight = (float)(rand() % 100) / 100);

	bool SetWeight(unsigned int node_index, float weight);

	struct Bond;

	Bond& GetBondToNext(Node* node);

	Bond& GetBondToPrev(Node* node);

	double Handle(const NNData& input) const;

private:
	struct Bond
	{
		Node* node;
		float weight;

		Bond(Node* node, float weidht)
			: node(node), weight(weight)
		{

		}
	};

	//relationships between layers
	std::vector<Bond> m_next;
	std::vector<Bond> m_previous;
};

