#include "Plotter.h"
#include"vector.h"
#include"strlib.h"
using namespace std;

void runPlotterScript(istream& input) {
    PenStyle style={1,"black"};
    double x0=0,y0=0,x1=0,y1=0;
    auto draw=false;
    for(string line;getline(input,line);){
        Vector<string> word=stringSplit(line," ");
        auto command=toLowerCase(word[0]);
        if(command=="penup") draw=false;
        else if(command=="pendown") draw=true;
        else if(command=="moveabs"){
            x1=stringToReal(word[1]);
            y1=stringToReal(word[2]);
            if(draw) drawLine(x0,y0,x1,y1,style);
            x0=x1;
            y0=y1;
        }
        else if(command=="moverel"){
            x1=stringToReal(word[1])+x0;
            y1=stringToReal(word[2])+y0;
            if(draw) drawLine(x0,y0,x1,y1,style);
            x0=x1;
            y0=y1;
        }
        else if(command=="pencolor"){
            style.color=word[1];
        }
        else if(command=="penwidth"){
            style.width=stringToReal( word[1]);
        }
    }
}
