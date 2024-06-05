#include "Order.h"


Order:: Order (int id1, int Idcustomer, int distance1) : id(id1), customerId(Idcustomer), distance(distance1), status(OrderStatus :: PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)  {}

int Order :: getCollectorId() const {
    return  collectorId;
}

int Order :: getId() const {
    return id;
}

int Order :: getCustomerId() const
{
    return customerId;
}

void Order ::  setStatus(OrderStatus status1)
{
    status = status1;
}

void Order :: setCollectorId (int collectorId1)
{
    collectorId = collectorId1;
}

void Order :: setDriverId ( int driverId1){
    driverId = driverId1;
}

int Order :: getDriverId() const
{
    return driverId;
}

OrderStatus Order :: getStatus() const
{
    return status;
}

const string Order :: toString() const {
    string driver = "None";
    int driverId = getDriverId();
    if (driverId != -1){
        driver = std::to_string(driverId);
    }

    string collector = "None";
    int collectorId = getCollectorId();
    if (collectorId != -1){
        collector = std::to_string(collectorId);
    }

    string output =
        "OrderId: " + std::to_string(id) +"\n"
        "OrderStatus: " + orderStatusToString(status) + "\n"
        "CustomerID: " + std::to_string(customerId) + "\n"
        "Collector: " + collector + "\n"
        "Driver: " + driver;
    
    return output;
}

Order* Order :: clone() const {
    return new Order (*this);
}

const string Order :: orderStatusToString(OrderStatus status) const{
     switch (status) {
        case OrderStatus::PENDING:
            return "Pending";
        case OrderStatus::COLLECTING:
            return "Collecting";
        case OrderStatus::COMPLETED:
            return "COMPLETED";
        case OrderStatus::DELIVERING:
            return "Delivering";
        default:
            return "UNKNOWN";
    }
}

const int Order :: getOrderDistance() const{
    return distance;
}

const string Order :: closeInfo() const{
    // OrderID: <order_1_id> , CustomerID<customer_1_id> , OrderStatus: <order_1_status>
    string output =
        "OrderId: " + std::to_string(id) + " ,"
        "CustomerID: " + std::to_string(customerId) + " "
        "OrderStatus: " + orderStatusToString(status);
    
    return output;

}

