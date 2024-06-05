#include "Volunteer.h"
#include "WareHouse.h"
#include <string>


Volunteer :: Volunteer (int id1, const string &name1) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id1), name(name1),  type("l") , canMove(false) {}

Volunteer :: ~Volunteer(){}

int Volunteer :: getId() const{
    return id;
}

const string &Volunteer :: getName() const{
    return name;
}

int Volunteer :: getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer :: getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer :: isBusy() const{
    return (activeOrderId != -1);
}

CollectorVolunteer :: CollectorVolunteer (int id1, const string &name1, int coolDown1) 
: Volunteer(id1, name1), coolDown(coolDown1),  timeLeft(NO_ORDER), type("Collector") , canMove(false){}

CollectorVolunteer* CollectorVolunteer :: clone() const {
    return new CollectorVolunteer(*this);
}

CollectorVolunteer :: ~CollectorVolunteer(){}

void CollectorVolunteer :: step(){
    canMove = false;
   if( isBusy() && decreaseCoolDown())
   {
    completedOrderId = activeOrderId;
    activeOrderId = -1;
    timeLeft = -1;
    canMove = true;
   }
 
}

int CollectorVolunteer :: getNumOrdersLeft() const{
    return -1;
}

int CollectorVolunteer :: getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer :: getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer :: decreaseCoolDown() {
    timeLeft--;
    if (timeLeft ==0)
        return true;
    else
        return false;
}
bool CollectorVolunteer :: hasOrdersLeft() const{
    return true;
}

bool CollectorVolunteer :: canTakeOrder (const Order &order) const{
    return (!isBusy());
}

void CollectorVolunteer :: acceptOrder (const Order &order){
    timeLeft = coolDown;
    activeOrderId = order.getId();
}

string CollectorVolunteer :: toString() const{
    return "later";
}
string CollectorVolunteer :: getType() const{
    return type;
}

bool CollectorVolunteer :: getCanMove() const{
    return canMove;
}


LimitedCollectorVolunteer :: LimitedCollectorVolunteer (int id1, const string &name1, int coolDown1, int maxOrders1) : CollectorVolunteer(id1, name1, coolDown1), maxOrders(maxOrders1) ,ordersLeft(maxOrders1)  {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer :: clone() const {
    return new LimitedCollectorVolunteer (*this);
}

bool LimitedCollectorVolunteer :: hasOrdersLeft() const{
    if (ordersLeft > 0)
        return true;
    else
        return false;
}

bool LimitedCollectorVolunteer :: canTakeOrder(const Order &order) const{
    return( !isBusy() && hasOrdersLeft() );
}
void LimitedCollectorVolunteer :: acceptOrder(const Order &order){
    CollectorVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft -1;
}

int LimitedCollectorVolunteer :: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer :: getNumOrdersLeft() const{
    return ordersLeft;
}



string LimitedCollectorVolunteer :: toString() const
{
    return "later";
}

DriverVolunteer :: DriverVolunteer (int id1, const string &name1, int maxDistance1, int distancePerStep1 ) : Volunteer(id1, name1), maxDistance(maxDistance1),  distancePerStep(distancePerStep1), distanceLeft(maxDistance1), type("Driver") , canMove(false){}

DriverVolunteer* DriverVolunteer :: clone() const {
    return new DriverVolunteer(*this);
}

DriverVolunteer :: ~DriverVolunteer(){}

string DriverVolunteer :: getType() const{
    return type;
}
int DriverVolunteer :: getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer :: getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer :: getDistancePerStep() const{
    return distancePerStep;
}

int DriverVolunteer :: getTimeLeft() const{
    return distanceLeft;
}

bool DriverVolunteer :: decreaseDistanceLeft()
{
    distanceLeft = distanceLeft - distancePerStep;
    if(distanceLeft <= 0)
    {
        distanceLeft = 0;
        return true;
    }
    else
        return false;
}

bool DriverVolunteer :: hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer :: canTakeOrder(const Order &order) const{
    if (maxDistance >= order.getOrderDistance() && !isBusy() )
        return true;
    else
        return false;
}

void DriverVolunteer :: acceptOrder( const Order &order) {
    distanceLeft = order.getOrderDistance();
    activeOrderId = order.getId();
}

void DriverVolunteer :: step(){
    canMove = false;
    if( isBusy() && decreaseDistanceLeft())
    {
        completedOrderId = activeOrderId;
        activeOrderId = -1;
        distanceLeft = 0;
        canMove = true;

    }
}

std :: string DriverVolunteer :: toString() const {
    return "later";
}

int DriverVolunteer :: getNumOrdersLeft() const{
    return -1;
}

bool DriverVolunteer :: getCanMove() const
{
    return canMove;
}


LimitedDriverVolunteer :: LimitedDriverVolunteer(int id1, const string &name1, int maxDistance1, int distancePerStep1,int maxOrders1) 
: DriverVolunteer(id1, name1, maxDistance1, distancePerStep1), maxOrders(maxOrders1), ordersLeft(maxOrders1) {}

LimitedDriverVolunteer* LimitedDriverVolunteer :: clone() const {
    return new LimitedDriverVolunteer (*this);
}

int LimitedDriverVolunteer :: getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer :: getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer :: hasOrdersLeft() const{
        if (ordersLeft > 0)
        return true;
    else
        return false;
}

bool LimitedDriverVolunteer :: canTakeOrder(const Order &order) const{
    return(!isBusy() && order.getOrderDistance() <= getMaxDistance() && hasOrdersLeft());
}

void LimitedDriverVolunteer :: acceptOrder(const Order &order){
    DriverVolunteer :: acceptOrder(order);
    ordersLeft = ordersLeft -1;
}


string LimitedDriverVolunteer :: toString() const
{
    return "fgh";
}

