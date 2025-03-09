#include <iostream>
#include "Classes.h"
#include "UI_Functions.h"
#include <vector>
#include <string>

void addCategory(std::vector<C_BudgetCategory>& categories);
void addExpenseToCategory(std::vector<C_BudgetCategory>& categories);
void listCategoriesAndExpenses(const std::vector<C_BudgetCategory>& categories);
void deleteCategory(std::vector<C_BudgetCategory>& categories);
void deleteExpense(std::vector<C_BudgetCategory>& categories);
void analyzeBudget(const std::vector<C_BudgetCategory>& categories);
void renameCategory(std::vector<C_BudgetCategory>& categories);
void renameExpense(std::vector<C_BudgetCategory>& categories);
void setCategoryTimeFrame(std::vector<C_BudgetCategory>& categories, int selection);

void F_MainMenu(std::vector<C_BudgetCategory>& categories) {
    int selection = 0;
    float maxBudget = 0;
    
    // Main loop for the menu
    while (selection != 10) {
        std::cout << "1. List all categories\n";
        std::cout << "2. Add new category\n";
        std::cout << "3. Add new expense\n";
        std::cout << "4. Delete category\n";
        std::cout << "5. Delete expense\n";
        std::cout << "6. Rename category\n";
        std::cout << "7. Rename expense\n";
        std::cout << "8. Calculate and break down\n";
        std::cout << "9. Set time frame for a category\n";
        std::cout << "10. Exit\n";
        
        std::cout << "Please enter a number representing your selection: ";
        std::cin >> selection;

        switch (selection) {
            case 1:
                listCategoriesAndExpenses(categories); 
                break;
            case 2:
                addCategory(categories); 
                break;
            case 3:
                addExpenseToCategory(categories); 
                break;
            case 4:
                deleteCategory(categories); 
                break;
            case 5:
                deleteExpense(categories); 
                break;
            case 6:
                renameCategory(categories); 
                break;
            case 7:
                renameExpense(categories); 
                break;
            case 8:
                analyzeBudget(categories); 
                break;
            case 9:
                {
                    int categoryTimeFrame = 0;
                    std::cout << "Enter time frame (in days) for category: ";
                    std::cin >> categoryTimeFrame;
                    setCategoryTimeFrame(categories, categoryTimeFrame);
                    break;
                }
            case 10:
                std::cout << "Exiting program.\n"; 
                break;
            default:
                std::cout << "Invalid selection.\n"; 
                break;
        }
    }
}

void listCategoriesAndExpenses(const std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }

    for (const auto& cat : categories) {
        std::cout << "Category: " << cat.Name << (cat.IsNec ? " (Necessary)" : "") << "\n";
        if (cat.Expenses.empty()) {
            std::cout << "  No expenses recorded.\n";
        } else {
            for (const auto& exp : cat.Expenses) {
                std::cout << "  - " << exp.Name << "\n";
            }
        }
    }
}

void addCategory(std::vector<C_BudgetCategory>& categories) {
    std::string name = F_GetUI<std::string>("Enter category name: ", "Invalid input. Try again.");
    bool isNec = F_GetUI<int>("Is this category necessary? (1 = Yes, 0 = No): ", "Please enter 1 for Yes or 0 for No.") == 1;
    categories.emplace_back(name, isNec);
    std::cout << "Category '" << name << "' added successfully.\n";
}

void addExpenseToCategory(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available. Please add a category first.\n";
        return;
    }
    
    std::string catName = F_GetUI<std::string>("Enter category name to add an expense to: ", "Invalid input. Try again.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    std::string expName = F_GetUI<std::string>("Enter expense name: ", "Invalid input. Try again.");
    int expSep = F_GetUI<int>("Enter days between purchases: ", "Invalid input. Enter a number.");
    int subVal = F_GetUI<int>("Enter subjective value (1-10): ", "Invalid input. Enter a number between 1 and 10.");
    
    it->Expenses.emplace_back(expName, *it, expSep, subVal);
    std::cout << "Expense '" << expName << "' added to category '" << catName << "'.\n";
}

void deleteCategory(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }

    std::string catName = F_GetUI<std::string>("Enter category name to delete: ", "Invalid input.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    categories.erase(it);
    std::cout << "Category deleted.\n";
}

void deleteExpense(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }

    std::string catName = F_GetUI<std::string>("Enter category name to delete an expense from: ", "Invalid input.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    std::string expName = F_GetUI<std::string>("Enter expense name to delete: ", "Invalid input.");
    auto expIt = std::find_if(it->Expenses.begin(), it->Expenses.end(), [&expName](C_CategoryItem& exp) { return exp.Name == expName; });
    if (expIt == it->Expenses.end()) {
        std::cout << "Expense not found.\n";
        return;
    }

    it->Expenses.erase(expIt);
    std::cout << "Expense deleted.\n";
}

void renameCategory(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }

    std::string oldName = F_GetUI<std::string>("Enter category name to rename: ", "Invalid input.");
    auto it = std::find_if(categories.begin(), categories.end(), [&oldName](C_BudgetCategory& cat) { return cat.Name == oldName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    std::string newName = F_GetUI<std::string>("Enter new category name: ", "Invalid input.");
    it->Name = newName;
    std::cout << "Category renamed to '" << newName << "'.\n";
}

void renameExpense(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }

    std::string catName = F_GetUI<std::string>("Enter category name to rename an expense in: ", "Invalid input.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    std::string oldExpName = F_GetUI<std::string>("Enter expense name to rename: ", "Invalid input.");
    auto expIt = std::find_if(it->Expenses.begin(), it->Expenses.end(), [&oldExpName](C_CategoryItem& exp) { return exp.Name == oldExpName; });
    if (expIt == it->Expenses.end()) {
        std::cout << "Expense not found.\n";
        return;
    }

    std::string newExpName = F_GetUI<std::string>("Enter new expense name: ", "Invalid input.");
    expIt->Name = newExpName;
    std::cout << "Expense renamed to '" << newExpName << "'.\n";
}

void analyzeBudget(const std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available to analyze.\n";
        return;
    }

    for (const auto& cat : categories) {
        float total = 0;
        for (const auto& exp : cat.Expenses) {
            // Check if AvgExp has a value safely using a const reference
            if (exp.AvgExp) {  // This is a shorthand for checking has_value() in std::optional
                total += exp.AvgExp;  // Dereference safely
            }
        }
        std::cout << "Category: " << cat.Name << " - Total Expenses: " << total << "\n";
    }
}





void setCategoryTimeFrame(std::vector<C_BudgetCategory>& categories, int categoryTimeFrame) {
    if (categories.empty()) {
        std::cout << "No categories available to set time frame.\n";
        return;
    }

    std::string catName = F_GetUI<std::string>("Enter category name to set time frame: ", "Invalid input.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }

    // Set the time frame for the category here (for now, we're just printing the value).
    std::cout << "Setting time frame of " << categoryTimeFrame << " days for category '" << catName << "'.\n";
}