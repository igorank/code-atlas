#include "utils.h"

std::string truncateText(const std::string& text, size_t maxLength) {
    if (text.size() > maxLength) {
        return text.substr(0, maxLength) + "...";
    }
    return text;
}