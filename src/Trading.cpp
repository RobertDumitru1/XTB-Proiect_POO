//
// Created by dumro on 3/20/2026.
//
#include "Trading.h"

int Position::position_ids = 0;
int Transaction::transaction_ids = 0;

Position::Position(std::shared_ptr<Instrument> asset, double entry_price, double quantity, int leverage_used)
    : id(++position_ids), asset(asset), entry_price(entry_price), quantity(quantity), leverage_used(leverage_used) {
    if (leverage_used > 0)
        this->margin_blocked = (entry_price * quantity) / leverage_used;
}


int Position::getId() const { return id; }

std::shared_ptr<Instrument> Position::getInstrument() const { return asset; }

void Portfolio::addPosition(std::shared_ptr<Position> position) {
    active_positions.push_back(position);
}

double Position::getQuantity() const { return quantity; }

double Position::getMarginBlocked() const { return margin_blocked; }

double Position::getEntryPrice() const { return entry_price; }

std::shared_ptr<Position> Portfolio::findPosition(int id) {
    for (const auto &w : active_positions) {
        if (w && id == w->getId()) return w; // w este deja un shared_ptr
    }
    return nullptr;
}

void Portfolio::removePosition(int id) {
    for (auto it = active_positions.begin(); it != active_positions.end(); ++it) {
        if ((*it)->getId() == id) {
            active_positions.erase(it);
            return;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Portfolio& port) {
    os << "=== PORTOFOLIU CURENT ===\n";
    if (port.active_positions.empty()) os << "  Portofoliul este gol.\n";
    else {
        for (const auto& pos : port.active_positions) os << pos << "\n";
    }
    return os;
}

Transaction::Transaction(const std::string &symbol, double price, TipTranzactie tip)
    : id(++transaction_ids), asset_symbol(symbol), price(price), tip(tip) {}


std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    std::string tip_str = (t.tip == TipTranzactie::BUY) ? "BUY " : "SELL";
    os << "Tranzactie [ID: " << t.id << "] " << tip_str << " | " << t.asset_symbol << " | Pret: " << t.price << "$";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    if (pos.asset) {
        os << "  [ID: " << pos.id << "] -> " << pos.quantity << " x " << pos.asset->getSymbol()
           << " | Leverage: x" << pos.leverage_used
           << " | Pret intrare: " << pos.entry_price << "$ | Marja: " << pos.margin_blocked << "$";
    }
    return os;
}

int Position::getPositionsCreated() {
    return position_ids;
}
