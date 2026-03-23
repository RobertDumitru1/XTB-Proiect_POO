#include <chrono>
#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Instrument.h"
#include <nlohmann/json.hpp>

// class FileWatcher {
// private:
//     std::filesystem::path pathWatched = "../companies_data_copy.json";
// public:
//     // FileWatcher(std::string pathWatched) : pathWatched(pathWatched) {};
//     void WatchFile() {
//         std::filesystem::file_time_type last_time(std::filesystem::last_write_time(pathWatched)), currentTime;
//         std::chrono::milliseconds time(1000);
//         int x = 20;
//         while (--x) {
//             currentTime = std::filesystem::last_write_time(pathWatched);
//             if (currentTime != last_time) {
//                 std::cout << "Fisierul a fost modificat\n";
//                 last_time = currentTime;
//             }
//             else {
//                 std::cout << "Fisierul nu a fost modificat\n";
//             }
//             std::this_thread::sleep_for(time);
//         }
//     }
//
// };


using json = nlohmann::json;

// std::ofstream fout("date.json");

std::vector<Instrument*> ParseCompanies() {
    std::ifstream fin("./companies_data_copy.json");
    json companies = json::parse(fin);
    std::vector<Instrument*> companies_data;
    for (auto &company : companies.items()) {
        if (company.value()["type"] == "stock") {
            companies_data.emplace_back(new PhysicalAsset(company.value()["name"], company.key(),company.value()["price"], company.value()["dividents"]));
        }
        else {
            companies_data.emplace_back(new Derivative(company.value()["name"], company.key(), company.value()["price"], company.value()["leverage"], company.value()["swap_fee"]));
        }
    }
    fin.close();
    return companies_data;
}


int main() {
    std::filesystem::path pathWatched = "./companies_data_copy.json";
    std::filesystem::file_time_type last_time(std::filesystem::last_write_time(pathWatched)), currentTime;
    std::chrono::milliseconds time(1000);
    int x = 20;
    while (--x) {
        currentTime = std::filesystem::last_write_time(pathWatched);
        if (currentTime != last_time) {
            // std::cout << "Fisierul a fost modificat\n";
            last_time = currentTime;

            auto vec = ParseCompanies();
            for (auto& w : vec) {
                std::cout << *w << "\n";
            }
            std::cout << "\n\n";
        }
        // else {
        //     std::cout << "Fisierul nu a fost modificat\n";
        // }
        std::this_thread::sleep_for(time);
    }
}

















// #define _WIN32_WINNT 0x0A00
//
// #include <iostream>
// #include <fstream>
// #include <nlohmann/json.hpp>
//
// // #define CPPHTTPLIB_OPENSSL_SUPPORT
// #include "Instrument.h"
// #include "include/httplib.hpp"
//
// using json = nlohmann::json;
//
// std::ifstream fin("../companies_data.json");
// std::ofstream fout("date.json");
//
// std::vector<Instrument*> ParseCompanies() {
//     json companies = json::parse(fin);
//     std::vector<Instrument*> companies_data;
//     for (auto &company : companies.items()) {
//         if (company.value()["type"] == "stock") {
//             companies_data.emplace_back(new PhysicalAsset(company.value()["name"], company.key(),company.value()["price"], company.value()["dividents"]));
//         }
//         else {
//             companies_data.emplace_back(new Derivative(company.value()["name"], company.key(), company.value()["price"], company.value()["leverage"], company.value()["swap_fee"]));
//         }
//     }
// }

// int main() {
//     json companies = json::parse(fin);
//     std::cout<< companies << "\n\n";
//     for (auto &w : companies) {
//         std::cout << w << "\n";
//     }
// }