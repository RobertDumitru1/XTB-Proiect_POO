#include "ConsoleApp.h"
#include "Instrument.h"
#include "Trading.h"
#include "InstrumentFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

ConsoleApp::ConsoleApp() : is_running(true), market(nullptr), user(nullptr) {
    std::vector<std::shared_ptr<Instrument>> insts;

    std::ifstream file("companies_data_copy.json");
    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        if (j.is_object()) {
            for (auto& [symbol, data] : j.items()) {
                auto inst = InstrumentFactory::createFromJson(symbol, data);
                if (inst != nullptr) {
                    insts.push_back(inst);
                }
            }
        }
    }

    market = std::make_shared<Market>(insts);
    market->addObserver(this);
    user = std::make_unique<User>("Trader", "12345", "pass", 10000.0, 0.0, market);
    market->startMarket();
}

ConsoleApp::~ConsoleApp() {
    if (market != nullptr) {
        market->stopMarket();
    }
}

void ConsoleApp::onPricesUpdated() {
    printUI();
}

void ConsoleApp::printUI() {
    std::cout << "\n=========================================\n";
    std::cout << "          PLATFORMA DE TRADING (LIVE)\n";
    std::cout << "=========================================\n";
    if (user) {
        std::cout << "Balanta Disponibila: " << user->getAvailableBalance() << " USD\n";
    } else {
        std::cout << "Utilizator neinitializat.\n";
    }
    std::cout << "Pozitii globale active in piata: " << Position::getPositionsCreated() << "\n";
    std::cout << "-----------------------------------------\n";

    if (market != nullptr) {
        std::cout << *market;
    }
    std::cout << "-----------------------------------------\n";

    if (user != nullptr) {
        std::cout << user->getPortfolio();
        printLatestRecords(user->getHistory(), 5);
    }
    std::cout << "-----------------------------------------\n";
}

void ConsoleApp::processCommand(const std::string& commandLine) {
    if (commandLine.empty()) return;

    std::stringstream ss(commandLine);
    std::string actiune;
    ss >> actiune;

    if (actiune == "exit") {
        is_running = false;
    } else if (actiune == "refresh") {
        printUI();
    } else if (actiune == "buy") {
        std::string symbol;
        double cantitate;
        if (ss >> symbol >> cantitate) {
            try {
                user->buyAsset(symbol, cantitate);
            } catch (const std::exception& e) {
                std::cout << "[EROARE TRANZACTIE] " << e.what() << "\n";
            }
        } else {
            std::cout << "[EROARE] Format invalid. Foloseste: buy <simbol> <cantitate>\n";
        }
    } else if (actiune == "sell") {
        int id;
        if (ss >> id) {
            try {
                user->sellPosition(id);
            } catch (const std::exception& e) {
                std::cout << "[EROARE TRANZACTIE] " << e.what() << "\n";
            }
        } else {
            std::cout << "[EROARE] Format invalid. Foloseste: sell <ID>\n";
        }
    } else {
        std::cout << "[SISTEM] Comanda necunoscuta: " << actiune << "\n";
    }
}

void ConsoleApp::run() {
    printUI();

    std::ifstream file("tastatura.txt");
    std::string command;

    while (is_running && std::getline(file, command)) {
        processCommand(command);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}