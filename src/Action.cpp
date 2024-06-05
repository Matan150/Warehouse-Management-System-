#include "Action.h"
#include <iostream>


std::string actionStatusToString(ActionStatus status) {
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

std::string customerTypeToString(CustomerType type) {
    switch (type) {
        case CustomerType::Soldier:
            return "Soldier";
        case CustomerType::Civilian:
            return "Civilian";
        default:
            return "UNKNOWN";
    }
}


BaseAction::BaseAction() : errorMsg("ERROR: "), status(ActionStatus::COMPLETED){}

BaseAction::~BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}


void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}


SimulateStep::SimulateStep(int _numOfSteps) : numOfSteps(_numOfSteps) {}

void SimulateStep :: act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    for(int i = 0;i < numOfSteps; i++)
    {
        wareHouse.step();
    }
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}

string SimulateStep:: toString() const{
    string output = "SimulateStep " + std::to_string(numOfSteps)+ " " ;

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


AddOrder::AddOrder(int id) : customerId(id){}
void AddOrder::act(WareHouse &wareHouse) {
    if(customerId > wareHouse.getCustomerCounter())
    {
        error("Cannot place this order");
    }
    else {
        Customer& customer = wareHouse.getCustomer(customerId);
        if(!customer.canMakeOrder())
        {
            error("Cannot place this order");   
        }
        else
        {
            int OrderId = wareHouse.getOrderCounter();
            customer.addOrder(OrderId);
            int distance = customer.getCustomerDistance();
            Order* newOrder = new Order(OrderId, customerId, distance);
            newOrder->setStatus(OrderStatus::PENDING);
            wareHouse.addOrder(newOrder);
        }
        
    }
    wareHouse.addAction(this);
}



string AddOrder::toString() const {
    return "order " + std::to_string(customerId) + " " + actionStatusToString(this->getStatus());
}

//TODO
AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);
}


AddCustomer::AddCustomer(const string &customerName, const string &_customerType, int distance, int maxOrders)
    : customerName(customerName),distance(distance), maxOrders(maxOrders),
    customerType((_customerType == "soldier")? CustomerType::Soldier : CustomerType::Civilian) {}

void AddCustomer::act(WareHouse &wareHouse) {
    Customer* newCustomer = nullptr;
    if (customerType == CustomerType::Soldier){
        newCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    else{

        newCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(newCustomer);
    complete();
    wareHouse.addAction(this);
}

//TODO
AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}


string AddCustomer::toString() const {
    return "Customer: " + customerName + " " + customerTypeToString(customerType) + " "+ std::to_string(distance) + " "+ std::to_string(maxOrders) + " COMPLETED";
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus:: act(WareHouse& wareHouse){
    wareHouse.addAction(this);
    if(wareHouse.orderExists(orderId)){
        Order& order = wareHouse.getOrder(orderId);
        std::cout << order.toString() << std::endl;
    }
    else{
        error("Order doesn't exist");
        return;
    }

    complete();
}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus:: toString() const{
    string output = "orderStatus " + std::to_string(orderId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if(wareHouse.customerExists(customerId)){
        Customer& customer = wareHouse.getCustomer(customerId);
        std::cout<< "CustomerID: " << customerId << std::endl;
        for (int orderId : customer.getOrdersIds())
        {
            Order& order = wareHouse.getOrder(orderId);
            std::cout<< "OrderID: " << orderId << std::endl;
            std::cout<< "OrderStatus: " << order.orderStatusToString(order.getStatus())<< std::endl;
        }
        std::cout << "numOrdersLeft: " << customer.getMaxOrders()-customer.getNumOrders()<<std::endl;
        complete();
    }    
    else{
        error("Customer doesn't exist");
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    return "customerStatus " + std::to_string(customerId) + " " + actionStatusToString(getStatus());
}


PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if (wareHouse.volunteerExists(volunteerId)) 
    {   
        Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
        std::cout<< "VolunteerID: " << volunteerId <<std::endl;
        std::cout<< std::boolalpha << "isBusy: " << ((volunteer.isBusy())?"True" : "False") <<std::endl;
        if(volunteer.isBusy()){
            std::cout<< "OrderID: "<< volunteer.getActiveOrderId() <<std::endl;
            std::cout<< "TimeLeft: " << volunteer.getTimeLeft() << std::endl;
        }
        else{
            std::cout<< "OrderID: None" <<std::endl;
            std::cout<< "TimeLeft: None" <<std::endl;
        }
        std::cout<< "OrdersLeft: " << ((volunteer.getNumOrdersLeft() == -1)?"No Limit" : std::to_string (volunteer.getNumOrdersLeft()))<<std::endl;
        complete();
    }
    else 
    {
        error("Volunteer doesn't exist");
    }
    wareHouse.addAction(this); 
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    return "VolunteerStatus " + std::to_string(volunteerId)+ " " + actionStatusToString(getStatus());
}


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    vector<BaseAction*> actionsLog = wareHouse.getActions();
    for (BaseAction* action : actionsLog){
        std::cout << action->toString() << std::endl;
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log COMPLETED";
}


// Close
Close::Close() {}

void Close::act(WareHouse &wareHouse) 
{
    wareHouse.addAction(this);
    wareHouse.close();
    
}

Close *Close::clone() const {
    return new Close(*this);
}

string Close::toString() const 
{
   return "close COMPLETED";
}


// BackupWareHouse
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) 
{
    if (backup != nullptr) 
    {
        delete backup;
    }
    backup = new WareHouse(wareHouse);
    wareHouse.addAction(this);    
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup COMPLETED";
}


// RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) 
{
    if (backup == nullptr) {
       error("No backup available");
    }
    else{
        complete();
        wareHouse = *backup;
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "Restore " + actionStatusToString(getStatus());
}
