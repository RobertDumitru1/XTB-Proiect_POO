//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_MARKET_H
#define XTB_PROIECT_POO_MARKET_H

#include <atomic>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include "Instrument.h"

class Market {
    std::vector<std::shared_ptr<Instrument>> available_instruments;
    std::thread price_thread;
    std::atomic<bool> is_running{false};
    mutable std::mutex market_mutex;

    void changePrices() const;
public:
    Market() = default;
    explicit Market(const std::vector<std::shared_ptr<Instrument>>& insts);
    Market(const Market& other);
    Market& operator=(Market other);
    ~Market();

    [[nodiscard]] std::shared_ptr<Instrument> findInstrument(const std::string& symbol) const;
    void startMarket();
    void stopMarket();
    const std::vector<std::shared_ptr<Instrument>>& getInstruments() const;
    friend std::ostream& operator<<(std::ostream& os, const Market& m);
    friend void swap(Market& first, Market& second) noexcept;
};
#endif