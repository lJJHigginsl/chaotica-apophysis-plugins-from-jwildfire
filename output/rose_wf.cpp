#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: rose_wf

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/RoseWFFunc.java

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
	double amp;
	int waves;
	int filled;

} Variables;

#define APO_VARIABLE_PREFIX "rose_wf_"
#include "plugin.h"

APO_PLUGIN("rose_wf");
APO_VARIABLES(
	VAR_REAL(amp, 0.5),
	VAR_INTEGER(waves, 4),
	VAR_INTEGER(filled, 0)

);



int PluginVarPrepare(Variation* vp)
{

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    double a = atan2(FTx, FTy);
    double r = (sqrt(FTx * FTx + FTy * FTy) + SMALL_EPSILON);

    r = VAR(amp) * cos(VAR(waves) * a);

    if (VAR(filled) == 1) {
      r *= GOODRAND_01();
    }

    double nx = sin(a) * r;
    double ny = cos(a) * r;

    FPx += VVAR * nx;
    FPy += VVAR * ny;
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
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.cos;
// import static org.jwildfire.base.mathlib.MathLib.sin;
// 
// public class RoseWFFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_AMP = "amp";
//   private static final String PARAM_WAVES = "waves";
//   private static final String PARAM_FILLED = "filled";
//   private static final String[] paramNames = {PARAM_AMP, PARAM_WAVES, PARAM_FILLED};
// 
//   private double amp = 0.5;
//   private int waves = 4;
//   private int filled = 0;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     double a = pAffineTP.getPrecalcAtan();
//     double r = pAffineTP.getPrecalcSqrt();
// 
//     r = amp * cos(waves * a);
// 
//     if (filled == 1) {
//       r *= pContext.random();
//     }
// 
//     double nx = sin(a) * r;
//     double ny = cos(a) * r;
// 
//     pVarTP.x += pAmount * nx;
//     pVarTP.y += pAmount * ny;
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
//     return new Object[]{amp, waves, filled};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_AMP.equalsIgnoreCase(pName))
//       amp = pValue;
//     else if (PARAM_WAVES.equalsIgnoreCase(pName))
//       waves = Tools.FTOI(pValue);
//     else if (PARAM_FILLED.equalsIgnoreCase(pName))
//       filled = Tools.FTOI(pValue);
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "rose_wf";
//   }
// 
// }
// 
