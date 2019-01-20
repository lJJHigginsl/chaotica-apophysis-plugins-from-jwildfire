#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: jubiQ

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/JubiQFunc.java

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
	int power; // genRandomPower()
	double dist;
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double qat;
	double qax;
	double qay;
	double qaz;
	double qbt;
	double qbx;
	double qby;
	double qbz;
	double qct;
	double qcx;
	double qcy;
	double qcz;
	double qdt;
	double qdx;
	double qdy;
	double qdz;
	int _absN;
	double _cN;

} Variables;

#define APO_VARIABLE_PREFIX "jubiQ_"
#include "plugin.h"

APO_PLUGIN("jubiQ");
APO_VARIABLES(
	VAR_INTEGER(power, 0),
	VAR_REAL(dist, 1.0),
	VAR_REAL(a, 1.0),
	VAR_REAL(b, 0.0),
	VAR_REAL(c, 0.0),
	VAR_REAL(d, 1.0),
	VAR_REAL(e, 0.0),
	VAR_REAL(f, 0.0),
	VAR_REAL(qat, 0.0),
	VAR_REAL(qax, 0.0),
	VAR_REAL(qay, 0.0),
	VAR_REAL(qaz, 0.0),
	VAR_REAL(qbt, 0.0),
	VAR_REAL(qbx, 0.0),
	VAR_REAL(qby, 0.0),
	VAR_REAL(qbz, 0.0),
	VAR_REAL(qct, 0.0),
	VAR_REAL(qcx, 0.0),
	VAR_REAL(qcy, 0.0),
	VAR_REAL(qcz, 0.0),
	VAR_REAL(qdt, 1.0),
	VAR_REAL(qdx, 0.0),
	VAR_REAL(qdy, 0.0),
	VAR_REAL(qdz, 0.0)

);


int genRandomPower (Variation* vp) {

    int res = (int) (GOODRAND_01() * 5.0 + 2.5);
    return GOODRAND_01() < 0.5 ? res : -res;
}


int PluginVarPrepare(Variation* vp)
{
    VAR(_absN) = iabs((VAR(power)));
    VAR(_cN) = VAR(dist) / VAR(power) * 0.5;

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
     /* A mix of julian2 by Xyrus02, http://xyrus02.deviantart.com/art/JuliaN2-Plugin-for-Apophysis-136717838 
     and Mobiq by zephyrtronium https://zephyrtronium.deviantart.com/art/Mobiq-Apophysis-Plugin-170449212 by Brad Stefanov*/


    double t1 = VAR(qat);
    double t2 = FTx;
    double t3 = VAR(qbt);
    double t4 = VAR(qct);
    double t5 = VAR(qdt);
    double x1 = VAR(qax);
    double x2 = FTy;
    double x3 = VAR(qbx);
    double x4 = VAR(qcx);
    double x5 = VAR(qdx);
    double y1 = VAR(qay);
    double y2 = FTz;
    double y3 = VAR(qby);
    double y4 = VAR(qcy);
    double y5 = VAR(qdy);
    double z1 = VAR(qaz);
    /* z2 is 0 and simplified out (there is no fourth generated coordinate). */
    double z3 = VAR(qbz);
    double z4 = VAR(qcz);
    double z5 = VAR(qdz);

    double nt = t1 * t2 - x1 * x2 - y1 * y2 + t3;
    double nx = t1 * x2 + x1 * t2 - z1 * y2 + x3;
    double ny = t1 * y2 + y1 * t2 + z1 * x2 + y3;
    double nz = z1 * t2 + x1 * y2 - y1 * x2 + z3;
    double dt = t4 * t2 - x4 * x2 - y4 * y2 + t5;
    double dx = t4 * x2 + x4 * t2 - z4 * y2 + x5;
    double dy = t4 * y2 + y4 * t2 + z4 * x2 + y5;
    double dz = z4 * t2 + x4 * y2 - y4 * x2 + z5;
    double ni = VVAR / (sqr(dt) + sqr(dx) + sqr(dy) + sqr(dz));


    if (VAR(power) == 0)
      return TRUE;
    double x = VAR(a) * FTx + VAR(b) * FTy + (nt * dt + nx * dx + ny * dy + nz * dz) * ni + VAR(e);
    double y = VAR(c) * FTx + VAR(d) * FTy + (nx * dt - nt * dx - ny * dz + nz * dy) * ni + VAR(f);
    double sina = 0.0, cosa = 0.0;
    double angle = (atan2(y, x) + M_2PI * (GOODRAND_0X(INT_MAX) % VAR(_absN))) / VAR(power);
    double r = VVAR * pow(sqr(x) + sqr(y), VAR(_cN));

    sina = sin(angle);
    cosa = cos(angle);
    FPx += r * cosa;
    FPy += r * sina;

    double z = FTz / 2;
    double r2d = sqr(FTx) + sqr(FTy);
    double r3d = sqrt(r2d + sqr(z));
    double r2 = VVAR / (sqrt(r3d) * r3d);

    FPz += (ny * dt - nt * dy - nz * dx + nx * dz) * ni + r2 * z;



    return TRUE;
}





// original java file embedded here:
//
// /*
//   Jwildfire - an image and animation processor written in Java 
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
// public class JubiQFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_POWER = "power";
//   private static final String PARAM_DIST = "dist";
//   private static final String PARAM_A = "a";
//   private static final String PARAM_B = "b";
//   private static final String PARAM_C = "c";
//   private static final String PARAM_D = "d";
//   private static final String PARAM_E = "e";
//   private static final String PARAM_F = "f";
//   private static final String PARAM_AT = "qat";
//   private static final String PARAM_AX = "qax";
//   private static final String PARAM_AY = "qay";
//   private static final String PARAM_AZ = "qaz";
//   private static final String PARAM_BT = "qbt";
//   private static final String PARAM_BX = "qbx";
//   private static final String PARAM_BY = "qby";
//   private static final String PARAM_BZ = "qbz";
//   private static final String PARAM_CT = "qct";
//   private static final String PARAM_CX = "qcx";
//   private static final String PARAM_CY = "qcy";
//   private static final String PARAM_CZ = "qcz";
//   private static final String PARAM_DT = "qdt";
//   private static final String PARAM_DX = "qdx";
//   private static final String PARAM_DY = "qdy";
//   private static final String PARAM_DZ = "qdz";
//   private static final String[] paramNames = {PARAM_POWER, PARAM_DIST, PARAM_A, PARAM_B, PARAM_C, PARAM_D, PARAM_E, PARAM_F, PARAM_AT, PARAM_AX, PARAM_AY, PARAM_AZ, PARAM_BT, PARAM_BX, PARAM_BY, PARAM_BZ, PARAM_CT, PARAM_CX, PARAM_CY, PARAM_CZ, PARAM_DT, PARAM_DX, PARAM_DY, PARAM_DZ};
// 
//   private int power = genRandomPower();
//   private double dist = 1.0;
//   private double a = 1.0;
//   private double b = 0.0;
//   private double c = 0.0;
//   private double d = 1.0;
//   private double e = 0.0;
//   private double f = 0.0;
//   private double qat = 0.0;
//   private double qax = 0.0;
//   private double qay = 0.0;
//   private double qaz = 0.0;
//   private double qbt = 0.0;
//   private double qbx = 0.0;
//   private double qby = 0.0;
//   private double qbz = 0.0;
//   private double qct = 0.0;
//   private double qcx = 0.0;
//   private double qcy = 0.0;
//   private double qcz = 0.0;
//   private double qdt = 1.0;
//   private double qdx = 0.0;
//   private double qdy = 0.0;
//   private double qdz = 0.0;
// 
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//      /* A mix of julian2 by Xyrus02, http://xyrus02.deviantart.com/art/JuliaN2-Plugin-for-Apophysis-136717838 
//      and Mobiq by zephyrtronium https://zephyrtronium.deviantart.com/art/Mobiq-Apophysis-Plugin-170449212 by Brad Stefanov*/
// 
// 
//     double t1 = qat;
//     double t2 = pAffineTP.x;
//     double t3 = qbt;
//     double t4 = qct;
//     double t5 = qdt;
//     double x1 = qax;
//     double x2 = pAffineTP.y;
//     double x3 = qbx;
//     double x4 = qcx;
//     double x5 = qdx;
//     double y1 = qay;
//     double y2 = pAffineTP.z;
//     double y3 = qby;
//     double y4 = qcy;
//     double y5 = qdy;
//     double z1 = qaz;
//     /* z2 is 0 and simplified out (there is no fourth generated coordinate). */
//     double z3 = qbz;
//     double z4 = qcz;
//     double z5 = qdz;
// 
//     double nt = t1 * t2 - x1 * x2 - y1 * y2 + t3;
//     double nx = t1 * x2 + x1 * t2 - z1 * y2 + x3;
//     double ny = t1 * y2 + y1 * t2 + z1 * x2 + y3;
//     double nz = z1 * t2 + x1 * y2 - y1 * x2 + z3;
//     double dt = t4 * t2 - x4 * x2 - y4 * y2 + t5;
//     double dx = t4 * x2 + x4 * t2 - z4 * y2 + x5;
//     double dy = t4 * y2 + y4 * t2 + z4 * x2 + y5;
//     double dz = z4 * t2 + x4 * y2 - y4 * x2 + z5;
//     double ni = pAmount / (sqr(dt) + sqr(dx) + sqr(dy) + sqr(dz));
// 
// 
//     if (power == 0)
//       return;
//     double x = a * pAffineTP.x + b * pAffineTP.y + (nt * dt + nx * dx + ny * dy + nz * dz) * ni + e;
//     double y = c * pAffineTP.x + d * pAffineTP.y + (nx * dt - nt * dx - ny * dz + nz * dy) * ni + f;
//     double sina = 0.0, cosa = 0.0;
//     double angle = (atan2(y, x) + M_2PI * (pContext.random(Integer.MAX_VALUE) % _absN)) / power;
//     double r = pAmount * pow(sqr(x) + sqr(y), _cN);
// 
//     sina = sin(angle);
//     cosa = cos(angle);
//     pVarTP.x += r * cosa;
//     pVarTP.y += r * sina;
// 
//     double z = pAffineTP.z / 2;
//     double r2d = sqr(pAffineTP.x) + sqr(pAffineTP.y);
//     double r3d = sqrt(r2d + sqr(z));
//     double r2 = pAmount / (sqrt(r3d) * r3d);
// 
//     pVarTP.z += (ny * dt - nt * dy - nz * dx + nx * dz) * ni + r2 * z;
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
//     return new Object[]{power, dist, a, b, c, d, e, f, qat, qax, qay, qaz, qbt, qbx, qby, qbz, qct, qcx, qcy, qcz, qdt, qdx, qdy, qdz};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_POWER.equalsIgnoreCase(pName))
//       power = Tools.FTOI(pValue);
//     else if (PARAM_DIST.equalsIgnoreCase(pName))
//       dist = pValue;
//     else if (PARAM_A.equalsIgnoreCase(pName))
//       a = pValue;
//     else if (PARAM_B.equalsIgnoreCase(pName))
//       b = pValue;
//     else if (PARAM_C.equalsIgnoreCase(pName))
//       c = pValue;
//     else if (PARAM_D.equalsIgnoreCase(pName))
//       d = pValue;
//     else if (PARAM_E.equalsIgnoreCase(pName))
//       e = pValue;
//     else if (PARAM_F.equalsIgnoreCase(pName))
//       f = pValue;
//     else if (PARAM_AT.equalsIgnoreCase(pName))
//       qat = pValue;
//     else if (PARAM_AX.equalsIgnoreCase(pName))
//       qax = pValue;
//     else if (PARAM_AY.equalsIgnoreCase(pName))
//       qay = pValue;
//     else if (PARAM_AZ.equalsIgnoreCase(pName))
//       qaz = pValue;
//     else if (PARAM_BT.equalsIgnoreCase(pName))
//       qbt = pValue;
//     else if (PARAM_BX.equalsIgnoreCase(pName))
//       qbx = pValue;
//     else if (PARAM_BY.equalsIgnoreCase(pName))
//       qby = pValue;
//     else if (PARAM_BZ.equalsIgnoreCase(pName))
//       qbz = pValue;
//     else if (PARAM_CT.equalsIgnoreCase(pName))
//       qct = pValue;
//     else if (PARAM_CX.equalsIgnoreCase(pName))
//       qcx = pValue;
//     else if (PARAM_CY.equalsIgnoreCase(pName))
//       qcy = pValue;
//     else if (PARAM_CZ.equalsIgnoreCase(pName))
//       qcz = pValue;
//     else if (PARAM_DT.equalsIgnoreCase(pName))
//       qdt = pValue;
//     else if (PARAM_DX.equalsIgnoreCase(pName))
//       qdx = pValue;
//     else if (PARAM_DY.equalsIgnoreCase(pName))
//       qdy = pValue;
//     else if (PARAM_DZ.equalsIgnoreCase(pName))
//       qdz = pValue;
// 
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "jubiQ";
//   }
// 
//   private int genRandomPower() {
//     int res = (int) (Math.random() * 5.0 + 2.5);
//     return Math.random() < 0.5 ? res : -res;
//   }
// 
//   private int _absN;
//   private double _cN;
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     _absN = iabs(Tools.FTOI(power));
//     _cN = dist / power * 0.5;
//   }
// 
// }
// 
