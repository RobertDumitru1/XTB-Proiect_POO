//
// Created by dumro on 3/20/2026.
//

#include <iostream>
#include "User.h"

#include "Exceptions.h"

User::User(const std::string& n, const std::string& c, const std::string& p,
           Currency curr, double avail, double inv, const Portfolio& port,
           const std::vector<Transaction>& hist, std::shared_ptr<Market> m)
    : name(n), cnp(c), password(p), currency(curr), available_balance(avail),
      invested_balance(inv), portfolio(port), history(hist), market(m) {}


void User::buyAsset(const std::string& symbol, double quantity) {
    if (!this->market) {
        throw TradingException("Utilizatorul nu este conectat la nicio piata.");
    }

    if (quantity <= 0) {
        throw InvalidQuantityException(quantity);
    }

    double leverage = 1.0;
    auto inst = market->findInstrument(symbol);
    auto dptr = std::dynamic_pointer_cast<Derivative>(inst);

    if (dptr != nullptr) {
        leverage = dptr->getLeverage();
    }

    double requiredMargin = inst->calculateMargin(quantity);

    if (this->available_balance < requiredMargin) {
        throw InsufficientFundsException(requiredMargin, this->available_balance);
    }

    available_balance -= requiredMargin;
    invested_balance += requiredMargin;
    auto new_pos = std::make_shared<Position>(inst, inst->getPrice(), quantity, (int)leverage);

    portfolio.addPosition(new_pos);
    history.emplace_back(symbol, inst->getPrice(), TipTranzactie::BUY);

    std::cout << "Ai cumparat " << quantity << " " << symbol << "!\n";
}

void User::sellPosition(int id) {
    auto pos = portfolio.findPosition(id);
    if (!pos) {
        std::cout << "Eroare: Pozitia cu ID-ul " << id << " nu a fost gasita.\n";
        return;
    }
    auto inst = pos->getInstrument();

    available_balance += pos->getMarginBlocked() + (inst->getPrice() - pos->getEntryPrice()) * pos->getQuantity();
    invested_balance -= pos->getMarginBlocked();

    history.emplace_back(inst->getSymbol(), inst->getPrice(), TipTranzactie::SELL);
    std::cout << "Ai vandut " << pos->getQuantity() << " " << inst->getSymbol() << "!\n";
    portfolio.removePosition(id);
}

void User::printHistory() const {
    for (const auto& t : history) std::cout << t << "\n";
}

double User::getAvailableBalance() const {
    return available_balance;
}

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "User: " << u.name << " | Balanta: " << u.available_balance << nume_monede[u.currency] << "\n" << u.portfolio;
    return os;
}