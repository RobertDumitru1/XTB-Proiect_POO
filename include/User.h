//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_USER_H
#define XTB_PROIECT_POO_USER_H

#include <iostream>

#include "Constants.h"
#include "Trading.h"
#include "Market.h"

class User {
private:
    std::string name = "", cnp = "", password = "";
    Currency currency = USD;
    double available_balance = 0.0;
    double invested_balance = 0.0;
    Portfolio portfolio;
    std::vector<Transaction> history;
    Market* market = nullptr;

public:
    User() = default;

    User(const std::string& n, const std::string& c, const std::string& p, Currency curr,
         double avail, double inv, const Portfolio& port, const std::vector<Transaction>& hist, Market* m)
        : name(n), cnp(c), password(p), currency(curr), available_balance(avail),
          invested_balance(inv), portfolio(port), history(hist), market(m) {}

    void buyAsset(const std::string& symbol, double quantity) {
        if (!market) return;
        auto* instrument = market->findInstrument(symbol);
        if (!instrument) {
            std::cout << "Instrumentul " << symbol << " nu exista!\n";
            return;
        }

        int leverage = 1;
        if (instrument->getTip() == TipInstrument::DERIVATE && fin_tastatura.is_open()) {
            fin_tastatura >> leverage;
        }

        double margin = (instrument->getPrice() * quantity) / leverage;
        if (available_balance >= margin) {
            available_balance -= margin;
            invested_balance += margin;
            portfolio.addPosition(Position(instrument, instrument->getPrice(), quantity, leverage));
            history.push_back(Transaction(symbol, instrument->getPrice(), TipTranzactie::BUY));
            std::cout << "Succes: " << quantity << " " << symbol << "\n";
        } else {
            std::cout << "Fonduri insuficiente pentru " << symbol << "!\n";
        }
    }

    void sellPosition(const int position_id) {
        if (!position_id) {
            std::cout << "Pozitia introdusa a fost inchisa sau nu exista\n";
            return;
        }
        Position *pos = portfolio.findPosition(position_id);
        if (!pos) {
            std::cout << "Pozitia introdusa a fost inchisa sau nu exista\n";
            return;
        }
        const Instrument *inst = pos->getInstrument();
        pos->setClosePrice(inst->getPrice());
        this->available_balance += pos->getMarginBlocked() + (inst->getPrice() - pos->getEntryPrice()) * pos->getQuantity();
        this->invested_balance -= pos->getMarginBlocked();
        this->history.push_back(Transaction(inst->getSymbol(), inst->getPrice(), TipTranzactie::SELL));

        this->portfolio.removePosition(position_id);
    }

    void printHistory() const {
        std::cout << "=== ISTORIC TRANZACTII (" << name << ") ===\n";
        if (history.empty()) {
            std::cout << "Nu exista tranzactii.\n";
            return;
        }
        for (const auto& t : history) {
            std::cout << t << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

inline std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "Utilizator: " << u.name << " | Balanta: " << u.available_balance << "$ CNP:" << u.cnp << " Parola: " << u.password << " Currency: " << nume_monede[u.currency] << "\n" << u.portfolio;
    return os;
}

#endif //XTB_PROIECT_POO_USER_H