#pragma once
#include <vector>
#include <sqlite3.h>
#include "book_model.h"

class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

    std::vector<BookModel> GetBooks();
    void AddBook(const BookModel& book);
    void UpdateBook(const BookModel& book);
    void DeleteBook(int bookId);

private:
    sqlite3* db;
};

