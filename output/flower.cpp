#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: flower

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/FlowerFunc.java

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
	double holes;
	double petals;

} Variables;

#define APO_VARIABLE_PREFIX "flower_"
#include "plugin.h"

APO_PLUGIN("flower");
APO_VARIABLES(
	VAR_REAL(holes, 0.40),
	VAR_REAL(petals, 7.0)

);



int PluginVarPrepare(Variation* vp)
{

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    /* cyberxaos, 4/2007 */
    double theta = atan2(FTx, FTy);
    double d = (sqrt(FTx * FTx + FTy * FTy) + SMALL_EPSILON);
    if (d == 0) {
      return TRUE;
    }
    double r = VVAR * (GOODRAND_01() - VAR(holes)) * cos(VAR(petals) * theta) / d;
    FPx += r * FTx;
    FPy += r * FTy;
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
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.cos;
// 
// public class FlowerFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   public static final String PARAM_HOLES = "holes";
//   public static final String PARAM_PETALS = "petals";
// 
//   private static final String[] paramNames = {PARAM_HOLES, PARAM_PETALS};
// 
//   private double holes = 0.40;
//   private double petals = 7.0;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* cyberxaos, 4/2007 */
//     double theta = pAffineTP.getPrecalcAtanYX();
//     double d = pAffineTP.getPrecalcSqrt();
//     if (d == 0) {
//       return;
//     }
//     double r = pAmount * (pContext.random() - holes) * cos(petals * theta) / d;
//     pVarTP.x += r * pAffineTP.x;
//     pVarTP.y += r * pAffineTP.y;
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
//     return new Object[]{holes, petals};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_HOLES.equalsIgnoreCase(pName))
//       holes = pValue;
//     else if (PARAM_PETALS.equalsIgnoreCase(pName))
//       petals = pValue;
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "flower";
//   }
// 
// }
// 
