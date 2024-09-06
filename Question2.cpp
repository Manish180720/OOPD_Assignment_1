#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cmath>

using namespace std;

const int MAX_YEARS = 30;
double growthRate[MAX_YEARS];
double inflationRate[MAX_YEARS];
int yearData[MAX_YEARS];
int recordCount = 0;

bool loadData(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == nullptr)
    {
        cerr << "Error: Unable to open file " << fileName << endl;
        return false;
    }

    char line[128];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        if (recordCount >= MAX_YEARS)
            break;

        int day, month, year;
        if (sscanf(line, "%d-%d-%d,%lf,%lf", &day, &month, &year, &growthRate[recordCount], &inflationRate[recordCount]) == 5)
        {
            yearData[recordCount] = year;
            recordCount++;
        }
    }

    fclose(file);
    return true;
}

int findYearIndex(int year)
{
    for (int i = 0; i < recordCount; i++)
    {
        if (yearData[i] == year)
            return i;
    }
    return -1;
}

double calculateAdjustedPrice(int startYear, int endYear, double cost, double &ltcgTax)
{
    int startIndex = findYearIndex(startYear);
    int endIndex = findYearIndex(endYear);

    if (startIndex == -1 || endIndex == -1 || startIndex > endIndex)
    {
        cerr << "Error: Invalid year range." << endl;
        return -1;
    }

    double adjustedPrice = cost;
    for (int i = startIndex; i < endIndex; i++)
    {
        adjustedPrice *= (1 + (growthRate[i]) / 100.0);
    }

    double gain = adjustedPrice - cost;
    ltcgTax = (gain > 0) ? gain * 0.125 : 0.0;

    return adjustedPrice;
}

void processTransaction()
{
    double costPrice;
    int purchaseYear, sellingYear;

    cout << "Enter the purchase price (Rs): ";
    cin >> costPrice;

    cout << "Enter the year of purchase: ";
    cin >> purchaseYear;

    cout << "Enter the year of selling: ";
    cin >> sellingYear;

    double ltcgTax;
    double finalSellingPrice = calculateAdjustedPrice(purchaseYear, sellingYear, costPrice, ltcgTax);

    if (finalSellingPrice < 0)
    {
        cerr << "Error occurred during calculation." << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Final selling price (inflation-adjusted): Rs " << finalSellingPrice << endl;
    cout << "LTCG Tax (20%): Rs " << ltcgTax << endl;
}

int main()
{
    const char *fileName = "price-inflation.csv";

    if (!loadData(fileName))
    {
        return 1;
    }

    processTransaction();

    return 0;
}
