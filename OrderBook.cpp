#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
using namespace std;

/** construct, reading a csv data file */
OrderBook::OrderBook(string filename){
    orders = CSVReader::readCSV(filename);
}
/** return vector if all known products in the dataset */
vector<string> OrderBook::getKnownProducts(){
    vector<string> products;

    map<string, bool> prodMap;

    for (OrderBookEntry& e : orders){
        prodMap[e.product] = true;
    }

    for(auto const& e : prodMap){
        products.push_back(e.first);
    }
    return products;
}
/** return vector of Orders according to the sent filters */
vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, string product, string timestamp){
    vector<OrderBookEntry> orders_sub;

    for(OrderBookEntry& e : orders){
        if(e.orderType == type &&
           e.product == product &&
           e.timestamp == timestamp){
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}
/** return vector of Orders according to the sent filters without timestamp*/
vector<OrderBookEntry> OrderBook::getProductAndTypeVector(OrderBookType type, string product){
    vector<OrderBookEntry> orders_sub;

    for(OrderBookEntry& e : orders){
        if(e.orderType == type &&
           e.product == product){
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(vector<OrderBookEntry>& orders){
    double max = orders[0].price;
    for (OrderBookEntry& e : orders){
        if(e.price > max){
            max = e.price;
        }
    }
    return max;
}

double OrderBook::getLowPrice(vector<OrderBookEntry>& orders){
    double min = orders[0].price;
    for (OrderBookEntry& e : orders){
        if(e.price < min){
            min = e.price;
        }
    }
    return min;
}

string OrderBook::getEarliestTime(){
    return orders[0].timestamp;
}

string OrderBook::getNextTime(string timestamp){

    string next_timestamp = "";
    for(OrderBookEntry& e : orders){
        if(e.timestamp> timestamp){
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == ""){
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

string OrderBook::getPreviousTime(string timestamp){

    string last_timestamp = "";
    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        OrderBookEntry &e = *it;
        if (e.timestamp < timestamp) {
            last_timestamp = e.timestamp;
            break;
        }
    }
    if (last_timestamp == "") {
        last_timestamp = orders.back().timestamp;
    }
    return last_timestamp;
}


//Iterate the vector orders backwards to find the last timestamp
double OrderBook::getLastPrice(string timestamp){

    double last_price = 0;
    for(auto it = orders.rbegin(); it != orders.rend(); ++it){
        OrderBookEntry& e = *it;
        if(e.timestamp< timestamp){
            last_price = e.price;
            break;
        }
    }
    if (last_price == 0){
        last_price = orders.back().price;
    }
    return last_price;
}

void OrderBook::insertOrder(OrderBookEntry& order) {
    orders.push_back(order);
    sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

vector<OrderBookEntry> OrderBook::matchAsksToBids(string product, string timestamp){
    //asks = orderbook.asks
    vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    //bids = orderbook.bids
    vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    //sales = []
    vector<OrderBookEntry> sales;

    //sort asks lowest first
    sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

    //sort bids lowest first
    sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    //for ask in asks
    for(OrderBookEntry& ask : asks){
        //for bids in bids
        for(OrderBookEntry& bid : bids){
            if (bid.price >= ask.price){

                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if(bid.username == "simuser"){
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;;
                }
                if(ask.username == "simuser"){
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }

                if(bid.amount == ask.amount){
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                }
                if(bid.amount > ask.amount){
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }
                if(bid.amount < ask.amount && bid.amount > 0){
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;


}

