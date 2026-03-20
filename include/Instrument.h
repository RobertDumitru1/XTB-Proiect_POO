//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_INSTRUMENT_H
#define XTB_PROIECT_POO_INSTRUMENT_H

#include <string>
#include <iostream>
#include "Constants.h"

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

inline std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    os << "[" << inst.symbol << "] " << inst.name << " - Pret: " << inst.current_price << "$";
    return os;
}

#endif //XTB_PROIECT_POO_INSTRUMENT_H