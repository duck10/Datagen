// svg_color.cpp
//
// svg reads from standard input and writes to standard output. It writes
// an svg file for displaying the data set (input as a n-space vector in
// csv). The command line is read to get a scale factor for the display.
// Only the first two coordinates are plotted.
//
// This is intended to be used with other programs for generating test data
// sets for NearTree.
//

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <sstream>
#include <vector>

#include "CividisScaleSVG.h"
#include "Data2CSV.h"
#include "vector_3d.h"

void OutputComments( const std::vector<std::string>& comments) {

   if (!comments.empty()) {
      fprintf(stdout, "<!--\n");
      for (unsigned long i = 0; i < comments.size(); ++i)
         fprintf(stdout, "# %s\n", comments[i].c_str());
      fprintf(stdout, "-->\n");
   }
}

void OutputData_2D(
   const std::vector<std::pair<Vector_3, std::string> > & points, 
   std::vector<double>& distances, const double radius,
   const long sqSide,
   const long picSize) {

   double maxx = -DBL_MAX;
   double minx = DBL_MAX;
   double maxy = -DBL_MAX;
   double miny = DBL_MAX;
   for (unsigned int i = 0; i < points.size(); ++i)
   {
      maxx = MAX(maxx, points[i].first[0]);
      maxy = MAX(maxy, points[i].first[1]);
      minx = MIN(minx, points[i].first[0]);
      miny = MIN(miny, points[i].first[1]);
   }

   const double scale = MAX((maxx - minx), (maxy - miny)) / (2.0*picSize) * radius;
   const double xPicCenter = sqSide / 2.0;
   const double yPicCenter = sqSide / 2.0;
   const Vector_3 picCenter(xPicCenter, yPicCenter, 0.0);

   std::ostringstream ostr;
   for ( unsigned long i=0; i<points.size(); ++i ) {
      const Vector_3 x = points[i].first * distances[i];
      const Vector_3 xx = x/scale + picCenter;
      ostr << xx[0] << " " << xx[1] << ", ";
   }

   fprintf(stdout,
      "<polygon fill=\"white\" stroke=\"black\" stroke-width=\"5\" points=\"%s \" />", ostr.str().c_str());

   fprintf(stdout,
      "<text x=\"90\" y=\"1490\"  stroke=\"red\" font-family=\"sans-serif\" font-size=\"40\" fill=\"black\"> Range  %f to %f </text>\n", 
      *std::min_element(distances.begin(), distances.end()), *std::max_element(distances.begin(), distances.end()));

}

double GetRadius(const std::vector<std::string>& comments) {
   unsigned long i;
   double radius = DBL_MAX;
   for (i = 0; i < comments.size(); ++i)
      if (comments[i].find("Radius") != std::string::npos) {
         std::istringstream istr(comments[i]);
         std::string s;
         for (unsigned long i = 0; i < 4; ++i) istr >> s;
         istr >> radius;
         break;
      }
   return radius;
}

double GetMinValue(const std::vector<std::string>& comments) {
   unsigned long i;
   double minbest;
   for (i = 0; i < comments.size(); ++i)
      if (comments[i].find("min") != std::string::npos) {
         std::istringstream istr(comments[i]);
         std::string s;
         for (unsigned long i = 0; i < 5; ++i) istr >> s;
         istr >> minbest;
         break;
      }
   return minbest;
}

double GetMaxValue(const std::vector<std::string>& comments) {
   unsigned long i;
   double maxbest;
   for (i = 0; i < comments.size(); ++i) {
      std::string s = comments[i];
      if (s.find("max") != std::string::npos) {
         std::string stemp;
         std::istringstream istr(s);
         for (unsigned long i = 0; i < 5; ++i) istr >> stemp;
         istr >> maxbest;
         break;
      }
   }
   return maxbest;
}

/*
   std::cout << "# Title Delone variety and input centering " << deloneVariety << "  "  << latticeCentering << std::endl;
   std::cout << "# Radius of confusion " << radius << std::endl;
   std::cout << "# min/max best Sella values " << minbest << "  " << maxbest << std::endl;
*/

std::string  GetType(const std::vector<std::string>& comments) {
   unsigned long i;
   std::string type;
   std::string centering;
   for ( i=0; i<comments.size(); ++i )
      if (comments[i].find( "Delone") != std::string::npos) {
         std::string s;
         std::istringstream istr(comments[i]);
         for (unsigned long i = 0; i < 7; ++i) istr >> s;
         istr >> type;
         istr >> centering;
         break;
      }
   return type+" "+centering;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SVG_Line_Output(
   const long picSize,
   const std::vector<std::pair<Vector_3, std::string> > & points,
   std::vector<double>& distances,
   std::vector<std::string> comments)
   //---------------------------------------------------------------------
{
   const double radius = GetRadius(comments);
   const std::string deloneType = GetType(comments);
   const long sqSide = (long)(3. * picSize);
   fprintf(stdout, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
   fprintf(stdout, "<svg width=\"%ld\" height=\"%ld\" version=\"1.1\"\n", sqSide, sqSide);
   fprintf(stdout, "xmlns=\"http://www.w3.org/2000/svg\">\n");
   fprintf(stdout, "<rect width=\"%ld\" height=\"%ld\" style=\"fill:lightgrey; stroke-width:10;  stroke:rgb(0,0,0)\"/>\n",
      sqSide, sqSide);

   OutputComments(comments);
   OutputData_2D(points, distances, radius, sqSide, picSize);
   //const std::string cividisscale = Cividis::CividisScaleSVG(sqSide, 3);
   //fprintf(stdout, "%s", cividisscale.c_str());


   fprintf(stdout, "<text x=\"20\" y=\"80\" font-family=\"sans-serif\" font-size=\"60\"> %s </text>\n", deloneType.c_str());
   fprintf(stdout, "</svg>\n");
}

void SetColors(std::vector<std::pair< Vector_3, std::string> >& v,
   const std::vector<double>& distances) {
   const double minvalue = *std::min_element(distances.begin(), distances.end());
   const double maxvalue = *std::max_element(distances.begin(), distances.end());
   ColorRange cr(minvalue, maxvalue);

   for ( unsigned long i=0; i<v.size(); ++i ) {
      v[i].second = Cividis::OrdinalToCividisHexString(cr.ColorIndex(distances[i]));
   }
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

    std::vector<std::pair< Vector_3, std::string> > v;
    std::vector<std::string> comments;
    std::vector<double> distances;
    ReadXYZColor(std::cin, v, distances, comments);

    SetColors(v, distances);

    SVG_Line_Output( (long)scale, v, distances, comments );

    return 0;}


