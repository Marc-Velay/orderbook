// Header file for input output functions
#include <iostream>
using namespace std;

#include "order_book.hpp"

struct OrderBook orderbook;

int order_counter = 0;

int add_order(OrderBook* ob, Order* new_order) {



    return 0;
}

Order make_new_order(int id, string side, double price, double quantity) {
    Order new_order;
    new_order.id = id;
    new_order.side = side;
    new_order.price = price;
    new_order.quantity = quantity;
    new_order.next_price = NULL;
    new_order.next_order = NULL;
    new_order.parent = NULL;

    return new_order;
}


int main() {
  
    cout << "Making Order Book\n";

    cout << "Adding random orders to buy and sell side\n";

    Order new_order = 


    return 0;
}
