#ifndef OOP_ETF_H
#define OOP_ETF_H

#include "Instrument.h"

class ETF : public Instrument {
    double management_fee = 0.0;
    void print(std::ostream& os) const override;

public:
    ETF() = default;
    ETF(const std::string& name, const std::string& symbol, double current_price, double management_fee);
    [[nodiscard]] TipInstrument getTip() const override;
    [[nodiscard]] std::unique_ptr<Instrument> clone() const override;
    double calculateMargin(double quantity) const override;
};


#endif //OOP_ETF_H