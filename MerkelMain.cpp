#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "AdvisorBot.h"
#include "global.h"
//global variable to use as a switch between merkelmain menu and advisorbot menu
bool exitLoop;
//initiate OrderBook object
OrderBook orderBook{(R"(C:\Users\rami0\CLionProjects\mid term\src\20200601.csv)")};


std::string currentTime;

MerkelMain::MerkelMain(){


}

void MerkelMain::init()
{
    //while exitLoop is false, continue displaying main menu
    exitLoop = false;
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);
    std::cout << exitLoop << std::endl;

    while(!exitLoop)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;
    //advisorbot
    std::cout << "7: Advisorbot " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;

}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for(string const& p : orderBook.getKnownProducts()){
        cout << "Product: " << p << endl;
        vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);

        cout << "Asks seen: " << entries.size() << endl;
        cout << "Max ask: " << OrderBook::getHighPrice(entries) << endl;
        cout << "Min ask: " << OrderBook::getLowPrice(entries) << endl;

    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3){
        std::cout << "MerkelMain::enterAsk Bad input" << std::endl;
    }
    else{
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1],
                                                         tokens[2],
                                                         currentTime,
                                                         tokens[0],
                                                         OrderBookType::ask);
            obe.username = "simuser";
            if(wallet.canFulfillOrder(obe)){
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else{
               std::cout << "Wallet has insufficient funds" << std::endl;
            }

        }catch(const std::exception& e){
            std::cout << "MerkelMain::enterAsk Bad input!" << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3){
        std::cout << "MerkelMain::enterBid Bad input" << std::endl;
    }
    else{
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1],
                                                         tokens[2],
                                                         currentTime,
                                                         tokens[0],
                                                         OrderBookType::bid);
            obe.username = "simuser";
            if(wallet.canFulfillOrder(obe)){
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else{
                std::cout << "Wallet has insufficient funds" << std::endl;
            }

        }catch(const std::exception& e){
            std::cout << "MerkelMain::enterBid Bad input!" << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for(OrderBookEntry& sale : sales){
        std::cout << "Sale amount: " << sale.price << " amount " << sale.amount << std::endl;
        if(sale.username == "simuser"){
            //update the wallet
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-7" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption) {
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) {
        printHelp();
    }
    if (userOption == 2) {
        printMarketStats();
    }
    if (userOption == 3) {
        enterAsk();
    }
    if (userOption == 4) {
        enterBid();
    }
    if (userOption == 5) {
        printWallet();
    }
    if (userOption == 6) {
        gotoNextTimeframe();
    }
    if (userOption == 7) {
        //exit main menu loop and go to advisorbot menu
        std::cout << "Entering advisorbot sub-menu" << std::endl;
        exitLoop = true;
        AdvisorBot abApp{};
        abApp.init();
    }

}