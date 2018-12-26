// NNStat.cpp
//
//
// This is intended to be used with other programs for generating test data
// sets for NearTree. NNStat is designed to provide some basic measures
// about the data sets for NearTree.
//
#include <iostream>
#include <cmath>

#include <ctime>

#include "TNear.h"

#include "Data2CSV.h"

void Dummy( int, char*[] )
{
}

    CNearTree<vecN> nt;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char* argv[])
//---------------------------------------------------------------------
{
    Dummy( argc, argv );
    std::vector<vecN> v = ReadGeneralFile( std::cin );

    const clock_t tc0 = std::clock();
    nt.insert( v );
    const clock_t tc1 = std::clock();
    fprintf( stdout, "Build tree,%f,%d\n", double(tc1-tc0)/ (double)CLOCKS_PER_SEC, (int)nt.size( ) );

    double smallestNear = DBL_MAX;
    double largestNear = -DBL_MAX;
    double largest = -DBL_MAX;
    double avgNearest = 0.0;
    const double hausdorff = nt.GetDimEstimate( );
    const size_t n = nt.size( );

    if ( n <2 )
    {
        return( 1 );
    }

    std::vector<vecN> knear( v[0].dim );
    vecN far(nt[0]);
    for ( size_t i=0; i<n; ++i )
    {
        vecN probe( v[0].dim );
        knear.clear( );
        // one of the knear is the probe and the other (larger Norm) is the nearest
        probe = v[i];
        nt.FindK_NearestNeighbors( 2, DBL_MAX, knear, probe );
        const double small = MAX( (probe-knear[0]).Norm( ), (probe-knear[1]).Norm( ) );
        smallestNear = MIN( smallestNear, small );
        largestNear  = MAX( largestNear,  small );
        avgNearest += small;

        nt.FarthestNeighbor( far, probe );
        largest = MAX( largest, (probe-far).Norm( ) );
    }

    avgNearest /= double( n-1 );

    fprintf( stdout, " v.size %ld\n dimension %d\n smallestNear-near %g\n largest-near %g\n largest-far %g\n average nearest %g\n estimated-dimension %g\n", 
       (long)nt.size( ), v[0].dim, smallestNear, largestNear, largest, avgNearest, hausdorff );

    bool bCopyInput = false;
    if ( bCopyInput )
    {
        WriteVectorFile( v );
    }

	return 0;
}


