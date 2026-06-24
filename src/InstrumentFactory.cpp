#include "InstrumentFactory.h"
#include "ETF.h"

std::shared_ptr<Instrument> InstrumentFactory::createFromJson(const std::string& symbol, const nlohmann::json& data) {
    std::string type = data.value("type", "STOCK");
    std::string name = data.value("name", "Unknown");
    double price = data.value("price", 100.0);

    if (type == "ETF") {
        double fee = data.value("management_fee", 0.01);
        return std::make_shared<ETF>(name, symbol, price, fee);
    } else if (type == "DERIVATIVE") {
        int leverage = data.value("leverage", 10);
        double swap_fee = data.value("swap_fee", 0.05);
        return std::make_shared<Derivative>(name, symbol, price, leverage, swap_fee);
    } else if (type == "CRYPTO") {
        double network_fee = data.value("network_fee", 1.5);
        bool is_staked = data.value("is_staked", false);
        return std::make_shared<CryptoAsset>(name, symbol, price, network_fee, is_staked);
    } else {
        double dividend = data.value("dividend_yield", 0.02);
        return std::make_shared<PhysicalAsset>(name, symbol, price, dividend);
    }
}