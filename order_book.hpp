#include <iostream>
using namespace std;

double TICK_SIZE = 0.001;

class Order {
    public:
        int id;
        string side;
        double price;
        double quantity;
        Order* next_price;  // Pointer to order with next price, higher or lower based on side
        Order* next_order;  // Pointer to the next order with the same price, based on arrival time 
        Order* parent;


        void print(Order& order) {
            cout << "Order ID: " << order.id << endl;
            cout << "Order Side: " << order.side << endl;
            cout << "Order Price: " << order.price << endl;
            cout << "Order Quantity: " << order.quantity << endl;
        }

        double get_value(Order& order) {
            return order.price * order.quantity;
        }
};

class OrderBook {
    public:
        Order* head;
        Order* tail;
        Order* buy_head;
        Order* buy_tail;
        Order* sell_head;
        Order* sell_tail;

        int add_order(Order* new_order) {
            if (new_order->side == "buy") {
                if (buy_head == NULL) {
                    buy_head = new_order;
                    buy_tail = new_order;
                } else {
                    Order* current_order = buy_head;
                    while (current_order != NULL) {
                        if (current_order->price == new_order->price) {
                            new_order->next_order = current_order->next_order;
                            current_order->next_order = new_order;
                            break;
                        } else if (current_order->price > new_order->price) {
                            new_order->next_price = current_order;
                            current_order = new_order;
                            break;
                        } else {
                            current_order = current_order->next_price;
                        }
                    }
                }
            } else {
                if (sell_head == NULL) {
                    sell_head = new_order;
                    sell_tail = new_order;
                } else {
                    Order* current_order = sell_head;
                    while (current_order != NULL) {
                        if (current_order->price == new_order->price) {
                            new_order->next_order = current_order->next_order;
                            current_order->next_order = new_order;
                            break;
                        } else if (current_order->price < new_order->price) {
                            new_order->next_price = current_order;
                            current_order = new_order;
                            break;
                        } else {
                            current_order = current_order->next_price;
                        }
                    }
                }
            }
            return 0;
        }

};