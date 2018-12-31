// svg.cpp
//
// svg reads from standard input and writes to standard output. It writes
// an svg file for displaying the data set (input as a n-space vector in
// csv). The command line is read to get a scale factor for the display.
// Only the first two coordinates are plotted.
//
// This is intended to be used with other programs for generating test data
// sets for NearTree.
//

#include <vector>
#include <iostream>

#include "Data2CSV.h"
#include "vector_3d.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SVG_Color_Output(
    const long picSize,
    const std::vector<std::pair<Vector_3,std::string> > & points )
//---------------------------------------------------------------------
{
    const long sqSide = (long)(3. * picSize);
    fprintf( stdout, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fprintf( stdout, "<svg width=\"%ld\" height=\"%ld\" version=\"1.1\"\n", sqSide, sqSide);
    fprintf( stdout, "xmlns=\"http://www.w3.org/2000/svg\">\n");
    fprintf( stdout, "<rect width=\"%ld\" height=\"%ld\" style=\"fill:rgb(255,255,255); stroke-width:10;  stroke:rgb(0,0,0)\"/>\n",
        sqSide, sqSide);


    double maxx = -DBL_MAX;
    double minx = DBL_MAX;
    double maxy = -DBL_MAX;
    double miny = DBL_MAX;
    for( unsigned int i=0; i<points.size( ); ++i )
    {
       maxx = MAX(maxx,points[i].first[0]);
       maxy = MAX(maxy,points[i].first[1]);
       minx = MIN(minx,points[i].first[0]);
       miny = MIN(miny,points[i].first[1]);
    }

    const double scale = MAX( (maxx-minx), (maxy-miny) ) / (2.0*picSize);
    const double xcenter = (maxx + minx) / 2.0;
    const double ycenter = (maxy + miny) / 2.0;
    const double xPicCenter = sqSide / 2.0;
    const double yPicCenter = sqSide / 2.0;

    for( unsigned int i=0; i<points.size( ); ++i )
    {
       const int radius = 2;
       const double xd = (points[i].first[0] - xcenter) / scale - xPicCenter;
       const double yd = (points[i].first[1] - ycenter) / scale - yPicCenter;
       const int x = -int(xd);
       const int y = -int(yd);
       const std::string& color = points[i].second;
       fprintf(stdout, "<circle r=\"%d\" cx=\"%d\" cy=\"%d\" stroke=\"#%s\" stroke-width=\"2\" fill=\"#%s\"/>\n",
          radius, x, y, color.c_str(), color.c_str());
    }

    fprintf( stdout, "</svg>\n");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char* argv[])
//---------------------------------------------------------------------
{
    double scale = 0.0;
    // see if there is a scale factor on the command line
    if ( argc>1 )
        scale = atof( argv[1] );

    if ( scale <= 0.0 ) scale = 500.0;

    std::vector<std::pair< Vector_3, std::string> > v =ReadXYColor( std::cin );

    SVG_Color_Output( (long)scale, v );

    return 0;}


