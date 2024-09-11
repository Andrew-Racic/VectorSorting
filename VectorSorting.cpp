//============================================================================
// Name        : VectorSorting.cpp
// Author      : Andrew Racic
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <vector>  //Added this to include the directive for std::vector
#include <cstdlib> //Added for atof
#include <string> 

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declaration of strToDouble function.
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Forward declaration for loadBids function
vector<Bid> loadBids(string csvPath);

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";  //Prompts user for bidId
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";  //Prompts user for title.
    getline(cin, bid.title);

    cout << "Enter fund: ";  //Prompts user for fund.
    cin >> bid.fund;

    cout << "Enter amount: ";  //Prompts user for amount and converts it to double.
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the end
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {  //Remove the specified character from the string.
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());  //Convert the string to a double
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    int low = begin;
    int high = end;
    string pivot = bids[(begin + end) / 2].title;  //Use the middle element as the pivot.

    while (true) {  //Increment low index while bids[low].title is less than pivot
        while (bids[low].title < pivot) {
            ++low;
        }
        while (bids[high].title > pivot) {  //Decrement high index while bids[high].title is greater than pivot.
            --high;
        }
        if (low >= high) {  //If low index is greater than or equal to high index, partioning is done.
            return high;
        }
        swap(bids[low], bids[high]);  //Swap the elements at low and high indices
        ++low;
        --high;
    }
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    if (begin >= end) {
        return; // Base case: already sorted or empty
    }
    int pivotIndex = partition(bids, begin, end);  //Partition the vector and get the pivot index
    //Recursively sort the two partitions.
    quickSort(bids, begin, pivotIndex);
    quickSort(bids, pivotIndex + 1, end);
}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    size_t size = bids.size();
    //Loop over each position in the vector.
    for (size_t pos = 0; pos < size - 1; ++pos) {
        int minIndex = pos;

        // Find the minimum title among remaining elements.
        for (size_t i = pos + 1; i < size; ++i) {
            if (bids[i].title < bids[minIndex].title) {
                minIndex = i;
            }
        }  //Swap the minimum element with the current position.
        swap(bids[pos], bids[minIndex]);
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all bids.
    vector<Bid> bids;

    // Define a timer variable.
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {  //Display menu options.
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            //Load the bids from the CSV file.
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result.
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read.
            for (size_t i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

            // FIXME (1b): Invoke the selection sort and report timing results
        case 3:
            ticks = clock();  //Initialize a timer variable before sorting.
            selectionSort(bids);  //Perform selection sort on the bids.
            ticks = clock() - ticks;  //Calculate elapsed time and display result.
            cout << "Selection sort completed in " << ticks << " clock ticks." << endl;
            cout << "Selection sort completed in " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds." << endl;
            break;

            // FIXME (2b): Invoke the quick sort and report timing results
        case 4:
            ticks = clock();  //Initialize a timer variable before sorting.
            quickSort(bids, 0, bids.size() - 1);  //Perform a quicksort on the bids.
            ticks = clock() - ticks;  //Calculate elapsed time and display result.
            cout << "Quick sort completed in " << ticks << " clock ticks." << endl;
            cout << "Quick sort completed in " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds." << endl;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}