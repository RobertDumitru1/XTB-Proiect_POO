#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <thread>

#include "include/Instrument.h"
#include "include/Market.h"
#include "include/Trading.h"
#include "include/User.h"
#include "include/Constants.h"

std::ifstream fin_tastatura("tastatura.txt");

int main() {
    std::cout << ">>> INITIALIZARE PIATA\n";
    std::vector<Instrument*> de_adaugat;
    de_adaugat.push_back(new PhysicalAsset("Apple Inc.", "AAPL", 150.0, 2.5));
    de_adaugat.push_back(new PhysicalAsset("Tesla Inc.", "TSLA", 240.0, 0.0));
    de_adaugat.push_back(new Derivative("Bitcoin Perpetual", "BTC-PERP", 60000.0, 10, 0.01));

    Market bursa_valori(de_adaugat);

    for (auto* i : de_adaugat) delete i;
    de_adaugat.clear();

    std::cout << "Deschidem piata principala...\n";
    bursa_valori.startMarket();

    std::cout << "\n>>> OBSERVAM FLUCTUATIILE PIETEI TIMP DE 10 SECUNDE\n";
    for (int i = 0; i <= 10; ++i) {
        std::cout << "--- Secunda " << i << " ---\n";
        std::cout << bursa_valori << "\n";

        if (i < 10) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    std::cout << "-------------------------------------------------------\n\n";

    std::cout << ">>> CREARE UTILIZATOR\n";
    Portfolio portofoliu_initial;
    std::vector<Transaction> istoric_initial;
    User client("Andrei Ionescu", "5010101123456", "parola123", USD,
                10000.0, // Balanta disponibila
                0.0,     // Balanta investita
                portofoliu_initial, istoric_initial, &bursa_valori);

    std::cout << client << "\n";

    std::cout << ">>> TESTARE ACHIZITII (BUY)\n";
    client.buyAsset("AAPL", 10.0);
    client.buyAsset("BTC-PERP", 0.5);
    client.buyAsset("FAKECOIN", 100);
    client.buyAsset("TSLA", 10000.0);

    std::cout << "\n>>> STARE DUPA ACHIZITII:\n" << client << "\n";

    std::cout << ">>> ASTEPTAM FLUCTUATII DE PRET\n";
    for (int i = 1; i <= 2; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Update preturi\n" << bursa_valori << "\n";
    }

    std::cout << ">>> TESTARE VANZARE \n";
    std::cout << "Incercam sa vindem AAPL (ID 1)\n";
    client.sellPosition(1);

    std::cout << "Incercam sa vindem o pozitie inexistenta (ID 99)\n";
    client.sellPosition(99);

    std::cout << "\n>>> STARE FINALA CLIENT:\n" << client << "\n";

    std::cout << ">>> VERIFICARE ISTORIC TRANZACTII\n";
    client.printHistory();

    std::cout << "\n>>> INCHIDERE PIATA\n";
    bursa_valori.stopMarket();

    return 0;
}
