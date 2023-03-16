
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

//declare OrderBook object here as extern to be used in AdvisorBot as well
extern OrderBook orderBook;

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);



//        OrderBook orderBook{(R"(C:\Users\rami0\CLionProjects\merklerex_start_topic_4\src\20200601.csv)")};
//        OrderBook orderBook{(R"(C:\Users\rami0\CLionProjects\merklerex_start_topic_3\src\20200317.csv)")};

        Wallet wallet;
}; 
