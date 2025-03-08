#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Function to generate and display a menu
void F_MenuGen(const vector<string>& menuItems);

// Helper function to list menu items
void F_MenuList(const vector<string>& menuItems);

// Template function to safely get user input with validation
template <typename T>
T F_GetUI(const string& prompt, const string& errorMessage) {
    T input;
    cout << prompt;
    while (!(cin >> input)) {
        cout << errorMessage << endl;
        cin.clear();  // Clear error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
        cout << prompt;
    }
    return input;
}

#endif // UI_FUNCTIONS_H
