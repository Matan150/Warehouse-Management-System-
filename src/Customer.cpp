#include "Customer.h"


Customer::Customer(int _id, const string &_name, int _locationDistance, int _maxOrders):
id(_id), name(_name), locationDistance(_locationDistance), maxOrders(_maxOrders), ordersId({}){}
Customer :: ~Customer(){}

const string &Customer :: getName() const{
    return name;
}

int Customer :: getId() const{
    return id;
}

int Customer :: getCustomerDistance() const{
    return locationDistance;
}

int Customer :: getMaxOrders() const{
    return maxOrders;
}

int Customer :: getNumOrders() const{
    return ordersId.size();
}

bool Customer :: canMakeOrder() const{
    return getNumOrders() < maxOrders;
}

const vector<int> &Customer :: getOrdersIds() const{
    return ordersId;
}

int Customer :: addOrder(int orderId){
    if (canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId;
    }
    else {
        return -1;
    }
}


SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    :Customer(id, name, locationDistance, maxOrders){}

SoldierCustomer *SoldierCustomer :: clone() const {
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
    :Customer(id, name, locationDistance, maxOrders){}

CivilianCustomer *CivilianCustomer::clone() const{
    return new CivilianCustomer(*this);
}