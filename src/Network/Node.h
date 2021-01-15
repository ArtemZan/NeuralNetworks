#pragma once
#include <vector>

namespace nn
{
	struct NNData
	{
		int m_size;
		double* m_data;
		NNData(int size = 0);
		NNData(double* data, int size);
		NNData(const NNData& data);
		const NNData& operator= (const NNData& data);
		inline double& operator[](const int index) { return m_data[index]; }
		~NNData();
	};


	class Node
	{
	public:
		Node();

		void BindTo(Node* node, float weight = -1);

		bool SetWeight(unsigned int node_index, float weight);

		struct Bond;

		Bond& GetBondToNext(Node* node);
		Bond& GetBondToPrev(Node* node);

		double Handle(const NNData& input) const;

		void IncreaseOutput();
		void DecreaseOutput();

	private:
		struct Bond
		{
			Node* node;
			float weight;

			Bond(Node* node, float weight)
				: node(node), weight(weight)
			{

			}
		};

		//relationships between layers
		std::vector<Bond> m_next;
		std::vector<Bond> m_previous;
	};
}
