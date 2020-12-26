#include <stdio.h>
#include "life.h"

// You are only allowed to change the contents of this file with respect 
// to improving the performance of this program



// If you need to initialize some data structures for your implementation
// of life then make changes to the following function, otherwise 
// do not change this function.

void initLife() {
}



// When completed this file will contain several versions of the life() function and 
// conditional compilation will be used to determine which version runs.  Each 
// version will be surrounded with  an ifdef, undef, and endif as illustrated 
// immediately below this comment block. To select the version of the life function 
// to use, put the #define USE USE immediately infront of the version of the life() 
// to compile.  Your version of life() must produce a correct result. 

// You are required to document the changes you make in the README.txt file. For each entry
// in the README.txt file there is to be a version of the matching  life() function here such
// that the markers can see, and run if needed,  the actual code that you used 
// to produce your results for the logged change to the life function.  


#ifdef USE
int life(long oldWorld[N][N], long newWorld[N][N]) {
  return 0;
}
#undef USE
#endif



#define USE


// For each version of life you are testing duplicate the code between the 
// ifdef and endif and make your changes. To use the new version, move the the #define 
// just about this comment to be just in front of the ifdef for the new version.

#ifdef USE

int life(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j, k, l;
  
  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      newWorld[i][j] =  0;
    }

    int col_l, col_r, row_t, row_b, lr_sum, cur_val;
    int col, row;
    int q = -1;

    // Log2
    // Modification: change the loop to access array in row-major order.
    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
    //         evicted.
    // Count the cells to the top left
//    for (j = 0; j < N; j++) {
//        row = (j - 1 + N ) % N;
//        for (i = 0; i < N; i++) {
//            col = (i -1 + N) % N;
//            newWorld[j][i] += oldWorld[row][col];
//        }
//    }

    // Log3
    // Modification: change the loop to access array in row-major order.
    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
    //         evicted.
    // Count the cells immediately above
//    for (j = 0; j < N; j++) {
//        row = (j - 1 + N ) % N;
//        for (i = 0; i < N; i++) {
//            newWorld[j][i] += oldWorld[row][i];
//        }
//    }

    // Log4
    // Modification: change the loop to access array in row-major order.
    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
    //         evicted.
    // Count the cells to the top right
//    for (j = 0; j < N; j++) {
//        row = (j - 1  + N ) % N;
//        for (i = 0; i < N; i++) {
//            col = (i + 1 + N) % N;
//            newWorld[j][i] += oldWorld[row][col];
//        }
//    }

//    // Log11
//    // Modification: Merge the three for-loops for adding top right, immediately above, and top left cells into one
//    //               for-loop.
//    // Reason: Intead of doing three for-loops seperately, doing them in one for-loop can reduce the number of accessing
//    //         data, so that the program would be speeded up.
//     for (j = 0; j < N; j++) {
//        row = (j - 1  + N ) % N;
//        for (i = 0; i < N; i++) {
//            col_l = (i - 1 + N) % N;
//            col_r = (i + 1 + N) % N;
//            newWorld[j][i] += (oldWorld[row][i] + oldWorld[row][col_l] + oldWorld[row][col_r]);
//        }
//     }

//    // Log5
//    // Modification: change the loop to access array in row-major order.
//    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
//    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
//    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
//    //         evicted.
//    // Count the cells to the immediate left
//    for (j = 0; j < N; j++) {
//        for (i = 0; i < N; i++) {
//            col = (i -1 + N) % N;
//            newWorld[j][i] += oldWorld[j][col];
//        }
//    }
//
//    // Log6
//    // Modification: change the loop to access array in row-major order.
//    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
//    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
//    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
//    //         evicted.
//    // Count the cells to the immediate right
//    for (j = 0; j < N; j++) {
//        for (i = 0; i < N; i++) {
//            col = (i + 1 + N) % N;
//            newWorld[j][i] += oldWorld[j][col];
//        }
//    }

//    // log12
//    //  Modification: Merge the three for-loops for adding the current row.
//    //  Reason: Merging into one for-loop could reduce the number of accessing same part of data,
//    //          so that the speed of the program would be increased.
//    for (j = 0; j < N; j++) {
//        for (i = 0; i < N; i++) {
//            col_r = (i + 1 + N) % N;
//            col_l = (i -1 + N) % N;
//            newWorld[j][i] += oldWorld[j][col_r] + oldWorld[j][col_l];
//        }
//    }

//    // Log7
//    // Modification: change the loop to access array in row-major order.
//    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
//    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
//    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
//    //         evicted.
//    // Count the cells to the bottom left
//    for (j = 0; j < N; j++) {
//        row = (j + 1 + N ) % N;
//        for (i = 0; i < N; i++) {
//            col = (i - 1 + N) % N;
//            newWorld[j][i] += oldWorld[row][col];
//        }
//    }
//
//    // Log8
//    // Modification: change the loop to access array in row-major order.
//    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
//    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
//    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
//    //         evicted.
//    // Count the cells immediately below
//    for (j = 0; j < N; j++) {
//        row = (j + 1  + N ) % N;
//        for (i = 0; i < N; i++) {
//            newWorld[j][i] += oldWorld[row][i];
//        }
//    }
//
//    // Log9
//    // Modification: change the loop to access array in row-major order.
//    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
//    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
//    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
//    //         evicted.
//    // Count the cells to the bottom right
//    for (j = 0; j < N; j++) {
//        row = (j + 1  + N ) % N;
//        for (i = 0; i < N; i++) {
//            col = (i + 1 + N) % N;
//            newWorld[j][i] += oldWorld[row][col];
//        }
//    }

//    // Log13
//    // Modification: Merge the three for-loops for adding the next row.
//    // Reason: Merging into one for-loop could reduce the number of accessing same part of data,
//    //         so that the speed of the program would be increased.
//    for (j = 0; j < N; j++) {
//        row = (j + 1  + N ) % N;
//        for (i = 0; i < N; i++) {
//            col_r = (i + 1 + N) % N;
//            col_l = (i -1 + N) % N;
//            newWorld[j][i] += (oldWorld[row][i] + oldWorld[row][col_l] + oldWorld[row][col_r]);
//        }
//    }

    // Log14
    // Modification: Calculation the value of the current cell, the one above the current cell,
    //               and the one below the current cell together.
    // Reason: Merge three steps into one big step to reduce data access,
    //         so that the speed of the program would be increased.
    for (i = 0; i < N; i++) {
        row_t = (i - 1 + N ) % N;
        row_b = (i + 1 + N ) % N;
        for (j = 0; j < N; j++) {
            col_l = (j - 1 + N) % N;
            col_r = (j + 1 + N) % N;
            lr_sum = oldWorld[i][col_l] + oldWorld[i][col_r];
            newWorld[row_t][j] += lr_sum + oldWorld[i][j];
            newWorld[i][j] += lr_sum;
            newWorld[row_b][j] += lr_sum + oldWorld[i][j];
        }
    }

    // Check each cell to see if it should come to life, continue to live, or die
    int alive = 0;

    // Log10
    // Modification: change the loop to access array in row-major order.
    // Reason: Initially, when it was accessing data in column-major order, every time it reach a new column,
    //         there would be some misses even though we've loaded it before but got evicted, so we need to load again.
    //         Therefore, when accessing data in row-major order, we can use all lines we've loaded before it got
    //         evicted.
    //printWorld(newWorld);
    for (j = 0; j < N; j++)
        for (i = 0; i < N; i++) {
            newWorld[j][i] = checkHealth(newWorld[j][i], oldWorld[j][i]);
            alive += newWorld[j][i] ? 1:0;
        }

    return alive;
}
#undef USE
#endif
