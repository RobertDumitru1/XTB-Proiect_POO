#include "Instrument.h"

const std::string& Instrument::getSymbol() const { return symbol; }

double Instrument::getPrice() const { return current_price; }

Instrument::Instrument(const std::string& name, const std::string& symbol, double current_price)
    : name(name), symbol(symbol), current_price(current_price) {}

void Instrument::print(std::ostream& os) const {
    os << "[" << symbol << "] " << name << " - Pret: " << current_price << "$";
}
std::ostream& operator<<(std::ostream& os, const Instrument& inst) {
    inst.display(os);
    return os;
}

TipInstrument PhysicalAsset::getTip() const { return TipInstrument::STOCK; }

std::unique_ptr<Instrument> PhysicalAsset::clone() const {
    return std::make_unique<PhysicalAsset>(*this);
}
PhysicalAsset::PhysicalAsset(const std::string& name, const std::string& symbol, double current_price, double dividend_yield)
    : Instrument(name, symbol, current_price), dividend_yield(dividend_yield) {
    this->tip_instrument = TipInstrument::STOCK;
}

void PhysicalAsset::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Dividend: " << dividend_yield << "%";
}

TipInstrument Derivative::getTip() const { return TipInstrument::DERIVATE; }

std::unique_ptr<Instrument> Derivative::clone() const {
    return std::make_unique<Derivative>(*this);
}
Derivative::Derivative(const std::string& name, const std::string& symbol, double current_price, int leverage, double swap_fee)
    : Instrument(name, symbol, current_price), leverage(leverage), swap_fee(swap_fee) {
    this->tip_instrument = TipInstrument::DERIVATE;
}

int Derivative::getLeverage() const { return this->leverage; };

void Derivative::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Leverage: x" << leverage << " | Swap: " << swap_fee;
}

double PhysicalAsset::calculateMargin(double quantity) const {
    return this->current_price * quantity;
}

double Derivative::calculateMargin(double quantity) const {
    return (this->current_price * quantity) / this->leverage;
}

TipInstrument CryptoAsset::getTip() const {
    return TipInstrument::CRYPTO;
}

std::unique_ptr<Instrument> CryptoAsset::clone() const {
    return std::make_unique<CryptoAsset>(*this);
}

CryptoAsset::CryptoAsset(const std::string& name, const std::string& symbol, double current_price, double network_fee, bool is_staked)
    : Instrument(name, symbol, current_price), network_fee(network_fee), is_staked(is_staked) {
    this->tip_instrument = TipInstrument::CRYPTO;
}

void CryptoAsset::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Taxa retea: " << network_fee << "$ | Staking: " << (is_staked ? "Da" : "Nu");
}

double CryptoAsset::calculateMargin(double quantity) const {
    return (this->current_price * quantity) + network_fee;
}


void Instrument::display(std::ostream &os) const {
    print(os);
}
