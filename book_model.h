#pragma once
#include <string>

class BookModel {
public:
    int id;
    std::string title;
    std::string author;
    std::string language;
    std::string publisher;
    std::string isbn;
    int year;
    std::string description;
};
