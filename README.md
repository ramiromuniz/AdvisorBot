AdvisorBot for Cryptocurrency Trading
This project is an advisorBot built in C++ that helps you trade in a cryptocurrency exchange app through different command prompts available for the user. The bot uses a CSV reader with orderbook entries, and the data is parsed to make calculations with it.


*Usage*
The advisorBot provides several commands for the user to interact with the orderbook and make trading decisions. The available commands are:

help - displays a list of available commands.
products - displays a list of known products in the orderbook.
asks product timestamp - returns a vector of ask orders for the specified product and timestamp.
bids product timestamp - returns a vector of bid orders for the specified product and timestamp.
sales product timestamp - returns a vector of sales for the specified product and timestamp.
high product timestamp - returns the highest price for the specified product and timestamp.
low product timestamp - returns the lowest price for the specified product and timestamp.
earliest - returns the earliest timestamp in the orderbook.
next timestamp - returns the next timestamp after the specified timestamp in the orderbook.
previous timestamp - returns the previous timestamp before the specified timestamp in the orderbook.
lastprice timestamp - returns the last price for the specified timestamp.
To use the advisorBot, simply compile the C++ files and run the executable. The bot will prompt you to enter a command, and you can enter any of the commands listed above. Follow the prompts to input the necessary information for each command.
