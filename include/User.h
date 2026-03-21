//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_USER_H
#define XTB_PROIECT_POO_USER_H

#include "Trading.h"
#include "Market.h"

class User {
    std::string name = "", cnp = "", password = "";
    Currency currency = USD;
    double available_balance = 0.0;
    double invested_balance = 0.0;
    Portfolio portfolio;
    std::vector<Transaction> history;
    Market* market = nullptr;

public:
    User() = default;
    User(const std::string& n, const std::string& c, const std::string& p, Currency curr, double avail, double inv, const Portfolio& port, const std::vector<Transaction>& hist, Market* m);
    void buyAsset(const std::string& symbol, double quantity);
    void sellPosition(int position_id);
    void printHistory() const;
    friend std::ostream& operator<<(std::ostream& os, const User& u);
};
#endif