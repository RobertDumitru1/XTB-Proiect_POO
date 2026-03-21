#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Constants.h"
#include "Instrument.h"
#include "Market.h"
#include "Trading.h"
#include "User.h"

void printSeparator(const std::string& title = "") {
    if (title.empty()) {
        std::cout << std::string(80, '-') << "\n";
    } else {
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << ">>> " << title << " <<<\n";
        std::cout << std::string(80, '=') << "\n";
    }
}

int main() {
    // fin_tastatura.open("tastatura.txt");

    printSeparator("START SIMULARE PLATFORMA BROKERAJ XTB");
    std::cout << "[SYSTEM] Initializare sistem de trading online\n";

    printSeparator("INITIALIZARE PIATA SI INSTRUMENTE FINANCIARE");
    std::vector<Instrument*> instruments;

    instruments.push_back(new PhysicalAsset("Apple Inc.", "AAPL", 175.50, 0.5));
    instruments.push_back(new PhysicalAsset("Microsoft Corp.", "MSFT", 405.20, 0.8));
    instruments.push_back(new PhysicalAsset("NVIDIA Corporation", "NVDA", 850.00, 0.04));
    instruments.push_back(new PhysicalAsset("Tesla Inc.", "TSLA", 195.30, 0.0));
    instruments.push_back(new PhysicalAsset("Amazon.com Inc.", "AMZN", 178.10, 0.0));
    instruments.push_back(new PhysicalAsset("Banca Transilvania", "TLV.RO", 28.50, 5.2));
    instruments.push_back(new PhysicalAsset("OMV Petrom", "SNP.RO", 0.65, 8.5));
    instruments.push_back(new PhysicalAsset("Hidroelectrica", "H2O.RO", 125.00, 6.1));
    instruments.push_back(new PhysicalAsset("Romgaz", "SNG.RO", 55.20, 7.3));

    instruments.push_back(new Derivative("Euro / US Dollar", "EURUSD", 1.0850, 30, -0.005));
    instruments.push_back(new Derivative("British Pound / US Dollar", "GBPUSD", 1.2640, 30, -0.006));
    instruments.push_back(new Derivative("US Dollar / Japanese Yen", "USDJPY", 151.20, 30, 0.002));
    instruments.push_back(new Derivative("Bitcoin / US Dollar", "BTCUSD", 67000.00, 2, -1.5));
    instruments.push_back(new Derivative("Ethereum / US Dollar", "ETHUSD", 3500.00, 2, -0.8));
    instruments.push_back(new Derivative("Solana / US Dollar", "SOLUSD", 185.50, 2, -0.2));
    instruments.push_back(new Derivative("Gold Ounce / US Dollar", "XAUUSD", 2150.00, 20, -1.2));
    instruments.push_back(new Derivative("Crude Oil (WTI)", "OIL.WTI", 82.30, 10, -0.5));
    instruments.push_back(new Derivative("US 500 Index (S&P500)", "US500", 5200.50, 20, -2.5));
    instruments.push_back(new Derivative("Germany 40 Index (DAX)", "DE40", 18200.00, 20, -3.0));

    Market xtb_market(instruments);
    std::cout << "[MARKET ENGINE] S-au incarcat " << instruments.size() << " instrumente financiare.\n";
    xtb_market.startMarket();
    std::cout << "[MARKET ENGINE] Thread-ul de actualizare a preturilor a pornit cu succes.\n";

    printSeparator("LIVE TICKER: Urmarim fluctuatia pietei inainte de deschidere");
    std::cout << "[SYSTEM] Afisam preturile in timp real timp de 5 secunde\n";

    for (int i = 1; i <= 5; ++i) {
        std::cout << "\n[TICKER] Secunda " << i << " / 5:\n";
        std::cout << xtb_market << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[SYSTEM] Ticker live incheiat. Preturile vor continua sa se modifice in fundal.\n";

    printSeparator("CREARE SI AUTENTIFICARE UTILIZATOR");

    Portfolio portofoliu_curent;
    std::vector<Transaction> istoric_curent;

    User trader1("Ion Popescu", "1950505123456", "Parola123", USD, 50000.0, 0.0, portofoliu_curent, istoric_curent, &xtb_market);

    std::cout << "[LOGIN] Autentificare reusita. Bun venit, Ion!\n";
    std::cout << trader1 << "\n";

    printSeparator("SESIUNE DE TRADING:");
    std::cout << "[TRADER] Se analizeaza piata si se plaseaza primele ordine\n\n";

    trader1.buyAsset("AAPL", 50.0);
    trader1.buyAsset("NVDA", 10.0);
    trader1.buyAsset("TLV.RO", 1000.0);
    trader1.buyAsset("H2O.RO", 200.0);

    trader1.buyAsset("EURUSD", 2.5);
    trader1.buyAsset("XAUUSD", 5.0);
    trader1.buyAsset("BTCUSD", 0.5);
    trader1.buyAsset("OIL.WTI", 15.0);
    trader1.buyAsset("GBPUSD", 2.0);

    std::cout << "\n[TRADER] Portofoliul dupa primele ordine:\n";
    std::cout << trader1 << "\n";

    std::cout << "[SYSTEM] >> Asteptam 4 secunde pentru volatilitatea pietei (simulare timp) <<\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "[TRADER] Preturile au fluctuat. Se analizeaza inchiderea unor pozitii.\n\n";
    std::cout << "[ORDIN] Se trimite cerere de inchidere pentru pozitia #5 (EURUSD)\n";
    trader1.sellPosition(5);

    std::cout << "[ORDIN] Se trimite cerere de inchidere pentru pozitia #6 \n";
    trader1.sellPosition(6);

    std::cout << "\n[TRADER] Sumar:\n";
    std::cout << trader1 << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    trader1.buyAsset("US500", 10.0);
    trader1.buyAsset("SOLUSD", 100.0);
    trader1.buyAsset("DE40", 5.0);

    std::cout << "[ORDIN] Se trimite cerere de vanzare totala pentru pozitia #2 (NVDA)\n";
    trader1.sellPosition(2);

    printSeparator("6. INCHIDEREA ZILEI DE TRANZACTIONARE");
    std::cout << "[TRADER] Se inchid toate pozitiile ramase pentru a securiza fondurile.\n\n";

    trader1.sellPosition(1);
    trader1.sellPosition(3);
    trader1.sellPosition(4);
    trader1.sellPosition(7);
    trader1.sellPosition(8);
    trader1.sellPosition(9);
    trader1.sellPosition(10);
    trader1.sellPosition(11);
    trader1.sellPosition(12);

    std::cout << "\n[TEST VALIDARE] Incercare vanzare pozitie deja inchisa (ID 2):\n";
    trader1.sellPosition(2);

    printSeparator("7. RAPORT FINAL SI EXTRAS DE CONT");

    std::cout << trader1 << "\n";
    printSeparator();

    std::cout << "[RAPORT TRANZACTII] Extras istoric operatiuni:\n";
    trader1.printHistory();
    printSeparator();

    std::cout << "\n[SYSTEM] Se initiaza oprirea sistemului\n";

    xtb_market.stopMarket();
    std::cout << "[MARKET ENGINE] Piata a fost inchisa (Closed). Thread-ul oprit.\n";

    for (Instrument* inst : instruments) {
        delete inst;
    }
    instruments.clear();
    std::cout << "[MEMORY] Memoria alocata instrumentelor a fost eliberata cu succes.\n";

    printSeparator("SIMULARE INCHEIATA CU SUCCES (EXIT CODE 0)");
    return 0;
}