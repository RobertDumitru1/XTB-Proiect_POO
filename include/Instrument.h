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
    Instrument(const std::string& name, const std::string& symbol, double current_price);
    virtual ~Instrument() = default;

    std::string getSymbol() const;
    double getPrice() const;
    virtual TipInstrument getTip() const = 0;
    virtual Instrument* clone() const = 0;
    virtual void print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
    friend class Market;
};

class PhysicalAsset : public Instrument {
    double dividend_yield = 0.0;
public:
    PhysicalAsset() = default;
    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield);
    TipInstrument getTip() const override;
    Instrument* clone() const override;
    void print(std::ostream& os) const override;
};

class Derivative : public Instrument {
    int max_leverage = 1;
    double swap_fee = 0.0;
public:
    Derivative() = default;
    Derivative(const std::string& name, const std::string& symbol, double current_price, int max_leverage, double swap_fee);
    TipInstrument getTip() const override;
    Instrument* clone() const override;
    void print(std::ostream& os) const override;
};

inline std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    inst.print(os);
    return os;
}
#endif