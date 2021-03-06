#define PLUGIN_WARNING "_WARNING_empty_shim_for_jw_workflow"
/*
    Apophysis Plugin: lazysusan

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/LazySusanFunc.java

    Automatically converted by @mwegner

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
	double space;
	double twist;
	double spin;
	double x;
	double y;

} Variables;

#define APO_VARIABLE_PREFIX "lazysusan_"
#include "plugin.h"

APO_PLUGIN("lazysusan");
APO_VARIABLES(
	VAR_REAL(space, 0.40),
	VAR_REAL(twist, 0.20),
	VAR_REAL(spin, 0.10),
	VAR_REAL(x, 0.10),
	VAR_REAL(y, 0.20)

);


int PluginVarPrepare(Variation* vp)
{
    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
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
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.*;
// 
// public class LazySusanFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_SPACE = "space";
//   private static final String PARAM_TWIST = "twist";
//   private static final String PARAM_SPIN = "spin";
//   private static final String PARAM_X = "x";
//   private static final String PARAM_Y = "y";
// 
//   private static final String[] paramNames = {PARAM_SPACE, PARAM_TWIST, PARAM_SPIN, PARAM_X, PARAM_Y};
// 
//   private double space = 0.40;
//   private double twist = 0.20;
//   private double spin = 0.10;
//   private double x = 0.10;
//   private double y = 0.20;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* Lazysusan in the Apophysis Plugin Pack */
// 
//     double xx = pAffineTP.x - x;
//     double yy = pAffineTP.y + y;
//     double rr = sqrt(xx * xx + yy * yy);
// 
//     if (rr < pAmount) {
//       double a = atan2(yy, xx) + spin + twist * (pAmount - rr);
//       double sina = sin(a);
//       double cosa = cos(a);
//       rr = pAmount * rr;
// 
//       pVarTP.x += rr * cosa + x;
//       pVarTP.y += rr * sina - y;
//     } else {
//       rr = pAmount * (1.0 + space / rr);
// 
//       pVarTP.x += rr * xx + x;
//       pVarTP.y += rr * yy - y;
//     }
//     if (pContext.isPreserveZCoordinate()) {
//       pVarTP.z += pAmount * pAffineTP.z;
//     }
// 
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{space, twist, spin, x, y};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_SPACE.equalsIgnoreCase(pName))
//       space = pValue;
//     else if (PARAM_TWIST.equalsIgnoreCase(pName))
//       twist = pValue;
//     else if (PARAM_SPIN.equalsIgnoreCase(pName))
//       spin = pValue;
//     else if (PARAM_X.equalsIgnoreCase(pName))
//       x = pValue;
//     else if (PARAM_Y.equalsIgnoreCase(pName))
//       y = pValue;
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "lazysusan";
//   }
// 
// }
// 
