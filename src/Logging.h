#pragma once
#include <vector>
#include <string>

namespace nn
{
	namespace logging
	{
		enum NNerror
		{
			NO_ERROR, SIZE_MISMATCH, DEFUNCT_LAYER, NULL_BUFFER, INVALID_DIMENSION
		};

		extern const char* ErrorDescriptions[];

		void Error(NNerror type);

		std::vector<NNerror>& GetErrors();

		inline std::string GetErrorMessage(NNerror error) { return ErrorDescriptions[int(error)]; }

		void logErrors(const char* filename, int line);

#define NN_ERROR(e) if(NNerror::e != NNerror::NO_ERROR) Error(NNerror::e)

#define NN_CALL(x) nn::logging::GetErrors().clear();																						\
x;																																			\
			nn::logging::logErrors(__FILE__, __LINE__);																									
	}
}