#include <string>
#include <vector>
using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include "Order.h"
#include "Customer.h"
#include "WareHouse.h"
#include "Volunteer.h"
#include "Action.h"

WareHouse :: WareHouse(const string &configFilePath): isOpen(false), actionsLog({}), volunteers({}), pendingOrders({}), inProcessOrders({}), completedOrders({}), customers({}), customerCounter(0), volunteerCounter(0), orderCounter(0), isBackup(false){
    // Open the file for reading
    std::ifstream inputFile(configFilePath);
    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }
    // Read and print the contents of the file line by line
    std::string line;
    std::vector<std::string> words;
    while (std::getline(inputFile, line)) {
        if(!line.empty() && line[0]!='#'){
            // Create a string stream from the line
            std::istringstream iss(line);

            // Iterate over the first 6 words in the line 
            std::string word;
            int i = 0;
            while (iss >> word && i<=5) {
                words.push_back(word);
                i+=1;
            }
            // Filtering the line to its object and creating one
            if(words[0]=="customer"){
                if(words[2] == "soldier"){
                    customers.push_back(new SoldierCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }
                else{
                    customers.push_back(new CivilianCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }           
                customerCounter+=1;
            }
            else{
                if(words[2] == "limited_collector"){
                    volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }
                else if(words[2] == "collector"){
                    volunteers.push_back(new CollectorVolunteer(volunteerCounter,words[1],std::stoi(words[3])));
                }
                else if(words[2] == "limited_driver"){
                    volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4]),std::stoi(words[5])));
                }
                else if(words[2] == "driver"){
                    volunteers.push_back(new DriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }        
                volunteerCounter+=1;
            } 
            // emptying the words vect
            words.clear();
        }
    }
    // Close the file
    inputFile.close();
}
void WareHouse :: start(){
    open();
    std :: cout << "please enter action to execute"<< std :: endl;
    string action;
    while(isOpen)
     {
        std::getline(std::cin,action);
        std::istringstream iss(action);
        std::vector<std::string> words;
        string firstWord;
        while(iss>>firstWord)
        {
         words.push_back(firstWord);   
        }
        if (words[0] == "step") {
            string number;
            number = words[1];
            if ( isNumber(number)){
                int numOfSteps = stoi(number);
                SimulateStep* step = new SimulateStep(numOfSteps);
                step->act(*this);
            }
            continue;
        }

        if (words[0] == "order") {
            string number;
            number = words[1];
            if (isNumber(number)){
                int id = stoi(number);
                AddOrder* addOrder = new AddOrder(id);
                addOrder->act(*this);
            }
            continue;
        }

        if (words[0] == "customer") {
            string name;
            string type;
            string distance;
            string maxOrder;
            name = words[1];
            type = words[2];
            distance = words[3];
            maxOrder = words[4];
            if(isNumber(distance) && isNumber(maxOrder)){
                AddCustomer* addCustomer = new AddCustomer(name, type, stoi(distance), stoi(maxOrder));
                addCustomer -> act(*this);
            }
            continue;
        }

        if (words[0] == "orderStatus") {
            string number;
            number = words[1];
            if( isNumber(number)){
                PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(number));
                orderStatus -> act(*this);
            }
            continue;
        }

        if (words[0] == "customerStatus") {
            string number;
            number = words[1];
            if( isNumber(number)){
                PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(number));
                customerStatus -> act(*this);
            }
            continue;
        }

        if (words[0] == "volunteerStatus") {
            string number;
            number = words[1];
            if(isNumber(number)){
                PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(number));
                volunteerStatus -> act(*this);
            }
            continue;
        }

        if (words[0] == "log") {
            PrintActionsLog* log = new PrintActionsLog();
            log -> act(*this);
        }

        if (words[0] == "close") {
            Close* cl = new Close(); 
            cl -> act(*this);
        }

        if (words[0] == "backup") {
            BackupWareHouse* bac = new BackupWareHouse();
            bac -> act(*this);
            this->setIsBeckup();
        }

        if (words[0] == "restore") {
            RestoreWareHouse* restore = new RestoreWareHouse();
            restore -> act(*this);
        } 
    }
}
void WareHouse :: addOrder(Order* order) 
{
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse :: addAction (BaseAction* action)
{
    actionsLog.push_back(action);
}

bool WareHouse:: customerExists(int id) const{
    return id<customerCounter;
}

Customer& WareHouse :: getCustomer (int customerId) const
{
    for (Customer* customer : customers) 
    {
        if (customer->getId() == customerId) 
        {
            return *customer;
        }
    }
    throw std::runtime_error("This is getCustomer error.");
}

const int WareHouse :: getCustomerCounter() const{
    return customerCounter;
}

bool WareHouse:: volunteerExists(int id) const{
    for (Volunteer* volunteer : volunteers) 
    {
        if (volunteer->getId() == id) 
        {
            return true;
        }
    }
    return false;
}

Volunteer& WareHouse :: getVolunteer (int volunteerId) const
{
    for (Volunteer* volunteer : volunteers) 
    {
        if (volunteer->getId() == volunteerId) 
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("This is getVolunteer error.");
}

const int WareHouse :: getVolunteerCounter() const{
    return volunteerCounter;
}

bool WareHouse:: orderExists(int id) const{
    return (id<orderCounter && id != -1);
}


Order& WareHouse:: getOrder(int orderId) const{
    for (const auto& order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    throw std::runtime_error("This is a getOrder error.");
}

const int WareHouse :: getOrderCounter() const{
    return orderCounter;
}

void WareHouse:: close(){
    for (Order* order : pendingOrders){
        std::cout << order->closeInfo() << std::endl;
    }
    for (Order* order : inProcessOrders){
        std::cout << order->closeInfo() << std::endl;
    }
    for (Order* order : completedOrders){
        std::cout << order->closeInfo() << std::endl;
    }
    isOpen = false;
}
void WareHouse:: open(){
     if (!isOpen) 
    {
        isOpen = true;
        std::cout << "Warehouse is open!" << std::endl;
    } 
    else
    {
        std::cout << "The Warehouse is already open." << std::endl;
        close();
    }
}

void WareHouse:: addCustomer(Customer* Customer){
    customers.push_back(Customer);
    customerCounter++;
}

void WareHouse:: addVolunteer(Volunteer* Volunteer){
    volunteers.push_back(Volunteer);
    volunteerCounter++;
}

void WareHouse:: cleanUp(){
    for(Order* order: pendingOrders){
        delete order;
        order = nullptr;
    }

    for(Order* order : inProcessOrders){
        delete order;
        order = nullptr;
    }

    for(Order* order : completedOrders){
        delete order;
        order = nullptr;
    }

    for(Customer* customer : customers){
        delete customer;
        customer = nullptr;
    }

    for(Volunteer* volunteer : volunteers){
        delete volunteer;
        volunteer = nullptr;
    }

    for(BaseAction* action : actionsLog){
        delete action;
        action = nullptr;
    }

}

const vector<BaseAction*>& WareHouse :: getActions() const
{
    return actionsLog;
}

const vector<Order*>& WareHouse :: getPending() const
{
    return pendingOrders;
}

const vector<Order*>& WareHouse :: getProgress() const
{
    return inProcessOrders;
}

const vector<Order*>& WareHouse :: getCompleted() const
{
    return completedOrders;
}

void WareHouse:: step()
{
    //step 1
    
    auto orderPend = pendingOrders.begin();
    while (orderPend != pendingOrders.end())
    {
        Order* ordP = *orderPend;

        if (ordP->getStatus() == OrderStatus::COLLECTING && canContinue(*ordP, "Driver")){ 
            orderPend = pendingOrders.erase(orderPend);
            continue;
        }
        if (ordP->getStatus() == OrderStatus::PENDING && canContinue(*ordP, "Collector")){ 
            orderPend = pendingOrders.erase(orderPend);
            continue; 
        }

        ++orderPend;
    }

    //step 2
    auto vol = volunteers.begin();
    while ( vol != volunteers.end())
    {
        (*vol)->step(); 
        ++vol;
    }

    //step3 
    auto orderPro = inProcessOrders.begin();
    while (orderPro != inProcessOrders.end())
    {
        Order* ord = *orderPro;
        int collectorId = ord->getCollectorId();
        int driverId = ord->getDriverId();

        //if (driverId > -1)
        if(volunteerExists(driverId))
        {
            Volunteer* driver = &(getVolunteer(driverId));
            if (ord->getStatus() == OrderStatus::DELIVERING && driver->getCanMove())
            {
                ord->setStatus(OrderStatus::COMPLETED);
                completedOrders.push_back(ord);
                orderPro = inProcessOrders.erase(orderPro);
                continue;
            }
        }
        //if (collectorId > -1)
        if(volunteerExists(collectorId))
        {
            Volunteer* collector = &(getVolunteer(collectorId));
            if (ord->getStatus() == OrderStatus::COLLECTING && collector->getCanMove())
            {
                pendingOrders.push_back(ord);
                orderPro = inProcessOrders.erase(orderPro);
                continue;
            }
        }

        ++orderPro;
    }




    //step 4
    auto volToDelet = volunteers.begin();
    while ( volToDelet != volunteers.end())
    {
        Volunteer* voltemp = *volToDelet;
        if (!voltemp->isBusy() && voltemp->getNumOrdersLeft() == 0)
        {
            volToDelet = volunteers.erase(volToDelet);
            delete voltemp;
            --volToDelet;
        }
        ++volToDelet;

    }
}


bool WareHouse:: canContinue(Order& order, string type){
    for(Volunteer* volunteer : volunteers){
        if (volunteer->getType() == type && volunteer -> canTakeOrder(order)) {
                volunteer->acceptOrder(order); // Update ordersLeft volunteer member in case he is Limited volunteer
                if (type == "Collector"){
                    order.setStatus(OrderStatus::COLLECTING);
                    order.setCollectorId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    return true;
                }
                if (type == "Driver"){
                    order.setStatus(OrderStatus::DELIVERING);
                    order.setDriverId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    return true;
                }
            }
    }
    return false;
}
bool WareHouse:: getIsBeckup() const{
    return isBackup;
}

void WareHouse::setIsBeckup() {
    isBackup = true;
}

bool WareHouse :: isNumber(string& num)
{
    for (char c : num) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}


//copy vectors of objects
template<typename T>
vector<T*> WareHouse::copyVectors(const vector<T*>& other) {
    vector<T*> newVector;
    for (const auto& obj : other) {
        newVector.push_back(obj->clone());
    }
    return newVector;
}

//destructor
WareHouse :: ~WareHouse(){
    for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for (Order* order : pendingOrders) {
            delete order;
        }
        for (Order* order : inProcessOrders) {
            delete order;
        }
        for (Order* order : completedOrders) {
            delete order;
        }
        for (Customer* customer : customers) {
            delete customer;
        }
}


//copy constructor
WareHouse::WareHouse(const WareHouse& other) :
    isOpen(other.isOpen), 
    actionsLog(copyVectors<BaseAction>(other.actionsLog)),
    volunteers(copyVectors<Volunteer>(other.volunteers)),
    pendingOrders(copyVectors<Order>(other.pendingOrders)),
    inProcessOrders(copyVectors<Order>(other.inProcessOrders)),
    completedOrders(copyVectors<Order>(other.completedOrders)),
    customers(copyVectors<Customer>(other.customers)),
    customerCounter(other.customerCounter), 
    volunteerCounter(other.volunteerCounter), 
    orderCounter(other.orderCounter), 
    isBackup(other.isBackup) 
{}

//copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other){
    if (this != &other) {
        isOpen = other.isOpen;
        for (int i = 0; i< (int)actionsLog.size(); i++){
            if (actionsLog.at(i) != nullptr){
                delete actionsLog.at(i);
            }
        }
        actionsLog.clear();
        for (int i = 0; i< (int) other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }
        for (int i = 0; i<(int) volunteers.size(); i++){
            if (volunteers.at(i) != nullptr){
                delete volunteers.at(i);
            }
        }
        volunteers.clear();
        for (int i = 0; i<(int) other.volunteers.size(); i++){
            volunteers.push_back(other.volunteers.at(i)->clone());
        }
        for (int i = 0; i< (int)pendingOrders.size(); i++){
            if (pendingOrders.at(i) != nullptr){
                delete pendingOrders.at(i);
            }
        }
        pendingOrders.clear();
        for (int i = 0; i< (int)other.pendingOrders.size(); i++){
            pendingOrders.push_back(other.pendingOrders.at(i)->clone());
        }
        for (int i = 0; i< (int)inProcessOrders.size(); i++){
            if (inProcessOrders.at(i) != nullptr){    
                delete inProcessOrders.at(i);
            }
        }
        inProcessOrders.clear();
        for (int i = 0; i< (int)other.inProcessOrders.size(); i++){
            inProcessOrders.push_back(other.inProcessOrders.at(i)->clone());
        }
        for (int i = 0; i<(int) completedOrders.size(); i++){
            if (completedOrders.at(i) != nullptr){    
                delete completedOrders.at(i);
            }
        }
        completedOrders.clear();
        for (int i = 0; i<(int) other.completedOrders.size(); i++){
            completedOrders.push_back(other.completedOrders.at(i)->clone());
        }
        for (int i = 0; i< (int)customers.size(); i++){
            if (customers.at(i) != nullptr){    
                delete customers.at(i);
            }
        }
        customers.clear();
        for (int i = 0; i< (int)other.customers.size(); i++){
            customers.push_back(other.customers.at(i)->clone());
        }
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        isBackup = other.isBackup;
    }
    return *this;
}

//move constructor
WareHouse::WareHouse(WareHouse&& other) noexcept
    : isOpen(other.isOpen), actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)), pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)), completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)), customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
      isBackup(other.isBackup) {}

//move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        for (auto action : actionsLog) {
        delete action;
        }
    
    for (auto volunteer : volunteers) {
        delete volunteer;
        }

    for (auto order : pendingOrders) {
        delete order;
        }

    for (auto order : inProcessOrders) {
        delete order;
        }

    for (auto order : completedOrders) {
        delete order;
        }

    for (auto customer : customers) {
        delete customer;
        }

        actionsLog = move(other.actionsLog);
        volunteers = move(other.volunteers);
        pendingOrders = move(other.pendingOrders);
        inProcessOrders = move(other.inProcessOrders);
        completedOrders = move(other.completedOrders);
        customers = move(other.customers);
        isOpen = move(other.isOpen);
        customerCounter = move(other.customerCounter);
        volunteerCounter = move(other.volunteerCounter);
        orderCounter = move(other.orderCounter);
        isBackup = move(other.isBackup);
    }
    return *this;
}