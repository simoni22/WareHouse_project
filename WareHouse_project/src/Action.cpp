#include "Action.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <iostream>
using namespace std;
#include <sstream>
using std::iterator;
#include "Volunteer.h"
#include "Customer.h"
#include "WareHouse.h"
#include <algorithm>

enum class ActionStatus;
enum class CustomerType;
enum class OrderStatus;

extern WareHouse *backup; // backup = nullptr 

// constructor
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {} 

ActionStatus BaseAction::getStatus() const 
{
    return status;
}

string BaseAction::statusToString(ActionStatus status) const 
{
    string output;
    if (status == ActionStatus::COMPLETED)
        output = "COMPLETED";
    else 
        output = "ERROR";
    return output;
}

CustomerType BaseAction::stringToCusType(string cusType) const 
{ // added by us
    CustomerType output;
    if (cusType == "soldier")
        output = CustomerType::Soldier;
    else 
        output = CustomerType::Civilian;
    return output;
}

string BaseAction::ordStatusToString(OrderStatus ordstat) const 
{
    string output;
    if (ordstat == OrderStatus::PENDING)
        output = "Pending";
    else if (ordstat == OrderStatus::COLLECTING)
        output = "Collecting";
    else if (ordstat == OrderStatus::DELIVERING)
        output = "Delivering";
    else
        output = "Completed";
    return output;
}

// Helper function: for PrintCustomersId method:
string BaseAction::PrintCustStatHelper (WareHouse &wareHouse, int cust) 
{
    stringstream ss;
    ss << "CustomerID: " << cust << endl;
    // go through all orders vector of specific customer, and print their id and status
    vector<int> temp (wareHouse.getCustomer(cust).getOrdersIds());
    int size = temp.size();
    for (int i = 0; i < size; i++) 
    {
        ss << "OrderID: " << temp.at(i) << endl;
        Order currOrder (wareHouse.getOrder(temp.at(i)));
        ss << "OrderStatus: " << ordStatusToString(currOrder.getStatus()) << endl;
    }
    int numordersLeft = wareHouse.getCustomer(cust).getMaxOrders() - wareHouse.getCustomer(cust).getNumOrders();
    ss << "numOrdersLeft: " << numordersLeft << endl;
    return ss.str();
}

void BaseAction::complete() 
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) 
{
    status = ActionStatus::ERROR;
    errorMsg = errorMsg; 
    cout << errorMsg << endl;
}

string BaseAction::getErrorMsg() const 
{
    return errorMsg;
}


// Class of SimulateStep
SimulateStep::SimulateStep (int numOfSteps)
: numOfSteps(numOfSteps) {}


void SimulateStep::act(WareHouse &wareHouse) 
{
    for (int i = 0; i < numOfSteps; i++)
    {
        step1(wareHouse);
        wareHouse.step234();
    }
    complete();
    wareHouse.addAction(this);
}


// Helper function for SimulateStep - step 1
void SimulateStep::step1(WareHouse &wareHouse) 
{
    for (Order* order : wareHouse.getPendingVector()) // iterates pendingOrders
    {
        for (Volunteer* volunteer : wareHouse.getVolVector()) // iterates volunteers
        {
            if (volunteer->canTakeOrder(*order)) 
            {
                volunteer->acceptOrder(*order);
                wareHouse.moveOrder(order, volunteer->getId()); // using Helper function - just below
                break;
            }
        }
    }
}

string SimulateStep::toString() const 
{ 
    stringstream ss;
    ss << "simulateStep " << numOfSteps << " COMPLETED" << endl;
    return ss.str();
}

SimulateStep *SimulateStep::clone() const 
{
    return new SimulateStep(*this);
}


// Class of AddOrder
AddOrder::AddOrder (int id)
: customerId(id) {}

void AddOrder::act (WareHouse &wareHouse) 
{
    if (customerId >= wareHouse.getCustomerCount() || !(wareHouse.getCustomer(customerId).canMakeOrder()))
            error("Error: Cannot place this order");
    else 
    {
        Order* addOrd = new Order (wareHouse.getOrderCount(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
        wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCount());
        wareHouse.addOrder(addOrd);
        complete();
    }
    wareHouse.addAction(this);
}

string AddOrder::toString() const 
{
    stringstream ss;
    ss << "Order " << customerId << " " << statusToString(getStatus()) << endl;
    return ss.str();
}

AddOrder *AddOrder::clone() const 
{
    return new AddOrder(*this);
}


// Class of AddCustomer
AddCustomer::AddCustomer (const string &customerName, const string &customerType, int distance, int maxOrders)
: customerName(customerName), customerType(stringToCusType(customerType)), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) 
{
    int CusId = wareHouse.getCustomerCount();
    if (customerType == CustomerType::Soldier) 
    {
        SoldierCustomer* currCus = new SoldierCustomer(CusId, customerName, distance, maxOrders);
        wareHouse.addSolCustomer (currCus);
    }
    else 
    {
        CivilianCustomer* currCus = new CivilianCustomer(CusId, customerName, distance, maxOrders);
        wareHouse.addCivCustomer (currCus);
    }
    complete();
    wareHouse.addAction(this);
}

AddCustomer *AddCustomer::clone() const 
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const 
{
    stringstream ss;
    ss << "Customer " << customerName << " ";

    if (customerType == CustomerType::Soldier)
        ss << "soldier" << " ";
    else if (customerType == CustomerType::Civilian)
        ss << "civilian" << " ";
    else 
        return "incorrect input"; // what should we print if the input was written incorrectly????
    ss << distance << " " << maxOrders << " COMPLETED" << endl;;
    return ss.str();
}


// Class of PrintOrderStatus
PrintOrderStatus::PrintOrderStatus (int id)
: orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) 
{
    if (orderId >= wareHouse.getOrderCount() || wareHouse.getOrder(orderId).getId() == -1) 
        error("Error: Order doesn't exist");
    else 
    {
        Order ord = wareHouse.getOrder(orderId);
        cout << ord.toString() << endl;
        complete();
    }
    wareHouse.addAction(this);
}

PrintOrderStatus *PrintOrderStatus::clone() const 
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const 
{
    stringstream ss;
    ss << "orderStatus " << orderId << " " << statusToString(getStatus()) << endl;
    return ss.str();
}


// Class of PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus (int customerId)
: customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) 
{
    if (customerId >= wareHouse.getCustomerCount())
        error ("Error: Customer doesn't exist");
    else 
    {
        cout << PrintCustStatHelper(wareHouse, customerId) << endl; // Helper function in the bottom of this page
        complete();
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const 
{
    return new PrintCustomerStatus (*this);
}

string PrintCustomerStatus::toString() const 
{
    stringstream ss;
    ss << "customerStatus " << customerId << " " << statusToString(getStatus()) << endl;
    return ss.str();
}


// class of PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) 
: volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) 
{
    if (volunteerId >= wareHouse.getVolunteerCount() || wareHouse.getVolunteer(volunteerId).getId() == -1)
        error("Error: Volunteer doesn't exist");
    else 
    {
        cout << wareHouse.getVolunteer(volunteerId).toString() << endl;
        complete();
    }
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const 
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const 
{
    stringstream ss;
    ss << "volunteerStatus " << volunteerId << " " << statusToString(getStatus()) << endl;
    return ss.str();
}


// Class of PrintActionsLog
PrintActionsLog::PrintActionsLog () {}

void PrintActionsLog::act(WareHouse &wareHouse) 
{
    vector<BaseAction*> temp (wareHouse.getActions());
    for (BaseAction *action:temp) {
        cout << action->toString();
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const 
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const 
{
    stringstream ss;
    ss << "log COMPLETED" << endl;
    return ss.str();
}


// Class of Close
Close::Close () {}

void Close::act(WareHouse &wareHouse) 
{
    stringstream ss;
    int numOfOrd = wareHouse.getOrderCount();
    for (int i = 0; i < numOfOrd; i++) 
    {
        ss << "OrderID: " << i;
        ss << ", CustomerID: " << wareHouse.getOrder(i).getCustomerId();
        ss << ", Status: " << ordStatusToString(wareHouse.getOrder(i).getStatus()) << endl;
    }
    cout << ss.str() << endl;
    complete();
    wareHouse.addAction(this);
    wareHouse.close(); // free all memory
}

Close *Close::clone() const 
{
    return new Close(*this);
}

string Close::toString() const 
{
    return "";
}


// Class of BackupWareHouse
BackupWareHouse::BackupWareHouse () {}

void BackupWareHouse::act(WareHouse &wareHouse) 
{ 
    if (backup != nullptr)
        delete backup;
    // Create a new WareHouse object to hold the backup
    backup = new WareHouse(wareHouse); // Create a copy of the wareHouse
    complete();
    wareHouse.addAction(this);
}

BackupWareHouse *BackupWareHouse::clone() const 
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const 
{
    stringstream ss;
    ss << "Backup COMPLETED" << endl; 
    return ss.str();
}


// Class of RestoreWareHouse
RestoreWareHouse::RestoreWareHouse () {}

void RestoreWareHouse::act(WareHouse &wareHouse) 
{
    if (backup == nullptr) // validate there is a backup
        error ("Error: No backup available");
    else 
    { // using move assignment method - erase current wareHouse and change it to be the backup
        wareHouse = *backup;
        complete();
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const 
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const 
{
    stringstream ss;
    ss << "Restore " << statusToString(getStatus()) << endl;
    return ss.str();
}