//
// Created by dumro on 3/20/2026.
//

#include <iostream>
#include "User.h"

User::User(const std::string& n, const std::string& c, const std::string& p, Currency curr, double avail, double inv, const Portfolio& port, const std::vector<Transaction>& hist, Market* m)
    : name(n), cnp(c), password(p), currency(curr), available_balance(avail), invested_balance(inv), portfolio(port), history(hist), market(m) {}

void User::buyAsset(const std::string& symbol, double quantity) {
    if (!market) return;
    auto* inst = market->findInstrument(symbol);
    if (!inst) { std::cout << "Nu exista!\n"; return; }

    int leverage = 1;
    if (inst->getTip() == TipInstrument::DERIVATE && fin_tastatura.is_open()) fin_tastatura >> leverage;

    double margin = (inst->getPrice() * quantity) / leverage;
    if (available_balance >= margin) {
        available_balance -= margin; invested_balance += margin;
        portfolio.addPosition(Position(inst, inst->getPrice(), quantity, leverage));
        history.emplace_back(symbol, inst->getPrice(), TipTranzactie::BUY);

        std::cout << "Ai cumparat " << quantity << " " << symbol << "!\n";
    }
}

void User::sellPosition(int id) {
    Position *pos = portfolio.findPosition(id);
    if (!pos) return;
    Instrument *inst = pos->getInstrument();
    available_balance += pos->getMarginBlocked() + (inst->getPrice() - pos->getEntryPrice()) * pos->getQuantity();
    invested_balance -= pos->getMarginBlocked();
    history.emplace_back(inst->getSymbol(), inst->getPrice(), TipTranzactie::SELL);
    std::cout << "Ai vandut " << pos->getQuantity() << " " << pos->getInstrument()->getSymbol() << "!\n";
    portfolio.removePosition(id);
}

void User::printHistory() const {
    for (const auto& t : history) std::cout << t << "\n";
}

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "User: " << u.name << " | Balanta: " << u.available_balance << nume_monede[u.currency] << "\n" << u.portfolio;
    return os;
}