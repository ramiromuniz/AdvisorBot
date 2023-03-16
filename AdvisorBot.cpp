#include "AdvisorBot.h"
#include "MerkelMain.h"
#include <algorithm>
#include "global.h"
#include "OrderBook.h"
#include "CSVReader.h"
#include <tuple>
#include <sstream>
#include <iterator>

//constructor
AdvisorBot::AdvisorBot(){

}
//initialize advisorbot
void AdvisorBot::init()
{
    string input;
    exitLoop = true;
    //enter advisorbot menu when exits merkelmain menu and viceversa
    while(exitLoop)
    {
        input = getUserInput();
        processUserInput(input);
    }
}

/********   get only valid user inputs in a robust way   *******/

string AdvisorBot::getUserInput() {

    int timesteps;

    std::string userInput;
    std::cout << "Please enter a command, or 'help' for a list of commands" << std::endl;
    std::getline(std::cin, userInput);

    //count number of words in command input
    std::vector<std::string> words = CSVReader::tokenise(userInput, ' ');
    int numWords = words.size();

    // array of available commands
    std::vector<string> cmd{"help", "prod", "min", "max", "avg", "predict", "time", "step", "back"};
    std::vector<string> cmdWithoutArguments{"help", "prod", "time", "step", "back"};


// Check if the input has the correct number of arguments
    if (numWords == 1) {
        //if its one word check if its inside the valid single word command vector
        if (std::find(cmdWithoutArguments.begin(), cmdWithoutArguments.end(), words[0]) != cmdWithoutArguments.end()) {
            return userInput;
        }
        else {
            //word that is not a command
            std::cout << "Invalid command. Expecting a valid command." << std::endl;
            return "";
        }

    // Check if the user input starts with 'help' and check second word for valid command
    }
    else if (numWords == 2 && words[0] == "help") {
        //check if the argument is inside the commands vector
        if (std::find(cmd.begin(), cmd.end(), words[1]) != cmd.end()) {
            return userInput;
        }
        else {
            std::cout << "Invalid command after `help`. Expecting a single valid command after `help`."
                      << std::endl;
            return "";
        }

    }

    //if user inputs 3 words and the first is 'min'
    else if (numWords == 3 && words[0] == "min") {
        bool isValidProduct = false;

        // Check if the second and third words are a valid product and OrderBookType
        for (const auto& product : orderBook.getKnownProducts()) {
            if (words[1] == product) {
                isValidProduct = true;
                break;
            }
        }
        if (!isValidProduct) {
            std::cout << "Invalid Product argument." << std::endl;
            return "";
        }
        else{
            // check if third word is a valid type
            if(words[2] == "bid" || words[2] == "ask"){
                return userInput;
            }
            else{
                std::cout << "Invalid OrderBookType (ask/bid)." << std::endl;
                return "";
            }
        }
        return "";
    }


    // check if command starts with max
    else if (numWords == 3 && words[0] == "max"){
        bool isValidProduct = false;

        // Check if the second and third words are a valid product and OrderBookType
        for (const auto& product : orderBook.getKnownProducts()) {
            if (words[1] == product) {
                isValidProduct = true;
                break;
            }
        }
        if (!isValidProduct) {
            std::cout << "Invalid Product argument." << std::endl;
            return "";
        }
        else{
            // check if third word is a valid type
            if(words[2] == "bid" || words[2] == "ask"){
            return userInput;
            }
            else{
                std::cout << "Invalid OrderBookType (ask/bid)." << std::endl;
                return "";
            }
        }
        return "";
    }

    //if 4 commands were entered check if the first word is avg and if the rest are valid inputs
    else if (numWords == 4 && words[0] == "avg"){

        bool isValidProduct = false;


        // Check if the second and third words are a valid product and OrderBookType
        for (const auto& product : orderBook.getKnownProducts()) {
            if (words[1] == product) {
                isValidProduct = true;
                break;
            }
        }
        if (!isValidProduct) {
            std::cout << "Invalid Product argument." << std::endl;
            return "";
        }
        else if(words[2] != "bid" && words[2] != "ask"){
            std::cout << "Invalid OrderBookType (ask/bid)." << std::endl;
            return "";
        }
        //check if timestep argument can be converted to int
        else {
            // Convert the timesteps string to an integer
            try {
                timesteps = std::stoi(words[3]);
                return userInput;
            }
            catch (std::invalid_argument& e) {
                std::cout << "Invalid timestep argument. Expecting a positive integer." << std::endl;
                return "";
            }
            catch (std::out_of_range& e) {
                std::cout << "Timestep argument out of range. Expecting an integer." << std::endl;
                return "";
            }
        }
        return "";
    }
    else if (numWords == 4 && words[0] == "predict") {

        bool isValidProduct = false;

        // Check if the second and third words are a valid product and OrderBookType
        for (const auto& product : orderBook.getKnownProducts()) {
            if (words[2] == product) {
                isValidProduct = true;
                break;
            }
        }
        if (!isValidProduct) {
            std::cout << "Invalid Product argument." << std::endl;
            return "";
        }
        else {
            // Check if the first word is a valid type
            if (words[1] == "max" || words[1] == "min") {
                // Check if the third word is a valid type
                if (words[3] == "bid" || words[3] == "ask") {
                    return userInput;
                }
                else {
                    std::cout << "Invalid OrderBookType (ask/bid)." << std::endl;
                    return "";
                }
            }
            else {
                std::cout << "Invalid type (max/min)." << std::endl;
                return "";
            }
        }
    }

    else{
        std::cout << "Invalid command." << std::endl;
        return "";
    }
    std::cout << "Invalid command." << std::endl;
    return "";
}

//process user input
void AdvisorBot::processUserInput(std::string input) {
    if(input.empty()) return;

    // Check if input is a single-word command
    if (input == "help") {
        AdvisorBot::help();
    } else if (input == "prod") {
        AdvisorBot::prod();
    } else if (input == "time") {
        AdvisorBot::time();
    } else if (input == "step") {
        AdvisorBot::step();
    } else if (input == "back") {
        AdvisorBot::back();
    } else {
        // Input must be a multi-word command, so tokenise it into separate words
        std::vector<std::string> words = CSVReader::tokenise(input, ' ');
        int numWords = words.size();
        if(numWords == 2 && words[0] == "help"){
            AdvisorBot::help(words[1]);
        }
        else if (numWords == 3 && words[0] == "min") {
            AdvisorBot::min(words[1], OrderBookEntry::stringToOrderBookType(words[2]));
        } else if (numWords == 3 && words[0] == "max") {
            AdvisorBot::max(words[1], OrderBookEntry::stringToOrderBookType(words[2]));
        } else if (numWords == 4 && words[0] == "avg") {
            AdvisorBot::avg(words[1], OrderBookEntry::stringToOrderBookType(words[2]), std::stoi(words[3]));
        } else if (numWords == 4 && words[0] == "predict") {
            AdvisorBot::predict(words[1], words[2], OrderBookEntry::stringToOrderBookType(words[3]));
        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}

//help command function
void AdvisorBot::help() {
    std::cout << "The available commands are (expected parameters in <>):\n"
              << "1. 'help'\n"
              << "2. 'help <cmd>'\n"
              << "3. 'prod'\n"
              << "4. 'min <prod> <ask/bid>'\n"
              << "5. 'max <prod> <ask/bid>'\n"
              << "6. 'avg <prod> <ask/bid> <time_steps>\n"
              << "7. 'predict <max/min> <prod> <ask/bid>'\n"
              << "8. 'time'\n"
              << "9. 'step'\n"
              //My own command
              << "10. 'back'\n";

}

//help <cmd> command function
void AdvisorBot::help(std::string cmd) {
    if (cmd == "prod") {
        std::cout << "List all known cryptocurrency products \n";
    }
    if (cmd == "help") {
        std::cout << "List of all available commands\n";
    }
    if (cmd == "time") {
        std::cout << "Show the current time step\n";
    }
    if (cmd == "step") {
        std::cout << "Advance to the next timeframe\n";
    }
    if (cmd == "min") {
        std::cout << "Find minimum bid or ask for product in current time step\n";
    }
    if (cmd == "max") {
        std::cout << "Find maximum bid or ask for product in current time step\n";
    }
    if (cmd == "avg") {
        std::cout << "Compute average ask or bid for the sent product over the sent number\n"
                     "of time steps.\n";
    }
    if (cmd == "predict") {
        std::cout << "Predict max or min ask or bid for the sent product for the next time\n"
                     "step\n";
    }
    if (cmd == "back") {
        std::cout << "Go back to the main menu\n";
    }
}
//Time function
void AdvisorBot::time() {
    std::cout << "The current time is: " << currentTime << std::endl;
}

//Step function
void AdvisorBot::step() {
    //assign the next timestamp value to currentTime
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at " << currentTime << std::endl;
}

//Min function
void AdvisorBot::min(std::string product, OrderBookType type){

    // Get all orders of the specified product and type in the current time step
    std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, currentTime);
    // If there are no orders, print an error message
    if (entries.empty()) {
        std::cout << "No " << OrderBookEntry::orderBookTypeToString(type) << " orders found for product " << product << " at time step " << currentTime << std::endl;
        return;
    }

    // Get the lowest price from the entries vector
    double minPrice = OrderBook::getLowPrice(entries);
    std::cout << "The min " << OrderBookEntry::orderBookTypeToString(type) << " for " << product << " is " << minPrice << std::endl;
}

//Max function
void AdvisorBot::max(std::string product, OrderBookType type){

    // Get all orders of the specified product and type in the current time step
    std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, currentTime);
    // If there are no orders, print an error message
    if (entries.empty()) {
        std::cout << "No " << OrderBookEntry::orderBookTypeToString(type) << " orders found for product " << product << " at time step " << currentTime << std::endl;
        return;
    }

    // Get the highest price from the entries vector
    double minPrice = OrderBook::getHighPrice(entries);
    std::cout << "The max " << OrderBookEntry::orderBookTypeToString(type) << " for " << product << " is " << minPrice << std::endl;
}

//Prod function
void AdvisorBot::prod() {
//print all products available in orderBook
    for (const auto& product : orderBook.getKnownProducts()) {
        std::cout << product << ", ";
    }
    std::cout << std::endl;
}

//Avg function
void AdvisorBot::avg(std::string product, OrderBookType type, int timesteps) {
// declare new variable for currentTime to be used in the for loop without changing the currentTime in the exchange app
    string tempTime = currentTime;
    double total = 0;
    //this loop will set tempTime to the previous timestamp and get a vector with all entries of the product and type in the previous timestamp
    for(int i = 0; i < timesteps; i++){
        double sum = 0;
        tempTime = orderBook.getPreviousTime(tempTime);
        std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, tempTime);
        // this loop will add all the prices of the entries to the sum variable
        for (int j = 0; j < entries.size(); j++){
            sum += entries[j].price;
        }
        //calculate average of all entry prices in the timestamp
        double avg = sum / entries.size();
        //store it in the total variable
        total += avg;
    }
    //calculate the average of all the averages (different timestamps specified by timesteps)
    double final_avg = total / timesteps;
    std::cout << "The average " << product << " " << OrderBookEntry::orderBookTypeToString(type) << " price over the last " << timesteps << " timesteps was " << final_avg << "." << std::endl;
}

//Predict function
void AdvisorBot::predict(std::string min_max, std::string product, OrderBookType type) {
    // retrieve the required data for the prediction
    std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, currentTime);
    int n = entries.size();

    // check if we have enough data to make a prediction
    if (n < 2) {
        std::cout << "Not enough entries to make a prediction." << std::endl;
    }

// perform linear regression on the data
    double sum_x = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_xx = 0;
    for (int i = 0; i < n; i++) {
        double x = i; //indices of the entries vector
        double y = entries[i].price; //prices of the entries
        sum_x += x; //sum of all x values
        sum_y += y; //sum of all y values
        sum_xy += x * y; //sum of all x*y values
        sum_xx += x * x; //sum of all x^2 values
    }
    //calculate the slope
    double m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    //calculate the y-intercept
    double b = (sum_y - m * sum_x) / n;

// use the linear regression equation to predict the next value
    double prediction = m * n + b;

// print the prediction
    std::cout << "The " << min_max << " " << OrderBookEntry::orderBookTypeToString(type) << " " << product << " price prediction for the next timestamp is " << prediction << "." << std::endl;

}

//Back command function
void AdvisorBot::back(){
    //go back to main menu (merkelmain)
    exitLoop = false;
}
