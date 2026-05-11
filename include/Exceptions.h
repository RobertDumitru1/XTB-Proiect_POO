//
// Created by dumro on 5/9/2026.
//

#ifndef TRADING_EXCEPTIONS_HPP
#define TRADING_EXCEPTIONS_HPP

#include <exception>
#include <string>

class TradingException : public std::exception {
protected:
    std::string message;
public:
    explicit TradingException(const std::string& msg);
    [[nodiscard]] const char* what() const noexcept override;
    
    virtual ~TradingException() = default;
};

class InstrumentNotFoundException : public TradingException {
public:
    explicit InstrumentNotFoundException(const std::string& symbol);
};

class InsufficientFundsException : public TradingException {
public:
    InsufficientFundsException(double required, double available);
};

class InvalidQuantityException : public TradingException {
public:
    explicit InvalidQuantityException(double quantity);
};

#endif // TRADING_EXCEPTIONS_HPP