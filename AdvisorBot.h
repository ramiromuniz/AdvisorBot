#pragma once

#include <iostream>
#include <vector>
#include "OrderBook.h"
#include "global.h"

class AdvisorBot {
public:
     AdvisorBot();
     void help();
     void help(std::string cmd);
     void prod();
     static void min(std::string product, OrderBookType type);
     static void max(std::string product, OrderBookType type);
     void avg(std::string product, OrderBookType type, int timesteps);
     void predict(std::string min_max, std::string product, OrderBookType type);
     void time();
     void step();
    static string getUserInput();
    void init();
    void processUserInput(string input);
    void back();

private:
    std::vector<std::string> products;  // Declare the vector as a member variable
};