//
// Created by dumro on 3/20/2026.
//


#ifndef XTB_PROIECT_POO_INSTRUMENT_H
#define XTB_PROIECT_POO_INSTRUMENT_H

#include <string>
#include <memory>
#include "Constants.h"

class Instrument {
protected:
    std::string name = "";
    std::string symbol = "";
    double current_price = 0.0;
    TipInstrument tip_instrument = TipInstrument::GENERAL;

    virtual void print(std::ostream& os) const = 0;
public:
    Instrument() = default;
    Instrument(const std::string& name, const std::string& symbol, double current_price);
    virtual ~Instrument() = default;

    [[nodiscard]] const std::string& getSymbol() const;
    [[nodiscard]] double getPrice() const;
    [[nodiscard]] virtual TipInstrument getTip() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Instrument> clone() const = 0;
    virtual double calculateMargin(double quantity) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Instrument& inst);
    friend class Market;

    void display(std::ostream& os) const;
};

class PhysicalAsset : public Instrument {
    double dividend_yield = 0.0;
public:
    PhysicalAsset() = default;
    PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield);
    [[nodiscard]]TipInstrument getTip() const override;
    [[nodiscard]]std::unique_ptr<Instrument>  clone() const override;
    void print(std::ostream& os) const override;
    double calculateMargin(double quantity) const override;
};

class Derivative : public Instrument {
    int leverage = 1;
    double swap_fee = 0.0;
public:
    Derivative() = default;
    Derivative(const std::string& name, const std::string& symbol, double current_price, int leverage, double swap_fee);
    [[nodiscard]]TipInstrument getTip() const override;
    [[nodiscard]]std::unique_ptr<Instrument>  clone() const override;
    [[nodiscard]]int getLeverage() const;
    void print(std::ostream& os) const override;

    double calculateMargin(double quantity) const override;
};

class CryptoAsset : public Instrument {
    double network_fee = 0.0;
    bool is_staked = false;
public:
    CryptoAsset() = default;
    CryptoAsset(const std::string& name, const std::string& symbol, double current_price, double network_fee, bool is_staked);

    [[nodiscard]] TipInstrument getTip() const override;
    [[nodiscard]] std::unique_ptr<Instrument> clone() const override;
    void print(std::ostream& os) const override;
    double calculateMargin(double quantity) const override;
};
#endif