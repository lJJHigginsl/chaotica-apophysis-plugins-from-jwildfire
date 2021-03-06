#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: eJulia

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/EJuliaFunc.java

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
	int power;
	int _sign;

} Variables;

#define APO_VARIABLE_PREFIX "eJulia_"
#include "plugin.h"

APO_PLUGIN("eJulia");
APO_VARIABLES(
	VAR_INTEGER(power, 2)

);


double sqrt_safe (Variation* vp, double x) {

    if (x <= 0.0)
      return 0.0;
    return sqrt(x);
}


int PluginVarPrepare(Variation* vp)
{
    VAR(_sign) = 1;
    if (VAR(power) < 0)
      VAR(_sign) = -1;

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    // eJulia by Michael Faber, http://michaelfaber.deviantart.com/art/eSeries-306044892
    double r2 = FTy * FTy + FTx * FTx;
    double tmp2;
    double x;
    if (VAR(_sign) == 1)
      x = FTx;
    else {
      r2 = 1.0 / r2;
      x = FTx * r2;
    }

    double tmp = r2 + 1.0;
    tmp2 = 2.0 * x;
    double xmax = (sqrt_safe(vp, tmp + tmp2) + sqrt_safe(vp, tmp - tmp2)) * 0.5;
    if (xmax < 1.0)
      xmax = 1.0;
    double sinhmu, coshmu, sinnu, cosnu;

    double mu = acosh(xmax); //  mu > 0
    double t = x / xmax;
    if (t > 1.0)
      t = 1.0;
    else if (t < -1.0)
      t = -1.0;

    double nu = acos(t); // -Pi < nu < Pi
    if (FTy < 0)
      nu *= -1.0;

    nu = nu / VAR(power) + M_2PI / VAR(power) * floor(GOODRAND_01() * VAR(power));
    mu /= VAR(power);

    sinhmu = sinh(mu);
    coshmu = cosh(mu);

    sinnu = sin(nu);
    cosnu = cos(nu);
    FPx += VVAR * coshmu * cosnu;
    FPy += VVAR * sinhmu * sinnu;

    if (true /* pContext\.isPreserveZCoordinate() */) {
      FPz += VVAR * FTz;
    }


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
// import org.jwildfire.base.Tools;
// import org.jwildfire.create.tina.base.Layer;
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.*;
// 
// public class EJuliaFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_POWER = "power";
// 
//   private static final String[] paramNames = {PARAM_POWER};
// 
//   private int power = 2;
// 
//   //Taking the square root of numbers close to zero is dangerous.  If x is negative
//   //due to floating point errors we get NaN results.
//   private double sqrt_safe(double x) {
//     if (x <= 0.0)
//       return 0.0;
//     return sqrt(x);
//   }
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     // eJulia by Michael Faber, http://michaelfaber.deviantart.com/art/eSeries-306044892
//     double r2 = pAffineTP.y * pAffineTP.y + pAffineTP.x * pAffineTP.x;
//     double tmp2;
//     double x;
//     if (_sign == 1)
//       x = pAffineTP.x;
//     else {
//       r2 = 1.0 / r2;
//       x = pAffineTP.x * r2;
//     }
// 
//     double tmp = r2 + 1.0;
//     tmp2 = 2.0 * x;
//     double xmax = (sqrt_safe(tmp + tmp2) + sqrt_safe(tmp - tmp2)) * 0.5;
//     if (xmax < 1.0)
//       xmax = 1.0;
//     double sinhmu, coshmu, sinnu, cosnu;
// 
//     double mu = acosh(xmax); //  mu > 0
//     double t = x / xmax;
//     if (t > 1.0)
//       t = 1.0;
//     else if (t < -1.0)
//       t = -1.0;
// 
//     double nu = acos(t); // -Pi < nu < Pi
//     if (pAffineTP.y < 0)
//       nu *= -1.0;
// 
//     nu = nu / power + M_2PI / power * floor(pContext.random() * power);
//     mu /= power;
// 
//     sinhmu = sinh(mu);
//     coshmu = cosh(mu);
// 
//     sinnu = sin(nu);
//     cosnu = cos(nu);
//     pVarTP.x += pAmount * coshmu * cosnu;
//     pVarTP.y += pAmount * sinhmu * sinnu;
// 
//     if (pContext.isPreserveZCoordinate()) {
//       pVarTP.z += pAmount * pAffineTP.z;
//     }
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{power};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_POWER.equalsIgnoreCase(pName))
//       power = Tools.FTOI(pValue);
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "eJulia";
//   }
// 
//   private int _sign;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     _sign = 1;
//     if (power < 0)
//       _sign = -1;
//   }
// 
// }
// 
