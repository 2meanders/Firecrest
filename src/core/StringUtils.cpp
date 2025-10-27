#include "StringUtils.h"

namespace fc::utils {

	// trim from start (in place)
	inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
			}));
	}

	// trim from end (in place)
	inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	// trim from both ends (in place)
	void trim(std::string& str) {
		rtrim(str);
		ltrim(str);
	}

	std::vector<std::string> strsplit(std::string str, const std::string& delimiter, bool keepDelimiter) {
		std::vector<std::string> tokens;
		if (str.find(delimiter) == std::string::npos) {
			tokens.push_back(str);
			return tokens;
		}
		size_t offset = 0;
		std::string token;
		while ((offset = str.find(delimiter)) != std::string::npos) {
			if(keepDelimiter) {
				token = str.substr(0, offset + delimiter.length());
			} else {
				token = str.substr(0, offset);
			}
				tokens.push_back(token);
			str.erase(0, offset + delimiter.length());
		}
		tokens.push_back(str);
		return tokens;
	}
	
	std::string whitespaceToSpace(const std::string& str) {
		std::string result;
		bool lastCharWasWhitespace = false;

		for (unsigned char c : str) {
			if (std::isspace(c)) {
				if (!lastCharWasWhitespace) {
					result += ' ';
					lastCharWasWhitespace = true;
				}
			}
			else {
				result += c;
				lastCharWasWhitespace = false;
			}
		}

		return result;
	}
	std::string combine(const std::vector<std::string> strings, std::string separator) {
		std::string result = "";
		for (uint32_t i = 0; i < strings.size() - 1; i++) {
			result += strings[i] + separator;
		}
		result += strings[strings.size() - 1];
		return result;
	}
	std::string getPath(const std::string& path) {
		const size_t slashPos = path.find_last_of("/\\");
		std::string filePath = path.substr(0, slashPos + 1);
		return filePath;
	}

    bool isWhitespace(const std::string &str) {
		for(const char& c : str) {
			if (std::isspace(c)) return true;
		}
        return false;
    }

    /*std::string trim(const std::string& str, const std::string whitespace) {
		const size_t strBegin = str.find_first_not_of(whitespace);
		const size_t strEnd = str.find_last_not_of(whitespace);
		const size_t strRange = strEnd - strBegin + 1;
		return str.substr(strBegin, strRange);
	}*/
}