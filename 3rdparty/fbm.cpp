// *****************************************************************************
// fbm.cpp                                                         Tao3D project
// *****************************************************************************
//
// File description:
//
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2013, Christophe de Dinechin <christophe@taodyne.com>
// (C) 2011,2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

#include <QtGlobal>

#include <time.h>
#include <stdlib.h>
#include "fbm.h"

#if defined(Q_CC_MSVC)
#pragma warning(disable:4244)
#endif

/* Definitions used by the noise2() functions */

//#define B 0x100
//#define BM 0xff
#define B 0x20
#define BM 0x1f

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

static int   p[B + B + 2];
static float g3[B + B + 2][3];
static float g2[B + B + 2][2];
static float g1[B + B + 2];
static int   start = 1;

static void init(void);

#define s_curve(t) ( t * t * (3. - 2. * t) )

#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.;
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

float noise3(float vec[3])
{
    int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
    register int i, j;

    if (start) {
	start = 0;
	init();
    }

    setup(0, bx0,bx1, rx0,rx1);
    setup(1, by0,by1, ry0,ry1);
    setup(2, bz0,bz1, rz0,rz1);

    i = p[ bx0 ];
    j = p[ bx1 ];

    b00 = p[ i + by0 ];
    b10 = p[ j + by0 ];
    b01 = p[ i + by1 ];
    b11 = p[ j + by1 ];

    t  = s_curve(rx0);
    sy = s_curve(ry0);
    sz = s_curve(rz0);


    q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
    q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
    a = lerp(t, u, v);

    q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
    q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
    b = lerp(t, u, v);

    c = lerp(sy, a, b);

    q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
    q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
    a = lerp(t, u, v);

    q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
    q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
    b = lerp(t, u, v);

    d = lerp(sy, a, b);

    return lerp(sz, c, d);
}

static void normalize2(float v[2])
{
    float s;

    s = sqrt(v[0] * v[0] + v[1] * v[1]);
    v[0] = v[0] / s;
    v[1] = v[1] / s;
}

static void normalize3(float v[3])
{
    float s;

    s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] = v[0] / s;
    v[1] = v[1] / s;
    v[2] = v[2] / s;
}

static void init(void)
{
    int i, j, k;

    for (i = 0 ; i < B ; i++) {
	p[i] = i;

	g1[i] = (float)((qrand() % (B + B)) - B) / B;

	for (j = 0 ; j < 2 ; j++)
	g2[i][j] = (float)((qrand() % (B + B)) - B) / B;
	normalize2(g2[i]);

	for (j = 0 ; j < 3 ; j++)
	g3[i][j] = (float)((qrand() % (B + B)) - B) / B;
	normalize3(g3[i]);
    }

    while (--i) {
	k = p[i];
	p[i] = p[j = qrand() % B];
	p[j] = k;
    }

    for (i = 0 ; i < B + 2 ; i++) {
	p[B + i] = p[i];
	g1[B + i] = g1[i];
	for (j = 0 ; j < 2 ; j++)
	    g2[B + i][j] = g2[i][j];
	for (j = 0 ; j < 3 ; j++)
	    g3[B + i][j] = g3[i][j];
    }
}
