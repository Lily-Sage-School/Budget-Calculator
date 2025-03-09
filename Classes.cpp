#include "Classes.h" 
#include <cmath>

//==============================
// C_CategoryItem Methods for manipulating the class.
//==============================

C_CategoryItem::C_CategoryItem(const std::string& Name, C_BudgetCategory& Parent, int ExpSep, int SubVal, 
                               const std::vector<float>& IndExps, float AvgExp, bool hasAvgExp)
: Name(Name), ParCat(Parent), ExpSep(ExpSep), SubVal(SubVal), IndExps(IndExps), AvgExp(AvgExp), hasAvgExp(hasAvgExp) {}

// Regular deletion method. Use this when an expense is supposed to delete itself.
void C_CategoryItem::SelfDelete() {
    IndExps.clear();
    ParCat.InvDelEntry(Name);
    delete this;
}

// Top-down deletion method: Deletes the item instance without reaching out to the parent category.
// This lets category deletion methods clean out items within that category without having to make a couple of function calls.
void C_CategoryItem::SelfDelNoEl() {
    IndExps.clear();
    delete this;
}

// Array entry deletion method: Use this to remove a specific expense from the list.
void C_CategoryItem::DelEntry(float Value) {
    auto it = std::find(IndExps.begin(), IndExps.end(), Value);
    if (it != IndExps.end()) {
        IndExps.erase(it);
    }
}

// Average calculation method. Attempts to calculate the average expense of the instance if it's not already present.
void C_CategoryItem::CalcAvg() {
    if (IndExps.empty()) {
        AvgExp = 0.0f; // Default average value when no expenses exist
        hasAvgExp = false; // No average calculated
        return;
    }
    float sum = 0;
    for (float val : IndExps) {
        sum += val;
    }
    AvgExp = sum / IndExps.size();
    hasAvgExp = true; // Set the flag to true, indicating the average has been calculated
}


//==============================
// C_BudgetCategory Methods
//==============================

C_BudgetCategory::C_BudgetCategory(const std::string& name, bool isNec, float subBudget)
    : Name(name), IsNec(isNec), SubBudget(subBudget) {}

void C_BudgetCategory::DeleteEntry(const std::string& itemName) {
    auto it = std::remove_if(Expenses.begin(), Expenses.end(),
        [&](C_CategoryItem& item) {
            if (item.Name == itemName) {
                item.SelfDelNoEl();
                return true;
            }
            return false;
        });
    Expenses.erase(it, Expenses.end());
}

// Remove an instance from the vector without informing it. Used when normally deleting an expense.
void C_BudgetCategory::InvDelEntry(const std::string& itemName) {
    auto it = std::remove_if(Expenses.begin(), Expenses.end(),
        [&](const C_CategoryItem& item) {
            return item.Name == itemName;
        });
    Expenses.erase(it, Expenses.end());
}

// Delete all expenses and then die. Used when deleting whole categories.
void C_BudgetCategory::SelfDelete() {
    for (auto& item : Expenses) {
        item.SelfDelNoEl();
    }
    Expenses.clear();
    delete this;
}

// Add an expense to Expenses.
void C_BudgetCategory::AddEntry(const C_CategoryItem& item) {
    Expenses.push_back(item);
}

// Return an expense for use in other places.
C_CategoryItem* C_BudgetCategory::GetEntry(const std::string& itemName) {
    for (auto& item : Expenses) {
        if (item.Name == itemName) {
            return &item;
        }
    }
    return nullptr;
}

// Calculate average cost for entire category over the whole budget period based on the time between each expense and their average costs.
float C_BudgetCategory::CalcAvg(int BRange) {
    float total = 0;
    for (auto& item : Expenses) {
        item.CalcAvg();
        if (item.hasAvgExp) { // Check if the average exists
            int multiplier = std::round(static_cast<float>(BRange) / item.ExpSep);
            total += item.AvgExp * multiplier;
        }
    }
    return total;
}


//==============================
// Global Functions
//==============================

// Function for automatically generating PascalCase versions of each name for use as class instance names.
std::string PascalCase(const std::string& Input) {
    std::string Result = "C_";
    bool capitalize = true;
    for (char c : Input) {
        if (c == ' ') {
            capitalize = true;
        } else {
            Result += capitalize ? toupper(c) : c;
            capitalize = false;
        }
    }
    return Result;
}

// Create new Category. 
C_BudgetCategory F_CreCat(const std::string& name, bool isNec, float subBudget) {
    return C_BudgetCategory(PascalCase(name), isNec, subBudget);
}

// Create a new Expense.
C_CategoryItem F_CreItem(const std::string& name, C_BudgetCategory& parent, int expSep, int subVal, 
                         const std::vector<float>& indExps, float AvgExp, bool hasAvgExp) {
    return C_CategoryItem(PascalCase(name), parent, expSep, subVal, indExps, AvgExp, hasAvgExp);
}

// Rename a Category. 
void F_ReCat(C_BudgetCategory& cat, const std::string& newName) {
    cat.Name = PascalCase(newName);
}

// Rename an Expense.
void F_ReItem(C_CategoryItem& item, const std::string& newName) {
    item.Name = PascalCase(newName);
}