#include "Customer.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <iostream>
using namespace std;
#include <sstream>

 // memeber representing the number of orders the customer made.

Customer::Customer (int id, const string &name, int locationDistance, int maxOrders)
: id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(), numOrders(0) {}

const string &Customer::getName() const 
{
    return name;
}

int Customer::getId() const 
{ // returns ID of the customer
    return id;
}

int Customer::getCustomerDistance() const 
{
    return locationDistance;
}

int Customer::getMaxOrders() const 
{
    return maxOrders;
}

int Customer::getNumOrders() const 
{ //Returns num of orders the customer has made so far
    return numOrders;
}

bool Customer::canMakeOrder() const 
{
    return numOrders < maxOrders;
}

const vector<int> &Customer::getOrdersIds() const 
{ 
    return ordersId;
}

int Customer::addOrder(int orderId) 
{ //return OrderId if order was added successfully, -1 otherwise
    if (!canMakeOrder()) 
        return -1;
    ordersId.push_back(orderId);
    numOrders++;
    return orderId;
}


// Expanding Customer class
SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders)
: Customer (id, name, locationDistance, maxOrders) {}

SoldierCustomer *SoldierCustomer::clone() const 
{
    return new SoldierCustomer(*this); // Call the SoldierCustomer copy constructor
}

// Expanding Customer class
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders)
: Customer (id, name, locationDistance, maxOrders) {}

CivilianCustomer *CivilianCustomer::clone() const 
{
    return new CivilianCustomer(*this);
}