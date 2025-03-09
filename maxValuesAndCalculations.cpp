#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <cmath>
#include "UI_Functions.h"
#include "Classes.h"  // Include only the header for the classes, not the .cpp file.

void F_SetMaxB(std::vector<C_BudgetCategory>& categories) {
    float maxBudget;
    std::string input;

    while (true) {
        std::cout << "Please enter your maximum budget for all categories: ";
        std::cin >> input;
        try {
            maxBudget = std::stof(input);
            if (maxBudget <= 0) {
                std::cout << "Sorry, your budget is invalid. Please enter a positive budget.\n";
                continue;
            }
            break;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Sorry, your budget is invalid. Please enter a numerical value.\n";
        }
    }

    // Now update all categories
    for (auto& category : categories) {
        category.SetSubBudget(maxBudget);  // Replaced setBudget with SetSubBudget
    }

    std::cout << "Your maximum budget for all categories is set to: " << maxBudget << std::endl;
}

void F_SetBRange(C_BudgetCategory& category, float maxBudget) {
    float subBudget;
    std::string input;

    while (true) {
        std::cout << "Please enter your budget for " << category.Name << ": ";
        std::cin >> input;
        try {
            subBudget = std::stof(input);
            if (subBudget <= 0) {
                std::cout << "Sorry, your budget is invalid. Please enter a positive value for the budget.\n";
                continue;
            }
            if (subBudget > maxBudget) {
                std::cout << "Sorry, your budget exceeds the maximum budget (" << maxBudget << "). Please enter a budget smaller or equal to the maximum.\n";
                continue;
            }
            break;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Sorry, your budget value is invalid. Please enter a numerical value.\n";
        }
    }

    category.SetSubBudget(subBudget);  // Replaced setBudget with SetSubBudget
    std::cout << "Budget for category '" << category.Name << "' has been set to: $" << subBudget << std::endl;
}


void F_CalcAndBreakDown(int BRange, std::vector<C_BudgetCategory>& categories, float maxBudget) {
    float totalTotal = 0;

    for (auto& category : categories) {
        if (BRange == 0) {  // Ensure time frame exists
            std::cout << "Time frame for category '" << category.Name << "' is not set. Skipping calculation.\n";
            continue;
        }

        float categoryTotal = 0;
        std::cout << "\nCategory: " << category.Name << "\n";

        categoryTotal = category.CalcAvg(BRange);

        totalTotal += categoryTotal;
        std::cout << "Category total for " << category.Name << ": " << categoryTotal << std::endl;
    }

    std::cout << "\nTotal expenses across all categories: " << totalTotal << std::endl;

    if (totalTotal > maxBudget) {
        std::cout << "You are over budget by: " << totalTotal - maxBudget << std::endl;
    }
    else {
        std::cout << "You have " << maxBudget - totalTotal << " remaining in your budget." << std::endl;
    }
}