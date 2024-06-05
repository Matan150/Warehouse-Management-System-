#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        void cleanUp();
        bool customerExists(int id) const;
        Customer &getCustomer(int customerId) const;
        bool volunteerExists(int id) const;
        Volunteer &getVolunteer(int volunteerId) const;
        bool orderExists(int id) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        const vector<Order*> &getPending() const;
        const vector<Order*> &getProgress() const;
        const vector<Order*> &getCompleted() const;
        void close();
        void open();
        bool isNumber(string& num);
        void setIsBeckup();
        const int getCustomerCounter() const;
        const int getVolunteerCounter() const;
        const int getOrderCounter() const;
        void step();
        bool getIsBeckup() const;
        bool canContinue(Order& order, string type);

        
        template<typename T>
        vector<T*> copyVectors(const vector<T*>& source);
        ~WareHouse();
        WareHouse(const WareHouse& other);
        WareHouse& operator=(const WareHouse& other);
        WareHouse(WareHouse&& other) noexcept;
        WareHouse& operator=(WareHouse&& other) noexcept;





    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;
        bool isBackup;
        
};

