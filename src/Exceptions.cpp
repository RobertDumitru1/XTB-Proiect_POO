//
// Created by dumro on 5/9/2026.
//
#include "Exceptions.h"

TradingException::TradingException(const std::string& msg) : message(msg) {}

const char* TradingException::what() const noexcept {
    return message.c_str();
}

InstrumentNotFoundException::InstrumentNotFoundException(const std::string& symbol)
    : TradingException("Eroare Market: Instrumentul cu simbolul '" + symbol + "' nu a fost gasit.") {}

InsufficientFundsException::InsufficientFundsException(double required, double available)
    : TradingException("Eroare Cont: Fonduri insuficiente pentru tranzactie. Necesar: " +
                       std::to_string(required) + ", Disponibil: " + std::to_string(available) + ".") {}

InvalidQuantityException::InvalidQuantityException(double quantity)
    : TradingException("Eroare Validare: Cantitatea introdusa (" + std::to_string(quantity) +
                       ") este invalida. Trebuie sa fie un numar strict pozitiv.") {}
