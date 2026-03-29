//
// Created by dumro on 3/20/2026.
//


#ifndef XTB_PROIECT_POO_INSTRUMENT_H
#define XTB_PROIECT_POO_INSTRUMENT_H

#include <string>
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

    [[nodiscard]] const std::string& getSymbol() const;
    [[nodiscard]] double getPrice() const;
    [[nodiscard]] virtual TipInstrument getTip() const = 0;
    [[nodiscard]] virtual Instrument* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual int getLeverage() const;

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
    friend class Market;
};

class PhysicalAsset : public Instrument {
    double dividend_yield = 0.0;
public:
    PhysicalAsset() = default;
    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield);
    [[nodiscard]]TipInstrument getTip() const override;
    [[nodiscard]]Instrument* clone() const override;
    void print(std::ostream& os) const override;
};

class Derivative : public Instrument {
    int leverage = 1;
    double swap_fee = 0.0;
public:
    Derivative() = default;
    Derivative(const std::string& name, const std::string& symbol, double current_price, int leverage, double swap_fee);
    [[nodiscard]]TipInstrument getTip() const override;
    [[nodiscard]]Instrument* clone() const override;
    [[nodiscard]]int getLeverage() const override;
    void print(std::ostream& os) const override;


};
#endif