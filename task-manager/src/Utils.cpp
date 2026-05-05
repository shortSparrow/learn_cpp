#include "StringUtils.h"
#include <algorithm>
#include <cctype>

namespace Utils {
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), 
                       [](unsigned char c){ return std::toupper(c); });
        return result;
    }

}