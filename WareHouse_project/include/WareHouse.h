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

        WareHouse(const WareHouse &other); // copy constructor
        WareHouse &operator=(const WareHouse &other); // copy assignment
        WareHouse(WareHouse &&other); // move constructor
        WareHouse &operator=(WareHouse &&other); // move assignment
        ~WareHouse(); // destructor

        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        
        
        // function added by us:
        vector<Volunteer*> getVolVector() const;
        vector<Order*> getPendingVector() const;
        vector<Order*> getinProcVector() const;
        vector<Order*> getCompletedVector() const;
        vector<Customer*> getCusVector() const;
        int getCustomerCount() const;
        int getVolunteerCount() const;
        int getOrderCount() const; 
        void addSolCustomer(SoldierCustomer* customer);
        void addCivCustomer(CivilianCustomer* customer);
        vector<vector<string>> parseText(const string &configFilePath);
        void startHelper(vector<string> vec);
        void moveOrder(Order* ord, int id); // gets an order, and transfer to relevant status and vector
        void step234(); // Helper function for steps 2-4 in one iteration


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
        int orderCounter; // added by us, counting the orders added
};