// NNStat.cpp
//
//
// This is intended to be used with other programs for generating test data
// sets for NearTree. NNStat is designed to provide some basic measures
// about the data sets for NearTree.
//
#include <iostream>
#include <cmath>

#include "TNear.h"

#include "Data2CSV.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char* argv[])
//---------------------------------------------------------------------
{
    std::vector<vecN> v = ReadGeneralFile( std::cin );
    bool Do_Random_Insertion = true;

    if ( argc > 1 )
    {
        if ( argv[1][0] == '?' )
        {
            fprintf( stdout, "CSV,targets,probes,oldTime,old,new,default,old/default,dim. Est,diam. Est,meanSpacing est,var spacing est\n" );
            exit(0);
        }
        else if ( argv[1][0] == 'i' || argv[1][0] == 'I'  )
        {
            Do_Random_Insertion = false;
        }

    }

    CNearTree<vecN> nt( v );

    if ( Do_Random_Insertion )
    {
        nt.CompleteDelayedInsertRandom( );
    }
    else
    {
        nt.CompleteDelayedInsert( );
    }

    fprintf( stdout, "Tree completed\n" );

    const double hausdorff = nt.GetDimEstimate( );

    const long depth = (long)nt.GetDepth( );

    fprintf( stdout, " v.size %ld\n  depth %ld\n dimension %d\n estimated-dimension %g\n",
        (long)nt.size( ), depth, v[0].dim, hausdorff );

    bool bCopyInput = false;
    if ( bCopyInput )
    {
        WriteVectorFile( v );
    }

	return 0;
}


