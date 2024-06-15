#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

//int main()
//{
//    // Open SQLite database connection
//    sqlite3* db;
//    int status = sqlite3_open("tictactoe3.db", &db);
//    if (status != SQLITE_OK) {
//        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//        return 1;
//    }
//    //Create the players table with fields for Player vs.Player and Player vs.AI statistics
//    //create players table
//    /*const char* createTableSQL = "CREATE TABLE players (id INTEGER PRIMARY KEY, email TEXT UNIQUE, password TEXT, name TEXT, city TEXT, age INTEGER, pvp_win_count INTEGER DEFAULT 0, pvp_lose_count INTEGER DEFAULT 0,pvp_total_games INTEGER DEFAULT 0, pve_win_count INTEGER DEFAULT 0, pve_lose_count INTEGER DEFAULT 0, pve_total_games INTEGER DEFAULT 0, total_games INTEGER DEFAULT 0, current_date TEXT, last_login_date TEXT)";
//    char* errorMessage = nullptr;
//    status = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
//    if (status != SQLITE_OK) {
//        std::cerr << "Error creating table: " << errorMessage << std::endl;
//        sqlite3_free(errorMessage);
//        sqlite3_close(db);
//        return 1;
//    }
//    cout << "Table created successfully\n";*/
//    /*const char* createTableSQL = "CREATE TABLE games (id INTEGER PRIMARY KEY, player1_email TEXT , player2_email TEXT, game_status INTEGER, date TEXT)";
//    char* errorMessage = nullptr;
//    status = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
//    if (status != SQLITE_OK) {
//        std::cerr << "Error creating table: " << errorMessage << std::endl;
//        sqlite3_free(errorMessage);
//        sqlite3_close(db);
//        return 1;
//    }
//    cout << "Table created successfully\n";*/
//    /*const char* createTableSQL = "CREATE TABLE game_moves (move_id INTEGER PRIMARY KEY, game_id INTEGER, move_number INTEGER, player_email TEXT, row INTEGER, column INTEGER, move_value INTEGER, FOREIGN KEY(game_id) REFERENCES games(game_id))";
//    char* errorMessage = nullptr;
//    status = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
//    if (status != SQLITE_OK) {
//        std::cerr << "Error creating table: " << errorMessage << std::endl;
//        sqlite3_free(errorMessage);
//        sqlite3_close(db);
//        return 1;
//    }
//    cout << "Table created successfully\n";*/
//    return 0;
//}
