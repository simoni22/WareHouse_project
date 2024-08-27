#include "Volunteer.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <iostream>
using namespace std;
#include <sstream>
#define NO_ORDER -1

Volunteer::Volunteer (int id, const string &name)
: completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const 
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const 
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const 
{
    return completedOrderId;
}

bool Volunteer::isBusy() const 
{
    return activeOrderId != -1;
}

void Volunteer::setCompletedOrder() 
{
    completedOrderId = NO_ORDER;
}


// Class of CollectorVolunteer
CollectorVolunteer::CollectorVolunteer (int id, string name, int coolDown)
: Volunteer(id, name), coolDown(coolDown), timeLeft(0) {} 

CollectorVolunteer *CollectorVolunteer::clone() const 
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() 
{
    decreaseCoolDown();
    if (getTimeLeft() == 0) 
    {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const 
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const 
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() 
{
    timeLeft--;
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const 
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getStatus() == OrderStatus::PENDING);
}

void CollectorVolunteer::acceptOrder(const Order &order) 
{
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const 
{
    stringstream ss;
    string s = "False";
    if (isBusy())
        s = "True";
    ss << "VolunteerID: " << getId() << endl;
    ss << "isBusy: " << s << endl;
    ss << "OrderID: " << (activeOrderId == NO_ORDER ? "None" : to_string(activeOrderId)) << endl;
    ss << "TimeLeft: " << (activeOrderId == NO_ORDER ? "None" : to_string(timeLeft)) << endl;
    ss << "OrdersLeft: No Limit"  << endl;
    return ss.str();
}


// Class of LimitedCollectorId
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders)
: CollectorVolunteer (id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone () const 
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const 
{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const 
{
    if (hasOrdersLeft() && !isBusy() && order.getStatus() == OrderStatus::PENDING)
        return true;
    return false;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) 
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const 
{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const 
{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const 
{
    stringstream ss;
    string s = "False";
    if (isBusy())
        s = "True";
    ss << "VolunteerID: " << getId() << endl;
    ss << "isBusy: " << s << endl;
    ss << "OrderID: " << (activeOrderId == NO_ORDER ? "None" : to_string(activeOrderId)) << endl;
    ss << "TimeLeft: " << (activeOrderId == NO_ORDER ? "None" : to_string(getTimeLeft())) << endl;
    ss << "OrdersLeft: " << ordersLeft << endl;
    return ss.str();
}


// Class of DriverVolunteer
DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
: Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const 
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const 
{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const 
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const 
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() 
{
    distanceLeft -= distancePerStep;
    if (distanceLeft <= 0)
    {
        distanceLeft = 0;
        return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const 
{
    return true; // not limited
}

bool DriverVolunteer::canTakeOrder(const Order &order) const 
{
    return (!isBusy() && order.getDistance() <= getMaxDistance() && order.getStatus() == OrderStatus::COLLECTING);
}

void DriverVolunteer::acceptOrder(const Order &order) 
{
    distanceLeft = order.getDistance();
    activeOrderId = order.getId();
}

void DriverVolunteer::step() 
{ 
    decreaseDistanceLeft();
    if (getDistanceLeft() == 0) 
    {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const 
{
    stringstream ss;
    string s = "False";
    if (isBusy())
        s = "True";
    ss << "VolunteerID: " << getId() << endl;
    ss << "isBusy: " << s << endl;
    ss << "OrderID: " << (activeOrderId == NO_ORDER ? "None" : to_string(activeOrderId)) << endl;
    ss << "DistanceLeft: " << (activeOrderId == NO_ORDER ? "None" : to_string(distanceLeft)) << endl;
    ss << "OrdersLeft: No Limit" << endl;
    return ss.str();
}


// Class of LimitedDriverVolunteer:
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) 
: DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const 
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const 
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const 
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const 
{
    return (ordersLeft > 0);
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const 
{ 
    return (DriverVolunteer::canTakeOrder(order) && ordersLeft > 0);
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) 
{
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const 
{
    stringstream ss;
    string s = "False";
    if (isBusy())
        s = "True";
    ss << "VolunteerID: " << getId() << endl;
    ss << "isBusy: " << s << endl;
    ss << "OrderID: " << (activeOrderId == NO_ORDER ? "None" : to_string(activeOrderId)) << endl;
    ss << "DistanceLeft: " << (activeOrderId == NO_ORDER ? "None" : to_string(getDistanceLeft())) << endl;
    ss << "OrdersLeft: " << ordersLeft << endl;
    return ss.str();
}