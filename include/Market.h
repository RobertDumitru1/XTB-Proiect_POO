//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_MARKET_H
#define XTB_PROIECT_POO_MARKET_H

#include <iostream>
#include <atomic>
#include <random>
#include <thread>
#include <vector>

#include "Instrument.h"

class Market {
private:
    std::vector<Instrument*> available_instruments;
    std::thread price_thread;
    std::atomic<bool> is_running{false};

    void changePrices() const{
        while (this->is_running) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-5.0, 5.0);
            for (auto *w : available_instruments) {
                w->current_price = w->current_price + w->current_price * dis(gen) / 100;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

public:
    Market() = default;

    explicit Market(const std::vector<Instrument*>& insts){
        for (const auto* i : insts) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market(const Market& other) : is_running(false) {
        for (const auto* i : other.available_instruments) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market& operator=(const Market& other) {
        if (this != &other) {
            for (auto* i : available_instruments) delete i;
            this->stopMarket();
            this->available_instruments.clear();
            for (const auto* i : other.available_instruments) {
                if (i) this->available_instruments.push_back(i->clone());
            }
            this->is_running.store(false);
        }
        return *this;
    }

    ~Market() {
        this->stopMarket();
        for (auto* i : available_instruments) delete i;
    }

     Instrument* findInstrument(const std::string& symbol) const {
        for (auto* i : available_instruments) {
            if (i->getSymbol() == symbol) return i;
        }
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Market& m);

    void startMarket() {
        if (!this->is_running.load()) {
            this->is_running.store(true);
            price_thread = std::thread(&Market::changePrices, this);
        }
    }

    void stopMarket() {
        this->is_running.store(false);
        if (price_thread.joinable()) {
            price_thread.join();
        }

    }
};

inline std::ostream& operator<<(std::ostream& os, const Market& m) {
    os << "=== PIATA DE INSTRUMENTE ===\n";
    for (const auto* inst : m.available_instruments) {
        if (inst) os << *inst << "\n";
    }
    return os;
}

#endif //XTB_PROIECT_POO_MARKET_H