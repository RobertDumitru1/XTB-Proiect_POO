//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_TRADING_H
#define XTB_PROIECT_POO_TRADING_H

#include "Constants.h"
#include "Instrument.h"
#include <vector>

class Position {
    static int position_ids;
    int id = 0;
    Instrument* asset = nullptr;
    double entry_price = 0.0;
    double quantity = 0.0;
    int leverage_used = 1;
    double margin_blocked = 0.0;
public:
    Position() = default;
    Position(Instrument* asset, double entry_price, double quantity, int leverage_used);
    [[nodiscard]] int getId() const;
    [[nodiscard]] Instrument* getInstrument() const;
    void setClosePrice(double price);
    [[nodiscard]] double getQuantity() const ;
    [[nodiscard]] double getMarginBlocked() const;
    [[nodiscard]] double getEntryPrice() const;
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

class Portfolio {
    std::vector<Position> active_positions;
public:
    Portfolio() = default;
    void addPosition(const Position& position);
    Position* findPosition(int id);
    void removePosition(int id);
    friend std::ostream& operator<<(std::ostream& os, const Portfolio& port);
};

class Transaction {
    static int transaction_ids;
    int id = 0;
    std::string asset_symbol = "";
    double price = 0.0;
    TipTranzactie tip = TipTranzactie::BUY;
public:
    Transaction() = default;
    Transaction(const std::string &symbol, double price, TipTranzactie tip);
    friend std::ostream& operator<<(std::ostream& os, const Transaction& t);
};
#endif