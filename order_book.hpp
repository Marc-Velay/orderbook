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
        Order* last_price_order; // Pointer to the last order at the same price, based on arrival time
        Order* parent;

        // Default constructor
        Order() : id(0), side("buy"), price(0.0), quantity(0.0), next_price(nullptr), next_order(nullptr), parent(nullptr) {}

        // Copy constructor
        Order(const Order& other) {
            id = other.id;
            side = other.side;
            price = other.price;
            quantity = other.quantity;
            next_price = nullptr;  // Do not copy pointers, as this is a deep copy
            next_order = nullptr;
            parent = nullptr;
        }

        // Assignment operator
        Order& operator=(const Order& other) {
            if (this == &other) {
                return *this; // Handle self-assignment
            }
            id = other.id;
            side = other.side;
            price = other.price;
            quantity = other.quantity;
            next_price = nullptr;  // Do not copy pointers, as this is a deep copy
            next_order = nullptr;
            parent = nullptr;
            return *this;
        }


        void print() {
            cout << "Order ID: " << id << endl;
            cout << "Order Side: " << side << endl;
            cout << "Order Price: " << price << endl;
            cout << "Order Quantity: " << quantity << endl;
            cout << "Order Value: " << get_value() << endl;
        }

        double get_value() {
            return price * quantity;
        }
};

class OrderBook {
    public:
        // Order* head;
        // Order* tail;
        Order* buy_head;
        Order* buy_tail;
        Order* sell_head;
        Order* sell_tail;

        int add_order(Order* order);
        int remove_order(Order* order);
        Order* execute_order(Order* order);
        void OrderTree(string side);
        void OrderTreeValue(string side);
        void OrderTreeQuantity(string side);

};