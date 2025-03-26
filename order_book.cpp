// Header file for input output functions
#include <iostream>
using namespace std;

#include "order_book.hpp"

// struct OrderBook orderbook;

int order_counter = 0;

void OrderBook::OrderTree(string side) {
    Order* head = nullptr;
    Order* n_order = nullptr;

    if (side == "buy") {
        head = buy_head;
    } else {
        head = sell_head;
    }

    // while(head && head->next_price != nullptr) {
    while(head != nullptr) {
        cout << "Price: " << head->price << " Order IDs " << head->id;
        n_order = head;
        while (n_order != nullptr) {
            if (n_order->next_order != nullptr) { 
                cout << " " << n_order->next_order->id;
            }
            n_order = n_order->next_order;
        }
        
        cout << endl;
        head = head->next_price;
    }
}


void OrderBook::OrderTreeValue(string side) {
    Order* head = nullptr;
    Order* n_order = nullptr;

    if (side == "buy") {
        head = buy_head;
    } else {
        head = sell_head;
    }

    // while(head && head->next_price != nullptr) {
    while(head != nullptr) {
        n_order = head;
        float total_value = n_order->get_value();
        while (n_order != nullptr) {
            if (n_order->next_order != nullptr) { 
                total_value += n_order->next_order->get_value();
            }
            n_order = n_order->next_order;
        }
        
        
        cout << "Price: " << head->price << " -- Total value " << total_value << endl;
        head = head->next_price;
    }
}

int OrderBook::add_order(Order* new_order) {
    Order* new_insert = new Order(*new_order);
    if (new_order->side == "buy") {
        if (buy_head == NULL) {
            cout << "Creating new head" << endl;
            new_insert->last_price_order = new_insert;
            buy_head = new_insert;
            buy_tail = new_insert;
        } else {
            Order* current_order = buy_head;
            // Assumes that the new buy order does not cross with the existing sell orders
            // Check if the new order is the highest buy price, replacing head_buy
            if (new_order->price > buy_head->price) {
                cout << "Replacing head" << endl;
                new_insert->next_price = buy_head;
                buy_head->parent = new_insert;
                buy_head = new_insert;
                buy_head->last_price_order = buy_head;
                return 0;
            }
            while (current_order != nullptr) {
                // case 1: new price matches existing buy order, add it to queue for that price
                // case 2: new lowest buy price: add to tail
                if (current_order->price == new_order->price) {
                    cout << "Adding a new order to existing price list" << endl;
                    new_insert->parent = current_order->last_price_order;
                    current_order->last_price_order->next_order = new_insert;
                    current_order->last_price_order = new_insert;
                    break;
                } else if (current_order->next_price == nullptr) {
                    //Create a new price tail
                    cout << "Creating new price" << endl;
                    current_order->next_price = new_insert;
                    new_insert->parent = buy_tail;
                    buy_tail = new_insert;
                    buy_tail->last_price_order = buy_tail;
                    buy_tail->next_price = nullptr;
                    break;
                } else {
                    cout << "Going down price list" << endl;
                    current_order = current_order->next_price;
                }
            }
        }
    } else {
        
        if (sell_head == NULL) {
            cout << "Creating new head" << endl;
            new_insert->last_price_order = new_insert;
            sell_head = new_insert;
            sell_tail = new_insert;
        } else {
            Order* current_order = sell_head;
            // Assumes that the new buy order does not cross with the existing sell orders
            // Check if the new order is the highest buy price, replacing head_buy
            if (new_order->price < sell_head->price) {
                cout << "Replacing head" << endl;
                new_insert->next_price = sell_head;
                sell_head->parent = new_insert;
                sell_head = new_insert;
                sell_head->last_price_order = sell_head;
                return 0;
            }
            while (current_order != nullptr) {
                // case 1: new price matches existing buy order, add it to queue for that price
                // case 2: new lowest buy price: add to tail
                cout << "current order price: " << current_order->price << endl;
                cout << "new order price: " << new_order->price << endl;
                if (current_order->price == new_order->price) {
                    cout << "Adding a new order to existing price list" << endl;
                    // while (current_order->next_order != nullptr) {
                    //     current_order = current_order->next_order;
                    // }
                    // current_order->next_order = new_insert;
                    // new_insert->parent = current_order;
                    new_insert->parent = current_order->last_price_order;
                    current_order->last_price_order->next_order = new_insert;
                    current_order->last_price_order = new_insert;
                    break;
                } else if (current_order->next_price == nullptr) {
                    //Create a new price tail
                    cout << "Creating new price" << endl;
                    current_order->next_price = new_insert;
                    new_insert->parent = sell_tail;
                    sell_tail = new_insert;
                    sell_tail->last_price_order = sell_tail;
                    break;
                } else {
                    cout << "Going down price list" << endl;
                    current_order = current_order->next_price;
                }
            }
        }
    }
    return 0;
}

Order make_new_order(int id, string side, double price, double quantity) {
    Order new_order;
    new_order.id = id;
    new_order.side = side;
    new_order.price = price;
    new_order.quantity = quantity;
    new_order.next_price = nullptr;
    new_order.next_order = nullptr;
    new_order.parent = nullptr;

    return new_order;
}


int main() {
  
    cout << "Making Order Book\n";
    OrderBook order_book;

    cout << "Adding random orders to buy and sell side\n";

    Order new_order = make_new_order(order_counter++, "buy", 1.0, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.99, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.99, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.98, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.98, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.98, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 0.97, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "buy", 1.01, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;

    order_book.OrderTree("buy");
    cout << "-------------" << endl;
    order_book.OrderTreeValue("buy");

    
    new_order = make_new_order(order_counter++, "sell", 1.02, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;


    order_book.OrderTree("sell");

    return 0;
}
