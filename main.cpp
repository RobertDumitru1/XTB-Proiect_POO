#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <thread>

class Market;

// Folosim un nume mai explicit pentru a evita conflictele
std::ifstream fin_tastatura("tastatura.txt");

enum Currency { USD, EUR, RON };
enum TipInstrument { GENERAL, STOCK, DERIVATE };
enum TipTranzactie {BUY, SELL};
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

     std::string getSymbol() const { return this->symbol; }
     double getPrice() const { return this->current_price; }

     virtual TipInstrument getTip() const = 0;
     virtual Instrument* clone() const = 0;

    virtual void print(std::ostream& os) const {
        os << "[" << symbol << "] " << name << " - Pret: " << current_price << "$";
    }

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
    friend class Market;
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
    static int position_ids;
    int id = 0;
    Instrument* asset = nullptr;
    double entry_price = 0.0;
    double close_price = 0.0;
    double quantity = 0.0;
    int leverage_used = 1;
    double margin_blocked = 0.0;
public:
    Position() = default;

    Position(Instrument* asset, double entry_price, double quantity, int leverage_used)
        : id(++position_ids), asset(asset), entry_price(entry_price), quantity(quantity), leverage_used(leverage_used) {
        if (leverage_used > 0)
            this->margin_blocked = (entry_price * quantity) / leverage_used;
    }

    int getId() const {
        return this->id;
    }

    Instrument* getInstrument() const {
        return this->asset;
    }
    void setClosePrice(const int price) {
        this->close_price = price;
    }

    double getQuantity() const{
        return this->quantity;
    }

    double getMarginBlocked() const {
        return this->margin_blocked;
    }

    double getEntryPrice() const {
        return this->entry_price;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    if (pos.asset) {
        os << "  [ID: " << pos.id << "] -> " << pos.quantity << " x " << pos.asset->getSymbol()
           << " | Leverage: x" << pos.leverage_used
           << " | Pret intrare: " << pos.entry_price << "$ | Marja: " << pos.margin_blocked << "$";
    }
    return os;
}
int Position::position_ids = 0;


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

    Position* findPosition(const int id) {
        for (auto &w : this->active_positions) {
            if (id == w.getId()) {
                return &w;
            }
        }
        return nullptr;
    }

    void removePosition(const int id) {
        for (auto it = active_positions.begin(); it != active_positions.end(); ++it) {
            if (it->getId() == id) {
                active_positions.erase(it);
                return;
            }
        }
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
    static int transaction_ids;
    int id = 0;
    std::string asset_symbol = "";
    double price = 0.0;
    TipTranzactie tip = TipTranzactie::BUY;
public:
    Transaction() = default;

    Transaction(const std::string &symbol, double price, TipTranzactie tip)
        : id(++transaction_ids), asset_symbol(symbol), price(price), tip(tip) {}

    friend std::ostream& operator<<(std::ostream& os, const Transaction& t);
};

int Transaction::transaction_ids = 0;

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    std::string tip_str = (t.tip == TipTranzactie::BUY) ? "BUY " : "SELL";
    os << "Tranzactie [ID: " << t.id << "] " << tip_str << " | " << t.asset_symbol
       << " | Pret: " << t.price << "$";
    return os;
}

class Market {
private:
    std::vector<Instrument*> available_instruments;
    std::thread price_thread;
    std::atomic<bool> is_running{false};

    void changePrices() const{
        while (this->is_running) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-5.0, 5.0);
            for (auto *w : available_instruments) {
                w->current_price = w->current_price + w->current_price * dis(gen) / 100;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

public:
    Market() = default;

    explicit Market(const std::vector<Instrument*>& insts){
        for (const auto* i : insts) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market(const Market& other) : is_running(false) {
        for (const auto* i : other.available_instruments) {
            if (i) this->available_instruments.push_back(i->clone());
        }
    }

    Market& operator=(const Market& other) {
        if (this != &other) {
            for (auto* i : available_instruments) delete i;
            this->stopMarket();
            this->available_instruments.clear();
            for (const auto* i : other.available_instruments) {
                if (i) this->available_instruments.push_back(i->clone());
            }
            this->is_running.store(false);
        }
        return *this;
    }

    ~Market() {
        this->stopMarket();
        for (auto* i : available_instruments) delete i;
    }

     Instrument* findInstrument(const std::string& symbol) const {
        for (auto* i : available_instruments) {
            if (i->getSymbol() == symbol) return i;
        }
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Market& m);

    void startMarket() {
        if (!this->is_running.load()) {
            this->is_running.store(true);
            price_thread = std::thread(&Market::changePrices, this);
        }
    }

    void stopMarket() {
        this->is_running.store(false);
        if (price_thread.joinable()) {
            price_thread.join();
        }

    }
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

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "Utilizator: " << u.name << " | Balanta: " << u.available_balance << "$ CNP:" << u.cnp << " Parola: " << u.password << " Currency: " << nume_monede[u.currency] << "\n" << u.portfolio;
    return os;
}

int main() {
    std::cout << ">>> INITIALIZARE PIATA\n";
    std::vector<Instrument*> de_adaugat;
    de_adaugat.push_back(new PhysicalAsset("Apple Inc.", "AAPL", 150.0, 2.5));
    de_adaugat.push_back(new PhysicalAsset("Tesla Inc.", "TSLA", 240.0, 0.0));
    de_adaugat.push_back(new Derivative("Bitcoin Perpetual", "BTC-PERP", 60000.0, 10, 0.01));

    Market bursa_valori(de_adaugat);

    for (auto* i : de_adaugat) delete i;
    de_adaugat.clear();

    std::cout << "Deschidem piata principala...\n";
    bursa_valori.startMarket();

    std::cout << "\n>>> OBSERVAM FLUCTUATIILE PIETEI TIMP DE 10 SECUNDE\n";
    for (int i = 0; i <= 10; ++i) {
        std::cout << "--- Secunda " << i << " ---\n";
        std::cout << bursa_valori << "\n";

        if (i < 10) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    std::cout << "-------------------------------------------------------\n\n";

    std::cout << ">>> CREARE UTILIZATOR\n";
    Portfolio portofoliu_initial;
    std::vector<Transaction> istoric_initial;
    User client("Andrei Ionescu", "5010101123456", "parola123", USD,
                10000.0, // Balanta disponibila
                0.0,     // Balanta investita
                portofoliu_initial, istoric_initial, &bursa_valori);

    std::cout << client << "\n";

    std::cout << ">>> TESTARE ACHIZITII (BUY)\n";
    client.buyAsset("AAPL", 10.0);
    client.buyAsset("BTC-PERP", 0.5);
    client.buyAsset("FAKECOIN", 100);
    client.buyAsset("TSLA", 10000.0);

    std::cout << "\n>>> STARE DUPA ACHIZITII:\n" << client << "\n";

    std::cout << ">>> ASTEPTAM FLUCTUATII DE PRET\n";
    for (int i = 1; i <= 2; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Update preturi\n" << bursa_valori << "\n";
    }

    std::cout << ">>> TESTARE VANZARE \n";
    std::cout << "Incercam sa vindem AAPL (ID 1)\n";
    client.sellPosition(1);

    std::cout << "Incercam sa vindem o pozitie inexistenta (ID 99)\n";
    client.sellPosition(99);

    std::cout << "\n>>> STARE FINALA CLIENT:\n" << client << "\n";

    std::cout << ">>> VERIFICARE ISTORIC TRANZACTII\n";
    client.printHistory();

    std::cout << "\n>>> INCHIDERE PIATA\n";
    bursa_valori.stopMarket();

    return 0;
}
