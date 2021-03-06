#define PLUGIN_WARNING "NOTE_modded_for_jwildfire_workflow"
/*
    Apophysis Plugin: bwrands

    Port of: https://github.com/thargor6/JWildfire/blob/master/src/org/jwildfire/create/tina/variation/BWRandsFunc.java

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
	double cellsize;
	double space;
	double gain;
	double inner_twist;
	double outer_twist;
	int seed;
	double rrot;
	double rmin;
	double loonie_chance;
	double petals_chance;
	int minpetals;
	int maxpetals;
	double _g2;
	double _r2;
	double _rfactor;
	int _petx;
	int _pety;

} Variables;

#define APO_VARIABLE_PREFIX "bwrands_"
#include "plugin.h"

APO_PLUGIN("bwrands");
APO_VARIABLES(
	VAR_REAL(cellsize, 1.0),
	VAR_REAL(space, 0.0),
	VAR_REAL(gain, 1.25),
	VAR_REAL(inner_twist, 0.0),
	VAR_REAL(outer_twist, 0.0),
	VAR_INTEGER(seed, 3210),
	VAR_REAL(rrot, 1.0),
	VAR_REAL(rmin, 0.25),
	VAR_REAL(loonie_chance, 0.50),
	VAR_REAL(petals_chance, 0.50),
	VAR_INTEGER(minpetals, 3),
	VAR_INTEGER(maxpetals, 20)

);


int bytemix (Variation* vp, int a, int b) {

    return ((a & 0x5A5A) ^ (b & 0xA5A5));
}

int bytexim (Variation* vp, int a, int b) {
 // variation...
    return ((a & 0xAAAA) ^ (b & 0x5555));
}

int byteshf (Variation* vp, int a, int b) {

    return ((a << 8) ^ (b >> 8)) & 0xFFFF;
}

int byteprimes (Variation* vp, int a, int b) {
 // variation...
    return ((a * 857 - 4) & 0xFF00 ^ (-b * 977 + 8) & 0x00FF);
}


int PluginVarPrepare(Variation* vp)
{
    double radius = 0.5 * (VAR(cellsize) / (1.0 + VAR(space) * VAR(space)));

    // g2 is multiplier for radius
    VAR(_g2) = sqr(VAR(gain)) / radius + 1.0e-6;

    // Start max_bubble as maximum x or y value before applying bubble
    double max_bubble = VAR(_g2) * radius;

    if (max_bubble > 2.0) {
      // Values greater than 2.0 "recurve" round the back of the bubble
      max_bubble = 1.0;
    } else {
      // Expand smaller bubble to fill the VAR(space)
      max_bubble *= 1.0 / ((max_bubble * max_bubble) / 4.0 + 1.0);
    }

    VAR(_r2) = radius * radius;
    VAR(_rfactor) = radius / max_bubble;

    VAR(_petx) = MIN(VAR(minpetals), VAR(maxpetals));
    VAR(_pety) = iabs(VAR(maxpetals) - VAR(minpetals));
    // if (VAR(pety) >= EPS) VAR(pety) += 1;

    return TRUE;
}

int PluginVarCalc(Variation* vp)
{
    // bwrands, by dark-beam
    double Vx, Vy; // V is "global" vector,
    double Cx, Cy; // C is "cell centre" vector
    double Lx, Ly; // L is "local" bubble vector
    double r, theta, s, c;
    double Vv2, flwr;

    Vx = FTx;
    Vy = FTy;

    if (fabs(VAR(cellsize)) < SMALL_EPSILON) {
      // Linear if cells are too small
      FPx += VVAR * Vx;
      FPy += VVAR * Vy;
      if (true /* pContext\.isPreserveZCoordinate() */) {
        FPz += VVAR * FTz;
      }
      return TRUE;
    }

    int Ix = (int) floor(Vx / VAR(cellsize));
    int Iy = (int) floor(Vy / VAR(cellsize));

    Cx = (Ix + 0.5) * VAR(cellsize);
    Cy = (Iy + 0.5) * VAR(cellsize);

    // mess with Ix & Iy to get random
    int xx = Ix ^ 0xB641;
    int yy = Iy ^ 0x9D81;
    int xy = xx * yy + VAR(seed);
    xx &= 0xFFFF;
    yy &= 0xFFFF;
    xy &= 0xFFFF;
    // no less than 3 remixes are needed to get good randomness
    // use always identical mixes or you lose some data (you don't want this!)
    int tt = bytemix(vp, xx, yy);
    yy = bytemix(vp, yy, xx);
    xx = tt;
    tt = byteshf(vp, xx, xy);
    yy = byteshf(vp, xy, yy);
    xx = tt;
    tt = bytexim(vp, xx, yy);
    yy = bytexim(vp, yy, xx);
    xx = tt;
    double Ssz = ((double) xx) / 65535.0; // to get a range not 0-1 for circles size edit here
    Ssz = VAR(rmin) + Ssz * (1.0 - VAR(rmin));
    double Aan = VAR(rrot) * M_2PI * ((double) yy) / 65535.0; // to get a range not 0-2pi for rotations size edit here
    tt = byteprimes(vp, xx, yy);
    yy = byteprimes(vp, yy, xx);
    xx = tt;
    double LoonieChance = -((double) xx) / 65535.0 + VAR(loonie_chance); // 0.5 for a chance 50% !
    double PetalsChance = 0.0;
    if (LoonieChance < 0)
      PetalsChance = LoonieChance + VAR(petals_chance);
    // user choice but don't upset modulus...
    double NPetals;
    if (VAR(_pety) == 0)
      NPetals = (double) VAR(_petx); // ... when min = max
    else
      NPetals = (double) (VAR(_petx) + (yy >> 3) % (1 + VAR(_pety))); // yy last byte is not enough random. But yy >> 3 is good

    if (LoonieChance <= PetalsChance)
      LoonieChance = -1.0; // the bigger probability must win.

    // if more random values are needed remix again :D

    Lx = Vx - Cx;
    Ly = Vy - Cy;

    Vv2 = Ssz * VAR(_r2);

    if ((Lx * Lx + Ly * Ly) > Vv2) {
      // Linear if outside the bubble
      FPx += VVAR * Vx;
      FPy += VVAR * Vy;
      if (true /* pContext\.isPreserveZCoordinate() */) {
        FPz += VVAR * FTz;
      }
      return TRUE;
    }

    if (LoonieChance > 0.0) {
      // We're in the loonie!
      // r = Vv2 / (Lx * Lx + Ly * Ly) - MAX(MIN(VAR(bwrands_gain),1.0),0.0); LOOKING BAD!!!
      r = Vv2 / (Lx * Lx + Ly * Ly) - 1.0;
      r = sqrt(r);
      Lx *= r;
      Ly *= r;
      Vv2 = 1.0; // recycled var
    } else if (PetalsChance > 0.0) {
      // We're in the petals!
      flwr = NPetals / M_2PI * (M_PI + atan2(Ly, Lx));
      flwr = flwr - (int) (flwr);
      flwr = fabs(flwr - 0.5) * 2.0;
      r = sqrt(Lx * Lx + Ly * Ly);

      // We need a little chaos game to fill the empty VAR(space) outside the flower.
      PetalsChance = GOODRAND_01();

      if (PetalsChance < .5 * (flwr + .5)) {
        // petals
        Lx *= (1.0 - r) * (flwr * 1.1); // 1.1 remove the ugly border
        Ly *= (1.0 - r) * (flwr * 1.1); // 1.1 remove the ugly border
      } else {
        Vv2 = sqrt(Vv2); // dist to circle border
        // filling the rest of the circle
        Lx *= (Vv2 - r * (1.0 - flwr)) / (r + SMALL_EPSILON);
        Ly *= (Vv2 - r * (1.0 - flwr)) / (r + SMALL_EPSILON);
      }

      Vv2 = 1.0; // recycled var
    } else {
      // We're in the bubble!
      // Bubble distortion on local co-ordinates:
      Lx *= VAR(_g2);
      Ly *= VAR(_g2);
      r = VAR(_rfactor) / ((Lx * Lx + Ly * Ly) / (4.0 * Ssz) + 1.0);
      Lx *= r;
      Ly *= r;
      Vv2 = sqrt(Ssz); // recycled var
    }
    // Spin around the centre:
    r = Vv2 * (Lx * Lx + Ly * Ly) / VAR(_r2); // r should be 0.0 - 1.0
    theta = VAR(inner_twist) * (1.0 - r) + VAR(outer_twist) * r;
    s = sin(theta + Aan);
    c = cos(theta + Aan);

    // Add rotated local vectors direct to centre (avoids use of temp storage)
    Vx = Cx + c * Lx + s * Ly;
    Vy = Cy - s * Lx + c * Ly;

    // Finally add values in
    FPx += VVAR * Vx;
    FPy += VVAR * Vy;
    if (true /* pContext\.isPreserveZCoordinate() */) {
      FPz += VVAR * FTz;
    }


    return TRUE;
}





// original java file embedded here:
//
// /*
//   JWildfire - an image and animation processor written in Java 
//   Copyright (C) 1995-2015 Andreas Maschke
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
// public class BWRandsFunc extends VariationFunc {
//   private static final long serialVersionUID = 1L;
// 
//   private static final String PARAM_CELLSIZE = "cellsize";
//   private static final String PARAM_SPACE = "space";
//   private static final String PARAM_GAIN = "gain";
//   private static final String PARAM_INNER_TWIST = "inner_twist";
//   private static final String PARAM_OUTER_TWIST = "outer_twist";
//   private static final String PARAM_SEED = "seed";
//   private static final String PARAM_RROT = "rrot";
//   private static final String PARAM_RMIN = "rmin";
//   private static final String PARAM_LOONIE_CHANCE = "loonie_chance";
//   private static final String PARAM_PETALS_CHANCE = "petals_chance";
//   private static final String PARAM_MIN_PETALS = "minpetals";
//   private static final String PARAM_MAX_PETALS = "maxpetals";
// 
//   private static final String[] paramNames = {PARAM_CELLSIZE, PARAM_SPACE, PARAM_GAIN, PARAM_INNER_TWIST, PARAM_OUTER_TWIST, PARAM_SEED, PARAM_RROT, PARAM_RMIN, PARAM_LOONIE_CHANCE, PARAM_PETALS_CHANCE, PARAM_MIN_PETALS, PARAM_MAX_PETALS};
// 
//   private double cellsize = 1.0;
//   private double space = 0.0;
//   private double gain = 1.25;
//   private double inner_twist = 0.0;
//   private double outer_twist = 0.0;
//   private int seed = 3210;
//   private double rrot = 1.0;
//   private double rmin = 0.25;
//   private double loonie_chance = 0.50;
//   private double petals_chance = 0.50;
//   private int minpetals = 3;
//   private int maxpetals = 20;
//   // precalculated
//   private double _g2;
//   private double _r2;
//   private double _rfactor;
//   private int _petx, _pety;
// 
//   @Override
//   public void transform(FlameTransformationContext pContext, XForm pXForm, XYZPoint pAffineTP, XYZPoint pVarTP, double pAmount) {
//     // bwrands, by dark-beam
//     double Vx, Vy; // V is "global" vector,
//     double Cx, Cy; // C is "cell centre" vector
//     double Lx, Ly; // L is "local" bubble vector
//     double r, theta, s, c;
//     double Vv2, flwr;
// 
//     Vx = pAffineTP.x;
//     Vy = pAffineTP.y;
// 
//     if (fabs(cellsize) < SMALL_EPSILON) {
//       // Linear if cells are too small
//       pVarTP.x += pAmount * Vx;
//       pVarTP.y += pAmount * Vy;
//       if (pContext.isPreserveZCoordinate()) {
//         pVarTP.z += pAmount * pAffineTP.z;
//       }
//       return;
//     }
// 
//     int Ix = (int) floor(Vx / cellsize);
//     int Iy = (int) floor(Vy / cellsize);
// 
//     Cx = (Ix + 0.5) * cellsize;
//     Cy = (Iy + 0.5) * cellsize;
// 
//     // mess with Ix & Iy to get random
//     int xx = Ix ^ 0xB641;
//     int yy = Iy ^ 0x9D81;
//     int xy = xx * yy + seed;
//     xx &= 0xFFFF;
//     yy &= 0xFFFF;
//     xy &= 0xFFFF;
//     // no less than 3 remixes are needed to get good randomness
//     // use always identical mixes or you lose some data (you don't want this!)
//     int tt = bytemix(xx, yy);
//     yy = bytemix(yy, xx);
//     xx = tt;
//     tt = byteshf(xx, xy);
//     yy = byteshf(xy, yy);
//     xx = tt;
//     tt = bytexim(xx, yy);
//     yy = bytexim(yy, xx);
//     xx = tt;
//     double Ssz = ((double) xx) / 65535.0; // to get a range not 0-1 for circles size edit here
//     Ssz = rmin + Ssz * (1.0 - rmin);
//     double Aan = rrot * M_2PI * ((double) yy) / 65535.0; // to get a range not 0-2pi for rotations size edit here
//     tt = byteprimes(xx, yy);
//     yy = byteprimes(yy, xx);
//     xx = tt;
//     double LoonieChance = -((double) xx) / 65535.0 + loonie_chance; // 0.5 for a chance 50% !
//     double PetalsChance = 0.0;
//     if (LoonieChance < 0)
//       PetalsChance = LoonieChance + petals_chance;
//     // user choice but don't upset modulus...
//     double NPetals;
//     if (_pety == 0)
//       NPetals = (double) _petx; // ... when min = max
//     else
//       NPetals = (double) (_petx + (yy >> 3) % (1 + _pety)); // yy last byte is not enough random. But yy >> 3 is good
// 
//     if (LoonieChance <= PetalsChance)
//       LoonieChance = -1.0; // the bigger probability must win.
// 
//     // if more random values are needed remix again :D
// 
//     Lx = Vx - Cx;
//     Ly = Vy - Cy;
// 
//     Vv2 = Ssz * _r2;
// 
//     if ((Lx * Lx + Ly * Ly) > Vv2) {
//       // Linear if outside the bubble
//       pVarTP.x += pAmount * Vx;
//       pVarTP.y += pAmount * Vy;
//       if (pContext.isPreserveZCoordinate()) {
//         pVarTP.z += pAmount * pAffineTP.z;
//       }
//       return;
//     }
// 
//     if (LoonieChance > 0.0) {
//       // We're in the loonie!
//       // r = Vv2 / (Lx * Lx + Ly * Ly) - MAX(MIN(VAR(bwrands_gain),1.0),0.0); LOOKING BAD!!!
//       r = Vv2 / (Lx * Lx + Ly * Ly) - 1.0;
//       r = sqrt(r);
//       Lx *= r;
//       Ly *= r;
//       Vv2 = 1.0; // recycled var
//     } else if (PetalsChance > 0.0) {
//       // We're in the petals!
//       flwr = NPetals / M_2PI * (M_PI + atan2(Ly, Lx));
//       flwr = flwr - (int) (flwr);
//       flwr = fabs(flwr - 0.5) * 2.0;
//       r = sqrt(Lx * Lx + Ly * Ly);
// 
//       // We need a little chaos game to fill the empty space outside the flower.
//       PetalsChance = pContext.random();
// 
//       if (PetalsChance < .5 * (flwr + .5)) {
//         // petals
//         Lx *= (1.0 - r) * (flwr * 1.1); // 1.1 remove the ugly border
//         Ly *= (1.0 - r) * (flwr * 1.1); // 1.1 remove the ugly border
//       } else {
//         Vv2 = sqrt(Vv2); // dist to circle border
//         // filling the rest of the circle
//         Lx *= (Vv2 - r * (1.0 - flwr)) / (r + SMALL_EPSILON);
//         Ly *= (Vv2 - r * (1.0 - flwr)) / (r + SMALL_EPSILON);
//       }
// 
//       Vv2 = 1.0; // recycled var
//     } else {
//       // We're in the bubble!
//       // Bubble distortion on local co-ordinates:
//       Lx *= _g2;
//       Ly *= _g2;
//       r = _rfactor / ((Lx * Lx + Ly * Ly) / (4.0 * Ssz) + 1.0);
//       Lx *= r;
//       Ly *= r;
//       Vv2 = sqrt(Ssz); // recycled var
//     }
//     // Spin around the centre:
//     r = Vv2 * (Lx * Lx + Ly * Ly) / _r2; // r should be 0.0 - 1.0
//     theta = inner_twist * (1.0 - r) + outer_twist * r;
//     s = sin(theta + Aan);
//     c = cos(theta + Aan);
// 
//     // Add rotated local vectors direct to centre (avoids use of temp storage)
//     Vx = Cx + c * Lx + s * Ly;
//     Vy = Cy - s * Lx + c * Ly;
// 
//     // Finally add values in
//     pVarTP.x += pAmount * Vx;
//     pVarTP.y += pAmount * Vy;
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
//     return new Object[]{cellsize, space, gain, inner_twist, outer_twist, seed, rrot, rmin, loonie_chance, petals_chance, minpetals, maxpetals};
//   }
// 
//   @Override
//   public void setParameter(String pName, double pValue) {
//     if (PARAM_CELLSIZE.equalsIgnoreCase(pName))
//       cellsize = pValue;
//     else if (PARAM_SPACE.equalsIgnoreCase(pName))
//       space = pValue;
//     else if (PARAM_GAIN.equalsIgnoreCase(pName))
//       gain = pValue;
//     else if (PARAM_INNER_TWIST.equalsIgnoreCase(pName))
//       inner_twist = pValue;
//     else if (PARAM_OUTER_TWIST.equalsIgnoreCase(pName))
//       outer_twist = pValue;
//     else if (PARAM_SEED.equalsIgnoreCase(pName))
//       seed = Tools.FTOI(pValue);
//     else if (PARAM_RROT.equalsIgnoreCase(pName))
//       rrot = limitVal(pValue, 0, 1.0);
//     else if (PARAM_RMIN.equalsIgnoreCase(pName))
//       rmin = limitVal(pValue, 0, 1.0);
//     else if (PARAM_LOONIE_CHANCE.equalsIgnoreCase(pName))
//       loonie_chance = limitVal(pValue, 0, 1.0);
//     else if (PARAM_PETALS_CHANCE.equalsIgnoreCase(pName))
//       petals_chance = limitVal(pValue, 0, 1.0);
//     else if (PARAM_MIN_PETALS.equalsIgnoreCase(pName))
//       minpetals = limitIntVal(Tools.FTOI(pValue), 1, 1000);
//     else if (PARAM_MAX_PETALS.equalsIgnoreCase(pName))
//       maxpetals = limitIntVal(Tools.FTOI(pValue), 1, 1000);
//     else
//       throw new IllegalArgumentException(pName);
//   }
// 
//   @Override
//   public String getName() {
//     return "bwrands";
//   }
// 
//   @Override
//   public void init(FlameTransformationContext pContext, Layer pLayer, XForm pXForm, double pAmount) {
//     double radius = 0.5 * (cellsize / (1.0 + space * space));
// 
//     // g2 is multiplier for radius
//     _g2 = sqr(gain) / radius + 1.0e-6;
// 
//     // Start max_bubble as maximum x or y value before applying bubble
//     double max_bubble = _g2 * radius;
// 
//     if (max_bubble > 2.0) {
//       // Values greater than 2.0 "recurve" round the back of the bubble
//       max_bubble = 1.0;
//     } else {
//       // Expand smaller bubble to fill the space
//       max_bubble *= 1.0 / ((max_bubble * max_bubble) / 4.0 + 1.0);
//     }
// 
//     _r2 = radius * radius;
//     _rfactor = radius / max_bubble;
// 
//     _petx = Math.min(minpetals, maxpetals);
//     _pety = iabs(maxpetals - minpetals);
//     // if (VAR(pety) >= EPS) VAR(pety) += 1;
//   }
// 
//   private int bytemix(int a, int b) {
//     return ((a & 0x5A5A) ^ (b & 0xA5A5));
//   }
// 
//   private int bytexim(int a, int b) { // variation...
//     return ((a & 0xAAAA) ^ (b & 0x5555));
//   }
// 
//   private int byteshf(int a, int b) {
//     return ((a << 8) ^ (b >> 8)) & 0xFFFF;
//   }
// 
//   private int byteprimes(int a, int b) { // variation...
//     return ((a * 857 - 4) & 0xFF00 ^ (-b * 977 + 8) & 0x00FF);
//   }
// 
// }
// 
