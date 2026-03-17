#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Folosim un nume mai explicit pentru a evita conflictele
std::ifstream fin_tastatura("tastatura.txt");

enum Currency { USD, EUR, RON };
enum TipInstrument { GENERAL, STOCK, DERIVATE };
const std::string nume_monede[] = { "USD", "EUR", "RON" };

class Instrument {
protected:
    std::string name = "";
    std::string symbol = "";
    double current_price = 0.0;
    TipInstrument tip_instrument = TipInstrument::GENERAL;
public:
    Instrument() = default;

    Instrument(const std::string& name, const std::string& symbol, double current_price)
        : name(name), symbol(symbol), current_price(current_price) {}

    virtual ~Instrument() = default;

     std::string getName() const { return this->name; }
     std::string getSymbol() const { return this->symbol; }
     double getPrice() const { return this->current_price; }

     virtual TipInstrument getTip() const = 0;
     virtual Instrument* clone() const = 0;

    virtual void print(std::ostream& os) const {
        os << "[" << symbol << "] " << name << " - Pret: " << current_price << "$";
    }

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
};

std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    os << "[" << inst.symbol << "] " << inst.name << " - Pret: " << inst.current_price << "$";
    return os;
}

class PhysicalAsset : public Instrument {
private:
    double dividend_yield = 0.0;
public:
    PhysicalAsset() = default;

    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield)
        : Instrument(name, symbol, current_price), dividend_yield(dividend_yield) {
        this->tip_instrument = TipInstrument::STOCK;
    }

    ~PhysicalAsset() override = default;

     TipInstrument getTip() const override { return TipInstrument::STOCK; }

     Instrument* clone() const override {
        return new PhysicalAsset(*this);
    }

    void print(std::ostream& os) const override {
         Instrument::print(os);
         os << " | Dividend: " << dividend_yield << "%";
     }
};

class Derivative : public Instrument {
private:
    int max_leverage = 1;
    double swap_fee = 0.0;
public:
    Derivative() = default;

    Derivative(const std::string& name, const std::string& symbol, double current_price, int max_leverage, double swap_fee)
        : Instrument(name, symbol, current_price), max_leverage(max_leverage), swap_fee(swap_fee) {
        this->tip_instrument = TipInstrument::DERIVATE;
    }

    ~Derivative() override = default;

     TipInstrument getTip() const override { return TipInstrument::DERIVATE; }

     Instrument* clone() const override {
        return new Derivative(*this);
    }

    void print(std::ostream& os) const override {
         Instrument::print(os);
         os << " | Leverage: x" << max_leverage << " | Swap: " << swap_fee;
     }
};

class Position {
private:
    Instrument* asset = nullptr;
    double entry_price = 0.0;
    double quantity = 0.0;
    int leverage_used = 1;
    double margin_blocked = 0.0;
public:
    Position() = default;

    Position(Instrument* asset, double entry_price, double quantity, int leverage_used)
        : asset(asset), entry_price(entry_price), quantity(quantity), leverage_used(leverage_used) {
        if (leverage_used > 0)
            this->margin_blocked = (entry_price * quantity) / leverage_used;
    }

     double getMarginBlocked() const { return margin_blocked; }
     Instrument* getAsset() const { return asset; }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    if (pos.asset) {
        os << "  -> " << pos.quantity << " x " << pos.asset->getSymbol()
           << " | Leverage: x" << pos.leverage_used
           << " | Pret intrare: " << pos.entry_price << "$ | Marja: " << pos.margin_blocked << "$";
    }
    return os;
}

class Portfolio {
private:
    std::vector<Position> active_positions;
public:
    Portfolio() = default;

    explicit Portfolio(const std::vector<Position>& active_positions)
        : active_positions(active_positions) {}

    void addPosition(const Position& position) {
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
    std::string asset_symbol = "";
    double entry_price = 0.0;
    double close_price = 0.0;
public:
    Transaction() = default;

    Transaction(const std::string &symbol, double entry, double close)
        : asset_symbol(symbol), entry_price(entry), close_price(close) {}

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

    explicit Market(const std::vector<Instrument*>& insts) {
        for (const auto* i : insts) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market(const Market& other) {
        for (const auto* i : other.available_instruments) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market& operator=(const Market& other) {
        if (this != &other) {
            for (auto* i : available_instruments) delete i;
            available_instruments.clear();
            for (auto* i : other.available_instruments) {
                if (i) this->available_instruments.push_back(i->clone());
            }
        }
        return *this;
    }

    ~Market() {
        for (auto* i : available_instruments) delete i;
    }

     Instrument* findInstrument(const std::string& symbol) const {
        for (auto* i : available_instruments) {
            if (i->getSymbol() == symbol) return i;
        }
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Market& m);
};

std::ostream& operator<<(std::ostream& os, const Market& m) {
    os << "=== PIATA DE INSTRUMENTE ===\n";
    for (const auto* inst : m.available_instruments) {
        if (inst) os << *inst << "\n";
    }
    return os;
}

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
            history.emplace_back(symbol, instrument->getPrice(), 0.0);
            std::cout << "Succes: " << quantity << " " << symbol << "\n";
        } else {
            std::cout << "Fonduri insuficiente pentru " << symbol << "!\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "Utilizator: " << u.name << " | Balanta: " << u.available_balance << "$ CNP:" << u.cnp << " Parola: " << u.password << " Currency: " << nume_monede[u.currency] << "\n" << u.portfolio;
    return os;
}

int main() {
    // Folosim un vector temporar pentru a popula piata
    std::vector<Instrument*> de_adaugat;
    de_adaugat.push_back(new PhysicalAsset("Apple Inc.", "AAPL", 150.5, 2.5));
    de_adaugat.push_back(new PhysicalAsset("Tesla Inc.", "TSLA", 240.0, 0.0));
    de_adaugat.push_back(new Derivative("Bitcoin Perpetual", "BTC-PERP", 65000.0, 10, 0.01));
    de_adaugat.push_back(new Derivative("Ethereum Futures", "ETH-FUT", 3500.0, 5, 0.02));

    // Initializam piata (aceasta va face deep copy prin clone())
    Market bursa_valori(de_adaugat);

    // Curatam vectorul temporar pentru a evita memory leaks (Market are deja copiile sale)
    for (auto* i : de_adaugat) delete i;
    de_adaugat.clear();

    // Afisam piata disponibila
    std::cout << bursa_valori << std::endl;

    // Crearea unui Utilizator ---
    Portfolio portofoliu_initial;
    std::vector<Transaction> istoric_initial;

    User client("Andrei Ionescu", "5010101123456", "parola123", USD,
                10000.0, // Balanta initiala
                0.0,     // Investitii initiale
                portofoliu_initial, istoric_initial, &bursa_valori);

    std::cout << "--- Stare Initiala Client ---" << std::endl;
    std::cout << client << std::endl;

    // Simularea tranzactiilor
    std::cout << "\n>>> Se proceseaza tranzactiile...\n" << std::endl;

    // Cumparam un activ fizic (leverage va fi 1 implicit)
    client.buyAsset("AAPL", 10); // 10 * 150.5 = 1505$ marja

    // Cumparam un derivat
    client.buyAsset("BTC-PERP", 0.5);

    // Incercam sa cumparam ceva ce nu exista
    client.buyAsset("GOOGLE", 1);

    // Incercam sa cumparam peste buget
    client.buyAsset("TSLA", 1000);

    std::cout << "\n--- Stare Finala Client ---" << std::endl;
    std::cout << client << std::endl;

    return 0;
}