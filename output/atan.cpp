#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: atan

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/AtanFunc.java

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
	int Mode;
	double Stretch;

} Variables;

#define APO_VARIABLE_PREFIX "atan_"
#include "plugin.h"

APO_PLUGIN("atan");
APO_VARIABLES(
	VAR_INTEGER(Mode, 0),
	VAR_REAL(Stretch, 1.0)

);



int PluginVarPrepare(Variation* vp)
{

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    /* atan by FractalDesire http://fractaldesire.deviantart.com/art/atan-Plugin-688802474 converted by Brad Stefanov */
    double norm = 1.0 / M_PI_2 * VVAR;


    switch (VAR(Mode)) {
      case 0:
        FPx += VVAR * FTx;
        FPy += norm * atan(VAR(Stretch) * FTy);
        if (true /* pContext\.isPreserveZCoordinate() */) {
          FPz += VVAR * FTz;
        }
        break;
      case 1:
        FPx += norm * atan(VAR(Stretch) * FTx);
        FPy += VVAR * FTy;
        if (true /* pContext\.isPreserveZCoordinate() */) {
          FPz += VVAR * FTz;
        }
        break;
      case 2:
        FPx += norm * atan(VAR(Stretch) * FTx);
        FPy += norm * atan(VAR(Stretch) * FTy);
        if (true /* pContext\.isPreserveZCoordinate() */) {
          FPz += VVAR * FTz;
        }
        break;
      default:
        break;
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
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.M_PI_2;
// import static org.jwildfire.base.mathlib.MathLib.atan;
// 
// public class AtanFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_MODE = "Mode";
//   private static final String PARAM_STRETCH = "Stretch";
// 
//   private static final String[] paramNames = {PARAM_MODE, PARAM_STRETCH};
//   private int Mode = 0;
//   private double Stretch = 1.0;
// 
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* atan by FractalDesire http://fractaldesire.deviantart.com/art/atan-Plugin-688802474 converted by Brad Stefanov */
//     double norm = 1.0 / M_PI_2 * pAmount;
// 
// 
//     switch (Mode) {
//       case 0:
//         pVarTP.x += pAmount * pAffineTP.x;
//         pVarTP.y += norm * atan(Stretch * pAffineTP.y);
//         if (pContext.isPreserveZCoordinate()) {
//           pVarTP.z += pAmount * pAffineTP.z;
//         }
//         break;
//       case 1:
//         pVarTP.x += norm * atan(Stretch * pAffineTP.x);
//         pVarTP.y += pAmount * pAffineTP.y;
//         if (pContext.isPreserveZCoordinate()) {
//           pVarTP.z += pAmount * pAffineTP.z;
//         }
//         break;
//       case 2:
//         pVarTP.x += norm * atan(Stretch * pAffineTP.x);
//         pVarTP.y += norm * atan(Stretch * pAffineTP.y);
//         if (pContext.isPreserveZCoordinate()) {
//           pVarTP.z += pAmount * pAffineTP.z;
//         }
//         break;
//       default:
//         break;
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
//     return new Object[]{Mode, Stretch};
//   }
// 
//   @Override
//   public String[] getParameterAlternativeNames() {
//     return new String[]{"atanMode", "atanStretch"};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_MODE.equalsIgnoreCase(pName))
//       Mode = limitIntVal(Tools.FTOI(pValue), 0, 2);
//     else if (PARAM_STRETCH.equalsIgnoreCase(pName))
//       Stretch = pValue;
// 
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "atan";
//   }
// 
// }
// 
