#pragma once
#include <vector>
#include <array>
#include <string>

namespace fc::utils {

	//std::string trim(const std::string& str, const std::string whitespace = " \t");
	void trim(std::string& str);
	std::vector<std::string> strsplit(std::string str, const std::string& delimiter, bool keepDelimiter=false);
	
	// Replaces all consecutive whitespaces with a single space
	std::string whitespaceToSpace(const std::string& str);

	std::string combine(const std::vector<std::string> strings, std::string separator = " ");
	
	// c:/folder1/folder2/file.txt -> c:/folder1/folder2/
	std::string getPath(const std::string& path);

	bool isWhitespace(const std::string& str);
}