// testmaxdist.cpp
//
// testMaxDist is designed as a test harness for NearTree. It reads a csv file
// of general vectors. The number of data elements in each vector must be
// consistent throughout the entire input file, but the dimension is not
// constrained. Various tests are/can be executed and reported. Originally
// designed to connect to DataGen and a few related programs. However, it
// still contains functions such as testHamm that generate their own data.

#include <cfloat>
#include <climits>
#include <ctime>
#include <string>

#include "Data2CSV.h"

#include "tnear.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

int g_errorCount;

/*=======================================================================*/
// testGeneral
//
// Designed to test input data sets of arbitrary size and dimension.
// The input can come from any source, but the main in testMaxDist
// gets its input from standard input.
/*=======================================================================*/
void testGeneral( const bool Do_Random_Insertion, const std::vector<vecN>& v )
//---------------------------------------------------------------------
{
    CNearTree<vecN> nt( v );

    if ( Do_Random_Insertion )
    {
        nt.CompleteDelayedInsertRandom( );
    }
    else
    {
        nt.CompleteDelayedInsert( );
    }

    if ( nt.empty( ) )
    {
        fprintf( stdout, "No input found\n" );
        return;
    }

    const int nTests = 10000;
    nt.SetFlags( CNearTree<vecN>::NTF_NoPrePrune );

    //const clock_t tc1 = std::clock();
    double radius = 10.0;
    bool bTest = true;
    //while ( radius > 1.0-8 )
    for ( int i=0; i<20; ++i )
    {
        long nodevisits1 = (long)nt.GetNodeVisits( );
        for ( int i=0; i<nTests; ++i )
        {
            vecN newPoint(v[0].dim);
            const vecN probe = vecN(v[0].dim);
            bTest = nt.NearestNeighbor( radius, newPoint, probe );
        }
        long nodevisits2 = (long)nt.GetNodeVisits( );
        const std::string result = bTest ? std::string("OK") : std::string("Failed");

        fprintf( stdout, "CSV-balanced, %g, %ld,%d, %s, %.3f,%.3f,%.3f,%.3f,%.2f\n",
            radius,
            (long)nt.size( ),
            nTests,
            result.c_str(),
            //((double)(tc2-tc1))/CLOCKS_PER_SEC,
            (double)(nodevisits2-nodevisits1)/(double)nTests,
            nt.GetDimEstimate(),
            (double)nt.GetDiamEstimate(),
            (double)nt.GetMeanSpacing(),
            (double)nt.GetVarSpacing());
        radius *= 0.5;
    }
    /*----------------------------end balanced tree test--------------------------------------------*/
}

/*=======================================================================*/
// testInputVector
//
// confirm that the input data vector elements have consistent dimension
/*=======================================================================*/
bool testInputVector( const std::vector<vecN>& v )
//---------------------------------------------------------------------
{
    int maxDim = INT_MIN;
    int minDim = INT_MAX;

    for ( unsigned int i=0; i<v.size( ); ++i )
    {
        const vecN& vi = v[i];
        if ( maxDim < vi.dim ) maxDim = vi.dim;
        if ( minDim > vi.dim ) minDim = vi.dim;
    }

    return ( (maxDim==minDim) && (maxDim>0) );
}

/*=======================================================================*/
int main( int argc, char* argv[] )
//---------------------------------------------------------------------
{
    bool Do_Random_Insertion = true;

    if ( argc > 1 )
    {
        if ( argv[1][0] == '?' )
        {
            fprintf( stdout, "CSV,size,test,oldTime,old,new,default,old/default,dim. Est,diam. Est,meanSpacing est,var spacing est\n" );
        }
        else if ( argv[1][0] == 'i' || argv[1][0] == 'I'  )
        {
            Do_Random_Insertion = false;
        }

        exit(0);
    }

    const std::vector<vecN> v = ReadGeneralFile( std::cin );

    if ( ! testInputVector( v ) )
    {
        fprintf( stdout, "Inconsistent csv input\n" );
    }
    else
    {
        testGeneral( Do_Random_Insertion, v );
    }

    return 0;
}

