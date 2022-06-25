#pragma once
#include "Node.h"
#include <vector>
#include <string>

#define NN_CALL(x) nn::errors.clear();																										  \
nn::warnings.clear();																														  \
x;																																			  \
int i = strlen(__FILE__);																													  \
for (int d = 0; d < 3;)																														  \
	if (__FILE__[--i] == '\\') d++;																											  \
for (nn::NNenum e : nn::errors)																												  \
{																																			  \
	std::cout << "[Error] : \"" << nn::NNerrorDescriptions[e] << "\" in " << __FILE__ + i + 1 << " line " << __LINE__ << std::endl;			  \
}																																			  \
for (nn::NNenum w : nn::warnings)																											  \
{																																			  \
	std::cout << "[Warning] : \"" << nn::NNwarningDescriptions[w] << "\" in " << __FILE__ + i + 1 << " line " << __LINE__ << std::endl;		  \
}


namespace nn
{
	enum NNenum
	{
		NO_ERROR, SIZE_MISMATCH, DEFUNCT_LAYER, NULL_BUFFER
	};

	extern std::vector<NNenum> errors;
	extern std::vector<NNenum> warnings;

	inline NNenum GetError() { if (errors.size())	  return NNenum::NO_ERROR; NNenum r = errors[0];   errors.erase(errors.begin());     return r; }
	inline NNenum GetWarning() { if (warnings.size()) return NNenum::NO_ERROR; NNenum r = warnings[0]; warnings.erase(warnings.begin()); return r; }


	extern const char* NNerrorDescriptions[];
	extern const char* NNwarningDescriptions[];

	class Network
	{
	public:
		Network(int layers_count = 0);

		~Network();

		void AddNode(int layer, bool bind = true);
		void AddNodes(int layer, int number, bool bind = true);

		std::vector<double> Handle(const std::vector<double>& input);

	private:
		std::vector<double> Handle(const std::vector<double>& input, int output_layer);
	public:

		void Tune(const std::vector<double>& input, const std::vector<double>& desired_output);
		void Tune(const std::vector<double>& input, const std::vector<double>& desired_output, int last_layer);

		std::vector< std::shared_ptr<Node>>& operator[](int index);

	private:
		//void HandlingInfo(const std::vector<double>& input, const std::vector<double>* outputs);

		std::vector<std::vector<std::shared_ptr<Node>>> m_layers;
	};

};