#include "WareHouse.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <iostream>
using namespace std;
#include <sstream>
using std::iterator;
#include "Volunteer.h"
#include <fstream>
#include "Action.h"
#include <algorithm>

extern WareHouse *backup;

WareHouse::WareHouse(const string &configFilePath) 
: isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{ 
    pendingOrders.clear();
    vector<vector<string>> textFileVectors = parseText(configFilePath);
    for (const auto &line : textFileVectors) 
    {
        const string &typeOfAction = line [2];
        const string &name = line [1];
        int firstParam = (line.size() > 3) ? std::stoi(line[3]) : 0;
        int secParam = (line.size() > 4) ? std::stoi(line[4]) : 0;
        int thirdParam = (line.size() > 5) ? std::stoi(line[5]) : 0;

        if (typeOfAction == "soldier") 
        {
            SoldierCustomer* newSC = new SoldierCustomer(customerCounter, name, firstParam, secParam);
            customerCounter++;
            customers.push_back(newSC);
        }
        else if (typeOfAction == "civilian") 
        {
            CivilianCustomer* newCC = new CivilianCustomer(customerCounter, name, firstParam, secParam);
            customerCounter++;
            customers.push_back(newCC);
        }
        else if (typeOfAction == "collector") 
        {
            CollectorVolunteer* newCV = new CollectorVolunteer(volunteerCounter, name, firstParam);
            volunteerCounter++;
            volunteers.push_back(newCV);
        }
        else if (typeOfAction == "limited_collector") 
        {
            LimitedCollectorVolunteer* newLCV = new LimitedCollectorVolunteer(volunteerCounter, name, firstParam, secParam);
            volunteerCounter++;
            volunteers.push_back(newLCV);
        }
        else if (typeOfAction == "driver") 
        {
            DriverVolunteer* newDV = new DriverVolunteer(volunteerCounter, name, firstParam, secParam);
            volunteerCounter++;
            volunteers.push_back(newDV);
        }
        else if (typeOfAction == "limited_driver") 
        {
            LimitedDriverVolunteer* newLDV = new LimitedDriverVolunteer(volunteerCounter, name, firstParam, secParam, thirdParam);
            volunteerCounter++;
            volunteers.push_back(newLDV);
        }
    }
}


////////////////////////// RULE OF 5

// copy constructor
WareHouse::WareHouse(const WareHouse &other)
: isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    for (auto customer : other.customers)
    {
        customers.push_back(customer->clone());
    }

    for (auto ordPen : other.pendingOrders)
    {
        pendingOrders.push_back(ordPen->clone());
    }

    for (auto inProc : other.inProcessOrders)
    {
        inProcessOrders.push_back(inProc->clone());
    }

    for (auto ordComp : other.completedOrders)
    {
        completedOrders.push_back(ordComp->clone());
    }

    for (auto vol : other.volunteers)
    {
        volunteers.push_back(vol->clone());
    }

    for (auto action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
}


// copy assignment
WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {
        isOpen = other.isOpen;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        customerCounter = other.customerCounter;

        for (auto customer : customers)
        {
            delete customer;
        }
        customers.clear();

        for (auto vol : volunteers)
        {
            delete vol;
        }
        volunteers.clear();

        for (auto ordPen : pendingOrders)
        {
            delete ordPen;
        }
        pendingOrders.clear();

        for (auto inProcOrd : inProcessOrders)
        {
            delete inProcOrd;
        }
        inProcessOrders.clear();

        for (auto ordComp : completedOrders)
        {
            delete ordComp;
        }
        completedOrders.clear();

        for (auto action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();

        for (auto customer : other.customers)
        {
            customers.push_back(customer->clone());
        }

        for (auto ordPen : other.pendingOrders)
        {
            pendingOrders.push_back(ordPen->clone());
        }

        for (auto inProcOrd : other.inProcessOrders)
        {
            inProcessOrders.push_back(inProcOrd->clone());
        }

        for (auto ordComp : other.completedOrders)
        {
            completedOrders.push_back(ordComp->clone());
        }

        for (auto vol : other.volunteers)
        {
            volunteers.push_back(vol->clone());
        }

        for (auto action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }
    }
    return *this;
}


// move constructor
WareHouse::WareHouse(WareHouse &&other)
: isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    customers = std::move(other.customers);
    inProcessOrders = std::move(other.inProcessOrders);
    volunteers = std::move(other.volunteers);
    pendingOrders = std::move(other.pendingOrders);
    completedOrders = std::move(other.completedOrders);
    actionsLog = std::move(other.actionsLog);

    // clear all other vectors:
    other.customers.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.actionsLog.clear();
}


// move assignment
WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if (this != &other)
    { //assign memebers 
        isOpen = other.isOpen;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        customerCounter = other.customerCounter;

        //delete and prevent memory leaks of this warehouse
        for (auto customer : customers)
        {
            delete customer;
        }
        customers.clear();

        for (auto vol : volunteers)
        {
            delete vol;
        }
        volunteers.clear();

        for (auto ordPen : pendingOrders)
        {
            delete ordPen;
        }
        pendingOrders.clear();

        for (auto inProcOrd : inProcessOrders)
        {
            delete inProcOrd;
        }
        inProcessOrders.clear();

        for (auto ordComp : completedOrders)
        {
            delete ordComp;
        }
        completedOrders.clear();

        for (auto action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
    
        // move from "other" to this warehouses
        customers = std::move(other.customers);
        inProcessOrders = std::move(other.inProcessOrders);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        completedOrders = std::move(other.completedOrders);
        actionsLog = std::move(other.actionsLog);
        

        // clear all other vectors:
        other.customers.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.actionsLog.clear();
    }
    return *this;
}


// destructor
WareHouse::~WareHouse()
{
    for (auto customer : customers)
    {
        delete customer;
    }
    customers.clear();

    for (auto vol : volunteers)
    {
        delete vol;
    }
    volunteers.clear();

    for (auto ordPen : pendingOrders)
    {
        delete ordPen;
    }
    pendingOrders.clear();

    for (auto inProcOrd : inProcessOrders)
    {
        delete inProcOrd;
    }
    inProcessOrders.clear();

    for (auto ordComp : completedOrders)
    {
        delete ordComp;
    }
    completedOrders.clear();

    for (auto action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();
}


void WareHouse::start() 
{
    isOpen = true;
    cout << "Warehouse is open!" << endl;
    
    while (isOpen) 
    {
        string userInput;
        vector<string> words;
        getline(cin, userInput);  // Get the entire line of input, including spaces
        stringstream ss(userInput);  // Create a stringstream for parsing
        string word;
        while (getline(ss, word, ' ')) 
        {  // Extract words separated by spaces
            words.push_back(word);  // Store each word in the vector
        }
        startHelper(words);
    }
}

void WareHouse::addOrder(Order* order)
{
    orderCounter++;
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action) 
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const 
{
    return *customers[customerId];
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const 
{
    int size = volunteers.size();
    for (int i = 0; i < size; i++) 
    {
        if (volunteers.at(i)->getId() == volunteerId) 
            return *volunteers.at(i);
    }
    // If not found, return default volunteer
    static CollectorVolunteer defaultVol(-1, "empty", -1);
    return defaultVol;
}

Order &WareHouse::getOrder(int orderId) const 
{ // iterates all orders in the warehouse to find the matching ID one
    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
            return *order;
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
            return *order;
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
            return *order;
    }
    // If not found, return default order
    static Order defaultOrd(-1,-1,-1);
    return defaultOrd;
}


const vector<BaseAction*> &WareHouse::getActions() const 
{
    return actionsLog;
}

void WareHouse::close() 
{ 
    isOpen = false;
}    

void WareHouse::open() 
{
    isOpen = true;
}


// functions added by us:

vector<Volunteer*> WareHouse::getVolVector() const 
{
    return volunteers;
}

vector<Order*> WareHouse::getPendingVector() const 
{
    return pendingOrders;
}

vector<Order*> WareHouse::getinProcVector() const 
{
    return inProcessOrders;
}

vector<Order*> WareHouse::getCompletedVector() const 
{
    return completedOrders;
}

vector<Customer*> WareHouse::getCusVector() const 
{
    return customers;
}

int WareHouse::getCustomerCount() const 
{
    return customerCounter;
}

int WareHouse::getVolunteerCount() const 
{
    return volunteerCounter;
}

int WareHouse::getOrderCount() const 
{
    return orderCounter;
}

void WareHouse::addSolCustomer(SoldierCustomer* customer) 
{
    customerCounter++;
    customers.push_back(customer);
}

void WareHouse::addCivCustomer(CivilianCustomer* customer) 
{
    customerCounter++;
    customers.push_back(customer);
}


// parse text file
vector<vector<string>> WareHouse::parseText(const string &configFilePath) 
{
    vector<vector<string>> output;
    ifstream inputFile(configFilePath);
    if (!inputFile.is_open()) 
    {
        cerr << "Error opening file: " << configFilePath << endl;
        return output;
    }
    string line;
    while (getline(inputFile, line)) 
    { // ignore files starting with #
        if (!line.empty() && line[0] != '#') 
        {
            istringstream iss(line);
            vector<string> words;
            while (iss >> skipws >> line) 
            {
                if (line [0] == '#') // exit the loop if we find '#' in the middle of the line
                    break;
                words.push_back(line);
            }
            output.push_back(words);
        }
    }
    inputFile.close();
    return output;
}


// Helper function for start method
void WareHouse::startHelper(vector<string> decodedString) 
{
    if (decodedString[0] == "order") 
    {
        int i = std::stoi(decodedString[1]);
        AddOrder* ord = new AddOrder(i);
        ord->act(*this);
    }
    else if (decodedString[0] == "step") 
    {
        SimulateStep* newStep = new SimulateStep(stoi(decodedString[1]));
        newStep->act(*this);
    }
    else if (decodedString[0] == "customer") 
    {
        AddCustomer* newCustomer = new AddCustomer(decodedString[1], decodedString[2], stoi(decodedString[3]), stoi(decodedString[4]));
        newCustomer->act(*this);
    }
    else if (decodedString[0] == "orderStatus") 
    {
        PrintOrderStatus* newOrderStatus = new PrintOrderStatus(stoi(decodedString[1]));
        newOrderStatus->act(*this);
    }
    else if (decodedString[0] == "customerStatus") 
    {
        PrintCustomerStatus* newCustomerStatus = new PrintCustomerStatus(stoi(decodedString[1]));
        newCustomerStatus->act(*this);
    }
    else if (decodedString[0] == "volunteerStatus") 
    {
        PrintVolunteerStatus* newVolunteerStatus = new PrintVolunteerStatus(stoi(decodedString[1]));
        newVolunteerStatus->act(*this);
    }
    else if (decodedString[0] == "log") 
    {
        PrintActionsLog* newActionsLog = new PrintActionsLog();
        newActionsLog->act(*this);
    }
    else if (decodedString[0] == "close") 
    {
        Close* newClose = new Close();
        newClose->act(*this);
    }
    else if (decodedString[0] == "backup") 
    {
        BackupWareHouse* newBackUp = new BackupWareHouse();
        newBackUp->act(*this);
    }
    else if (decodedString[0] == "restore")
    {
        RestoreWareHouse* newRestore = new RestoreWareHouse();
        newRestore->act(*this);
    }
    else
        cout << "mistyped action" << endl;
}


// Helper function: step 1
void WareHouse::moveOrder (Order* ord, int id) 
{ 
    // move order to relevent vector and change its status and volunteer id
    if (ord->getStatus() == OrderStatus::PENDING)
    {
        ord->setStatus(OrderStatus::COLLECTING);
        ord->setCollectorId(id);
    }
    else if (ord->getStatus() == OrderStatus::COLLECTING)
    {
        ord->setStatus(OrderStatus::DELIVERING);
        ord->setDriverId(id);
    }
    inProcessOrders.push_back(ord);
    int ordId = ord->getId();
    auto toErase = std::find_if(pendingOrders.begin(),
    pendingOrders.end(), [ordId] (const Order* element) 
    {return element != nullptr && element->getId() == ordId;});
    pendingOrders.erase(toErase);
}


// Helper function for SimulateStep - steps 2,3,4 in one iteration
void WareHouse::step234() 
{
    for (Volunteer* vol : volunteers)
    {
        if (vol->isBusy()) 
        {
            vol->step(); // step 2 in the method
            if (!vol->isBusy())
            // validate if vol just completed his assignment (step 3 in the method)
            {
                int currOrdId = vol->getCompletedOrderId(); // handle changes in completed order
                for (Order* ord : inProcessOrders)
                // searching completed order in the vector
                {
                    if (ord->getId() == currOrdId)
                    {
                        // handle the order: first find if collector or driver handled it
                        if (ord->getStatus() == OrderStatus::COLLECTING) 
                        {
                            pendingOrders.push_back(ord);
                            auto toErase = std::find_if(inProcessOrders.begin(), 
                            inProcessOrders.end(), [currOrdId] (const Order* element) 
                            {return element != nullptr && element->getId() == currOrdId;});
                            inProcessOrders.erase(toErase);
                            vol->setCompletedOrder(); // change completedOrderId field to -1
                        }
                        else if (ord->getStatus() == OrderStatus::DELIVERING)                        
                        {
                            ord->setStatus(OrderStatus::COMPLETED);
                            completedOrders.push_back(ord);
                            auto toErase = std::find_if(inProcessOrders.begin(),
                            inProcessOrders.end(), [currOrdId] (const Order* element) 
                            {return element != nullptr && element->getId() == currOrdId;});
                            inProcessOrders.erase(toErase);
                            vol->setCompletedOrder(); // change completedOrderId field to -1
                        }
                    }
                }
            }
            // erase limited volunteers - step 4
            if (!(vol->hasOrdersLeft()) && vol->getActiveOrderId() == -1 && vol->getCompletedOrderId() == -1)
            {
                int volId = vol->getId();
                for (auto it = volunteers.begin(); it != volunteers.end(); ++it) 
                {
                    if ((*it)->getId() == volId) 
                    {
                        delete vol;
                        volunteers.erase(it);
                    }
                }
            }
        }
    }    
}