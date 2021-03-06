#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: curl_sp

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/CurlSpFunc.java

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
	double pow;
	double c1; // -0.01
	double c2;
	double sx;
	double sy;
	double dc;
	double _c2_x2;
	double _dc_adjust;
	double _power_inv;
	double _power;

} Variables;

#define APO_VARIABLE_PREFIX "curl_sp_"
#include "plugin.h"

APO_PLUGIN("curl_sp");
APO_VARIABLES(
	VAR_REAL(pow, 1.0),
	VAR_REAL(c1, -0.01),
	VAR_REAL(c2, 0.03),
	VAR_REAL(sx, 0.0),
	VAR_REAL(sy, 0.0),
	VAR_REAL(dc, 0.0)

);


double spread (Variation* vp, double a, double b) {

    return (sqrt(sqr(a) + sqr(b)) * ((a) > 0 ? 1 : -1));
}

double range (Variation* vp, double a, double x, double b) {

    return ((x) < (a) ? (a) : (x) > (b) ? (b) : (x));
}

double powq4 (Variation* vp, double x, double y) {

    return (pow(fabs(x), y) * SGN(x));
}

double powq4c (Variation* vp, double x, double y) {

    return ((y) == 1 ? (x) : powq4(vp, x, y));
}

double zeps (Variation* vp, double x) {

    return ((x) == 0 ? SMALL_EPSILON : (x));
}


int PluginVarPrepare(Variation* vp)
{
    VAR(_power) = VAR(pow);
    VAR(_c2_x2) = 2.0 * VAR(c2);
    VAR(_dc_adjust) = 0.1 * VAR(dc);
    VAR(_power_inv) = 1.0 / zeps(vp, VAR(_power));

    if (VAR(_power) == 0) {
      VAR(_power) = SMALL_EPSILON;
    }

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    // curl_sp by Xyrus02, http://apophysis-7x.org/extensions
    double x = powq4c(vp, FTx, VAR(_power));
    double y = powq4c(vp, FTy, VAR(_power));
    double z = powq4c(vp, FTz, VAR(_power));

    double d = sqr(x) - sqr(y);

    double re = spread(vp, VAR(c1) * x + VAR(c2) * d, VAR(sx)) + 1.0;
    double im = spread(vp, VAR(c1) * y + VAR(_c2_x2) * x * y, VAR(sy));

    double c = powq4c(vp, sqr(re) + sqr(im), VAR(_power_inv));
    double r = VVAR / c;

    FPx += (x * re + y * im) * r;
    FPy += (y * re - x * im) * r;
    FPz += (z * VVAR) / c;

    TC = range(vp, 0, TC + VAR(_dc_adjust) * c, 1);


    return TRUE;
}





// original java file embedded here:
//
// /*
//   JWildfire - an image and animation processor written in Java 
//   Copyright (C) 1995-2011 Andreas Maschke
// 
//   This is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser 
//   General Public License as published by the Free Software Foundation; either version 2.1 of the 
//   License, or (at your option) any later version.
//  
//   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
//   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
//   Lesser General Public License for more details.
// 
//   You should have received a copy of the GNU Lesser General Public License along with this software; 
//   if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301 USA, or see the FSF site: http://www.fsf.org.
// */
// package org.jwildfire.create.tina.variation;
// 
// import org.jwildfire.create.tina.base.Layer;
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.*;
// 
// public class CurlSpFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_POW = "pow";
//   private static final String PARAM_C1 = "c1";
//   private static final String PARAM_C2 = "c2";
//   private static final String PARAM_SX = "sx";
//   private static final String PARAM_SY = "sy";
//   private static final String PARAM_DC = "dc";
//   private static final String[] paramNames = {PARAM_POW, PARAM_C1, PARAM_C2, PARAM_SX, PARAM_SY, PARAM_DC};
// 
//   private double pow = 1.0;
//   private double c1 = -0.01;
//   private double c2 = 0.03;
//   private double sx = 0.0;
//   private double sy = 0.0;
//   private double dc = 0.0;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     // curl_sp by Xyrus02, http://apophysis-7x.org/extensions
//     double x = powq4c(pAffineTP.x, power);
//     double y = powq4c(pAffineTP.y, power);
//     double z = powq4c(pAffineTP.z, power);
// 
//     double d = sqr(x) - sqr(y);
// 
//     double re = spread(c1 * x + c2 * d, sx) + 1.0;
//     double im = spread(c1 * y + c2_x2 * x * y, sy);
// 
//     double c = powq4c(sqr(re) + sqr(im), power_inv);
//     double r = pAmount / c;
// 
//     pVarTP.x += (x * re + y * im) * r;
//     pVarTP.y += (y * re - x * im) * r;
//     pVarTP.z += (z * pAmount) / c;
// 
//     pVarTP.color = range(0, pVarTP.color + dc_adjust * c, 1);
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{pow, c1, c2, sx, sy, dc};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_POW.equalsIgnoreCase(pName))
//       pow = pValue;
//     else if (PARAM_C1.equalsIgnoreCase(pName))
//       c1 = pValue;
//     else if (PARAM_C2.equalsIgnoreCase(pName))
//       c2 = pValue;
//     else if (PARAM_SX.equalsIgnoreCase(pName))
//       sx = pValue;
//     else if (PARAM_SY.equalsIgnoreCase(pName))
//       sy = pValue;
//     else if (PARAM_DC.equalsIgnoreCase(pName))
//       dc = pValue;
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "curl_sp";
//   }
// 
//   private double c2_x2, dc_adjust, power_inv, power;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     power = pow;
//     c2_x2 = 2.0 * c2;
//     dc_adjust = 0.1 * dc;
//     power_inv = 1.0 / zeps(power);
// 
//     if (power == 0) {
//       power = SMALL_EPSILON;
//     }
//   }
// 
//   private double spread(double a, double b) {
//     return (sqrt(sqr(a) + sqr(b)) * ((a) > 0 ? 1 : -1));
//   }
// 
//   private double range(double a, double x, double b) {
//     return ((x) < (a) ? (a) : (x) > (b) ? (b) : (x));
//   }
// 
//   private double powq4(double x, double y) {
//     return (pow(fabs(x), y) * sign(x));
//   }
// 
//   private double powq4c(double x, double y) {
//     return ((y) == 1 ? (x) : powq4(x, y));
//   }
// 
//   private double zeps(double x) {
//     return ((x) == 0 ? SMALL_EPSILON : (x));
//   }
// 
// }
// 
