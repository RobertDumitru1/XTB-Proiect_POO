#include <iostream>
#include <string>
#include <vector>

enum Currency {
    USD,
    EUR,
    RON
};
enum TipInstrument {
    STOCK,
    // CRIPTO,
    DERIVATE
};
class Instrument {
protected:
    std::string name;
    std::string symbol;
    double current_price;
    TipInstrument tip_instrument;
public:
    Instrument() = default;

    Instrument(const std::string& name, const std::string& symbol, double current_price)
        : name(name), symbol(symbol), current_price(current_price) {}

    Instrument(const Instrument& other) {
        this->name = other.name;
        this->symbol = other.symbol;
        this->current_price = other.current_price;
    }

    std::string getName() {
        return this->name;
    }
    std::string getSymbol() {
        return this->symbol;
    }
    double getPrice() {
        return this->current_price;
    }

    virtual TipInstrument getTip() = 0;
};

class PhysicalAsset : public Instrument {
private:
    double dividend_yield;
public:
    PhysicalAsset() = default;

    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield)
        : Instrument(name, symbol, current_price), dividend_yield(dividend_yield) {
        this->tip_instrument = TipInstrument::STOCK;
    }

    TipInstrument getTip() {
        return TipInstrument::STOCK;
    }
};

class Derivative : public Instrument {
private:
    int max_leverage;
    double swap_fee;
public:
    Derivative() = default;

    Derivative(const std::string& name, const std::string& symbol, double current_price, int max_leverage, double swap_fee)
        : Instrument(name, symbol, current_price), max_leverage(max_leverage), swap_fee(swap_fee) {
        this->tip_instrument = TipInstrument::DERIVATE;
    }

    TipInstrument getTip() {
        return TipInstrument::DERIVATE;
    }
};
class Position {
private:
    Instrument* asset;
    double entry_price;
    double quantity;
    int leverage_used;
    double margin_blocked;

public:
    Position() = default;

    Position(Instrument* asset, double entry_price, double quantity, int leverage_used)
        : asset(asset), entry_price(entry_price), quantity(quantity), leverage_used(leverage_used) {

        this->margin_blocked = (entry_price * quantity) / leverage_used;
    }

    double getMarginBlocked() const { return margin_blocked; }
    Instrument* getAsset() const { return asset; }
};

class Portfolio {
private:
    std::vector<Position*> active_positions;
public:
    Portfolio() = default;

    Portfolio(const std::vector<Position*>& active_positions)
        : active_positions(active_positions) {}

    Portfolio(const Portfolio &portfolio) {
        for (auto& w : portfolio.active_positions) {
            this->active_positions.push_back(new Position(*w));
        }
    }

    Portfolio& operator=(const Portfolio &portfolio) {
        active_positions.clear();
        for (auto& w : portfolio.active_positions) {
            this->active_positions.push_back(w);
        }
        return *this;
    }
    ~Portfolio() {
        for (auto* pos : active_positions) {
            delete pos;
        }
        active_positions.clear();
    }
    //todo ar putea fi private si friend cu user
    void addPosition(Position *position) {
        active_positions.push_back(position);
    }
};

class Transaction {
private:
    std::string asset_symbol;
    bool was_long;
    double entry_price;
    double close_price;
    double realized_profit;
public:
    Transaction() = default;

    Transaction(const std::string& asset_symbol, bool was_long, double entry_price,
                double close_price, double realized_profit)
        : asset_symbol(asset_symbol), was_long(was_long), entry_price(entry_price),
          close_price(close_price), realized_profit(realized_profit) {}
};


class Market {
private:
    std::vector<Instrument*> available_instruments;
public:
    Market() = default;

    Market(const std::vector<Instrument *> &available_instruments)
        : available_instruments(available_instruments) {}

    //TODO Rule of three

    // Market (const Market & market) {
    //
    // }

    ~Market() {
        for (auto &w : available_instruments) {
            delete w;
        }
    }

    Instrument* findInstrument(std::string symbol) {
        for (auto &w : available_instruments) {
            if (w->getSymbol() == symbol) {
                return w;
            }
        }

        std::cout << "Instrumentul cautat nu exista! \n\n";
        return nullptr;
    }

};

class User {
private:
    std::string name, cnp, password;
    Currency currency;
    double available_balance;
    double invested_balance;
    Portfolio portfolio;
    std::vector<Transaction> history;
    Market *market; // fiecare cont apartine unui market
public:
    User() = default;

    User(const std::string &name, const std::string &cnp, const std::string &password, Currency currency,
         double available_balance, double invested_balance, const Portfolio &portfolio, const std::vector<Transaction> &history,
         Market *market)
        : name(name),
          cnp(cnp),
          password(password),
          currency(currency),
          available_balance(available_balance),
          invested_balance(invested_balance),
          portfolio(portfolio),
          history(history), market(market) {}

    void buyAsset(std::string symbol, double quantity) {
        auto instrument = market->findInstrument(symbol);
        int leverage_used = 1;
        if (instrument == nullptr) {
            return;
        }
        else {
            if (instrument->getTip() == TipInstrument::DERIVATE) {
                //todo trebuie citit si levarage, la stock este 1:1
                // fin >> lev;
            }
            double required_margin = (instrument->getPrice() * quantity) / leverage_used;

            if (available_balance >= required_margin) {
                available_balance -= required_margin;
                invested_balance += required_margin;
                portfolio.addPosition(new Position(instrument, instrument->getPrice(), quantity, leverage_used));
            } else {
                std::cout << "Fonduri insuficiente!";
            }
        }
    }
};


int main() {

    return 0;
}