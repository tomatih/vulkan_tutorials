#pragma once

#include <string>
#include <vector>

namespace lve {

class LvePipeline{
public:
	// simple constructot
	LvePipeline(const std::string& vertFilePath, const std::string& fragFilePath);

private:
	// helper to read the compiled shader files
	static std::vector<char> readFile(const std::string& filePath);

	// simple pipeline createion
	void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
};

}