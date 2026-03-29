//
// Created by dumro on 3/20/2026.
//
#include "Instrument.h"

const std::string& Instrument::getSymbol() const { return symbol; }

double Instrument::getPrice() const { return current_price; }

Instrument::Instrument(const std::string& name, const std::string& symbol, double current_price)
    : name(name), symbol(symbol), current_price(current_price) {}

void Instrument::print(std::ostream& os) const {
    os << "[" << symbol << "] " << name << " - Pret: " << current_price << "$";
}
int Instrument::getLeverage() const { return -1; };

std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    inst.print(os);
    return os;
}

TipInstrument PhysicalAsset::getTip() const { return TipInstrument::STOCK; }

Instrument* PhysicalAsset::clone() const { return new PhysicalAsset(*this); }

PhysicalAsset::PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield)
    : Instrument(name, symbol, current_price), dividend_yield(dividend_yield) {
    this->tip_instrument = TipInstrument::STOCK;
}

void PhysicalAsset::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Dividend: " << dividend_yield << "%";
}

TipInstrument Derivative::getTip() const { return TipInstrument::DERIVATE; }

Instrument*  Derivative::clone() const { return new Derivative(*this); }

Derivative::Derivative(const std::string& name, const std::string& symbol, double current_price, int leverage, double swap_fee)
    : Instrument(name, symbol, current_price), leverage(leverage), swap_fee(swap_fee) {
    this->tip_instrument = TipInstrument::DERIVATE;
}

int Derivative::getLeverage() const { return this->leverage; };

void Derivative::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Leverage: x" << leverage << " | Swap: " << swap_fee;
}