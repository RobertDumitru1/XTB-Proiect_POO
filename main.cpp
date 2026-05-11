#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <cassert>
#include "Market.h"
#include "User.h"
#include "Instrument.h"
#include "Trading.h"
#include "Exceptions.h"

int main() {
    try {
        std::vector<std::shared_ptr<Instrument>> initial_assets;
        initial_assets.push_back(std::make_shared<PhysicalAsset>("Apple Inc.", "AAPL", 150.0, 1.2));
        initial_assets.push_back(std::make_shared<Derivative>("S&P 500 CFD", "SPX", 5000.0, 20, 0.05));
        initial_assets.push_back(std::make_shared<CryptoAsset>("Bitcoin", "BTC", 60000.0, 10.0, true));

        auto market = std::make_shared<Market>(initial_assets);
        market->startMarket();

        Portfolio empty_portfolio;
        std::vector<Transaction> empty_history;
        User user("Tester GitHub", "1234567890123", "secret", USD, 50000.0, 0.0, empty_portfolio, empty_history, market);

        user.buyAsset("AAPL", 10);
        user.buyAsset("SPX", 2);
        user.buyAsset("BTC", 0.1);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << user << "\n";

        user.sellPosition(1);

        try {
            user.buyAsset("INVALID", 1);
        } catch (const InstrumentNotFoundException& e) {
            std::cout << e.what() << "\n";
        }

        try {
            user.buyAsset("BTC", 1000);
        } catch (const InsufficientFundsException& e) {
            std::cout << e.what() << "\n";
        }

        user.printHistory();

        market->stopMarket();

        return 0;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}