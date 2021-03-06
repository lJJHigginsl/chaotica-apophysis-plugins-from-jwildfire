#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: pre_dcztransl

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/PreDCZTranslFunc.java

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
	double x0;
	double x1;
	double factor;
	int overwrite;
	int clamp;
	double _x0;
	double _x1;
	double _x1_m_x0;

} Variables;

#define APO_VARIABLE_PREFIX "pre_dcztransl_"
#include "plugin.h"

APO_PLUGIN("pre_dcztransl");
APO_VARIABLES(
	VAR_REAL(x0, 0.0),
	VAR_REAL(x1, 1.0),
	VAR_REAL(factor, 1.0),
	VAR_INTEGER(overwrite, 1),
	VAR_INTEGER(clamp, 0)

);



int PluginVarPrepare(Variation* vp)
{
    VAR(_x0) = VAR(x0) < VAR(x1) ? VAR(x0) : VAR(x1);
    VAR(_x1) = VAR(x0) > VAR(x1) ? VAR(x0) : VAR(x1);
    VAR(_x1_m_x0) = VAR(_x1) - VAR(_x0) == 0 ? SMALL_EPSILON : VAR(_x1) - VAR(_x0);

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    /* pre_dcztransl by Xyrus02, http://xyrus02.deviantart.com/art/DC-ZTransl-plugins-for-Apo7X-210719008?q=gallery%3Afractal-resources%2F24660058&qo=32 */
    double zf = VAR(factor) * (TC - VAR(_x0)) / VAR(_x1_m_x0);
    if (VAR(clamp) != 0)
      zf = zf < 0 ? 0 : zf > 1 ? 1 : zf;
    FTx = VVAR * FTx;
    FTy = VVAR * FTy;

    if (VAR(overwrite) == 0)
      FTz = VVAR * FTz * zf;
    else
      FTz = VVAR * zf;


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
// import static org.jwildfire.base.mathlib.MathLib.SMALL_EPSILON;
// 
// public class PreDCZTranslFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_X0 = "x0";
//   private static final String PARAM_X1 = "x1";
//   private static final String PARAM_FACTOR = "factor";
//   private static final String PARAM_OVERWRITE = "overwrite";
//   private static final String PARAM_CLAMP = "clamp";
// 
//   private static final String[] paramNames = {PARAM_X0, PARAM_X1, PARAM_FACTOR, PARAM_OVERWRITE, PARAM_CLAMP};
// 
//   private double x0 = 0.0;
//   private double x1 = 1.0;
//   private double factor = 1.0;
//   private int overwrite = 1;
//   private int clamp = 0;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* pre_dcztransl by Xyrus02, http://xyrus02.deviantart.com/art/DC-ZTransl-plugins-for-Apo7X-210719008?q=gallery%3Afractal-resources%2F24660058&qo=32 */
//     double zf = factor * (pAffineTP.color - _x0) / _x1_m_x0;
//     if (clamp != 0)
//       zf = zf < 0 ? 0 : zf > 1 ? 1 : zf;
//     pAffineTP.x = pAmount * pAffineTP.x;
//     pAffineTP.y = pAmount * pAffineTP.y;
// 
//     if (overwrite == 0)
//       pAffineTP.z = pAmount * pAffineTP.z * zf;
//     else
//       pAffineTP.z = pAmount * zf;
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{x0, x1, factor, overwrite, clamp};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_X0.equalsIgnoreCase(pName))
//       x0 = limitVal(pValue, 0.0, 1.0);
//     else if (PARAM_X1.equalsIgnoreCase(pName))
//       x1 = limitVal(pValue, 0.0, 1.0);
//     else if (PARAM_FACTOR.equalsIgnoreCase(pName))
//       factor = pValue;
//     else if (PARAM_OVERWRITE.equalsIgnoreCase(pName))
//       overwrite = limitIntVal(Tools.FTOI(pValue), 0, 1);
//     else if (PARAM_CLAMP.equalsIgnoreCase(pName))
//       clamp = limitIntVal(Tools.FTOI(pValue), 0, 1);
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "pre_dcztransl";
//   }
// 
//   private double _x0;
//   private double _x1;
//   private double _x1_m_x0;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     _x0 = x0 < x1 ? x0 : x1;
//     _x1 = x0 > x1 ? x0 : x1;
//     _x1_m_x0 = _x1 - _x0 == 0 ? SMALL_EPSILON : _x1 - _x0;
//   }
// 
//   @Override
//   public int getPriority() {
//     return -1;
//   }
// 
// }
// 
