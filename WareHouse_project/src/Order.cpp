#include "Order.h"
using std::string;
using std::vector;
#include <iostream>
using namespace std;
#include <sstream>

enum class OrderStatus;

Order::Order (int id, int customerId, int distance)
: id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const 
{
    return id; 
}

int Order::getCustomerId() const 
{
    return customerId;
}

void Order::setStatus (OrderStatus status) 
{
    this->status = status;
}

void Order::setCollectorId (int collectorId) 
{
    this->collectorId = collectorId;
}

void Order::setDriverId (int driverId) 
{
    this->driverId = driverId;
}

int Order::getCollectorId () const 
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const 
{
    return status;
}

const string Order::toString() const 
{
    stringstream ss;
    ss << "Order ID: " << this->id << endl;
    ss << "OrderStatus: " << ordStatusToString(status) << endl;  // Convert enum to string
    ss << "CustomerID: " << customerId << endl;
    ss << "Collector ID: " << (collectorId == NO_VOLUNTEER ? "None" : to_string(collectorId)) << endl;
    ss << "Driver ID: " << (driverId == NO_VOLUNTEER ? "None" : to_string(driverId)) << endl;
    return ss.str();
}

// added by us
int Order::getDistance() const 
{
    return distance;
}


// Helper fuction to print OrderStatus as string
string Order::ordStatusToString(OrderStatus ordstat) const 
{
    string output = "no output assigned";
    if (ordstat == OrderStatus::PENDING)
        output = "Pending";
    else if (ordstat == OrderStatus::COLLECTING)
        output = "Collecting";
    else if (ordstat == OrderStatus::DELIVERING)
        output = "Delivering";
    else if (ordstat == OrderStatus::COMPLETED)
        output = "Completed";
    return output;
}

Order *Order::clone() const
{
    return new Order(*this);
}