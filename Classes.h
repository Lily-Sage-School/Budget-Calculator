#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class C_BudgetCategory; // Forward declaration to prevent circular dependency issues.

// C_CategoryItem class.
class C_CategoryItem {
public:
    std::string Name; // Name for each instance of C_CategoryItem.
    std::vector<float> IndExps; // A list of floats representing individual expenses.
    float AvgExp; // The average expense, calculated based on IndExps.
    float SubBudget;
    float _budget;
    int timeFrame;
    int* timeFrameDays;
    bool hasAvgExp; // Flag indicating whether the average expense has been calculated.
    C_BudgetCategory& ParCat; // Reference to the parent budget category.
    int ExpSep; // The approximated number of days between individual purchases of this expense.
    int SubVal; // The subjective value the user assigns to this expense.

    float getBudget() const {return _budget;}
    void setBudget(float budget) { _budget = budget;}

    // Constructor.
    C_CategoryItem(const std::string& Name, C_BudgetCategory& Parent, int ExpSep, int SubVal,
        const std::vector<float>& IndExps = {}, float AvgExp = 0.0f, bool hasAvgExp = false)
: Name(Name), ParCat(Parent), ExpSep(ExpSep), SubVal(SubVal), IndExps(IndExps),
AvgExp(AvgExp), hasAvgExp(hasAvgExp), timeFrame(0), timeFrameDays(nullptr), SubBudget(0.0f), _budget(0.0f) {}

    // Methods.
    void SelfDelete();
    void SelfDelNoEl();
    void DelEntry(float value);
    void CalcAvg();
};

// C_BudgetCategory class.
class C_BudgetCategory {
public:
    std::string Name; // Name for display and searching.
    bool IsNec; // Indicates whether the category is considered necessary by the user.
    float SubBudget; // A secondary budget limit for this category (no longer optional).
    std::vector<C_CategoryItem> Expenses; // List of all associated category items (expenses).

    // Constructor.
    C_BudgetCategory(const std::string& Name, bool IsNec, float SubBudget = 0.0f)
    : Name(Name), IsNec(IsNec), SubBudget(SubBudget) {}
    // Methods.
    void DeleteEntry(const std::string& ItemName);
    void InvDelEntry(const std::string& ItemName);
    void SelfDelete();
    void AddEntry(const C_CategoryItem& Item);
    C_CategoryItem* GetEntry(const std::string& ItemName);
    float CalcAvg(int BRange);

     void SetSubBudget(float budget) {
        SubBudget = budget;  // Setting the sub-budget value for the category
    }

    void SetBudget(float budget) {
        for (auto& item : Expenses) {
            item.setBudget(budget);  // Assuming you want to set the same budget for all items
        }
    }

    void SetTimeFrame(int days) {
        for (auto& item : Expenses) {
            item.timeFrame = days;  // Assuming you want to set the same time frame for all items
        }
    }

    int GetTimeFrame() const {
        return Expenses.empty() ? 0 : Expenses.front().timeFrame; // Return the time frame of the first item (can be adjusted)
    }

    int* gettimeFrameDays() {
        return Expenses.empty() ? nullptr : Expenses.front().timeFrameDays;
    }
};

// Functions.
C_BudgetCategory F_CreCat(const std::string& Name, bool IsNec, float SubBudget = 0.0f);
C_CategoryItem F_CreItem(const std::string& Name, C_BudgetCategory& Parent, int ExpSep, int SubVal, 
                         const std::vector<float>& IndExps = {}, float AvgExp = 0.0f, bool hasAvgExp = false);

void F_ReCat(C_BudgetCategory& cat, const std::string& newName);
void F_ReItem(C_CategoryItem& item, const std::string& newName);

#endif // CLASSES_H