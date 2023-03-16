#include "CSVReader.h"
#include "rapidcsv.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

// New function to parse the CSV file with rapidcsv library, which doesn't need to use the tokenise function and it has better time complexity
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    // Use RapidCSV to open the CSV file
    rapidcsv::Document doc;
    try {
        doc.Load(csvFilename);
    } catch (const std::exception& e) {
        std::cout << "Error opening file: " << e.what() << std::endl;
        throw;
    }

    // Iterate over the rows in the CSV file
    for (std::size_t i = 0; i < doc.GetRowCount(); ++i)
    {
        // Get the values from the current row
        std::string timestamp = doc.GetCell<std::string>(0, i);
        std::string product = doc.GetCell<std::string>(1, i);
        std::string type = doc.GetCell<std::string>(2, i);
        double price = doc.GetCell<double>(3, i);
        double amount = doc.GetCell<double>(4, i);

        // Create an OrderBookEntry object with the extracted values
        OrderBookEntry obe{price,
                           amount,
                           timestamp,
                           product,
                           OrderBookEntry::stringToOrderBookType(type)};
        // Add the OrderBookEntry object to the entries vector
        entries.push_back(obe);
    }

    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
   std::vector<std::string> tokens;
   signed int start, end;
   std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
    start = end + 1;
    }while(end > 0);

   return tokens; 
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        //std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]<< std::endl;
        throw;        
    }

    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0],
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe; 
}

OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                   std::string amountString,
                                   std:: string timestamp,
                                   std::string product,
                                   OrderBookType orderType){
    double price, amount;

    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       timestamp,
                       product,
                       orderType};
    return obe;

}
      
