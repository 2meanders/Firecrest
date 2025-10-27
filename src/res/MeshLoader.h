#pragma once
#include <string>

#include "res/types.h"

namespace fc {
	namespace res {
		class ResourceManager;

		ModelHandle loadModel(ResourceManager& res, const std::string& modelPath, float vertexSearchCoverage = 0.1);
	}
}