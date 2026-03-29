//
// Created by dumro on 3/20/2026.
//
#include "Market.h"
#include <random>

void Market::changePrices() const {
    while (this->is_running) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-5.0, 5.0);

        for (auto *w : available_instruments) {
            w->current_price += w->current_price * dis(gen) / 100;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

Market::Market(const std::vector<Instrument*>& insts) : is_running(false) {
    for (const auto* i : insts) if (i) available_instruments.push_back(i->clone());
}

Market::Market(const Market& other) : is_running(false) {
    for (const auto* i : other.available_instruments) if (i) available_instruments.push_back(i->clone());
}

Market& Market::operator=(const Market& other) {
    if (this != &other) {
        stopMarket();
        for (auto* i : available_instruments) delete i;
        available_instruments.clear();
        for (const auto* i : other.available_instruments) if (i) available_instruments.push_back(i->clone());
    }
    return *this;
}

Market::~Market() {
    stopMarket();
    for (auto* i : available_instruments) delete i;
}

Instrument* Market::findInstrument(const std::string& symbol) const {
    for (auto* i : available_instruments) if (i->getSymbol() == symbol) return i;
    return nullptr;
}

void Market::startMarket() {
    if (!is_running.load()) {
        is_running = true;
        price_thread = std::thread(&Market::changePrices, this);
    }
}

void Market::stopMarket() {
    is_running = false;
    if (price_thread.joinable()) price_thread.join();
}

std::ostream& operator<<(std::ostream& os, const Market& m) {
    os << "=== PIATA DE INSTRUMENTE ===\n";
    for (const auto* inst : m.available_instruments) if (inst) os << *inst << "\n";
    return os;
}