#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: poincare3D

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/Poincare3DFunc.java

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
	double r;
	double a;
	double b;
	double _cx;
	double _cy;
	double _cz;
	double _c2;
	double _c2x;
	double _c2y;
	double _c2z;
	double _s2x;
	double _s2y;
	double _s2z;

} Variables;

#define APO_VARIABLE_PREFIX "poincare3D_"
#include "plugin.h"

APO_PLUGIN("poincare3D");
APO_VARIABLES(
	VAR_REAL(r, 0),
	VAR_REAL(a, 0),
	VAR_REAL(b, 0)

);



int PluginVarPrepare(Variation* vp)
{
    VAR(_cx) = -VAR(r) * cos(VAR(a) * M_PI_2) * cos(VAR(b) * M_PI_2);
    VAR(_cy) = VAR(r) * sin(VAR(a) * M_PI_2) * cos(VAR(b) * M_PI_2);
    VAR(_cz) = -VAR(r) * sin(VAR(b) * M_PI_2);

    VAR(_c2) = sqr(VAR(_cx)) + sqr(VAR(_cy)) + sqr(VAR(_cz));

    VAR(_c2x) = 2 * VAR(_cx);
    VAR(_c2y) = 2 * VAR(_cy);
    VAR(_c2z) = 2 * VAR(_cz);

    VAR(_s2x) = sqr(VAR(_cx)) - sqr(VAR(_cy)) - sqr(VAR(_cz)) + 1;
    VAR(_s2y) = sqr(VAR(_cy)) - sqr(VAR(_cx)) - sqr(VAR(_cz)) + 1;
    VAR(_s2z) = sqr(VAR(_cz)) - sqr(VAR(_cy)) - sqr(VAR(_cx)) + 1;

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    /* poincare3D by Zueuk, http://zueuk.deviantart.com/art/3D-Hyperbolic-tiling-plugins-169047926 */
    double r2 = sqr(FTx) + sqr(FTy) + sqr(FTz);

    double x2cx = VAR(_c2x) * FTx, y2cy = VAR(_c2y) * FTy, z2cz = VAR(_c2z) * FTz;

    double d = VVAR / (VAR(_c2) * r2 - x2cx - y2cy - z2cz + 1);

    FPx += d * (FTx * VAR(_s2x) + VAR(_cx) * (y2cy + z2cz - r2 - 1));
    FPy += d * (FTy * VAR(_s2y) + VAR(_cy) * (x2cx + z2cz - r2 - 1));
    FPz += d * (FTz * VAR(_s2z) + VAR(_cz) * (y2cy + x2cx - r2 - 1));


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
// public class Poincare3DFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_R = "r";
//   private static final String PARAM_A = "a";
//   private static final String PARAM_B = "b";
// 
//   private static final String[] paramNames = {PARAM_R, PARAM_A, PARAM_B};
// 
//   private double r = 0;
//   private double a = 0;
//   private double b = 0;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* poincare3D by Zueuk, http://zueuk.deviantart.com/art/3D-Hyperbolic-tiling-plugins-169047926 */
//     double r2 = sqr(pAffineTP.x) + sqr(pAffineTP.y) + sqr(pAffineTP.z);
// 
//     double x2cx = c2x * pAffineTP.x, y2cy = c2y * pAffineTP.y, z2cz = c2z * pAffineTP.z;
// 
//     double d = pAmount / (c2 * r2 - x2cx - y2cy - z2cz + 1);
// 
//     pVarTP.x += d * (pAffineTP.x * s2x + cx * (y2cy + z2cz - r2 - 1));
//     pVarTP.y += d * (pAffineTP.y * s2y + cy * (x2cx + z2cz - r2 - 1));
//     pVarTP.z += d * (pAffineTP.z * s2z + cz * (y2cy + x2cx - r2 - 1));
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{r, a, b};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_R.equalsIgnoreCase(pName))
//       r = pValue;
//     else if (PARAM_A.equalsIgnoreCase(pName))
//       a = pValue;
//     else if (PARAM_B.equalsIgnoreCase(pName))
//       b = pValue;
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "poincare3D";
//   }
// 
//   private double cx, cy, cz, c2, c2x, c2y, c2z, s2x, s2y, s2z;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     cx = -r * cos(a * M_PI_2) * cos(b * M_PI_2);
//     cy = r * sin(a * M_PI_2) * cos(b * M_PI_2);
//     cz = -r * sin(b * M_PI_2);
// 
//     c2 = sqr(cx) + sqr(cy) + sqr(cz);
// 
//     c2x = 2 * cx;
//     c2y = 2 * cy;
//     c2z = 2 * cz;
// 
//     s2x = sqr(cx) - sqr(cy) - sqr(cz) + 1;
//     s2y = sqr(cy) - sqr(cx) - sqr(cz) + 1;
//     s2z = sqr(cz) - sqr(cy) - sqr(cx) + 1;
//   }
// 
// }
// 
