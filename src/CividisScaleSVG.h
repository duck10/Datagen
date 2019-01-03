#ifndef CIVIDISSCALESVG_H
#define CIVIDISSCALESVG_H

#include <string>


class ColorRange {
public:
   ColorRange(const double minv, const double maxv);
   double ColorFraction(const double color);
   int ColorIndex(const double color);
private:
   const double m_min;
   const double m_max;
};


class Cividis {
public:
   static std::string CividisScaleSVG(const int height, const int strokeWidth);
   static std::string OrdinalToCividisHexString(const unsigned long n);
};

#endif // CIVIDISSCALESVG_H

