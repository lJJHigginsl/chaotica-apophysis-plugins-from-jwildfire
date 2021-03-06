#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: recurrenceplot

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/RecurrencePlotFunc.java

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#include "datahelpers.h"



typedef struct
{
	double _a = 0.5;
	double _b = 10.0;
	double _N = 700;
	int _id;
	int _k = 5;
	double _scale = 1.0;
	double _angle = 0.0;
	double _offset = 0.0;
	double _maxdist = 5;
	double _y1;
	double _y2;
	int _dc;
	double _oldx = 0.0;
	double _oldy = 0.0;
	double _ldcs;

} Variables;

#define APO_VARIABLE_PREFIX "recurrenceplot_"
#include "plugin.h"

APO_PLUGIN("recurrenceplot");
APO_VARIABLES(


);


double F6 (Variation* vp, int i) {

    double Summe = 0;
    Summe = VAR(_a) / (1 + exp(-VAR(_b) * (i - VAR(_N) / 2.0) / VAR(_N)));
    return Summe;
}

double F22 (Variation* vp, int i, int b) {

    double Sum = 0, x;
    x = (2.0 * i) / VAR(_N) - 1;

    switch (VAR(_b)) {
      case 0:
        Sum = VAR(_a);
        break;
      case 1:
        Sum = VAR(_a) * x;
        break;
      case 2:
        Sum = VAR(_a) * (3.0 * pow(x, 2) - 1.0) / 2.0;
        break;
      case 3:
        Sum = VAR(_a) * (5.0 * pow(x, 3) - 3.0 * x) / 2.0;
        break;
      case 4:
        Sum = VAR(_a) * (35.0 * pow(x, 4) - 30.0 * pow(x, 2) + 3) / 8.0;
        break;
      case 5:
        Sum = VAR(_a) * (63.0 * pow(x, 5) - 70.0 * pow(x, 3) + 15.0 * x) / 8.0;
        break;
      case 6:
        Sum = VAR(_a) * (231.0 * pow(x, 6) - 315.0 * pow(x, 4) + 105.0 * pow(x, 2) - 5) / 16.0;
        break;
      case 7:
        Sum = VAR(_a) * (429.0 * pow(x, 7) - 693.0 * pow(x, 5) + 315.0 * pow(x, 3) - 35.0 * x) / 16.0;
        break;
      case 8:
        Sum = VAR(_a) * (6435.0 * pow(x, 8) - 12012.0 * pow(x, 6) + 6930.0 * pow(x, 4) - 1260.0 * pow(x, 2) + 35) / 128.0;
        break;
      case 9:
        Sum = VAR(_a) * (12155.0 * pow(x, 9) - 25740.0 * pow(x, 7) + 18018.0 * pow(x, 5) - 4620.0 * pow(x, 3) + 315.0 * x) / 128.0;
        break;
      case 10:
        Sum = VAR(_a) * (46189.0 * pow(x, 10) - 109395.0 * pow(x, 8) + 90090.0 * pow(x, 6) - 30030.0 * pow(x, 4) + 3465.0 * pow(x, 2) - 63) / 256.0;
        break;
    }
    return Sum;
}

double F23 (Variation* vp, int i, int k) {

    double j;
    double Summe = 0;
    for (j = 1; j <= VAR(_k); j++) {
      // Rechteck
      Summe = Summe + sin(i * (2 * j - 1) * VAR(_b) * 2 * M_PI / (VAR(_N))) / (2 * j - 1);
    }
    Summe = Summe * (4 * VAR(_a) / M_PI);
    return Summe;
}

double F24 (Variation* vp, int i, int k) {

    double j;
    double Summe = 0;
    for (j = 1; j <= VAR(_k); j++) {
      // Sägezahn
      Summe = Summe + sin(i * j * VAR(_b) * 2 * M_PI / (VAR(_N))) / j;
    }
    Summe = Summe * (-2 * VAR(_a) / M_PI);
    return Summe;
}

double F25 (Variation* vp, int i, int k) {

    double j;
    double Summe = 0;
    for (j = 1; j <= VAR(_k); j++) {
      // Gleichgerichteter Sinus
      Summe = Summe + cos(i * (2 * j) * VAR(_b) * 2 * M_PI / (VAR(_N))) / ((2 * j - 1) * (2 * j + 1));
    }
    Summe = (4 * VAR(_a) / M_PI) * (0.5 - Summe);
    return Summe;
}

double F26 (Variation* vp, int i, int k) {

    double j;
    double Summe = 0;
    for (j = 1; j <= VAR(_k); j++) {
      // Parabel
      Summe = Summe + pow(-1.0, j) * cos((i) * (2 * j) * VAR(_b) * 1 * M_PI / (VAR(_N))) / (j * j);
    }
    Summe = (VAR(_a) * M_PI * M_PI / 3.0) + 4.0 * VAR(_a) * Summe;
    return Summe;
}

double F27 (Variation* vp, int i) {

    double Sinc = 0;
    // Sinc
    Sinc = VAR(_a) * sin(VAR(_b) * M_PI * (i - 0.5 * VAR(_N)) / VAR(_N)) / (VAR(_b) * M_PI * (i - 0.5 * VAR(_N)) / VAR(_N));
    return Sinc;
}

double F28 (Variation* vp, int i) {

    double Gabor = 0;
    // Gabor
    Gabor = VAR(_a) * exp(-VAR(_b) * (((i - 0.5 * VAR(_N)) / VAR(_N)) * (i - 0.5 * VAR(_N)) / VAR(_N))) * cos(2.0 * VAR(_k) * M_PI * i / VAR(_N));
    return Gabor;
}

double F29 (Variation* vp, int i, int b) {

    double Summe, x;
    x = (2.0 * i) / VAR(_N) - 1;
    Summe = 0.0;
    switch (VAR(_b)) {
      case 0:
        Summe = VAR(_a);
        break;
      case 1:
        Summe = VAR(_a) * x;
        break;
      case 2:
        Summe = VAR(_a) * (2.0 * pow(x, 2) - 1.0);
        break;
      case 3:
        Summe = VAR(_a) * (4.0 * pow(x, 3) - 3.0 * x);
        break;
      case 4:
        Summe = VAR(_a) * (8.0 * pow(x, 4) - 8.0 * pow(x, 2) + 1);
        break;
      case 5:
        Summe = VAR(_a) * (16.0 * pow(x, 5) - 20.0 * pow(x, 3) + 5.0 * x);
        break;
      case 6:
        Summe = VAR(_a) * (32.0 * pow(x, 6) - 48.0 * pow(x, 4) + 18.0 * pow(x, 2) - 1);
        break;
      case 7:
        Summe = VAR(_a) * (64.0 * pow(x, 7) - 112.0 * pow(x, 5) + 56.0 * pow(x, 3) - 7.0 * x);
        break;
      case 8:
        Summe = VAR(_a) * (128.0 * pow(x, 8) - 256.0 * pow(x, 6) + 160.0 * pow(x, 4) - 32.0 * pow(x, 2) + 1);
        break;
      case 9:
        Summe = VAR(_a) * (256.0 * pow(x, 9) - 576.0 * pow(x, 7) + 432.0 * pow(x, 5) - 120.0 * pow(x, 3) + 9.0 * x);
        break;
      case 10:
        Summe = VAR(_a) * (512.0 * pow(x, 10) - 1280.0 * pow(x, 8) + 1120.0 * pow(x, 6) - 400.0 * pow(x, 4) + 50.0 * pow(x, 2) - 1);
        break;
      case 11:
        Summe = VAR(_a) * (1024.0 * pow(x, 11) - 2816.0 * pow(x, 9) + 2816.0 * pow(x, 7) - 1232.0 * pow(x, 5) + 220.0 * pow(x, 3) - 11.0 * x);
        break;
      case 12:
        Summe = VAR(_a) * (2048.0 * pow(x, 12) - 6144.0 * pow(x, 10) + 6912.0 * pow(x, 8) - 3584.0 * pow(x, 6) + 840.0 * pow(x, 4) - 72.0 * pow(x, 2) + 1);
        break;
    }
    return Summe;
}

boolean dynamicParameterExpansion (Variation* vp) {

    return true;
}

boolean dynamicParameterExpansion (Variation* vp, String pName) {

    // preset_id doesn't really expand parameters, but it changes them; this will make them refresh
    if (PARAM_ID.equalsIgnoreCase(pName)) return true;
    else return false;
}


int PluginVarPrepare(Variation* vp)
{
   VAR(_a) = 0.5;
   VAR(_b) = 10.0;
   VAR(_N) = 700;
   VAR(_k) = 5;
   VAR(_scale) = 1.0;
   VAR(_angle) = 0.0;
   VAR(_offset) = 0.0;
   VAR(_maxdist) = 5;
   VAR(_oldx) = 0.0;
   VAR(_oldy) = 0.0;
    VAR(_ldcs) = 1.0 / (VAR(_scale) == 0.0 ? 10E-6 : VAR(_scale));

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    int i, j;
    double dist, x, y;

    i = (int) ((VAR(_N)) * GOODRAND_01());
    j = (int) ((VAR(_N)) * GOODRAND_01());


    if (VAR(_id) == 0) {
      VAR(_y1) = 50 * sin(i * 4 * M_PI / (1 * VAR(_N))) + 25 * sin(i * 40 * M_PI / (1 * VAR(_N)));
      VAR(_y2) = 50 * sin(j * 4 * M_PI / (1 * VAR(_N))) + 25 * sin(j * 40 * M_PI / (1 * VAR(_N)));
    } else if (VAR(_id) == 1) {
      VAR(_y1) = 10 * sin(i * 5 * M_PI / (1 * VAR(_N))) * 5 * sin(i * 15 * M_PI / (1 * VAR(_N)));
      VAR(_y2) = 10 * sin(j * 5 * M_PI / (1 * VAR(_N))) * 5 * sin(j * 15 * M_PI / (1 * VAR(_N)));
    } else if (VAR(_id) == 2) {
      VAR(_y1) = 50 * cos(i * 24 * M_PI / (1 * VAR(_N)) + 5 * sin(i * 6 * M_PI / (1 * VAR(_N))));
      VAR(_y2) = 50 * cos(j * 24 * M_PI / (1 * VAR(_N)) + 5 * sin(j * 6 * M_PI / (1 * VAR(_N))));
    } else if (VAR(_id) == 3) {
      VAR(_y1) = 50 * sin(i * 20 * M_PI / (1 * VAR(_N))) + 50 * sin(i * 22 * M_PI / (1 * VAR(_N)) + M_PI);
      VAR(_y2) = 50 * sin(j * 20 * M_PI / (1 * VAR(_N))) + 50 * sin(j * 22 * M_PI / (1 * VAR(_N)) + M_PI);
    } else if (VAR(_id) == 4) {
      VAR(_y1) = 50 * sin(i * 20 * M_PI / (1 * VAR(_N))) + 10 * sin(i * 22 * M_PI / (1 * VAR(_N)));
      VAR(_y2) = 50 * sin(j * 20 * M_PI / (1 * VAR(_N))) + 10 * sin(j * 22 * M_PI / (1 * VAR(_N)));
    } else if (VAR(_id) == 5) {
      VAR(_y1) = 50 * sin(i * VAR(_a) * M_PI / (1 * VAR(_N)) + 1 * sin(i * VAR(_a) * M_PI / (1 * VAR(_N))));
      VAR(_y2) = 50 * sin(j * VAR(_a) * M_PI / (1 * VAR(_N)) + 1 * sin(j * VAR(_a) * M_PI / (1 * VAR(_N))));
    } else if (VAR(_id) == 6)  // Sigmoid Function
    {
      VAR(_y1) = F6(vp, i);
      VAR(_y2) = F6(vp, j);
    } else if (VAR(_id) == 7) {
      VAR(_y1) = (400 / M_PI) * (sin(i * M_PI / (0.5 * VAR(_N))) + sin(i * 3 * M_PI / (0.5 * VAR(_N))) / 3.0 + sin(i * 5 * M_PI / (0.5 * VAR(_N))) / 5.0 + sin(i * 7 * M_PI / (0.5 * VAR(_N))) / 7.0);
      VAR(_y2) = (400 / M_PI) * (sin(j * M_PI / (0.5 * VAR(_N))) + sin(j * 3 * M_PI / (0.5 * VAR(_N))) / 3.0 + sin(j * 5 * M_PI / (0.5 * VAR(_N))) / 5.0 + sin(j * 7 * M_PI / (0.5 * VAR(_N))) / 7.0);
    } else if (VAR(_id) == 8) {
      VAR(_y1) = (400 / M_PI * M_PI) * (sin(i * M_PI / (0.5 * VAR(_N))) - sin(i * 3 * M_PI / (0.5 * VAR(_N))) / 9.0 + sin(i * 5 * M_PI / (0.5 * VAR(_N))) / 25.0 - sin(i * 7 * M_PI / (0.5 * VAR(_N))) / 49.0);
      VAR(_y2) = (400 / M_PI * M_PI) * (sin(j * M_PI / (0.5 * VAR(_N))) - sin(j * 3 * M_PI / (0.5 * VAR(_N))) / 9.0 + sin(j * 5 * M_PI / (0.5 * VAR(_N))) / 25.0 - sin(j * 7 * M_PI / (0.5 * VAR(_N))) / 49.0);
    } else if (VAR(_id) == 9) // tanh(VAR(_a)*x*M_PI)
    {
//			 VAR(_y1)=(400/M_PI*M_PI)*( sin(i*M_PI/(0.5*VAR(_N))) - sin(i*3*M_PI/(0.5*VAR(_N)))/9.0 + sin(i*5*M_PI/(0.5*VAR(_N)))/25.0 );
//			 VAR(_y2)=(400/M_PI*M_PI)*( sin(j*M_PI/(0.5*VAR(_N))) - sin(j*3*M_PI/(0.5*VAR(_N)))/9.0 + sin(j*5*M_PI/(0.5*VAR(_N)))/25.0 );
      VAR(_y1) = tanh(VAR(_a) * i * M_PI / VAR(_N));
      VAR(_y2) = tanh(VAR(_a) * j * M_PI / VAR(_N));
    } else if (VAR(_id) == 10) // tan(VAR(_a)* x * PI)
    {
      //		 VAR(_y1)=(400/M_PI*M_PI)*( sin(i*M_PI/(0.5*VAR(_N))) - sin(i*3*M_PI/(0.5*VAR(_N)))/9.0 );
      //		 VAR(_y2)=(400/M_PI*M_PI)*( sin(j*M_PI/(0.5*VAR(_N))) - sin(j*3*M_PI/(0.5*VAR(_N)))/9.0 );
      VAR(_y1) = tan(VAR(_a) * i * M_PI / VAR(_N));
      VAR(_y2) = tan(VAR(_a) * j * M_PI / VAR(_N));

    } else if (VAR(_id) == 11) {
      VAR(_y1) = (-200 / M_PI) * (sin(i * M_PI / (0.5 * VAR(_N))) + sin(i * 2 * M_PI / (0.5 * VAR(_N))) / 2.0 + sin(i * 3 * M_PI / (0.5 * VAR(_N))) / 3.0 + sin(i * 4 * M_PI / (0.5 * VAR(_N))) / 4.0 + sin(i * 5 * M_PI / (0.5 * VAR(_N))) / 5.0 + sin(i * 6 * M_PI / (0.5 * VAR(_N))) / 6.0 + sin(i * 7 * M_PI / (0.5 * VAR(_N))) / 7.0 + sin(i * 8 * M_PI / (0.5 * VAR(_N))) / 8.0);
      VAR(_y2) = (-200 / M_PI) * (sin(j * M_PI / (0.5 * VAR(_N))) + sin(j * 2 * M_PI / (0.5 * VAR(_N))) / 2.0 + sin(j * 3 * M_PI / (0.5 * VAR(_N))) / 3.0 + sin(j * 4 * M_PI / (0.5 * VAR(_N))) / 4.0 + sin(j * 5 * M_PI / (0.5 * VAR(_N))) / 5.0 + sin(j * 6 * M_PI / (0.5 * VAR(_N))) / 6.0 + sin(j * 7 * M_PI / (0.5 * VAR(_N))) / 7.0 + sin(j * 8 * M_PI / (0.5 * VAR(_N))) / 8.0);
    } else if (VAR(_id) == 12) {
      VAR(_y1) = 50 * sin(i * 40 * M_PI / (1 * VAR(_N))) + 50 * sin(i * 44 * M_PI / (1.0 * VAR(_N)));
      VAR(_y2) = 50 * sin(j * 40 * M_PI / (1 * VAR(_N))) + 50 * sin(j * 44 * M_PI / (1.0 * VAR(_N)));

    } else if (VAR(_id) == 13) {
      VAR(_y1) = 50 * sin(i * 40 * M_PI / (1 * VAR(_N))) + 50 * sin(i * 44 * M_PI / (1.0 * VAR(_N)) + M_PI);
      VAR(_y2) = 50 * sin(j * 40 * M_PI / (1 * VAR(_N))) + 50 * sin(j * 44 * M_PI / (1.0 * VAR(_N)) + M_PI);

    } else if (VAR(_id) == 14) {
      VAR(_y1) = 100 * sin(i * 16 * M_PI / (1 * VAR(_N))) - 500 + i * 0.25;
      VAR(_y2) = 100 * sin(j * 16 * M_PI / (1 * VAR(_N))) - 500 + j * 0.25;
    } else if (VAR(_id) == 15) {
      VAR(_y1) = 100 * sin(i * 16 * M_PI / (1 * VAR(_N)));
      VAR(_y2) = 100 * sin(j * 16 * M_PI / (1 * VAR(_N)));
    } else if (VAR(_id) == 16) //casi igual =15
    {
      //		 VAR(_y1)=100*sin(i*16*M_PI/(1*VAR(_N)))-5+i*0.0025;
      //		 VAR(_y2)=100*sin(j*16*M_PI/(1*VAR(_N)))-5+j*0.0025;
      VAR(_y1) = 1.0 / cos(i * VAR(_a) * M_PI / VAR(_N));
      VAR(_y2) = 1.0 / cos(j * VAR(_a) * M_PI / VAR(_N));
    } else if (VAR(_id) == 17) {
      VAR(_y1) = 50 * cos(i * 2 * M_PI * 25 / (1 * VAR(_N)) + 25 * sin(i * 2 * M_PI * 0.50 / (1 * VAR(_N))));
      VAR(_y2) = 50 * cos(j * 2 * M_PI * 25 / (1 * VAR(_N)) + 25 * sin(j * 2 * M_PI * 0.50 / (1 * VAR(_N))));
    } else if (VAR(_id) == 18) {   // VAR(_a)=5;// VAR(_b)=18;
      VAR(_y1) = VAR(_a) * exp(-VAR(_b) * (2.0 * VAR(_a) * i / VAR(_N) - VAR(_a)) * (2.0 * VAR(_a) * i / VAR(_N) - VAR(_a)));
      VAR(_y2) = VAR(_a) * exp(-VAR(_b) * (2.0 * VAR(_a) * j / VAR(_N) - VAR(_a)) * (2.0 * VAR(_a) * j / VAR(_N) - VAR(_a)));
    } else if (VAR(_id) == 19) {
      VAR(_y1) = 500 * sin(i * 10 * M_PI / (VAR(_N))) * exp(-5.0 * i / VAR(_N));
      VAR(_y2) = 500 * sin(j * 10 * M_PI / (VAR(_N))) * exp(-5.0 * j / VAR(_N));
    } else if (VAR(_id) == 20) {
      VAR(_y1) = 500 * sin(i * 20 * M_PI / (VAR(_N))) * exp(-3.0 * i / VAR(_N));
      VAR(_y2) = 500 * sin(j * 20 * M_PI / (VAR(_N))) * exp(-3.0 * j / VAR(_N));
    } else if (VAR(_id) == 21) {
      VAR(_y1) = 50 * tan(i * 2 * M_PI / (1 * VAR(_N)));
      VAR(_y2) = 50 * tan(j * 2 * M_PI / (1 * VAR(_N)));
    } else if (VAR(_id) == 22) {
      // VAR(_a)=490; // VAR(_b)=10;
      VAR(_y1) = F22(vp, i, (int) VAR(_b));
      VAR(_y2) = F22(vp, j, (int) VAR(_b));
    } else if (VAR(_id) == 23) {
      // VAR(_a)=500.0;VAR(_b)=2.0;VAR(_k)=5;
      VAR(_y1) = F23(vp, i, VAR(_k));
      VAR(_y2) = F23(vp, j, VAR(_k));
    } else if (VAR(_id) == 24) {
      // VAR(_a)=500.0;VAR(_b)=1.0;VAR(_k)=5;
      VAR(_y1) = F24(vp, i, VAR(_k));
      VAR(_y2) = F24(vp, j, VAR(_k));
    } else if (VAR(_id) == 25) {
      // VAR(_a)=500.0;VAR(_b)=1.0;VAR(_k)=5;
      VAR(_y1) = F25(vp, i, VAR(_k));
      VAR(_y2) = F25(vp, j, VAR(_k));
    } else if (VAR(_id) == 26) {
      // VAR(_a)=90.0;VAR(_b)=1.0;VAR(_k)=5;
      VAR(_y1) = F26(vp, i, VAR(_k));
      VAR(_y2) = F26(vp, j, VAR(_k));
    } else if (VAR(_id) == 27) {
      // VAR(_a)=1.0;VAR(_b)=10.0;
      VAR(_y1) = F27(vp, i);
      VAR(_y2) = F27(vp, j);
    } else if (VAR(_id) == 28) {
      // VAR(_a)=1.0;VAR(_b)=10.0;VAR(_k)=2;
      VAR(_y1) = F28(vp, i);
      VAR(_y2) = F28(vp, j);
    } else if (VAR(_id) == 29) {
      // VAR(_a)=475;VAR(_k)=5;
      VAR(_y1) = F29(vp, i, VAR(_k));
      VAR(_y2) = F29(vp, j, VAR(_k));
    }

    dist = sqrt((VAR(_y1) - VAR(_y2)) * (VAR(_y1) - VAR(_y2)));

    if (dist < VAR(_maxdist)) {
      x = i / VAR(_N) - 0.5;
      y = j / VAR(_N) - 0.5;

      FPx += VVAR * (x);
      FPy += VVAR * (y);
      VAR(_oldx) = FPx;
      VAR(_oldy) = FPy;
      if (VAR(_dc) != 0) {
        double s = sin(VAR(_angle));
        double c = cos(VAR(_angle));
        if (VAR(_dc) == 1)
          TC = fmod(fabs(0.5 * VAR(_ldcs) * FPx * FPy + VAR(_offset)), 1.0);
        if (VAR(_dc) == 2)
          TC = fmod(fabs(0.5 * (VAR(_ldcs) * ((c * FPx + s * FPy + VAR(_offset))) + 1.0)), 1.0);
      }

    } else {
      FPx = VAR(_oldx);
      FPy = VAR(_oldy);
    }



    return TRUE;
}





// original java file embedded here:
//
// package org.jwildfire.create.tina.variation;
// 
// /**
//  * recurrenceplot variation
//  *
//  * @author Jesus Sosa
//  * @date March 19, 2018
//  * based on a work of Jürgen Meier
//  * http://www.3d-meier.de/tut19/Seite0.html
//  */
// 
// 
// import org.jwildfire.create.tina.base.Layer;
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.*;
// 
// 
// public class RecurrencePlotFunc extends VariationFunc {
// 
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_ID = "func_id";
//   private static final String PARAM_A = "a";
//   private static final String PARAM_B = "b";
//   private static final String PARAM_K = "k";
// 
//   private static final String PARAM_N = "Density";
//   private static final String PARAM_DC = "dc";
//   private static final String PARAM_SCALE = "scale";
//   private static final String PARAM_ANGLE = "angle";
//   private static final String PARAM_OFFSET = "offset";
//   private static final String PARAM_MAXDIST = "tolerance";
// 
// 
//   private static final String[] paramNames = {PARAM_ID, PARAM_A, PARAM_B, PARAM_K, PARAM_N, PARAM_DC, PARAM_SCALE, PARAM_ANGLE, PARAM_OFFSET, PARAM_MAXDIST};
// 
//   double a = 0.5;
//   double b = 10.0;
//   double N = 700;
//   int id = 0;
//   int k = 5;
//   double scale = 1.0;
//   double angle = 0.0;
//   double offset = 0.0;
//   double maxdist = 5;
//   double y1, y2;
//   int dc = 0;
//   double oldx = 0.0, oldy = 0.0;
// 
//   private double ldcs;
// 
//   public double F6(int i) {
//     double Summe = 0;
//     Summe = a / (1 + exp(-b * (i - N / 2.0) / N));
//     return Summe;
//   }
// 
//   public double F22(int i, int b) {
//     double Sum = 0, x;
//     x = (2.0 * i) / N - 1;
// 
//     switch (b) {
//       case 0:
//         Sum = a;
//         break;
//       case 1:
//         Sum = a * x;
//         break;
//       case 2:
//         Sum = a * (3.0 * pow(x, 2) - 1.0) / 2.0;
//         break;
//       case 3:
//         Sum = a * (5.0 * pow(x, 3) - 3.0 * x) / 2.0;
//         break;
//       case 4:
//         Sum = a * (35.0 * pow(x, 4) - 30.0 * pow(x, 2) + 3) / 8.0;
//         break;
//       case 5:
//         Sum = a * (63.0 * pow(x, 5) - 70.0 * pow(x, 3) + 15.0 * x) / 8.0;
//         break;
//       case 6:
//         Sum = a * (231.0 * pow(x, 6) - 315.0 * pow(x, 4) + 105.0 * pow(x, 2) - 5) / 16.0;
//         break;
//       case 7:
//         Sum = a * (429.0 * pow(x, 7) - 693.0 * pow(x, 5) + 315.0 * pow(x, 3) - 35.0 * x) / 16.0;
//         break;
//       case 8:
//         Sum = a * (6435.0 * pow(x, 8) - 12012.0 * pow(x, 6) + 6930.0 * pow(x, 4) - 1260.0 * pow(x, 2) + 35) / 128.0;
//         break;
//       case 9:
//         Sum = a * (12155.0 * pow(x, 9) - 25740.0 * pow(x, 7) + 18018.0 * pow(x, 5) - 4620.0 * pow(x, 3) + 315.0 * x) / 128.0;
//         break;
//       case 10:
//         Sum = a * (46189.0 * pow(x, 10) - 109395.0 * pow(x, 8) + 90090.0 * pow(x, 6) - 30030.0 * pow(x, 4) + 3465.0 * pow(x, 2) - 63) / 256.0;
//         break;
//     }
//     return Sum;
//   }
// 
//   public double F23(int i, int k) {
//     double j;
//     double Summe = 0;
//     for (j = 1; j <= k; j++) {
//       // Rechteck
//       Summe = Summe + sin(i * (2 * j - 1) * b * 2 * M_PI / (N)) / (2 * j - 1);
//     }
//     Summe = Summe * (4 * a / M_PI);
//     return Summe;
//   }
// 
//   public double F24(int i, int k) {
//     double j;
//     double Summe = 0;
//     for (j = 1; j <= k; j++) {
//       // Sägezahn
//       Summe = Summe + sin(i * j * b * 2 * M_PI / (N)) / j;
//     }
//     Summe = Summe * (-2 * a / M_PI);
//     return Summe;
//   }
// 
//   public double F25(int i, int k) {
//     double j;
//     double Summe = 0;
//     for (j = 1; j <= k; j++) {
//       // Gleichgerichteter Sinus
//       Summe = Summe + cos(i * (2 * j) * b * 2 * M_PI / (N)) / ((2 * j - 1) * (2 * j + 1));
//     }
//     Summe = (4 * a / M_PI) * (0.5 - Summe);
//     return Summe;
//   }
// 
//   public double F26(int i, int k) {
//     double j;
//     double Summe = 0;
//     for (j = 1; j <= k; j++) {
//       // Parabel
//       Summe = Summe + pow(-1.0, j) * cos((i) * (2 * j) * b * 1 * M_PI / (N)) / (j * j);
//     }
//     Summe = (a * M_PI * M_PI / 3.0) + 4.0 * a * Summe;
//     return Summe;
//   }
// 
//   public double F27(int i) {
//     double Sinc = 0;
//     // Sinc
//     Sinc = a * sin(b * M_PI * (i - 0.5 * N) / N) / (b * M_PI * (i - 0.5 * N) / N);
//     return Sinc;
//   }
// 
//   public double F28(int i) {
//     double Gabor = 0;
//     // Gabor
//     Gabor = a * exp(-b * (((i - 0.5 * N) / N) * (i - 0.5 * N) / N)) * cos(2.0 * k * M_PI * i / N);
//     return Gabor;
//   }
// 
// 
//   public double F29(int i, int b) {
//     double Summe, x;
//     x = (2.0 * i) / N - 1;
//     Summe = 0.0;
//     switch (b) {
//       case 0:
//         Summe = a;
//         break;
//       case 1:
//         Summe = a * x;
//         break;
//       case 2:
//         Summe = a * (2.0 * pow(x, 2) - 1.0);
//         break;
//       case 3:
//         Summe = a * (4.0 * pow(x, 3) - 3.0 * x);
//         break;
//       case 4:
//         Summe = a * (8.0 * pow(x, 4) - 8.0 * pow(x, 2) + 1);
//         break;
//       case 5:
//         Summe = a * (16.0 * pow(x, 5) - 20.0 * pow(x, 3) + 5.0 * x);
//         break;
//       case 6:
//         Summe = a * (32.0 * pow(x, 6) - 48.0 * pow(x, 4) + 18.0 * pow(x, 2) - 1);
//         break;
//       case 7:
//         Summe = a * (64.0 * pow(x, 7) - 112.0 * pow(x, 5) + 56.0 * pow(x, 3) - 7.0 * x);
//         break;
//       case 8:
//         Summe = a * (128.0 * pow(x, 8) - 256.0 * pow(x, 6) + 160.0 * pow(x, 4) - 32.0 * pow(x, 2) + 1);
//         break;
//       case 9:
//         Summe = a * (256.0 * pow(x, 9) - 576.0 * pow(x, 7) + 432.0 * pow(x, 5) - 120.0 * pow(x, 3) + 9.0 * x);
//         break;
//       case 10:
//         Summe = a * (512.0 * pow(x, 10) - 1280.0 * pow(x, 8) + 1120.0 * pow(x, 6) - 400.0 * pow(x, 4) + 50.0 * pow(x, 2) - 1);
//         break;
//       case 11:
//         Summe = a * (1024.0 * pow(x, 11) - 2816.0 * pow(x, 9) + 2816.0 * pow(x, 7) - 1232.0 * pow(x, 5) + 220.0 * pow(x, 3) - 11.0 * x);
//         break;
//       case 12:
//         Summe = a * (2048.0 * pow(x, 12) - 6144.0 * pow(x, 10) + 6912.0 * pow(x, 8) - 3584.0 * pow(x, 6) + 840.0 * pow(x, 4) - 72.0 * pow(x, 2) + 1);
//         break;
//     }
//     return Summe;
//   }
// 
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     int i, j;
//     double dist, x, y;
// 
//     i = (int) ((N) * pContext.random());
//     j = (int) ((N) * pContext.random());
// 
// 
//     if (id == 0) {
//       y1 = 50 * sin(i * 4 * M_PI / (1 * N)) + 25 * sin(i * 40 * M_PI / (1 * N));
//       y2 = 50 * sin(j * 4 * M_PI / (1 * N)) + 25 * sin(j * 40 * M_PI / (1 * N));
//     } else if (id == 1) {
//       y1 = 10 * sin(i * 5 * M_PI / (1 * N)) * 5 * sin(i * 15 * M_PI / (1 * N));
//       y2 = 10 * sin(j * 5 * M_PI / (1 * N)) * 5 * sin(j * 15 * M_PI / (1 * N));
//     } else if (id == 2) {
//       y1 = 50 * cos(i * 24 * M_PI / (1 * N) + 5 * sin(i * 6 * M_PI / (1 * N)));
//       y2 = 50 * cos(j * 24 * M_PI / (1 * N) + 5 * sin(j * 6 * M_PI / (1 * N)));
//     } else if (id == 3) {
//       y1 = 50 * sin(i * 20 * M_PI / (1 * N)) + 50 * sin(i * 22 * M_PI / (1 * N) + M_PI);
//       y2 = 50 * sin(j * 20 * M_PI / (1 * N)) + 50 * sin(j * 22 * M_PI / (1 * N) + M_PI);
//     } else if (id == 4) {
//       y1 = 50 * sin(i * 20 * M_PI / (1 * N)) + 10 * sin(i * 22 * M_PI / (1 * N));
//       y2 = 50 * sin(j * 20 * M_PI / (1 * N)) + 10 * sin(j * 22 * M_PI / (1 * N));
//     } else if (id == 5) {
//       y1 = 50 * sin(i * a * M_PI / (1 * N) + 1 * sin(i * a * M_PI / (1 * N)));
//       y2 = 50 * sin(j * a * M_PI / (1 * N) + 1 * sin(j * a * M_PI / (1 * N)));
//     } else if (id == 6)  // Sigmoid Function
//     {
//       y1 = F6(i);
//       y2 = F6(j);
//     } else if (id == 7) {
//       y1 = (400 / M_PI) * (sin(i * M_PI / (0.5 * N)) + sin(i * 3 * M_PI / (0.5 * N)) / 3.0 + sin(i * 5 * M_PI / (0.5 * N)) / 5.0 + sin(i * 7 * M_PI / (0.5 * N)) / 7.0);
//       y2 = (400 / M_PI) * (sin(j * M_PI / (0.5 * N)) + sin(j * 3 * M_PI / (0.5 * N)) / 3.0 + sin(j * 5 * M_PI / (0.5 * N)) / 5.0 + sin(j * 7 * M_PI / (0.5 * N)) / 7.0);
//     } else if (id == 8) {
//       y1 = (400 / M_PI * M_PI) * (sin(i * M_PI / (0.5 * N)) - sin(i * 3 * M_PI / (0.5 * N)) / 9.0 + sin(i * 5 * M_PI / (0.5 * N)) / 25.0 - sin(i * 7 * M_PI / (0.5 * N)) / 49.0);
//       y2 = (400 / M_PI * M_PI) * (sin(j * M_PI / (0.5 * N)) - sin(j * 3 * M_PI / (0.5 * N)) / 9.0 + sin(j * 5 * M_PI / (0.5 * N)) / 25.0 - sin(j * 7 * M_PI / (0.5 * N)) / 49.0);
//     } else if (id == 9) // tanh(a*x*M_PI)
//     {
// //			 y1=(400/M_PI*M_PI)*( sin(i*M_PI/(0.5*N)) - sin(i*3*M_PI/(0.5*N))/9.0 + sin(i*5*M_PI/(0.5*N))/25.0 );
// //			 y2=(400/M_PI*M_PI)*( sin(j*M_PI/(0.5*N)) - sin(j*3*M_PI/(0.5*N))/9.0 + sin(j*5*M_PI/(0.5*N))/25.0 );
//       y1 = tanh(a * i * M_PI / N);
//       y2 = tanh(a * j * M_PI / N);
//     } else if (id == 10) // tan(a* x * PI)
//     {
//       //		 y1=(400/M_PI*M_PI)*( sin(i*M_PI/(0.5*N)) - sin(i*3*M_PI/(0.5*N))/9.0 );
//       //		 y2=(400/M_PI*M_PI)*( sin(j*M_PI/(0.5*N)) - sin(j*3*M_PI/(0.5*N))/9.0 );
//       y1 = tan(a * i * M_PI / N);
//       y2 = tan(a * j * M_PI / N);
// 
//     } else if (id == 11) {
//       y1 = (-200 / M_PI) * (sin(i * M_PI / (0.5 * N)) + sin(i * 2 * M_PI / (0.5 * N)) / 2.0 + sin(i * 3 * M_PI / (0.5 * N)) / 3.0 + sin(i * 4 * M_PI / (0.5 * N)) / 4.0 + sin(i * 5 * M_PI / (0.5 * N)) / 5.0 + sin(i * 6 * M_PI / (0.5 * N)) / 6.0 + sin(i * 7 * M_PI / (0.5 * N)) / 7.0 + sin(i * 8 * M_PI / (0.5 * N)) / 8.0);
//       y2 = (-200 / M_PI) * (sin(j * M_PI / (0.5 * N)) + sin(j * 2 * M_PI / (0.5 * N)) / 2.0 + sin(j * 3 * M_PI / (0.5 * N)) / 3.0 + sin(j * 4 * M_PI / (0.5 * N)) / 4.0 + sin(j * 5 * M_PI / (0.5 * N)) / 5.0 + sin(j * 6 * M_PI / (0.5 * N)) / 6.0 + sin(j * 7 * M_PI / (0.5 * N)) / 7.0 + sin(j * 8 * M_PI / (0.5 * N)) / 8.0);
//     } else if (id == 12) {
//       y1 = 50 * sin(i * 40 * M_PI / (1 * N)) + 50 * sin(i * 44 * M_PI / (1.0 * N));
//       y2 = 50 * sin(j * 40 * M_PI / (1 * N)) + 50 * sin(j * 44 * M_PI / (1.0 * N));
// 
//     } else if (id == 13) {
//       y1 = 50 * sin(i * 40 * M_PI / (1 * N)) + 50 * sin(i * 44 * M_PI / (1.0 * N) + M_PI);
//       y2 = 50 * sin(j * 40 * M_PI / (1 * N)) + 50 * sin(j * 44 * M_PI / (1.0 * N) + M_PI);
// 
//     } else if (id == 14) {
//       y1 = 100 * sin(i * 16 * M_PI / (1 * N)) - 500 + i * 0.25;
//       y2 = 100 * sin(j * 16 * M_PI / (1 * N)) - 500 + j * 0.25;
//     } else if (id == 15) {
//       y1 = 100 * sin(i * 16 * M_PI / (1 * N));
//       y2 = 100 * sin(j * 16 * M_PI / (1 * N));
//     } else if (id == 16) //casi igual =15
//     {
//       //		 y1=100*sin(i*16*M_PI/(1*N))-5+i*0.0025;
//       //		 y2=100*sin(j*16*M_PI/(1*N))-5+j*0.0025;
//       y1 = 1.0 / cos(i * a * M_PI / N);
//       y2 = 1.0 / cos(j * a * M_PI / N);
//     } else if (id == 17) {
//       y1 = 50 * cos(i * 2 * M_PI * 25 / (1 * N) + 25 * sin(i * 2 * M_PI * 0.50 / (1 * N)));
//       y2 = 50 * cos(j * 2 * M_PI * 25 / (1 * N) + 25 * sin(j * 2 * M_PI * 0.50 / (1 * N)));
//     } else if (id == 18) {   // a=5;// b=18;
//       y1 = a * exp(-b * (2.0 * a * i / N - a) * (2.0 * a * i / N - a));
//       y2 = a * exp(-b * (2.0 * a * j / N - a) * (2.0 * a * j / N - a));
//     } else if (id == 19) {
//       y1 = 500 * sin(i * 10 * M_PI / (N)) * exp(-5.0 * i / N);
//       y2 = 500 * sin(j * 10 * M_PI / (N)) * exp(-5.0 * j / N);
//     } else if (id == 20) {
//       y1 = 500 * sin(i * 20 * M_PI / (N)) * exp(-3.0 * i / N);
//       y2 = 500 * sin(j * 20 * M_PI / (N)) * exp(-3.0 * j / N);
//     } else if (id == 21) {
//       y1 = 50 * tan(i * 2 * M_PI / (1 * N));
//       y2 = 50 * tan(j * 2 * M_PI / (1 * N));
//     } else if (id == 22) {
//       // a=490; // b=10;
//       y1 = F22(i, (int) b);
//       y2 = F22(j, (int) b);
//     } else if (id == 23) {
//       // a=500.0;b=2.0;k=5;
//       y1 = F23(i, k);
//       y2 = F23(j, k);
//     } else if (id == 24) {
//       // a=500.0;b=1.0;k=5;
//       y1 = F24(i, k);
//       y2 = F24(j, k);
//     } else if (id == 25) {
//       // a=500.0;b=1.0;k=5;
//       y1 = F25(i, k);
//       y2 = F25(j, k);
//     } else if (id == 26) {
//       // a=90.0;b=1.0;k=5;
//       y1 = F26(i, k);
//       y2 = F26(j, k);
//     } else if (id == 27) {
//       // a=1.0;b=10.0;
//       y1 = F27(i);
//       y2 = F27(j);
//     } else if (id == 28) {
//       // a=1.0;b=10.0;k=2;
//       y1 = F28(i);
//       y2 = F28(j);
//     } else if (id == 29) {
//       // a=475;k=5;
//       y1 = F29(i, k);
//       y2 = F29(j, k);
//     }
// 
//     dist = sqrt((y1 - y2) * (y1 - y2));
// 
//     if (dist < maxdist) {
//       x = i / N - 0.5;
//       y = j / N - 0.5;
// 
//       pVarTP.x += pAmount * (x);
//       pVarTP.y += pAmount * (y);
//       oldx = pVarTP.x;
//       oldy = pVarTP.y;
//       if (dc != 0) {
//         double s = sin(angle);
//         double c = cos(angle);
//         if (dc == 1)
//           pVarTP.color = fmod(fabs(0.5 * ldcs * pVarTP.x * pVarTP.y + offset), 1.0);
//         if (dc == 2)
//           pVarTP.color = fmod(fabs(0.5 * (ldcs * ((c * pVarTP.x + s * pVarTP.y + offset)) + 1.0)), 1.0);
//       }
// 
//     } else {
//       pVarTP.x = oldx;
//       pVarTP.y = oldy;
//     }
// 
//   }
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     ldcs = 1.0 / (scale == 0.0 ? 10E-6 : scale);
//   }
// 
//   public String getName() {
//     return "recurrenceplot";
//   }
// 
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   public Object[] getParameterValues() {
//     return new Object[]{id, a, b, k, N, dc, scale, angle, offset, maxdist};
//   }
// 
//   public void setParameter(String pName, double pValue) {
//     if (pName.equalsIgnoreCase(PARAM_ID)) {
//       id = (int) pValue;
//       a = 0;
//       b = 0.0;
//       k = 0;
//       maxdist = 5;
//       if (id == 5) {
//         a = 1.9;
//       }
//       if (id == 6) {
//         a = 500;
//         b = 100;
//       }
//       if (id == 9) {
//         a = 1.0;
//         maxdist = 0.01;
//       }
//       if (id == 10) {
//         a = 4;
//         maxdist = 1;
//       }
//       if (id == 16) {
//         a = 4;
//         maxdist = 1;
//       }
//       if (id == 18) {
//         a = 2.0;
//         b = 1.0;
//         maxdist = 0.5;
//       }
//       if (id == 22) {
//         a = 490;
//         b = 10;
//       }
//       if (id == 23) {
//         a = 500;
//         b = 2.0;
//         k = 5;
//       }
//       if (id == 24 || id == 25) {
//         a = 500;
//         b = 1.0;
//         k = 5;
//       }
//       if (id == 26) {
//         a = 90;
//         b = 1.0;
//         k = 5;
//       }
//       if (id == 27) {
//         a = 1;
//         b = 10.0;
//         maxdist = 0.1;
//       }
//       if (id == 28) {
//         a = 0.6;
//         b = 10.0;
//         k = 6;
//         maxdist = 0.1;
//       }
//       if (id == 29) {
//         a = 475;
//         k = 5;
//       }
//     } else if (pName.equalsIgnoreCase(PARAM_A)) {
//       a = pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_B)) {
//       b = pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_N)) {
//       N = (int) pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_K)) {
//       k = (int) pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_DC)) {
//       dc = (int) pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_SCALE)) {
//       scale = pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_ANGLE)) {
//       angle = (int) pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_OFFSET)) {
//       offset = pValue;
//     } else if (pName.equalsIgnoreCase(PARAM_MAXDIST)) {
//       maxdist = pValue;
//     } else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public boolean dynamicParameterExpansion() {
//     return true;
//   }
// 
//   @Override
//   public boolean dynamicParameterExpansion(String pName) {
//     // preset_id doesn't really expand parameters, but it changes them; this will make them refresh
//     if (PARAM_ID.equalsIgnoreCase(pName)) return true;
//     else return false;
//   }
// }
// 
