//
// Created by dumro on 5/12/2026.
//

#include "ETF.h"

ETF::ETF(const std::string& name, const std::string& symbol, double current_price, double management_fee)
    : Instrument(name, symbol, current_price), management_fee(management_fee) {
    this->tip_instrument = TipInstrument::ETF;
}

void ETF::print(std::ostream& os) const {
    Instrument::print(os);
    os << " | Comision administrare: " << management_fee << "%";
}

TipInstrument ETF::getTip() const {
    return TipInstrument::ETF;
}

std::unique_ptr<Instrument> ETF::clone() const {
    return std::make_unique<ETF>(*this);
}

double ETF::calculateMargin(double quantity) const {
    double base_margin = this->current_price * quantity;
    return base_margin + (base_margin * management_fee / 100.0);
}