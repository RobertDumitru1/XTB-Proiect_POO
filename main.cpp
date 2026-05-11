#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include "Market.h"
#include "User.h"
#include "Instrument.h"
#include "Trading.h"
#include "Exceptions.h"

int main() {
    std::vector<std::shared_ptr<Instrument>> instruments;
    instruments.push_back(std::make_shared<PhysicalAsset>("Apple Inc.", "AAPL", 150.0, 1.5));
    instruments.push_back(std::make_shared<Derivative>("S&P 500", "SPX", 5100.0, 20, 1.5));
    instruments.push_back(std::make_shared<CryptoAsset>("Bitcoin", "BTC", 68000.0, 25.0, true));

    auto market = std::make_shared<Market>(instruments);

    market->startMarket();

    Portfolio initial_portfolio;
    std::vector<Transaction> initial_history;
    User user("Alexandru", "1990101123456", "parola123", USD, 20000.0, 0.0, initial_portfolio, initial_history, market);

    std::cout << *market << "\n";
    std::cout << user << "\n";

    try {
        user.buyAsset("AAPL", 10);
        user.buyAsset("SPX", 5);
        user.buyAsset("BTC", 0.05);
        user.buyAsset("FAKE", 100);
    } catch (const TradingException& e) {
        std::cout << "\nExceptie prinsa: " << e.what() << "\n\n";
    }

    std::cout << user << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << *market << "\n";

    user.sellPosition(1);

    std::cout << "\n" << user << "\n";

    std::cout << "=== ISTORIC TRANZACTII ===\n";
    user.printHistory();

    market->stopMarket();

    return 0;
}