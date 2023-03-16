#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

using namespace std;

class OrderBook{
public:
    /** construct, reading a csv data file */
    OrderBook(string filename);
    /** return vector if all known products in the dataset */
    vector<string> getKnownProducts();
    /** return vector of Orders according to the sent filters */
    vector<OrderBookEntry> getOrders(OrderBookType type, string product, string timestamp);
    /** return vector of Orders according to the sent filters without timestamp*/
    vector<OrderBookEntry> getProductAndTypeVector(OrderBookType type, string product);
    /** returns the earlest time in the orderbook*/
    string getEarliestTime();
    /** returns the next time after the sent time in the orderbook*
     * If there is no next timestamp, wraps around to te start*/
    string getNextTime(string timestamp);
    /** returns the previous time after the sent time in the orderbook*
 * If there is no previous timestamp, wraps around to the end*/
    string getPreviousTime(string timestamp);
    /** returns the previous time after the sent time in the orderbook*
 * If there is no previous timestamp, wraps around to the end*/
    double getLastPrice(string timestamp);

    void insertOrder(OrderBookEntry& order);

    vector<OrderBookEntry> matchAsksToBids(string product, string timestamp);

    static double getHighPrice(vector<OrderBookEntry>& orders);
    static double getLowPrice(vector<OrderBookEntry>& orders);
private:
    vector<OrderBookEntry> orders;

};