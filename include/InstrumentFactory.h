#ifndef OOP_INSTRUMENTFACTORY_H
#define OOP_INSTRUMENTFACTORY_H

#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "Instrument.h"

class InstrumentFactory {
public:
    static std::shared_ptr<Instrument> createFromJson(const std::string& symbol, const nlohmann::json& data);
};

#endif //OOP_INSTRUMENTFACTORY_H