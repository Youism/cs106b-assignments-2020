/*
 * File: ShiftScheduling.cpp
 * ---------------
 * This program determines the most efficient scheduling of shifts for a worker, given a number of shifts where each shift generates
 * a certain output, or value, per shift. The worker has a maximum number of hours available per week, and this program allows the
 * user to understand what is the optimal allocation of hours to different shifts in the week.
 */

#include "ShiftScheduling.h"
#include "Testing/ShiftSchedulingTests.h"
#include "vector.h"
#include "error.h"
using namespace std;

/* Function: scheduleRec
 * -----------------------
 * scheduleRec is a recursive function that takes as input shifts, which holds the shifts for which a decision is yet to be made
 * , and chosen, which holds the shifts for which decisions have already been made. The function ensures that shifts do not
 * exceed the maximum number of hours the worker can work per week, and that the worker doesn't have concurrent shifts. The function
 * then calculates the value of each combination of shifts and 'picks' the winner, returning a HashSet with the highest value
 * schedule for the week.
 */
HashSet<Shift> scheduleRec(const HashSet<Shift>& shifts, const HashSet<Shift>& chosen, int maxHours) {
    if(maxHours<0) error("The working hour cannot be zero");
    if(shifts.isEmpty()) return chosen;

    auto first=shifts.first();
    auto overlaps=false;
    auto totalLength=0;
    for(auto c :chosen){
        totalLength+=lengthOf(c);
        if(overlapsWith(first,c)) overlaps=true;
    }
    HashSet<Shift> left{};
    if(!overlaps&&totalLength+lengthOf(first)<=maxHours)
        left=scheduleRec(shifts-first,chosen+first,maxHours);
    auto right=scheduleRec(shifts-first,chosen,maxHours);
    double leftValue=0,rightValue=0;
    for(auto l:left) leftValue+=valueOf(l);
    for(auto r:right) rightValue+=valueOf(r);
    return leftValue>rightValue?left:right;
}

/* Function: highestValueScheduleFor
 * -----------------------
 * This function takes as input a constant HashSet of shifts that describe all the possible shifts a worker can have in a given week
 * and the maximum number of hours the worker can work. It returns a HashSet with the optimal allocation of shifts to maximize value,
 * or output, in the week.
 */
HashSet<Shift> highestValueScheduleFor(const HashSet<Shift>& shifts, int maxHours) {
    return scheduleRec(shifts, {}, maxHours);
}

/* * * * * * Test Cases * * * * * */


/* This nice utility function lets you call highestValueScheduleFor, passing in
 * a Vector of shifts rather than a Set. This makes it a bit easier to test things.
 * You shouldn't need this function outside of these test cases.
 */
HashSet<Shift> asSet(const Vector<Shift>& shifts) {
    HashSet<Shift> result;
    for (Shift s: shifts) {
        result += s;
    }
    return result;
}

ADD_TEST("User test: worker has 0 hours available this week") {
    HashSet<Shift> shifts = {
        { Day::TUESDAY, 10, 11, 1000 },  // Tuesday, 10AM - 11AM, value is 1000
    };
    EXPECT(highestValueScheduleFor(shifts, 0).isEmpty());
}

ADD_TEST("User test: Exact overlapping shifts have different values") {
    Vector<Shift> shifts = {
        { Day::WEDNESDAY, 9, 10, 10 },  // Wednesday,  9AM - 10AM, value is 0
        { Day::WEDNESDAY, 9, 10, 0},  // Wednesday, 9AM - 10AM, value is 10
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 100).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 100).contains(shifts[1]));
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

ADD_TEST("Provided Test: Pick a shift you do have time for.") {
    HashSet<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 24).size() == 1);
    EXPECT(highestValueScheduleFor(shifts, 24) == shifts);
}

ADD_TEST("Provided Test: Don't pick a shift you don't have time for.") {
    HashSet<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 3).isEmpty());
}

ADD_TEST("Provided Test: Don't pick overlapping shifts.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday,  9AM - 5PM, value is 1000
        { Day::MONDAY, 8, 18, 2000 },  // Monday, 10AM - 6PM, value is 2000
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 100).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 100).contains(shifts[1]));
}

ADD_TEST("Provided Test: Passes the example from the handout.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27 *
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28 *
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25 *
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25 *
    };

    /* Get back the solution. */
    HashSet<Shift> computedSolution = highestValueScheduleFor(asSet(shifts), 20);

    /* Form the correct answer. It's the starred entries. */
    HashSet<Shift> actualSolution = {
        shifts[0], shifts[1], shifts[2], shifts[14]
    };

    EXPECT(computedSolution == actualSolution);
}
