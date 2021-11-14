#include "RisingTides.h"
#include "Testing/RisingTidesTests.h"
#include "queue.h"
#include"grid.h"
#include"gridlocation.h"
using namespace std;

Grid<bool> floodedRegionsIn(const Grid<double>& terrain,
                            const Vector<GridLocation>& sources,
                            double height) {
    /* TODO: Delete this line and the next four lines, and implement this function. */
    Queue<GridLocation> begin_location;
    Grid<bool> result(terrain.numRows(),terrain.numCols(),false);
    for(auto source :sources){
        if(terrain[source]<=height){
            result[source]=true;
            begin_location.enqueue(source);
        }
    }

    while (!begin_location.isEmpty()) {
        auto current_location=begin_location.dequeue();
//        if(terrain.inBounds())
        for(int i=-1;i<2;i+=2){
            // left and right
            if(terrain.inBounds(current_location.row,current_location.col+i)){
                if(terrain[current_location.row][current_location.col+i]<=height &&
                        result[current_location.row][current_location.col+i]==false){
                    result[current_location.row][current_location.col+i]=true;
                    //warning
                    begin_location.enqueue({current_location.row,current_location.col+i});
                }
            }
            // top and bottom
            if(terrain.inBounds(current_location.row+i,current_location.col)){
                if(terrain[current_location.row+i][current_location.col]<=height &&
                        result[current_location.row+i][current_location.col]==false){
                    result[current_location.row+i][current_location.col]=true;
                    //warning
                    begin_location.enqueue({current_location.row+i,current_location.col});
                }
            }
        }
    }
    return result;
}


/***** Test Cases Below This Point *****/

ADD_TEST("a plenty of sources") {
    Grid<double> world = {
        { 3, 0, 2 },
        { 0, 1, 0 },
        { 1, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 },
        {2,2},
        {1,1},
        {0,2}
    };

    /* Everything should flood; there are no barriers to stop the water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 2.0);
    Grid<bool> expected = {
        {  false,  true,  true },
        {  true,  true,  true },
        {  true,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}
ADD_TEST("Nothing gets wet if there are no water sources.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        // empty
    };

    /* There are no water sources, so nothing should be underwater. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Everything gets wet if all locations are below the water level.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Everything should flood; there are no barriers to stop the water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true,  true },
        {  true,  true,  true },
        {  true,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Water can't cross a vertical levee.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 0, 2, 0 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Only locations to the left of the barrier should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true, false, false },
        {  true, false, false },
        {  true, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Water can't cross a diagonal levee.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Water only flows in the four cardinal directions, so it can't
     * pass through the barrier. Only the top should be flooded.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Water can't flow diagonally.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 2, 0, 2 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* Water should be trapped in the center, since it can't move
     * diagonally.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false,  true, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Water can flow in all cardinal directions.") {
    Grid<double> world = {
        { 2, 0, 2 },
        { 0, 0, 0 },
        { 2, 0, 2 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* The water in this case should flow up, down, left, and right. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false,  true, false },
        {  true,  true,  true },
        { false,  true, false }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Water can flow from multiple sources.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 },
        { 2, 2 }
    };

    /* Everything except the levee should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false,  true },
        { false,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

ADD_TEST("Handles a large, empty world quickly.") {
    Grid<double> world(100, 100); // Large world, everything defaults to 0 height.
    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* This may take a long time to complete if the solution is inefficient. Look
     * for things like
     *
     * 1. passing around large objects by *value* rather than by *reference*,
     * 2. revisiting the same squares multiple times (e.g. flooding the same
     *    cell many times due to not checking if something is flooded),
     *
     * etc.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);

    /* Everything should be flooded. */
    for (int row = 0; row < world.numRows(); row++) {
        for (int col = 0; col < world.numCols(); col++) {
            EXPECT_EQUAL(water[row][col], true);
        }
    }
}
