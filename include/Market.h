//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_MARKET_H
#define XTB_PROIECT_POO_MARKET_H

#include <atomic>
#include <thread>
#include <vector>
#include "Instrument.h"

class Market {
    std::vector<Instrument*> available_instruments;
    std::thread price_thread;
    std::atomic<bool> is_running{false};

    void changePrices() const;
public:
    Market() = default;
    explicit Market(const std::vector<Instrument*>& insts);
    Market(const Market& other);
    Market& operator=(const Market& other);
    ~Market();

    [[nodiscard]]Instrument* findInstrument(const std::string& symbol) const;
    void startMarket();
    void stopMarket();
    const std::vector<Instrument*>& getInstruments() const ;
    friend std::ostream& operator<<(std::ostream& os, const Market& m);
};
#endif