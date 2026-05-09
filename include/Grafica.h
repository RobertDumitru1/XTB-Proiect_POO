//
// Created by dumro on 5/6/2026.
//

#ifndef OOP_GRAPHICS_H
#define OOP_GRAPHICS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "User.h"
#include "Market.h"

class Screen {
protected:
    sf::RenderWindow& window;
    const sf::Font& font;
public:
    Screen(sf::RenderWindow &window, const sf::Font &font)
        : window(window), font(font) {}

    virtual ~Screen() = default;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
};

class LogInScreen : public Screen {
private:
    sf::RectangleShape userBox, emailBox, submitBox;
    sf::Text userLabel, emailLabel, submitLabel;
    sf::Text userDisplayText, emailDisplayText;

    std::string userS, emailS;
    bool userActive, emailActive;
    bool loginSuccess = false;

public:
    LogInScreen(sf::RenderWindow &window, const sf::Font &font)
        : Screen(window, font),
          userLabel(font, "Username:"),
          emailLabel(font, "Email:"),
          submitLabel(font, "Login"),
          userDisplayText(font),
          emailDisplayText(font),
          userActive(false),
          emailActive(false)
    {
        userBox.setSize({500, 60.f});
        userBox.setOutlineThickness(3);
        userBox.setPosition({150, 100});

        userLabel.setCharacterSize(20);
        userLabel.setPosition({150, 70});

        userDisplayText.setCharacterSize(24);
        userDisplayText.setFillColor(sf::Color::White);
        userDisplayText.setPosition({160, 110});

        emailBox.setSize({500, 60.f});
        emailBox.setOutlineThickness(3);
        emailBox.setPosition({150, 250});

        emailLabel.setCharacterSize(20);
        emailLabel.setPosition({150, 220});

        emailDisplayText.setCharacterSize(24);
        emailDisplayText.setFillColor(sf::Color::White);
        emailDisplayText.setPosition({160, 260});

        submitBox.setSize({200, 60});
        submitBox.setFillColor(sf::Color(50, 50, 200));
        submitBox.setPosition({300, 400});

        submitLabel.setCharacterSize(24);
        sf::FloatRect textBounds = submitLabel.getLocalBounds();
        submitLabel.setOrigin(textBounds.getCenter());
        submitLabel.setPosition(submitBox.getPosition() + (submitBox.getSize() / 2.f));

        userBox.setFillColor(sf::Color(30, 30, 30));
        emailBox.setFillColor(sf::Color(30, 30, 30));
    }

    bool isSuccessful() const {
        return loginSuccess;
    }

    void draw() override {
        window.draw(userBox);
        window.draw(userLabel);
        window.draw(userDisplayText);
        window.draw(emailBox);
        window.draw(emailLabel);
        window.draw(emailDisplayText);
        window.draw(submitBox);
        window.draw(submitLabel);
    }

    void update() override {
        userBox.setOutlineColor(userActive ? sf::Color::Cyan : sf::Color::White);
        emailBox.setOutlineColor(emailActive ? sf::Color::Cyan : sf::Color::White);
        userDisplayText.setString(userS);
        emailDisplayText.setString(emailS);
    }

    void handleEvent(const sf::Event& event) override {
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mouseData = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseData->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = sf::Vector2f(mouseData->position);

                userActive = userBox.getGlobalBounds().contains(mousePos);
                emailActive = emailBox.getGlobalBounds().contains(mousePos);

                if (submitBox.getGlobalBounds().contains(mousePos)) {
                    if (userS == "admin" && emailS == "admin@test.com") {
                        loginSuccess = true;
                    }
                }
            }
        }

        if (const auto* textData = event.getIf<sf::Event::TextEntered>()) {
            std::string* activeBuffer = nullptr;

            if (userActive) activeBuffer = &userS;
            else if (emailActive) activeBuffer = &emailS;

            if (activeBuffer) {
                if (textData->unicode == 8) {
                    if (!activeBuffer->empty()) activeBuffer->pop_back();
                }
                else if (textData->unicode >= 32 && textData->unicode < 128) {
                    if (activeBuffer->length() < 25) {
                        *activeBuffer += static_cast<char>(textData->unicode);
                    }
                }
            }
        }
    }
};

class MainScreen : public Screen {
private:
    User& currentUser;
    Market& market;

    sf::Text balanceText;
    sf::RectangleShape sidebarBackground;

    struct StockItem {
        std::string symbol;
        sf::RectangleShape box;
        sf::Text text;
        bool isHovered = false;

        StockItem(const std::string& sym, const sf::Font& f)
            : symbol(sym), text(f, sym, 20) {}
    };
    std::vector<StockItem> stockList;

    sf::RectangleShape rightPanel;
    sf::Text rightPanelTitle;
    sf::Text rightPanelPrice;
    sf::Text rightPanelPlaceholder;
    std::string selectedStock = "";

public:
    MainScreen(sf::RenderWindow &window, const sf::Font &font, User& user, Market& m)
        : Screen(window, font),
          currentUser(user),
          market(m),
          balanceText(font, "Balanta: $" + std::to_string(user.getAvailableBalance()), 24),
          rightPanelTitle(font, "", 36),
          rightPanelPrice(font, "", 28),
          rightPanelPlaceholder(font, "Selecteaza o actiune din stanga pentru a vizualiza detalii.", 20)
    {
        balanceText.setPosition({520, 20});
        balanceText.setFillColor(sf::Color(46, 204, 113));

        sidebarBackground.setSize({250, 800});
        sidebarBackground.setFillColor(sf::Color(20, 20, 25));

        float yPos = 80.f;

        for (const auto* inst : market.getInstruments()) {
            StockItem item(inst->getSymbol(), font);

            item.box.setSize({250, 60});
            item.box.setPosition({0, yPos});
            item.box.setFillColor(sf::Color(30, 30, 35));
            item.box.setOutlineThickness(1);
            item.box.setOutlineColor(sf::Color(40, 40, 45));

            item.text.setPosition({20, yPos + 16});
            item.text.setFillColor(sf::Color::White);

            stockList.push_back(item);
            yPos += 60.f;
        }

        rightPanel.setSize({800, 800});
        rightPanel.setPosition({250, 0});
        rightPanel.setFillColor(sf::Color(30, 30, 35));

        rightPanelTitle.setPosition({280, 80});
        rightPanelTitle.setFillColor(sf::Color::White);

        rightPanelPrice.setPosition({280, 130});
        rightPanelPrice.setFillColor(sf::Color(46, 204, 113));

        rightPanelPlaceholder.setPosition({300, 300});
        rightPanelPlaceholder.setFillColor(sf::Color(150, 150, 150));
    }

    void draw() override {
        window.draw(sidebarBackground);
        window.draw(rightPanel);

        for (const auto& item : stockList) {
            window.draw(item.box);
            window.draw(item.text);
        }

        window.draw(balanceText);

        if (selectedStock.empty()) {
            window.draw(rightPanelPlaceholder);
        } else {
            window.draw(rightPanelTitle);
            window.draw(rightPanelPrice);
        }
    }

    void update() override {
        balanceText.setString("Balanta: $" + std::to_string(currentUser.getAvailableBalance()));

        if (!selectedStock.empty()) {
            Instrument* inst = market.findInstrument(selectedStock);
            if (inst) {
                rightPanelPrice.setString("Pret curent: $" + std::to_string(inst->getPrice()));
            }
        }

        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePosWindow);

        for (auto& item : stockList) {
            bool isHovered = item.box.getGlobalBounds().contains(mousePos);

            if (item.symbol == selectedStock) {
                item.box.setFillColor(sf::Color(50, 50, 150));
            } else if (isHovered) {
                item.box.setFillColor(sf::Color(60, 60, 70));
            } else {
                item.box.setFillColor(sf::Color(30, 30, 35));
            }
        }
    }

    void handleEvent(const sf::Event& event) override {
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mouseData = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseData->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = sf::Vector2f(mouseData->position);

                for (const auto& item : stockList) {
                    if (item.box.getGlobalBounds().contains(mousePos)) {
                        selectedStock = item.symbol;
                        rightPanelTitle.setString("Tranzactionare: " + selectedStock);
                        break;
                    }
                }
            }
        }
    }
};

#endif //OOP_GRAPHICS_H