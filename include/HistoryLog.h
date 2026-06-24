#ifndef OOP_HISTORYLOG_H
#define OOP_HISTORYLOG_H

#include <vector>
#include <iostream>

template <typename T>
class HistoryLog {
private:
    std::vector<T> records;
public:
    HistoryLog() = default;

    void addRecord(const T& record) {
        records.push_back(record);
    }

    const std::vector<T>& getRecords() const {
        return records;
    }

    void clearLog() {
        records.clear();
    }

    [[nodiscard]] size_t size() const {
        return records.size();
    }
};

template <typename Container>
void printLatestRecords(const Container& logContainer, size_t count) {
    const auto& records = logContainer.getRecords();
    if (records.size() == 0) return;
    std::cout << "--- Afisare ultimele inregistrari ---\n";
    size_t start = (records.size() > count) ? (records.size() - count) : 0;
    
    for (size_t i = start; i < records.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << records[i] << "\n";
    }
}

#endif //OOP_HISTORYLOG_H