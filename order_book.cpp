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

    if (head == nullptr) {
        cout << "No orders in the book for " << side << " side" << endl;
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

void OrderBook::OrderTreeQuantity(string side) {
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
        float total_value = n_order->quantity;
        while (n_order != nullptr) {
            if (n_order->next_order != nullptr) { 
                total_value += n_order->next_order->quantity;
            }
            n_order = n_order->next_order;
        }
        
        
        cout << "Price: " << head->price << " -- Total value " << total_value << endl;
        head = head->next_price;
    }
}

// Execute an order. 
// Returns the order to be added to the order book, if it still has outstanding quantity, empty otherwise
// TODO: take into account value over quantity, with order partial fill due to unsufficient funds
Order* OrderBook::execute_order(Order* new_order) {
    Order* new_insert = new Order(*new_order);
    if (new_order->side == "buy") {
        cout << "Executing buy orders" << endl;
        while (sell_head != nullptr) {
            if (new_insert->price >= sell_head->price) {
                cout << "Buying from order ID " << sell_head->id << endl;
                if (new_insert->quantity >= sell_head->quantity) {
                    // Existing buy order was met fully, removing from OB and freeing memory
                    new_insert->quantity -= sell_head->quantity;
                    sell_head->quantity = 0;
                    // Deleting depleted order
                    Order* cleanup_head = sell_head;
                    if (sell_head->next_order != nullptr) {
                        // Set head to next order at the same price
                        sell_head->next_order->parent = nullptr;
                        sell_head->next_order->next_price = sell_head->next_price;
                        sell_head->next_order->last_price_order = sell_head->last_price_order;

                        if (sell_head->next_price != nullptr) {
                            sell_head->next_price->parent = sell_head->next_order;
                        }
                        sell_head = sell_head->next_order;
                        delete cleanup_head;
                    } else if (sell_head->next_price != nullptr){
                        // Otherwise go to order with next price
                        sell_head->next_price->parent = nullptr;
                        sell_head = sell_head->next_price;
                        delete cleanup_head;
                    } else {
                        // There are no more orders in the book for the buy side, set heads to nullptr
                        sell_head = nullptr;
                        sell_tail = nullptr;
                        delete cleanup_head;
                    }

                } else {
                    sell_head->quantity -= new_insert->quantity;
                    new_insert->quantity = 0;
                    break;
                }
            } else break;
        }
    } else {
        cout << "Executing sell orders" << endl;
        while (buy_head != nullptr) {
            if (new_insert->price <= buy_head->price) {
                cout << "Buying from order ID " << buy_head->id << endl;
                if (new_insert->quantity >= buy_head->quantity) {
                    // Existing buy order was met fully, removing from OB and freeing memory
                    new_insert->quantity -= buy_head->quantity;
                    buy_head->quantity = 0;
                    // Deleting depleted order
                    Order* cleanup_head = buy_head;
                    if (buy_head->next_order != nullptr) {
                        // Set head to next order at the same price
                        buy_head->next_order->parent = nullptr;
                        buy_head->next_order->next_price = buy_head->next_price;
                        buy_head->next_order->last_price_order = buy_head->last_price_order;

                        if (buy_head->next_price != nullptr) {
                            buy_head->next_price->parent = buy_head->next_order;
                        }
                        buy_head = buy_head->next_order;
                        delete cleanup_head;
                    } else if (buy_head->next_price != nullptr){
                        // Otherwise go to order with next price
                        buy_head->next_price->parent = nullptr;
                        buy_head = buy_head->next_price;
                        delete cleanup_head;
                    } else {
                        // There are no more orders in the book for the buy side, set heads to nullptr
                        buy_head = nullptr;
                        buy_tail = nullptr;
                        delete cleanup_head;
                    }

                } else {
                    buy_head->quantity -= new_insert->quantity;
                    new_insert->quantity = 0;
                    break;
                }
            } else break;
        }
    }
    return new_insert;
}

int OrderBook::add_order(Order* new_order) {
    Order* new_insert = new Order(*new_order);
    if (new_order->side == "buy") {
        if (sell_head && new_order->price > sell_head->price) {
            // Check that the new buy order is lower than the sell head, otherwise execute it until it isnt
            cout << "executing sell orders to meet buy order" << endl;
            new_insert = this->execute_order(new_order);
            new_insert->print();
            if (new_insert->quantity == 0) {
                cout << "Order was fully executed" << endl;
                return 0;
            } 
        }
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
        if (buy_head && new_order->price < buy_head->price) {
            // Check that the new buy order is lower than the sell head, otherwise execute it until it isnt
            cout << "executing buy orders to meet sell post" << endl;
            new_insert = this->execute_order(new_order);
            new_insert->print();
            if (new_insert->quantity == 0) {
                cout << "Order was fully executed" << endl;
                return 0;
            } 
        }
        
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

int OrderBook::remove_order(Order* order) {
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

    // Fill buy side with orders
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
    cout << "-------------" << endl;
    order_book.OrderTreeQuantity("buy");

    // Fill sell side with orders
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "sell", 1.02, 10.0);
    order_book.add_order(&new_order);
    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "sell", 0.97, 50.0);
    order_book.add_order(&new_order);

    cout << "-------------" << endl;
    order_book.OrderTree("sell");
    cout << "-------------" << endl;
    order_book.OrderTreeValue("sell");


    // Test Order execution with a sell order crossing gap
    cout << "------------- TESTING SELL" << endl;
    new_order = make_new_order(order_counter++, "sell", 0.97, 50.0);
    order_book.add_order(&new_order);

    cout << "-------------" << endl;
    order_book.OrderTree("buy");
    order_book.OrderTreeValue("buy");
    cout << "-------------" << endl;
    order_book.OrderTree("sell");
    order_book.OrderTreeQuantity("sell");

    cout << "-------------" << endl;
    new_order = make_new_order(order_counter++, "sell", 0.95, 40.0);
    order_book.add_order(&new_order);

    cout << "-------------" << endl;
    order_book.OrderTree("buy");
    order_book.OrderTreeQuantity("buy");
    cout << "-------------" << endl;
    order_book.OrderTree("sell");
    order_book.OrderTreeQuantity("sell");

    // Test Order execution with a buy order crossing gap
    cout << "------------- TESTING BUY" << endl;
    new_order = make_new_order(order_counter++, "buy", 1.03, 80.0);
    order_book.add_order(&new_order);

    cout << "-------------" << endl;
    order_book.OrderTree("buy");
    order_book.OrderTreeQuantity("buy");
    cout << "-------------" << endl;
    order_book.OrderTree("sell");
    order_book.OrderTreeQuantity("sell");


    return 0;
}
