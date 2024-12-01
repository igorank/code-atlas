#include <stdexcept>
#include "database.h"

Database::Database(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open the database.");
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            language TEXT NOT NULL,
            year INTEGER,
            description TEXT
        );
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, createTableSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare CREATE TABLE statement: " + std::string(sqlite3_errmsg(db)));
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute CREATE TABLE statement: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(stmt);
}

Database::~Database() {
    sqlite3_close(db);
}

std::vector<BookModel> Database::GetBooks() {
    std::vector<BookModel> books;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT * FROM books", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            books.push_back({
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
                });
        }
    }
    sqlite3_finalize(stmt);
    return books;
}

void Database::AddBook(const BookModel& book) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO books (title, author, language, year, description) VALUES (?, ?, ?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, book.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.language.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, book.year);
    sqlite3_bind_text(stmt, 5, book.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::UpdateBook(const BookModel& book) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE books SET title = ?, author = ?, language = ?, year = ?, description = ? WHERE id = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, book.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.language.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, book.year);
    sqlite3_bind_text(stmt, 5, book.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, book.id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void Database::DeleteBook(int bookId) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM books WHERE id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare DELETE statement.");
    }

    sqlite3_bind_int(stmt, 1, bookId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to delete the book.");
    }

    sqlite3_finalize(stmt);
}