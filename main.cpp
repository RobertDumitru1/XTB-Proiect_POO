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
#include "ETF.h"

int main() {
    try {
        std::vector<std::shared_ptr<Instrument>> initial_assets;
        initial_assets.push_back(std::make_shared<PhysicalAsset>("Apple Inc.", "AAPL", 150.0, 1.2));
        initial_assets.push_back(std::make_shared<Derivative>("S&P 500 CFD", "SPX", 5000.0, 20, 0.05));
        initial_assets.push_back(std::make_shared<ETF>("Vanguard S&P 500 ETF", "VOO", 450.0, 0.03));

        auto market = std::make_shared<Market>(initial_assets);

        const auto& all_instr = market->getInstruments();
        std::cout << "Instrumente in piata: " << all_instr.size() << "\n";

        market->startMarket();

        Portfolio empty_portfolio;
        std::vector<Transaction> empty_history;
        User user("Tester", "1234567890123", "pass", USD, 50000.0, 0.0, empty_portfolio, empty_history, market);

        user.buyAsset("AAPL", 10);

        std::cout << "Balanta disponibila: " << user.getAvailableBalance() << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));

        user.sellPosition(1);

        std::cout << "Total pozitii create in sistem: " << Position::getPositionsCreated() << "\n";

        try {
            user.buyAsset("INVALID", 1);
        } catch (const InstrumentNotFoundException& e) {
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