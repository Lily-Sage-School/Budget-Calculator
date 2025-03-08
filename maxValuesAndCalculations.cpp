#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>


void F_SetMaxB(std::vector<C_BudgetCategory>& categories) {
    float maxBudget; //Floating point value used to set the maximum budget to all categories, value that budgets for subcategories will be checked against
    std::string input;

    //Loop used to ensure proper input

    while (true) {
        std::cout << "Please enter your maximum budget for all categories: ";
        std::cin >> input;

        //Try is used to convert the input to a float value
        try {
            maxBudget = std::stof(input);  //This then converts a string input to a float

            //If statement checks if the value is negative or null value
            if (maxBudget <= 0) {
                std::cout << "Sorry, your budget is invalid. Please enter a positive budget.\n";
                continue;
            }
            break;  //Break statement exits the loop if the input is valid
        }
        catch (const std::invalid_argument&) { //Catch statement is used in case the user inputs a string or char into the budget
            std::cout << "Sorry, your budget is invalid. Please enter a numerical value.\n";
        }
    }

    //This then applies the maximum budget to all categories
    for (auto& category : categories) {
        category.SubBudget = maxBudget;
    }
    //The output statement tells the user what the maximum budget is set to
    std::cout << "Your maximum budget for all categories is set to: " << maxBudget << std::endl;
}

void F_SetBRange(C_BudgetCategory& category, float maxBudget) {
    float subBudget; //Float variable that holds the value of the budget for a subcategory
    std::string input;

    //This loop is used to keep asking the user until a valid value is entered.
    while (true) {
        std::cout << "Please enter your budget for " << category.Name << ": "; //This prompts the user to input the value for the specific category they want used
        std::cin >> input;

        //This try statement is to convert the input to a float value.
        try {
            subBudget = std::stof(input);  //This converts the string input to float

            //This if statement checks if the value input is negative or null
            if (subBudget <= 0) {
                std::cout << "Sorry, your budget is invalid. Please enter a positive value for the budget.\n";
                continue;
            }

            //This if statement checks if the budget for the subcategory exceeds the maximum budget applied to all categories
            if (subBudget > maxBudget) {
                std::cout << "Sorry, your budget exceeds the maximum budget (" << maxBudget << "). Please enter a budget smaller or equal to the maximum.\n";
                continue;
            }

            break;  //Break statement to exit the loop if input is valid
        }
        catch (const std::invalid_argument&) { //Catch statement used to find a char or string value instead of a float value
            std::cout << "Sorry, your budget value is invalid. Please enter a numerical value.\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "Sorry, your budget is too large. Please enter a smaller value.\n";
        }
    }

    //This then sets the budget for this subcategory
    category.SubBudget = subBudget;
    std::cout << "The budget for " << category.Name << " is set to: " << subBudget << std::endl;
}
//This function allows the user to input the time frame that their budget shall last for in days
void F_SetTimeFrame(C_BudgetCategory& category) {
    std::string input;
    float timeFrame;  // This will store the number of days, allowing for both integers and floats

    while (true) {
        std::cout << "Please enter the time frame (in days) for the budget of category '" << category.Name << "': ";
        std::cin >> input;

        try {
            timeFrame = std::stof(input);  //This then converts the input to a float value
            if (timeFrame <= 0) { //This if statement checks if the value entered is negative or null, which then prompts the user to input the value again
                std::cout << "The time frame must be a positive number. Please try again.\n";
                continue; //This then continues to the break statement
            }
            break; //This then breaks the user out of the loop
        }
        catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Please enter the number of days for the time frame of your budget.\n";
        }
    }

    category.TimeFrameDays = timeFrame;  //This then stores the time frame in the category's time frame attribute

    std::cout << "Time frame for category '" << category.Name << "' is set to: "
        << timeFrame << " days.\n";
}


//This function is used to calculate the total expenses, the expenses per category, and the expenses per subcategory
void F_CalcAndBreakDown(const std::vector<C_BudgetCategory>& categories, float maxBudget) {
    float totalTotal = 0; // Total expenses for all categories

    //Loop through categories
    for (const auto& category : categories) {
        if (!category.TimeFrameDays) {
            std::cout << "Time frame for category '" << category.Name << "' is not set. Skipping calculation for this category.\n";
            continue; //Skip this category if time frame is not set
        }

        float categoryTotal = 0; //Total expenses for this category
        std::cout << "\nCategory: " << category.Name << "\n";

        //Loop through expenses (subcategories) within this category
        for (const auto& expense : category.Expenses) {
            float expenseTotal = 0;

            expense.CalcAvg(); //Calculate the average expense for the subcategory

            if (expense.AvgExp) {
                //Ensure that expense.AvgExp is available before using it.
                int multiplier = std::round(static_cast<float>(*category.TimeFrameDays) / expense.ExpSep);
                expenseTotal = expense.AvgExp.value() * multiplier; 

                std::cout << "  Subcategory: " << expense.Name << ", Total expense over time frame: " << expenseTotal << std::endl;
            }
            else {
                std::cout << "  Subcategory: " << expense.Name << " has no average expense calculated.\n";
            }

            categoryTotal += expenseTotal; 
        }

        totalTotal += categoryTotal; 
        std::cout << "Category total for " << category.Name << ": " << categoryTotal << std::endl;
    }

    std::cout << "\nTotal expenses across all categories: " << totalTotal << std::endl;

    //Check against max budget
    if (totalTotal > maxBudget) {
        std::cout << "Sorry, you are over budget by: " << totalTotal - maxBudget << std::endl;
    }
    else {
        std::cout << "You have " << maxBudget - totalTotal << " remaining in your budget." << std::endl;
    }
}

void F_MainMenu(std::vector<C_BudgetCategory>& categories) {
    int selection = 0;

    float maxBudget = 0;  //Define maxBudget variable globally for use in calculations

    while (selection != 12) {
        std::cout << "1. Load from file\n";
        std::cout << "2. Save to file\n";
        std::cout << "3. List all categories\n";
        std::cout << "4. Add new category\n";
        std::cout << "5. Add new expense\n";
        std::cout << "6. Delete category\n";
        std::cout << "7. Delete expense\n";
        std::cout << "8. Rename category\n";
        std::cout << "9. Rename expense\n";
        std::cout << "10. Calculate and break down\n";
        std::cout << "11. Set time frame for a category\n";
        std::cout << "12. Exit\n";

        std::cout << "Please enter a number representing your selection: ";
        std::cin >> selection;

        switch (selection) {
        case 1: F_LoadFromFile(); break;
        case 2: F_SaveToFile(); break;
        case 3: F_ListAll(); break;
        case 4: F_NewCategory(); break;
        case 5: F_NewExpense(); break;
        case 6: F_DelCat(); break;
        case 7: F_DelExp(); break;
        case 8: F_RenameCat(); break;
        case 9: F_RenameExp(); break;
        case 10: F_CalcAndBreakDown(categories, maxBudget); break;  //Use maxBudget here
        case 11: {
            std::string categoryName;
            std::cout << "Enter the name of the category for which you want to set the time frame: ";
            std::cin >> categoryName;

            auto category = std::find_if(categories.begin(), categories.end(),
                [&categoryName](const C_BudgetCategory& cat) {
                    return cat.Name == categoryName;
                });

            if (category != categories.end()) {
                F_SetTimeFrame(*category);
            }
            else {
                std::cout << "Category not found.\n";
            }
            break;
        }
        case 12: std::cout << "Exiting program.\n"; break;
        default: std::cout << "Invalid selection.\n"; break;
        }
    }
}
