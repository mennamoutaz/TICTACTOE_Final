#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include "sqlite3.h"
#include <cstdint>
#include <chrono>
#include <ctime>
#include <thread>
#include <map>
#include <cstdint>
#include "authentication.h"
#include "c_time.h"
#include "stats.h"
#include "game_prep.h"

using namespace std;

std::string player1Email, player2Email;
//int GameMode;

class Game {
private:
    Player* player1;
    Player* player2;
    sqlite3* db; // Add the SQLite database connection 
    std::string player1Email; // Email of Player 1
    std::string player2Email; // Email of Player 2
    vector<vector<GameBoard>> allGames; // Store all games played
    int gameCounter; // Counter for the number of games played
    int gameMode; // 1 for Player vs. Player, 2 for Player vs. AI
    int move_num = 0;
    string players_move_email;
public:
    Game(Player* p1, Player* p2, sqlite3* dbConn, const std::string& email1, const std::string& email2, int mode)
        : player1(p1), player2(p2), db(dbConn), player1Email(email1), player2Email(email2), gameMode(mode) {}

    void play() {
        cout << "Welcome to Tic Tac Toe!" << endl;

        //insert game data into database
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::string currentDateStr = timeToString(now);

        std::string insertSQL = "INSERT INTO games (player1_email, player2_email, date) "
            "VALUES ('" + player1Email + "', '" + player2Email + "', '" + currentDateStr + "')";
        char* errMsg = nullptr;
        int status = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errMsg);

        if (status != SQLITE_OK) {
            std::cerr << "Error inserting data: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return;
        }
        int lastGameId = sqlite3_last_insert_rowid(db);

        // Initialize game settings based on the mode
        if (gameMode == 2) {
            cout << "You will be playing against an unbeatable AI." << endl;
            cout << "You will be X and the AI will be O. Let's begin!" << endl;
            
        }
        else if (gameMode == 1) {
            cout << "You will be playing against each other." << endl;
            cout << "Player 1 will be X and Player 2 will be O." << endl;
        }

        bool playAgain = true; // To determine whether to keep playing
        while (playAgain) { // Outer game loop
            
            GameBoard board; // Create a new board for each game
            vector<GameBoard> moves; // Store moves for the current game
            bool player1Turn = true;

            while (true) { // Inner game loop
                move_num++;
                board.display(); // Show current board state

                if (player1Turn) {
                    player1->makeMove(board); // Player 1's move
                    players_move_email = player1Email;
                }
                else {
                    player2->makeMove(board); // Player 2's (or AI's) move
                    players_move_email = player2Email;
                }

                moves.push_back(board); // Store the current board state
                

                int winner = board.checkWin(); // Check for win/draw
                if (winner != 0) {
                    board.display(); // Show the final board state
                    handleGameOutcome(db, player1Email, player2Email, winner, gameMode); // Update stats based on outcome and game mode

                    // Handle the winner announcement and stat updates
                    if (winner == 1) { // Player 1 wins
                        cout << "Player 1 wins!" << endl;
                        player1->win_count++;
                        player1->total_games++;
                        player2->lose_count++;
                        player2->total_games++;
                    }

                    else if (winner == -1) { // Player 2 wins
                        cout << "Player 2 wins!" << endl;
                        player2->win_count++;
                        player2->total_games++;
                        player1->lose_count++;
                        player1->total_games++;
                    }

                    else if (winner == 2) { // Draw/tie
                        cout << "It's a tie!" << endl;
                        player1->total_games++;
                        player2->total_games++;
                    }

                    std::string updateSQL = "UPDATE games SET game_status = '" + std::to_string(winner) + "' WHERE id = '" + std::to_string(lastGameId) + "';";
                    int updateStatus = sqlite3_exec(db, updateSQL.c_str(), nullptr, nullptr, &errMsg);

                    if (updateStatus != SQLITE_OK) {
                        std::cerr << "Error updating last login date: " << errMsg << std::endl;
                        sqlite3_free(errMsg);
                    }
                    break; // Exit the inner loop once the game is over
                }

                player1Turn = !player1Turn; // Toggle turns
            }

            allGames.push_back(moves); // Store the moves for this game

            
            // Ask if the players want to play again
            cout << "Do you want to play again? (1: Yes, 0: No): ";
            int playAgainChoice;
            cin >> playAgainChoice;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

            playAgain = (playAgainChoice == 1); // Continue if 'Yes', otherwise stop
        };
        // Game ended, optionally show statistics or other information    };

        cout << "1)Player1 stats" << endl << "2)Player 2 stats" << endl << "3) Show Moves" << endl;
        int choose_1;
        cin >> choose_1;

        int pvp_win_count;
        int pvp_lose_count;
        int pvp_total_games;
        int pve_win_count;
        int pve_lose_count;
        int pve_total_games;
        int total_games;
        int retrieved;

        if (choose_1 == 1) {
            //code1
            retrieved = 1 + getPlayerStats(db, player1Email, pvp_win_count, pvp_lose_count, pvp_total_games, pve_win_count, pve_lose_count, pve_total_games, total_games);
            cout << "Games won: " << pvp_win_count + pve_win_count << endl;
            cout << "Games lost: " << pvp_lose_count + pve_lose_count << endl;
            cout << "Games Played: " << total_games << endl;
        }
        else if (choose_1 == 2) {
            //code 2
            retrieved = 2 + getPlayerStats(db, player2Email, pvp_win_count, pvp_lose_count, pvp_total_games, pve_win_count, pve_lose_count, pve_total_games, total_games);
            cout << "Games won: " << pvp_win_count + pve_win_count << endl;
            cout << "Games lost: " << pvp_lose_count + pve_lose_count << endl;
            cout << "Games Played: " << total_games << endl;
        }
        else if (choose_1 == 3) {
            while (true) {
                int choose_2;
                int chosenGame;
                cout << "1) show final board" << endl << "2) show move by move" << endl;
                cin >> choose_2;
                cout << "Enter the number of the game you want to display: ";
                cin >> chosenGame;
                if (choose_2 == 1) {
                    //final board
                    cout << "Press any key to show the Final Form of game " << chosenGame << ":" << endl;
                    cin.get();
                    allGames[chosenGame - 1].back().display();
                }
                else if (choose_2 == 2) {
                    //move by move
                    if (chosenGame >= 1 && chosenGame <= allGames.size()) {
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
                        cout << "Press any key to start showing moves of Game " << chosenGame << ":" << endl;
                        cin.get();
                        for (size_t j = 0; j < allGames[chosenGame - 1].size(); ++j) {
                            if (j > 0) {
                                cout << "Press any key to show the next move:" << endl;
                                cin.get(); // Wait for user to press a key before displaying the next move
                            }
                            allGames[chosenGame - 1][j].display();
                            cout << endl;
                        }
                        // Prompt the user to display the final move separately
                        cout << "Press any key to show the Final Form of game " << chosenGame << ":" << endl;
                        cin.get();
                        allGames[chosenGame - 1].back().display(); // Display the final move
                    }
                    else {
                        cout << "Invalid game number." << endl;
                        break;
                    }
                }
                else {
                    cout << "invalid option" << endl;
                    break;
                }
                cout << "Do you want to display moves of another game? (1: Yes, 0: No): ";
                int displayAnotherGameChoice;
                cin >> displayAnotherGameChoice;
                if (displayAnotherGameChoice != 1) {
                    break;
                }

            }
        }

    }


};



int main() {
    sqlite3* db; // SQLite database connection
    const char* dbFilename = "tictactoe3.db"; // Database file name

    // Open the database connection
    if (sqlite3_open(dbFilename, &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return 1; // Exit on error
    }

    int gameMode;
    cout << "Choose game mode: 1. Player vs. Player, 2. Player vs. AI" << endl;
    cin >> gameMode;

    if (gameMode == 1) { // Player vs. Player

        // Ask if players have accounts
        int hasAccount1;
        cout << "Player 1, do you have an account? (1: Yes, 0: No): ";
        cin >> hasAccount1;
        if (hasAccount1 == 1) {
            player1Email = login(db); // Log in Player 1
        }
        else {
            player1Email = signup(db); // Sign up Player 1
        }

        int hasAccount2;
        cout << "Player 2, do you have an account? (1: Yes, 0: No): ";
        cin >> hasAccount2;
        if (hasAccount2 == 1) {
            player2Email = login(db); // Log in Player 2
        }
        else {
            player2Email = signup(db); // Sign up Player 2
        }

        // Set up the game for Player vs. Player
        Human1Player human1;
        Human2Player human2;
        Game game(&human1, &human2, db, player1Email, player2Email, gameMode);
        game.play(); // Start the game
    }
    else if (gameMode == 2) { // Player vs. AI
        player2Email = 123;
        int hasAccount;
        cout << "Do you have an account? (1: Yes, 0: No): ";
        cin >> hasAccount;
        if (hasAccount == 1) {
            player1Email = login(db); // Log in
        }
        else {
            player1Email = signup(db); // Sign up
        }

        Human1Player human;
        AIPlayer ai; // Set up AI
        Game game(&human, &ai, db, player1Email, "AI", gameMode);
        game.play(); // Start the game
    }
    else {
        cout << "Invalid choice." << endl;
    }

    // Close the database connection
    sqlite3_close(db);
    return 0;
}