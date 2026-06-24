#ifndef OOP_CONSOLEAPP_H
#define OOP_CONSOLEAPP_H

#include <memory>
#include <string>
#include "User.h"
#include "Market.h"
#include "Observer.h"

class ConsoleApp : public Observer {
private:
    bool is_running;
    std::shared_ptr<Market> market;
    std::unique_ptr<User> user;

    void printUI();
    void processCommand(const std::string& commandLine);

public:
    ConsoleApp();
    ~ConsoleApp() override;

    void onPricesUpdated() override;
    void run();
};

#endif //OOP_CONSOLEAPP_H