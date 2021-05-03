#ifndef COLOR_H
#define COLOR_H

#include <string>
using namespace std;

class Color
{
private:    
    string _colorList[12];
public:
    Color(){
        _colorList[0] = "LightCoral";
        _colorList[1] = "DarkMagenta";
        _colorList[2] = "DarkOrange";
        _colorList[3] = "Yellow";
        _colorList[4] = "DarkKhaki";
        _colorList[5] = "Lavender";
        _colorList[6] = "LightPink";
        _colorList[7] = "LawnGreen";
        _colorList[8] = "SteelBlue";
        _colorList[9] = "Teal";
        _colorList[10] = "SaddleBrown";
        _colorList[11] = "Cyan";
    }
    ~Color(){}

    string operator [] (int n) const { return _colorList[n%12]; }
    float opacity (int n) const { return (n/12)%2?0.3:1.0; }
};

#endif