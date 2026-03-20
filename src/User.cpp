//
// Created by dumro on 3/20/2026.
//

#include "User.h"

User::User(std::string n, std::string c, std::string p, Currency curr, double avail, double inv, Portfolio port, std::vector<Transaction> hist, Market* m)
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
        history.push_back(Transaction(symbol, inst->getPrice(), TipTranzactie::BUY));
    }
}

void User::sellPosition(int id) {
    Position *pos = portfolio.findPosition(id);
    if (!pos) return;
    Instrument *inst = pos->getInstrument();
    available_balance += pos->getMarginBlocked() + (inst->getPrice() - pos->getEntryPrice()) * pos->getQuantity();
    invested_balance -= pos->getMarginBlocked();
    history.push_back(Transaction(inst->getSymbol(), inst->getPrice(), TipTranzactie::SELL));
    portfolio.removePosition(id);
}

void User::printHistory() const {
    for (const auto& t : history) std::cout << t << "\n";
}

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "User: " << u.name << " | Balanta: " << u.available_balance << "$\n" << u.portfolio;
    return os;
}