#include "server/database/database.hpp"

#include <sqlite3.h>

#include <stdexcept>

Database::Database() {
    int returnCode = sqlite3_open("database.db", &database);

    if (returnCode != SQLITE_OK) {
        std::string msg = sqlite3_errmsg(database);
        sqlite3_close(database);
        throw std::runtime_error(msg);
    }
}

Database::~Database() {
    if (database)
        sqlite3_close(database);
}

void Database::errorCheck(int returnCode) {
    if (returnCode != SQLITE_OK) 
        throw std::runtime_error(sqlite3_errmsg(database));
}

void Database::createMRPTable() {
    std::string sql = "CREATE TABLE IF NOT EXISTS messages ("
                      "client_name TEXT NOT NULL, "
                      "utc_date TEXT NOT NULL, "
                      "message TEXT NOT NULL"
                      ");"; 
    
    int returnCode = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, nullptr);
    errorCheck(returnCode);
}

std::vector<MRPHeader> Database::getMRPHeaders(int numHeaders) {
    std::string sql = "SELECT client_name, utc_date, message "
                      "FROM messages "
                      "ORDER BY utc_date ASC "
                      "LIMIT ?;";
    
    sqlite3_stmt* stmt;

    errorCheck(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr));
    errorCheck(sqlite3_bind_int(stmt, 1, numHeaders));

    std::vector<MRPHeader> mrpHeaders{};
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string client = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string message = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        mrpHeaders.emplace_back(client, date, message);     
    }

    sqlite3_finalize(stmt);
    return mrpHeaders;
}

void Database::createMRPHeader(const MRPHeader& mrpHeader) {
    std::string sql = "INSERT INTO messages (client_name, utc_date, message) VALUES (?, ?, ?);";
    
    sqlite3_stmt* stmt;
    errorCheck(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr));

    errorCheck(sqlite3_bind_text(stmt, 1, mrpHeader.clientName.c_str(), -1, SQLITE_TRANSIENT));
    errorCheck(sqlite3_bind_text(stmt, 2, mrpHeader.utcDate.c_str(), -1, SQLITE_TRANSIENT));
    errorCheck(sqlite3_bind_text(stmt, 3, mrpHeader.messageData.c_str(), -1, SQLITE_TRANSIENT));

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

