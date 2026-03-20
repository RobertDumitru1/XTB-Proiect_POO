//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_TRADING_H
#define XTB_PROIECT_POO_TRADING_H


#include "Constants.h"
#include "Instrument.h"
#include <iostream>
#include <vector>

class Position {
private:
    static int position_ids;
    int id = 0;
    Instrument* asset = nullptr;
    double entry_price = 0.0;
    double close_price = 0.0;
    double quantity = 0.0;
    int leverage_used = 1;
    double margin_blocked = 0.0;
public:
    Position() = default;

    Position(Instrument* asset, double entry_price, double quantity, int leverage_used)
        : id(++position_ids), asset(asset), entry_price(entry_price), quantity(quantity), leverage_used(leverage_used) {
        if (leverage_used > 0)
            this->margin_blocked = (entry_price * quantity) / leverage_used;
    }

    int getId() const {
        return this->id;
    }

    Instrument* getInstrument() const {
        return this->asset;
    }
    void setClosePrice(const int price) {
        this->close_price = price;
    }

    double getQuantity() const{
        return this->quantity;
    }

    double getMarginBlocked() const {
        return this->margin_blocked;
    }

    double getEntryPrice() const {
        return this->entry_price;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

inline std::ostream& operator<<(std::ostream& os, const Position& pos) {
    if (pos.asset) {
        os << "  [ID: " << pos.id << "] -> " << pos.quantity << " x " << pos.asset->getSymbol()
           << " | Leverage: x" << pos.leverage_used
           << " | Pret intrare: " << pos.entry_price << "$ | Marja: " << pos.margin_blocked << "$";
    }
    return os;
}
int Position::position_ids = 0;


class Portfolio {
private:
    std::vector<Position> active_positions;
public:
    Portfolio() = default;

    explicit Portfolio(const std::vector<Position>& active_positions)
        : active_positions(active_positions) {}

    void addPosition(const Position& position) {
        active_positions.push_back(position);
    }

    Position* findPosition(const int id) {
        for (auto &w : this->active_positions) {
            if (id == w.getId()) {
                return &w;
            }
        }
        return nullptr;
    }

    void removePosition(const int id) {
        for (auto it = active_positions.begin(); it != active_positions.end(); ++it) {
            if (it->getId() == id) {
                active_positions.erase(it);
                return;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Portfolio& port);
};

inline std::ostream& operator<<(std::ostream& os, const Portfolio& port) {
    os << "=== PORTOFOLIU CURENT ===\n";
    if (port.active_positions.empty()) {
        os << "  Portofoliul este gol.\n";
    } else {
        for (const auto& pos : port.active_positions) {
            os << pos << "\n";
        }
    }
    return os;
}

class Transaction {
private:
    static int transaction_ids;
    int id = 0;
    std::string asset_symbol = "";
    double price = 0.0;
    TipTranzactie tip = TipTranzactie::BUY;
public:
    Transaction() = default;

    Transaction(const std::string &symbol, double price, TipTranzactie tip)
        : id(++transaction_ids), asset_symbol(symbol), price(price), tip(tip) {}

    friend std::ostream& operator<<(std::ostream& os, const Transaction& t);
};

int Transaction::transaction_ids = 0;

inline std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    std::string tip_str = (t.tip == TipTranzactie::BUY) ? "BUY " : "SELL";
    os << "Tranzactie [ID: " << t.id << "] " << tip_str << " | " << t.asset_symbol
       << " | Pret: " << t.price << "$";
    return os;
}

#endif //XTB_PROIECT_POO_TRADING_H