#define PLUGIN_WARNING "_WARNING_empty_shim_for_jw_workflow"
/*
    Apophysis Plugin: hypertile1

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/Hypertile1Func.java

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
	int p;
	int q;
	double _pa;
	double _r;

} Variables;

#define APO_VARIABLE_PREFIX "hypertile1_"
#include "plugin.h"

APO_PLUGIN("hypertile1");
APO_VARIABLES(
	VAR_INTEGER(p, 3),
	VAR_INTEGER(q, 7)

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
// import org.jwildfire.base.Tools;
// import org.jwildfire.create.tina.base.Layer;
// import org.jwildfire.create.tina.base.XForm;
// import org.jwildfire.create.tina.base.XYZPoint;
// 
// import static org.jwildfire.base.mathlib.MathLib.*;
// 
// public class Hypertile1Func extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_P = "p";
//   private static final String PARAM_Q = "q";
// 
//   private static final String[] paramNames = {PARAM_P, PARAM_Q};
// 
//   private int p = 3;
//   private int q = 7;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     /* hypertile1 by Zueuk, http://zueuk.deviantart.com/art/Hyperbolic-tiling-plugins-165829025?q=sort%3Atime+gallery%3AZueuk&qo=0 */
//     double rpa = pContext.random(Integer.MAX_VALUE) * this.pa;
// 
//     double sina = sin(rpa);
//     double cosa = cos(rpa);
// 
//     double re = this.r * cosa;
//     double im = this.r * sina;
// 
//     double a = pAffineTP.x + re, b = pAffineTP.y - im;
// 
//     double c = re * pAffineTP.x - im * pAffineTP.y + 1;
//     double d = re * pAffineTP.y + im * pAffineTP.x;
// 
//     double vr = pAmount / (sqr(c) + sqr(d));
// 
//     pVarTP.x += vr * (a * c + b * d);
//     pVarTP.y += vr * (b * c - a * d);
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
//     return new Object[]{p, q};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_P.equalsIgnoreCase(pName))
//       p = limitIntVal(Tools.FTOI(pValue), 3, Integer.MAX_VALUE);
//     else if (PARAM_Q.equalsIgnoreCase(pName))
//       q = limitIntVal(Tools.FTOI(pValue), 3, Integer.MAX_VALUE);
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "hypertile1";
//   }
// 
//   private double pa, r;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     this.pa = 2 * M_PI / this.p;
// 
//     double r2 = 1.0 - (cos(2 * M_PI / this.p) - 1.0) /
//             (cos(2 * M_PI / this.p) + cos(2 * M_PI / this.q));
//     if (r2 > 0)
//       this.r = 1.0 / sqrt(r2);
//     else
//       this.r = 1.0;
// 
//   }
// 
// }
// 
