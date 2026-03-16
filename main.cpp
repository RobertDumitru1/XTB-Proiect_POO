#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::ifstream fin("tastatura.txt");

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
//completa
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

    // Instrument(const Instrument& other) {
    //     this->name = other.name;
    //     this->symbol = other.symbol;
    //     this->current_price = other.current_price;
    //     this->tip_instrument = other.tip_instrument;
    // }
    //
    // Instrument& operator=(const Instrument &other) {
    //     this->name = other.name;
    //     this->symbol = other.symbol;
    //     this->current_price = other.current_price;
    //     this->tip_instrument = other.tip_instrument;
    //     return *this;
    // }
    //
    virtual ~Instrument() = default;

    std::string getName() const {
        return this->name;
    }
    std::string getSymbol() const {
        return this->symbol;
    }
    double getPrice() const {
        return this->current_price;
    }

    virtual TipInstrument getTip() const = 0;
    virtual Instrument* clone() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
};

std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    os << "[" << inst.symbol << "] " << inst.name << " - Pret: " << inst.current_price << "$";
    return os;
}

//todo constructor de copiere si operatorul =
class PhysicalAsset : public Instrument {
private:
    double dividend_yield;
public:
    PhysicalAsset() = default;

    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield)
        : Instrument(name, symbol, current_price), dividend_yield(dividend_yield) {
        this->tip_instrument = TipInstrument::STOCK;
    }
    ~PhysicalAsset() = default;
    TipInstrument getTip() const {
        return TipInstrument::STOCK;
    }
    Instrument* clone() const{
        return new PhysicalAsset(*this);
    }
};
//todo constructor de copiere si operatorul =
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
    ~Derivative() = default;
    TipInstrument getTip() const {
        return TipInstrument::DERIVATE;
    }
    Instrument* clone() const {
        return new Derivative(*this);
    }
};

//completa
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

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << "  -> " << pos.quantity << " x " << pos.asset->getSymbol()
       << " | Pret intrare: " << pos.entry_price << "$ | Marja: " << pos.margin_blocked << "$";
    return os;
}

class Portfolio {
private:
    std::vector<Position> active_positions;
public:
    Portfolio() = default;

    Portfolio(const std::vector<Position>& active_positions)
        : active_positions(active_positions) {}

    Portfolio(const Portfolio &portfolio) {
        for (auto& w : portfolio.active_positions) {
            this->active_positions.push_back(w);
        }
    }

    Portfolio& operator=(const Portfolio &portfolio) {
        active_positions.clear();
        for (auto& w : portfolio.active_positions) {
            this->active_positions.push_back(w);
        }
        return *this;
    }
    ~Portfolio() = default;
    //todo ar putea fi private si friend cu user
    void addPosition(Position position) {
        active_positions.push_back(position);
    }

    friend std::ostream& operator<<(std::ostream& os, const Portfolio& port);
};

std::ostream& operator<<(std::ostream& os, const Portfolio& port) {
    os << "=== PORTOFOLIU CURENT ===\n";
    if (port.active_positions.empty()) {
        os << "  Portofoliul este gol.\n";
    } else {
        for (const auto& pos : port.active_positions) {
            os << pos << "\n";
        }
    }
    return os;
}

class Transaction {
private:
    std::string asset_symbol;
    double entry_price;
    double close_price;
public:
    Transaction() = default;

    Transaction(const std::string &asset_symbol, double entry_price, double close_price)
        : asset_symbol(asset_symbol),
          entry_price(entry_price),
          close_price(close_price) {
    }

    friend std::ostream& operator<<(std::ostream& os, const Transaction& t);
};
std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    os << "Tranzactie: " << t.asset_symbol << " | Pret intrare: " << t.entry_price
       << "$ | Pret inchidere: " << t.close_price << "$";
    return os;
}

class Market {
private:
    std::vector<Instrument*> available_instruments;
public:
    Market() = default;

    Market(const std::vector<Instrument *> &available_instruments)
        : available_instruments(available_instruments) {}

    Market (const Market& market) {
        for (auto &w : market.available_instruments) {
            this->available_instruments.push_back(w->clone());
        }
    }

    Market& operator=(const Market& market) {
        if (this == &market) return *this;

        for (auto &w : available_instruments) {
            delete w;
        }
        available_instruments.clear();
        for (auto &w : market.available_instruments) {
            this->available_instruments.push_back(w->clone());
        }
        return *this;
    }

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

    friend std::ostream& operator<<(std::ostream& os, const Market& m);

};

std::ostream& operator<<(std::ostream& os, const Market& m) {
    os << "=== PIATA DE INSTRUMENTE ===\n";
    for (const auto& inst : m.available_instruments) {
        os << *inst << "\n";
    }
    return os;
}

class User {
private:
    std::string name, cnp, password;
    Currency currency;
    double available_balance;
    double invested_balance;
    Portfolio portfolio;
    std::vector<Transaction> history;
    Market *market;
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
          history(history),
          market(market) {}

    void buyAsset(std::string symbol, double quantity) {
        auto instrument = market->findInstrument(symbol);
        int leverage_used = 1;
        if (instrument == nullptr) {
            return;
        }
        else {
            if (instrument->getTip() == TipInstrument::DERIVATE) {
                fin >> leverage_used;
            }
            double required_margin = (instrument->getPrice() * quantity) / leverage_used;

            if (available_balance >= required_margin) {
                this->available_balance -= required_margin;
                this->invested_balance += required_margin;
                this->portfolio.addPosition(Position(instrument, instrument->getPrice(), quantity, leverage_used));
                history.push_back(Transaction(symbol, instrument->getPrice(), 0.0));
                std::cout << "Ai cumparat cu succes " << quantity << " " << symbol << "!\n";
            } else {
                std::cout << "Fonduri insuficiente!";
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "====================================\n";
    os << "Utilizator: " << u.name << "\n";
    os << "Balanta disponibila: " << u.available_balance << " $\n";
    os << "Bani investiti: " << u.invested_balance << " $\n";
    os << "------------------------------------\n";

    os << u.portfolio;

    os << "--- ISTORIC TRANZACTII ---\n";
    if (u.history.empty()) {
        os << "  Nicio tranzactie inregistrata.\n";
    } else {
        for (const auto& tx : u.history) {
            os << tx << "\n";
        }
    }
    os << "====================================\n";
    return os;
}

int main() {
    // Pregatim Piata
    std::vector<Instrument*> instrumenteInitiale;
    instrumenteInitiale.push_back(new PhysicalAsset("Apple Inc.", "AAPL", 150.5, 0.02));
    instrumenteInitiale.push_back(new PhysicalAsset("Tesla Inc.", "TSLA", 200.0, 0.00));
    instrumenteInitiale.push_back(new Derivative("Bitcoin", "BTC", 65000.0, 10, 0.001));

    // Cream Market-ul si il afisam ca sa verificam operatorul<<
    Market bursa(instrumenteInitiale);
    std::cout << bursa << "\n\n";

    // Cream un Utilizator nou
    std::vector<Transaction> istoricGol;
    Portfolio portofoliuGol;

    User client("Alex Popescu", "1900101123456", "parolaSuperSecreta", USD,
                5000.0, // Bani disponibili
                0.0,    // Bani investiti la inceput
                portofoliuGol, istoricGol, &bursa);

    std::cout << "--- Status inainte de actiuni ---\n";
    std::cout << client << "\n";

    // Incepem sa tranzactionam!
    std::cout << "\n>>> INCEPERE TRANZACTII >>>\n\n";

    // Cumparam 10 actiuni Apple
    client.buyAsset("AAPL", 10);

    // Cumparam 0.1 Bitcoin
    client.buyAsset("BTC", 0.1);

    // Incercam sa cumparam ceva ce nu exista ca sa testam logica de eroare
    client.buyAsset("AMZN", 5);

    // Incercam sa cumparam prea mult
    client.buyAsset("TSLA", 50);

    //  Afisam statusul final
    std::cout << "\n\n--- Status final ---\n";
    std::cout << client << "\n";

    return 0;
}