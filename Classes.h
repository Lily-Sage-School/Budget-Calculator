#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <optional>

class C_BudgetCategory; //Forward declaration so there's no risk of errors or unexpected behavior from circular dependencies.

//C_CategoryItem class.
class C_CategoryItem {
public:
    std::string Name; //Name for each instance of C_CategoryItem.
    std::vector<float> IndExps; //A list of floats the user can add or subtract from. Meant to represent individual expenses the user can recall, which is used to provide more accurate averages.
    std::optional<float> AvgExp; //The average price of this expense.
    C_BudgetCategory& ParCat; //Which category does this expense belong to?
    int ExpSep; //The approximated number of days between individual instances of this expense: If you buy this thing, how long until you buy it again?
    int SubVal; //The subjective value the user sees in this expense.

    //Constructor.
    C_CategoryItem(const std::string& Name, C_BudgetCategory& Parent, int ExpSep, int SubVal, 
                   const std::vector<float>& IndExps = {}, std::optional<float> AvgExp = std::nullopt);

    //Methods.
    void SelfDelete();
    void SelfDelNoEl();
    void DelEntry(float value);
    void CalcAvg();
};

//C_BudgetCategory class.
class C_BudgetCategory {
public:
    std::string Name; //Name, used for display and searching.
    bool IsNec; //Whether or not the category is considered "necessary" by the user.
    std::optional<float> SubBudget; //An optional secondary budget exclusive to this category. Basically a limit on how much someone wants to spend here.
    std::vector<C_CategoryItem> Expenses; //A list of all C_CategoryItem instances that are associated with this instance. So for example if this category was entertainment, the Expenses list would have things like movies or games.

    //Constructor.
    C_BudgetCategory(const std::string& Name, bool IsNec, std::optional<float> SubBudget = std::nullopt);

    //Methods.
    void DeleteEntry(const std::string& ItemName);
    void InvDelEntry(const std::string& ItemName);
    void SelfDelete();
    void AddEntry(const C_CategoryItem& Item);
    C_CategoryItem* GetEntry(const std::string& ItemName);
    float CalcAvg(int BRange);
};

//Functions.
C_BudgetCategory F_CreCat(const std::string& Name, bool IsNec, std::optional<float> SubBudget = std::nullopt);
C_CategoryItem F_CreItem(const std::string& Name, C_BudgetCategory& Parent, int ExpSep, int SubVal, 
                         const std::vector<float>& IndExps = {}, std::optional<float> AvgExp = std::nullopt);

void F_ReCat(C_BudgetCategory& cat, const std::string& newName);
void F_ReItem(C_CategoryItem& item, const std::string& newName);

#endif //CLASSES_H
