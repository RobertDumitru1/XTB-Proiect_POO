//
// Created by dumro on 3/20/2026.
//

#ifndef XTB_PROIECT_POO_CONSTANTS_H
#define XTB_PROIECT_POO_CONSTANTS_H

#include <string>
#include <fstream>

// inline std::ifstream fin_tastatura;

enum Currency { USD, EUR, RON };
enum TipInstrument { GENERAL, STOCK, DERIVATE, CRYPTO};
enum TipTranzactie { BUY, SELL };

const std::string nume_monede[] = { "USD", "EUR", "RON" };

#endif //XTB_PROIECT_POO_CONSTANTS_H