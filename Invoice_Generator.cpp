#include <iostream>

#include <iomanip>

#include <vector>

#include <string>

#include <ctime>    // For date and time stamp

#include <sstream>  // For generating unique invoice number

#include <algorithm>

using namespace std;



// Item class

class Item {

    string name, category;

    int quantity;

    double price;

public:

    Item(string n, string c, int q, double p) : name(n), category(c), quantity(q), price(p) {}



    double getTotal() const { return quantity * price; }

    string getName() const { return name; }

    string getCategory() const { return category; }

    int getQuantity() const { return quantity; }

    double getPrice() const { return price; }



    void displaySimple() const {

        cout << setw(20) << left << name

             << setw(10) << left << quantity

             << setw(10) << fixed << setprecision(2) << getTotal() << endl;

    }



    void displayDetailed() const {

        cout << setw(20) << left << name

             << setw(10) << left << category

             << setw(10) << left << quantity

             << setw(10) << fixed << setprecision(2) << price

             << setw(10) << fixed << setprecision(2) << getTotal() << endl;

    }



    void setQuantity(int q) { quantity = q; }

    void setPrice(double p) { price = p; }

};



// Abstract base class for invoice templates

class Template {

public:

    virtual void generateInvoice(const vector<Item>& items, double discount, double taxRate) const = 0;

    virtual ~Template() {}

};



// Simple template class

class SimpleTemplate : public Template {

public:

    void generateInvoice(const vector<Item>& items, double discount, double taxRate) const override {

        double total = 0;

        cout << setw(20) << left << "Item" << setw(10) << left << "Qty" << setw(10) << left << "Total" << endl;

        cout << "----------------------------------------------" << endl;



        for (const auto& item : items) {

            item.displaySimple();

            total += item.getTotal();

        }



        cout << "----------------------------------------------" << endl;

        cout << setw(30) << right << "Subtotal: " << fixed << setprecision(2) << total << endl;



        // Discount and tax

        double discountAmount = total * (discount / 100.0);

        double taxAmount = (total - discountAmount) * (taxRate / 100.0);

        double finalTotal = total - discountAmount + taxAmount;



        cout << setw(30) << right << "Discount (" << discount << "%): -" << discountAmount << endl;

        cout << setw(30) << right << "Tax (" << taxRate << "%): +" << taxAmount << endl;

        cout << setw(30) << right << "Grand Total: " << fixed << setprecision(2) << finalTotal << endl;

    }

};



// Detailed template class

class DetailedTemplate : public Template {

public:

    void generateInvoice(const vector<Item>& items, double discount, double taxRate) const override {

        double total = 0;

        cout << setw(20) << left << "Item" << setw(10) << left << "Category" << setw(10) << left << "Qty" << setw(10) << left << "Price" << setw(10) << left << "Total" << endl;

        cout << "-------------------------------------------------------------------------" << endl;



        for (const auto& item : items) {

            item.displayDetailed();

            total += item.getTotal();

        }



        cout << "-------------------------------------------------------------------------" << endl;

        cout << setw(40) << right << "Subtotal: " << fixed << setprecision(2) << total << endl;



        // Discount and tax

        double discountAmount = total * (discount / 100.0);

        double taxAmount = (total - discountAmount) * (taxRate / 100.0);

        double finalTotal = total - discountAmount + taxAmount;



        cout << setw(40) << right << "Discount (" << discount << "%): -" << discountAmount << endl;

        cout << setw(40) << right << "Tax (" << taxRate << "%): +" << taxAmount << endl;

        cout << setw(40) << right << "Grand Total: " << fixed << setprecision(2) << finalTotal << endl;

    }

};



// Helper function to generate unique invoice number

string generateInvoiceNumber() {

    static int counter = 1000;  // Start counter for invoice number

    time_t now = time(0);

    tm* ltm = localtime(&now);

    stringstream ss;

    ss << "INV" << (1900 + ltm->tm_year) << (1 + ltm->tm_mon) << ltm->tm_mday << "_" << counter++;

    return ss.str();

}



// Helper function to get current date and time

string getCurrentDateTime() {

    time_t now = time(0);

    tm* ltm = localtime(&now);

    stringstream ss;

    ss << (1 + ltm->tm_mon) << "/" << ltm->tm_mday << "/" << (1900 + ltm->tm_year) << " "

       << 1 + ltm->tm_hour << ":" << 1 + ltm->tm_min;

    return ss.str();

}



// Class to handle the invoice logic

class Invoice {

    vector<Item> items;

    Template* selectedTemplate;

    double discount;

    double taxRate;

    string customerName;

    string customerAddress;

    string invoiceNumber;

    string dateTime;

public:

    Invoice() : selectedTemplate(nullptr), discount(0), taxRate(0), invoiceNumber(generateInvoiceNumber()), dateTime(getCurrentDateTime()) {}



    void addItem(const Item& item) {

        items.push_back(item);

    }



    // Method to delete an item by its name

    void deleteItem(const string& itemName) {

        auto it = find_if(items.begin(), items.end(), [&](const Item& item) {

            return item.getName() == itemName;

        });

        if (it != items.end()) {

            items.erase(it);

            cout << "Item \"" << itemName << "\" has been removed from the invoice." << endl;

        } else {

            cout << "Item \"" << itemName << "\" not found in the invoice." << endl;

        }

    }



    // Method to modify the quantity and price of an existing item

    void modifyItem(const string& itemName, int newQuantity, double newPrice) {

        for (auto& item : items) {

            if (item.getName() == itemName) {

                item.setQuantity(newQuantity);

                item.setPrice(newPrice);

                cout << "Item \"" << itemName << "\" has been updated." << endl;

                return;

            }

        }

        cout << "Item \"" << itemName << "\" not found in the invoice." << endl;

    }



    void setCustomerInfo(const string& name, const string& address) {

        customerName = name;

        customerAddress = address;

    }



    void setDiscount(double d) {

        discount = d;

    }



    void setTaxRate(double t) {

        taxRate = t;

    }



    void chooseTemplate(int choice) {

        delete selectedTemplate;

        if (choice == 1)

            selectedTemplate = new SimpleTemplate();

        else if (choice == 2)

            selectedTemplate = new DetailedTemplate();

    }



    void generateInvoice() const {

        cout << "\n------------------- Invoice -------------------" << endl;

        cout << "Invoice Number: " << invoiceNumber << endl;

        cout << "Date: " << dateTime << endl;

        cout << "Customer: " << customerName << endl;

        cout << "Address: " << customerAddress << endl;

        cout << "------------------------------------------------" << endl;

        

        if (selectedTemplate)

            selectedTemplate->generateInvoice(items, discount, taxRate);

        else

            cout << "No template selected!" << endl;

    }



    string getInvoiceDetails() const {

        stringstream ss;

        ss << "Invoice Number: " << invoiceNumber << "\n"

           << "Date: " << dateTime << "\n"

           << "Customer: " << customerName << "\n"

           << "Address: " << customerAddress << "\n"

           << "Items in Invoice:\n";

        

        if (selectedTemplate) {

            // Displaying detailed information

            ss << setw(20) << left << "Item" << setw(10) << left << "Category" << setw(10) << left << "Qty" << setw(10) << left << "Price" << setw(10) << left << "Total" << endl;

            ss << "-------------------------------------------------------------------------\n";

            for (const auto& item : items) {

                ss << setw(20) << left << item.getName()

                   << setw(10) << left << item.getCategory()

                   << setw(10) << left << item.getQuantity()

                   << setw(10) << fixed << setprecision(2) << item.getPrice()

                   << setw(10) << fixed << setprecision(2) << item.getTotal() << endl;

            }

            ss << "-------------------------------------------------------------------------\n";

        }

        return ss.str();

    }



    vector<Item> getItems() const {

        return items;

    }



    ~Invoice() {

        delete selectedTemplate;

    }

};



// Invoice Manager class to manage multiple invoices

class InvoiceManager {

    vector<Invoice> invoices;



public:

    void createInvoice() {

        Invoice invoice;

        string name, address;

        

        cout << "Enter customer name: ";

        getline(cin >> ws, name);

        cout << "Enter customer address: ";

        getline(cin >> ws, address);

        invoice.setCustomerInfo(name, address);



        char addMoreItems = 'y';

        while (addMoreItems == 'y') {

            string itemName, itemCategory;

            int quantity;

            double price;



            cout << "Enter item name: ";

            getline(cin >> ws, itemName);

            cout << "Enter item category: ";

            getline(cin >> ws, itemCategory);

            cout << "Enter quantity: ";

            cin >> quantity;

            cout << "Enter price: ";

            cin >> price;



            Item item(itemName, itemCategory, quantity, price);

            invoice.addItem(item);



            cout << "Do you want to add another item? (y/n): ";

            cin >> addMoreItems;

        }



        char deleteOrModify = 'n';

        cout << "Do you want to delete or modify an item? (d/m/n): ";

        cin >> deleteOrModify;



        if (deleteOrModify == 'd') {

            string itemNameToDelete;

            cout << "Enter item name to delete: ";

            cin >> itemNameToDelete;

            invoice.deleteItem(itemNameToDelete);

        } else if (deleteOrModify == 'm') {

            string itemNameToModify;

            int newQuantity;

            double newPrice;

            cout << "Enter item name to modify: ";

            cin >> itemNameToModify;

            cout << "Enter new quantity: ";

            cin >> newQuantity;

            cout << "Enter new price: ";

            cin >> newPrice;

            invoice.modifyItem(itemNameToModify, newQuantity, newPrice);

        }



        double discount, taxRate;

        cout << "Enter discount percentage (or 0 for none): ";

        cin >> discount;

        invoice.setDiscount(discount);

        cout << "Enter tax rate percentage (or 0 for none): ";

        cin >> taxRate;

        invoice.setTaxRate(taxRate);



        int templateChoice;

        cout << "Select Invoice Template:\n1. Simple Template\n2. Detailed Template\nEnter your choice (1 or 2): ";

        cin >> templateChoice;

        invoice.chooseTemplate(templateChoice);



        invoice.generateInvoice(); // Generate current invoice

        invoices.push_back(invoice); // Store invoice

    }



    void viewInvoices() const {

        cout << "\n--------- All Generated Invoices ---------\n";

        for (const auto& invoice : invoices) {

            cout << invoice.getInvoiceDetails() << endl;

        }

    }

};



// Main function

int main() {

    InvoiceManager manager;

    char generateMore = 'y';



    while (generateMore == 'y') {

        manager.createInvoice();

        cout << "Do you want to generate another invoice? (y/n): ";

        cin >> generateMore;

    }



    char viewInvoicesOption;

    cout << "Would you like to view all past invoices? (y/n): ";

    cin >> viewInvoicesOption;



    if (viewInvoicesOption == 'y') {

        manager.viewInvoices();

    }



    return 0;

}
