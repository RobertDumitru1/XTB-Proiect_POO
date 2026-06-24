#ifndef OOP_OBSERVER_H
#define OOP_OBSERVER_H

class Observer {
public:
    virtual void onPricesUpdated() = 0;
    virtual ~Observer() = default;
};

#endif //OOP_OBSERVER_H