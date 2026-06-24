#include "User.h"
#include "Market.h"
#include "Instrument.h"
#include "Exceptions.h"
#include <iostream>

User::User(const std::string& n, const std::string& c, const std::string& p, double avail, double inv, std::shared_ptr<Market> m)
    : name(n), cnp(c), password(p), available_balance(avail),
      invested_balance(inv), market(m) {}

void User::buyAsset(const std::string& symbol, double quantity) {
    if (quantity <= 0) {
        throw InvalidQuantityException(quantity);
    }

    auto inst = market->findInstrument(symbol);
    double requiredMargin = inst->calculateMargin(quantity);

    if (this->available_balance < requiredMargin) {
        throw InsufficientFundsException(requiredMargin, this->available_balance);
    }

    available_balance -= requiredMargin;
    invested_balance += requiredMargin;

    int leverage = 1;

    auto dptr = std::dynamic_pointer_cast<Derivative>(inst);
    if (dptr != nullptr) {
        leverage = dptr->getLeverage();
    }

    auto new_pos = std::make_shared<Position>(inst, inst->getPrice(), quantity, leverage);
    portfolio.addPosition(new_pos);

    history.addRecord(Transaction(symbol, inst->getPrice(), TipTranzactie::BUY));
}

void User::sellPosition(int id) {
    auto pos = portfolio.findPosition(id);
    if (!pos) {
        throw TradingException("Eroare: Pozitia cu ID-ul " + std::to_string(id) + " nu a fost gasita.");
    }

    auto inst = pos->getInstrument();

    available_balance += pos->getMarginBlocked() + (inst->getPrice() - pos->getEntryPrice()) * pos->getQuantity();
    invested_balance -= pos->getMarginBlocked();

    history.addRecord(Transaction(inst->getSymbol(), inst->getPrice(), TipTranzactie::SELL));

    portfolio.removePosition(id);
}

double User::getAvailableBalance() const { return available_balance; }
const Portfolio& User::getPortfolio() const { return portfolio; }
const HistoryLog<Transaction>& User::getHistory() const { return history; }

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "User: " << u.name << " | Balanta: " << u.available_balance << "\n" << u.portfolio;
    return os;
}