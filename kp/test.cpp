#include <gtest/gtest.h>
#include "zmq.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

struct Player {
    int player_pid;
    std::string player_name;
};

struct Game {
    std::string game_id;
    std::vector<Player> players;
    int players_quantity;
    std::string number;
    int current_players;
    int cond; 
    Game() : players_quantity(0), current_players(0), cond(0) {}
};

extern int checknumber(std::string& inp, std::string& gamenumber);
extern std::string getGameName(std::string& str);
extern std::string getWinner(std::vector<Game>& g, int& win_pid);
extern std::string getCurrentstring(std::string& name, std::vector<Game>& g);
extern std::string getCurrentnumber(std::string& name, std::vector<Game>& g);
extern bool findgame(std::vector<Game>& g, Player player, std::string name);
extern bool checkgame(std::vector<Game>& g, std::string name);
extern void deletegame(std::string& gamename, std::vector<Game>& g);
extern int check_game_condition(std::vector<Game>& g, std::string& name);

TEST(GameLogicTest, CheckNumberTest) {
    std::string gamenumber = "1234";

    std::string correct_guess = "1234";
    EXPECT_EQ(checknumber(correct_guess, gamenumber), 1);

    std::string incorrect_guess = "5678";
    EXPECT_EQ(checknumber(incorrect_guess, gamenumber), -1);

    std::string partial_guess = "1243";
    EXPECT_EQ(checknumber(partial_guess, gamenumber), -1);

    std::string all_cows = "4321";
    EXPECT_EQ(checknumber(all_cows, gamenumber), -1);
}

TEST(GameLogicTest, GetWinnerTest) {
    std::vector<Game> games;
    Game game;
    game.game_id = "game1";
    
    Player player1 = {123, "Alice"};
    Player player2 = {456, "Bob"};
    game.players.push_back(player1);
    game.players.push_back(player2);
    
    games.push_back(game);
    
    int win_pid = 123;
    EXPECT_EQ(getWinner(games, win_pid), "Alice");
    
    win_pid = 456;
    EXPECT_EQ(getWinner(games, win_pid), "Bob");
    
    win_pid = 999;
    EXPECT_EQ(getWinner(games, win_pid), "");
}

TEST(GameLogicTest, GetCurrentstringTest) {
    std::vector<Game> games;
    Game game;
    game.game_id = "game1";
    
    Player player1 = {123, "Alice"};
    Player player2 = {456, "Bob"};
    game.players.push_back(player1);
    game.players.push_back(player2);
    
    games.push_back(game);
    
    std::string expected = "Alice Bob ";
    std::string game1 = "game1";
    EXPECT_EQ(getCurrentstring(game1, games), expected);
    
    std::string nonexistent = "nonexistent";
    EXPECT_EQ(getCurrentstring(nonexistent, games), "");
}

TEST(GameLogicTest, GetCurrentnumberTest) {
    std::vector<Game> games;
    Game game;
    game.game_id = "game1";
    game.players_quantity = 4;
    game.current_players = 2;
    games.push_back(game);
    
    std::string expected = "2/4";
    std::string game1 = "game1";
    EXPECT_EQ(getCurrentnumber(game1, games), expected);
    
    std::string nonexistent = "nonexistent";
    EXPECT_EQ(getCurrentnumber(nonexistent, games), "");
}

TEST(GameLogicTest, FindGameTest) {
    std::vector<Game> games;
    Game game;
    game.game_id = "game1";
    game.players_quantity = 2;
    game.current_players = 1;
    
    Player player1 = {123, "Alice"};
    game.players.push_back(player1);
    
    games.push_back(game);
    
    Player player2 = {456, "Bob"};
    EXPECT_TRUE(findgame(games, player2, "game1"));
    EXPECT_EQ(games[0].current_players, 2);
    
    Player player3 = {789, "Charlie"};
    EXPECT_FALSE(findgame(games, player3, "game1"));
}

TEST(GameLogicTest, CheckGameTest) {
    std::vector<Game> games;
    
    Game game;
    game.game_id = "game1";
    game.players_quantity = 2;
    game.current_players = 2;
    games.push_back(game);
    
    EXPECT_TRUE(checkgame(games, "game1"));
    
    Game game2;
    game2.game_id = "game2";
    game2.players_quantity = 2;
    game2.current_players = 1;
    games.push_back(game2);
    
    EXPECT_FALSE(checkgame(games, "game2"));
}


TEST(GameLogicTest, DeleteGameTest) {
    std::vector<Game> games;
    
    Game game;
    game.game_id = "game1";
    games.push_back(game);
    
    std::string game1 = "game1";
    deletegame(game1, games);
    EXPECT_TRUE(games.empty());
    
    std::string nonexistent = "nonexistent";
    deletegame(nonexistent, games);
    EXPECT_TRUE(games.empty());
}

TEST(GameLogicTest, CheckGameConditionTest) {
    std::vector<Game> games;
    Game game;
    game.game_id = "game1";
    game.cond = 0;
    games.push_back(game);
    
    std::string game1 = "game1";
    EXPECT_EQ(check_game_condition(games, game1), 0);
    EXPECT_EQ(games[0].cond, 1);
    
    EXPECT_EQ(check_game_condition(games, game1), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
