#ifndef XTB_PROIECT_POO_CONSTANTS_H
#define XTB_PROIECT_POO_CONSTANTS_H

#include <string>
#include <fstream>

// inline std::ifstream fin_tastatura;

enum Currency { USD, EUR, RON };
enum class TipInstrument { GENERAL, STOCK, DERIVATE, CRYPTO, ETF};
enum TipTranzactie { BUY, SELL };

const std::string nume_monede[] = { "USD", "EUR", "RON" };

#endif //XTB_PROIECT_POO_CONSTANTS_H