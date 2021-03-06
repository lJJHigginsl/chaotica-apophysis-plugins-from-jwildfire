#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: log_tile2

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/LogTile2Func.java

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
	double spreadx;
	double spready;
	double spreadz;

} Variables;

#define APO_VARIABLE_PREFIX "log_tile2_"
#include "plugin.h"

APO_PLUGIN("log_tile2");
APO_VARIABLES(
	VAR_REAL(spreadx, 2.0),
	VAR_REAL(spready, 2.0),
	VAR_REAL(spreadz, 0.0)

);



int PluginVarPrepare(Variation* vp)
{

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    // Log_tile by Zy0rg implemented into JWildfire by Brad Stefanov
    double spreadx = -VAR(spreadx);
    if (GOODRAND_01() < 0.5)
      spreadx = VAR(spreadx);
    double spready = -VAR(spready);
    if (GOODRAND_01() < 0.5)
      spready = VAR(spready);
    double spreadz = -VAR(spreadz);
    if (GOODRAND_01() < 0.5)
      spreadz = VAR(spreadz);
    FPx += VVAR * (FTx + spreadx * round(log(GOODRAND_01())));
    FPy += VVAR * (FTy + spready * round(log(GOODRAND_01())));
    FPz += VVAR * (FTz + spreadz * round(log(GOODRAND_01())));


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
// import static org.jwildfire.base.mathlib.MathLib.log;
// import static org.jwildfire.base.mathlib.MathLib.round;
// 
// public class LogTile2Func extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_X = "spreadx";
//   private static final String PARAM_Y = "spready";
//   private static final String PARAM_Z = "spreadz";
// 
//   private static final String[] paramNames = {PARAM_X, PARAM_Y, PARAM_Z};
// 
//   private double x = 2.0;
//   private double y = 2.0;
//   private double z = 0.0;
// 
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     // Log_tile by Zy0rg implemented into JWildfire by Brad Stefanov
//     double spreadx = -x;
//     if (pContext.random() < 0.5)
//       spreadx = x;
//     double spready = -y;
//     if (pContext.random() < 0.5)
//       spready = y;
//     double spreadz = -z;
//     if (pContext.random() < 0.5)
//       spreadz = z;
//     pVarTP.x += pAmount * (pAffineTP.x + spreadx * round(log(pContext.random())));
//     pVarTP.y += pAmount * (pAffineTP.y + spready * round(log(pContext.random())));
//     pVarTP.z += pAmount * (pAffineTP.z + spreadz * round(log(pContext.random())));
//   }
// 
//   @Override
//   public String[] getParameterNames() {
//     return paramNames;
//   }
// 
//   @Override
//   public Object[] getParameterValues() {
//     return new Object[]{x, y, z};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_X.equalsIgnoreCase(pName))
//       x = pValue;
//     else if (PARAM_Y.equalsIgnoreCase(pName))
//       y = pValue;
//     else if (PARAM_Z.equalsIgnoreCase(pName))
//       z = pValue;
// 
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "log_tile2";
//   }
// 
// }
