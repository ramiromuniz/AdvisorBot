cmake_minimum_required(VERSION 3.23)
project(merklerex_start_topic_4)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(merklerex_start_topic_4
        src/CSVReader.cpp
        src/CSVReader.h
        src/main.cpp
        src/MerkelMain.cpp
        src/MerkelMain.h
        src/OrderBookEntry.cpp
        src/OrderBookEntry.h
        src/OrderBook.cpp
        src/OrderBook.h
        src/Wallet.cpp
        src/Wallet.h
        src/AdvisorBot.cpp
        src/AdvisorBot.h
        src/global.h)
