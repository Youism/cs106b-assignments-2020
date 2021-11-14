/*
 * File: HumanPyramids.cpp
 * ---------------
 * A program that generates a Human Pyramid and calculates the weight that each individual is holding on his/her back.
 */

#include "HumanPyramids.h"
#include "Testing/HumanPyramidTests.h"
#include "hashmap.h"
#include "vector.h"
using namespace std;

// The constant WEIGHT holds the weight of each person, which in this example is held constant at 160 lb.
const double WEIGHT = 160;

/* Function: wrapperWeight
 * -----------------------
 * wrapperWeight takes as inputs the row and column of the person we want to explore, the height of the pyramid,
 * and a HashMap that does not require user input, as it is defined in a wrapper function below. The HashMap is passed
 * by reference as we intend to add new values to that map in each recursion. This function will look at the individual
 * and return how much weight that individual is holding.
 */
double wrapperWeight(int row, int col, int pyramidHeight, HashMap<Vector<int>, double>& table) {
    if(pyramidHeight<row+1||row<0||col>row||col<0||pyramidHeight<0) error("bound error!");
    if(row==0) return 0;
    if(row==1) return WEIGHT/2;

    if(table.containsKey({row,col})) return table[{row,col}];
    auto leftWeight=0.0,rightWeight=0.0;
    if(col-1>=0) leftWeight=(wrapperWeight(row-1,col-1,pyramidHeight,table)+WEIGHT)/2;
    if(col<=row-1) rightWeight=(wrapperWeight(row-1,col,pyramidHeight,table)+WEIGHT)/2;
    table[{row,col}]=leftWeight+rightWeight;
    return table[{row, col}];
}

/*
 * Function: weightOnBackOf
 * -------------------------
 * This function is the one that the user will be calling. It takes as arguments the row, column and height of the pyramid
 * It defines an empty HashMap that will be used to store the weight of people for which we have already done the calculations,
 * implementing memoization. Then it returns the wrapperWeight function with the HashMap.
 */
double weightOnBackOf(int row, int col, int pyramidHeight) {
    HashMap<Vector<int>, double> table = {};
    return wrapperWeight(row, col, pyramidHeight, table);
}


/* * * * * * Test cases below this point. * * * * * */

ADD_TEST("Provided Test: Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

ADD_TEST("Provided Test: Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
    EXPECT_ERROR(weightOnBackOf(-1, -4, 20));
     EXPECT_ERROR(weightOnBackOf(4, 10, 20));
}

ADD_TEST("Provided Test: Warning! The next test will hang if you haven't implemented memoization.") {
    // Nothing to see here, folks. Move along...
}

ADD_TEST("Provided Test: Function returns a value, even for someone deep in the pyramid.") {
    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

ADD_TEST("Test case: Function reports an error in case height is a negative integer") {
    EXPECT_ERROR(weightOnBackOf(0,0,-1));
}
