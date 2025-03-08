#include "UI_Functions.h"

// Function to generate and display the menu
void F_MenuGen(const vector<string>& menuItems) {
    cout << "===== MENU =====" << endl;
    F_MenuList(menuItems);
    cout << "Enter an option: ";
}

// Helper function to list menu items in order
void F_MenuList(const vector<string>& menuItems) {
    for (size_t i = 0; i < menuItems.size(); i++) {
        cout << i + 1 << ". " << menuItems[i] << endl;
    }
}
