#include <iostream>
#include "Classes.h"
#include "UI_Functions.h"
#include <vector>
#include <string>

void addCategory(std::vector<C_BudgetCategory>& categories);
void addExpenseToCategory(std::vector<C_BudgetCategory>& categories);
void listCategoriesAndExpenses(const std::vector<C_BudgetCategory>& categories);
void deleteCategoryOrExpense(std::vector<C_BudgetCategory>& categories);
void analyzeBudget(const std::vector<C_BudgetCategory>& categories);

int main() {
    std::vector<C_BudgetCategory> categories;

    while (true) {
        int selection = F_GetUI<int>("1. Add Category\n2. Add Expense\n3. List Categories\n4. Delete Category/Expense\n5. Analyze Budget\n6. Exit\nPlease choose an option: ", "Invalid selection. Please enter a number.");

        switch (selection) {
            case 1:
                addCategory(categories);
                break;
            case 2:
                addExpenseToCategory(categories);
                break;
            case 3:
                listCategoriesAndExpenses(categories);
                break;
            case 4:
                deleteCategoryOrExpense(categories);
                break;
            case 5:
                analyzeBudget(categories);
                break;
            case 6:
                return 0;
            default:
                std::cout << "Invalid selection. Please try again.\n";
        }
    }
    return 0;
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

void deleteCategoryOrExpense(std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available.\n";
        return;
    }
    
    std::string catName = F_GetUI<std::string>("Enter category name to delete an expense from (or delete the category itself): ", "Invalid input. Try again.");
    auto it = std::find_if(categories.begin(), categories.end(), [&catName](C_BudgetCategory& cat) { return cat.Name == catName; });
    if (it == categories.end()) {
        std::cout << "Category not found.\n";
        return;
    }
    
    std::string choice = F_GetUI<std::string>("Delete category entirely? (yes/no): ", "Invalid input.");
    if (choice == "yes") {
        categories.erase(it);
        std::cout << "Category deleted.\n";
        return;
    }
    
    std::string expName = F_GetUI<std::string>("Enter expense name to delete: ", "Invalid input. Try again.");
    auto expIt = std::find_if(it->Expenses.begin(), it->Expenses.end(), [&expName](C_CategoryItem& exp) { return exp.Name == expName; });
    if (expIt == it->Expenses.end()) {
        std::cout << "Expense not found.\n";
        return;
    }
    it->Expenses.erase(expIt);
    std::cout << "Expense deleted.\n";
}

void analyzeBudget(const std::vector<C_BudgetCategory>& categories) {
    if (categories.empty()) {
        std::cout << "No categories available to analyze.\n";
        return;
    }
    
    for (const auto& cat : categories) {
        float total = 0;
        for (const auto& exp : cat.Expenses) {
            if (exp.AvgExp) total += exp.AvgExp.value();
        }
        std::cout << "Category: " << cat.Name << " - Total Expenses: " << total << "\n";
    }
}
