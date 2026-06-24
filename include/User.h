#ifndef OOP_USER_H
#define OOP_USER_H

#include <string>
#include <memory>
#include "Trading.h"
#include "HistoryLog.h"

#include "Market.h"
#include "Constants.h"

class User {
private:
    std::string name;
    std::string cnp;
    std::string password;
    double available_balance;
    double invested_balance;
    Portfolio portfolio;
    HistoryLog<Transaction> history;

    std::shared_ptr<Market> market;

public:
    User(const std::string& n, const std::string& c, const std::string& p, double avail, double inv, std::shared_ptr<Market> m);
    void buyAsset(const std::string& symbol, double quantity);
    void sellPosition(int id);

    double getAvailableBalance() const;
    const Portfolio& getPortfolio() const;
    const HistoryLog<Transaction>& getHistory() const;

    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

#endif //OOP_USER_H