#include <iostream>
#include <vector>
#include "Logging.h"

namespace nn
{
	namespace logging
	{
		const char* ErrorDescriptions[]
		{
			"",
			"The size of input data doesn't match the layer's size",
			"The layer's index is too big or negative",
			"The buffer is nullptr",
			"The matrix has at least one of it's dimensions 0 or the vector is of length 0"
		};

		std::vector<NNerror> errors;

		void Error(NNerror type)
		{
			errors.emplace_back(type);
		}

		std::vector<NNerror>& GetErrors()
		{
			return errors;
		}
		void logErrors(const char* filename, int line)
		{
			int i = strlen(filename);
			for (int d = 0; d < 3;)
				if (filename[--i] == '\\') d++;
			for (nn::logging::NNerror e : nn::logging::GetErrors())
			{
				std::cout << "[Error] : \"" << nn::logging::GetErrorMessage(e) << "\" in " << filename + i + 1 << " line " << line << std::endl;
			}
		}
	}
}