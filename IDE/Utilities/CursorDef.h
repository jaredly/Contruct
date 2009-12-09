/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2005 by Joerg Koenig
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    J.Koenig@adg.de                 (company site)
//    Joerg.Koenig@rhein-neckar.de    (private site)
/////////////////////////////////////////////////////////////////////////////


#ifndef _CursorDef_h_
#define _CursorDef_h_

// For visual effects of the bit-arrays, we define some helpers:

#define ________	0x00
#define _______X	0x01
#define ______X_	0x02
#define ______XX	0x03
#define _____X__	0x04
#define _____X_X	0x05
#define _____XX_	0x06
#define _____XXX	0x07
#define ____X___	0x08
#define ____X__X	0x09
#define ____X_X_	0x0A
#define ____X_XX	0x0B
#define ____XX__	0x0C
#define ____XX_X	0x0D
#define ____XXX_	0x0E
#define ____XXXX	0x0F
#define ___X____	0x10
#define ___X__XX	0x13
#define ___XX___	0x18
#define ___XXXXX	0x1F
#define __X_____	0x20
#define __X___X_	0x22
#define __X___XX	0x23
#define __X__XXX	0x27
#define __XX____	0x30
#define __XX_X_X	0x35
#define __XX_XXX	0x37
#define __XXX___	0x38
#define __XXXX__	0x3C
#define __XXXX_X	0x3D
#define __XXXXXX	0x3F
#define _X_X_X__	0x54
#define _X_X_X_X	0x55
#define _XX_____	0x60
#define _XX__X_X	0x65
#define _XXX____	0x70
#define _XXXXXXX	0x7F
#define X_______	0x80
#define X____XXX	0x87
#define X__X__XX	0x93
#define X_X_____	0xA0
#define X_X_X__X	0xA9
#define X_X_X_XX	0xAB
#define X_X___XX	0xA3
#define X_X__XXX	0xA7
#define X_X_X_X_	0xAA
#define X_X_XX__	0xAC
#define X_X_X_XX	0xAB
#define X_XX__XX	0xB3
#define X_XXX_XX	0xBB
#define X_XXXX__	0xBC
#define X_XXXX_X	0xBD
#define X_XXXXXX	0xBF
#define XX______	0xC0
#define XX_____X	0xC1
#define XX___X_X	0xC5
#define XX___XXX	0xC7
#define XX__XX_X	0xCD
#define XX_X__XX	0xD3
#define XX_X_X_X	0xD5
#define XX_XXX_X	0xDD
#define XXX_____	0xE0
#define XXX__X_X	0xE5
#define XXX__XXX	0xE7
#define XXXX____	0xF0
#define XXXX___X	0xF1
#define XXXX__X_	0xF2
#define XXXX__XX	0xF3
#define XXXX_X__	0xF4
#define XXXX_X_X	0xF5
#define XXXX_XX_	0xF6
#define XXXX_XXX	0xF7
#define XXXXX___	0xF8
#define XXXXX__X	0xF9
#define XXXXX_X_	0xFA
#define XXXXX_XX	0xFB
#define XXXXXX__	0xFC
#define XXXXXX_X	0xFD
#define XXXXXXX_	0xFE
#define XXXXXXXX	0xFF

/*	Keep in mind:
	XOR masks: The 'X' describes white pixels on the screen
	AND masks: The 'X' describes transparent pixels
	Pixels that are '_' in both masks will be shown as black.
*/

// Hotspot for the cursor
static const int HSpotXNoDrop = 4;	// column in mask
static const int HSpotYNoDrop = 2;	// line in mask

static const BYTE XORmaskNoDrop[] = {
    ________,________,________,________,   // line 0
    ________,________,________,________,   // line 1
    ________,________,________,________,   // line 2
    _____X__,________,________,________,   // line 3
    _____XX_,________,________,________,   // line 4
    _____XXX,________,________,________,   // line 5
    _____XXX,X_______,________,________,   // line 6
    _____XXX,XX______,________,________,   // line 7
    _____XXX,XXX_____,________,________,   // line 8
    _____XXX,XXXX____,________,________,   // line 9
    _____XXX,XXXXX___,________,________,   // line 10
    _____XXX,XXXXXX__,________,________,   // line 11
    _____XXX,XXX_____,________,________,   // line 12
    _____XXX,_XX_____,________,________,   // line 13
    _____XX_,_XX_____,________,________,   // line 14
    _____X__,__XX____,________,________,   // line 15
    ________,__XX____,________,________,   // line 16
    ________,___XX___,________,________,   // line 17
    ________,XX_XXX_X,XXXXXXXX,________,   // line 18
    ________,X_X_XX__,X_X_X_X_,________,   // line 19
    ________,XX__XX_X,_X_X_X_X,________,   // line 20
    ________,X_X_____,X_______,________,   // line 21
    ________,XX_X_X_X,__XXXXXX,XXX_____,   // line 22
    ________,X_X_X_X_,X_X__XXX,__X_____,   // line 23
    ________,________,__X___X_,__X_____,   // line 24
    ________,________,__XX____,_XX_____,   // line 25
    ________,________,__XXX___,XXX_____,   // line 26
    ________,________,__XX____,_XX_____,   // line 27
    ________,________,__X___X_,__X_____,   // line 28
    ________,________,__X__XXX,__X_____,   // line 29
    ________,________,__XXXXXX,XXX_____,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskNoDrop[] = { 
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 0
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 1
    XXXX__XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 2
    XXXX___X,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 3
    XXXX____,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 4
    XXXX____,_XXXXXXX,XXXXXXXX,XXXXXXXX,   // line 5
    XXXX____,__XXXXXX,XXXXXXXX,XXXXXXXX,   // line 6
    XXXX____,___XXXXX,XXXXXXXX,XXXXXXXX,   // line 7
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 8
    XXXX____,_____XXX,XXXXXXXX,XXXXXXXX,   // line 9
    XXXX____,______XX,XXXXXXXX,XXXXXXXX,   // line 10
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 11
    XXXX____,________,XXXXXXXX,XXXXXXXX,   // line 12
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 13
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 14
    XXXX___X,_____XXX,XXXXXXXX,XXXXXXXX,   // line 15
    XXXX__XX,_____XXX,XXXXXXXX,XXXXXXXX,   // line 16
    XXXX_XX_,________,________,__XXXXXX,   // line 17
    XXXXXXX_,________,________,__XXXXXX,   // line 18
    XXXXXXX_,________,________,__XXXXXX,   // line 19
    XXXXXXX_,________,________,__XXXXXX,   // line 20
    XXXXXXX_,________,________,____XXXX,   // line 21
    XXXXXXX_,________,________,____XXXX,   // line 22
    XXXXXXX_,________,________,____XXXX,   // line 23
    XXXXXXX_,________,________,____XXXX,   // line 24
    XXXXXXX_,________,________,____XXXX,   // line 25
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 26
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 27
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 28
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 29
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 30
    XXXXXXXX,XXXXXXXX,X_______,____XXXX    // line 31
};

// Hotspot for the cursor
static const int HSpotXMoveAbove = 4;	// column in mask
static const int HSpotYMoveAbove = 2;	// line in mask

static const BYTE XORmaskMoveAbove[] = {
    ________,________,________,________,   // line 0
    ________,________,________,________,   // line 1
    ________,________,________,________,   // line 2
    _____X__,________,________,________,   // line 3
    _____XX_,________,________,________,   // line 4
    _____XXX,________,________,________,   // line 5
    _____XXX,X_______,________,________,   // line 6
    _____XXX,XX______,________,________,   // line 7
    _____XXX,XXX_____,________,________,   // line 8
    _____XXX,XXXX____,________,________,   // line 9
    _____XXX,XXXXX___,________,________,   // line 10
    _____XXX,XXXXXX__,________,________,   // line 11
    _____XXX,XXX_____,________,________,   // line 12
    _____XXX,_XX_____,________,________,   // line 13
    _____XX_,_XX_____,________,________,   // line 14
    _____X__,__XX____,________,________,   // line 15
    ________,__XX____,________,________,   // line 16
    ________,___XX___,________,________,   // line 17
    ________,XX_XXX_X,XXXXXXXX,________,   // line 18
    ________,X_X_XX__,X_X_X_X_,________,   // line 19
    ________,XX__XX_X,_X_X_X_X,________,   // line 20
    ________,X_X_____,_____X_X,________,   // line 21
    ________,XX_X_X_X,X_X_X_XX,________,   // line 22
    ________,X_X_X_X_,X_X__XXX,________,   // line 23
    ________,________,________,________,   // line 24
    ________,________,________,________,   // line 25
    ________,________,________,________,   // line 26
    ________,________,________,________,   // line 27
    ________,________,________,________,   // line 28
    ________,________,________,________,   // line 29
    ________,________,________,________,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskMoveAbove[] = { 
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 0
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 1
    XXXX__XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 2
    XXXX___X,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 3
    XXXX____,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 4
    XXXX____,_XXXXXXX,XXXXXXXX,XXXXXXXX,   // line 5
    XXXX____,__XXXXXX,XXXXXXXX,XXXXXXXX,   // line 6
    XXXX____,___XXXXX,XXXXXXXX,XXXXXXXX,   // line 7
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 8
    XXXX____,_____XXX,XXXXXXXX,XXXXXXXX,   // line 9
    XXXX____,______XX,XXXXXXXX,XXXXXXXX,   // line 10
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 11
    XXXX____,________,XXXXXXXX,XXXXXXXX,   // line 12
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 13
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 14
    XXXX___X,_____XXX,XXXXXXXX,XXXXXXXX,   // line 15
    XXXX__XX,_____XXX,XXXXXXXX,XXXXXXXX,   // line 16
    XXXX_XX_,________,________,__XXXXXX,   // line 17
    XXXXXXX_,________,________,__XXXXXX,   // line 18
    XXXXXXX_,________,________,__XXXXXX,   // line 19
    XXXXXXX_,________,________,__XXXXXX,   // line 20
    XXXXXXX_,________,________,__XXXXXX,   // line 21
    XXXXXXX_,________,________,__XXXXXX,   // line 22
    XXXXXXX_,________,________,__XXXXXX,   // line 23
    XXXXXXX_,________,________,__XXXXXX,   // line 24
    XXXXXXX_,________,________,__XXXXXX,   // line 25
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 26
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 27
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 28
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 29
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 30
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX    // line 31
};

static const int HSpotXCopyAbove = 4;	// column in mask
static const int HSpotYCopyAbove = 2;	// line in mask

static const BYTE XORmaskCopyAbove[] = {
    ________,________,________,________,   // line 0
    ________,________,________,________,   // line 1
    ________,________,________,________,   // line 2
    _____X__,________,________,________,   // line 3
    _____XX_,________,________,________,   // line 4
    _____XXX,________,________,________,   // line 5
    _____XXX,X_______,________,________,   // line 6
    _____XXX,XX______,________,________,   // line 7
    _____XXX,XXX_____,________,________,   // line 8
    _____XXX,XXXX____,________,________,   // line 9
    _____XXX,XXXXX___,________,________,   // line 10
    _____XXX,XXXXXX__,________,________,   // line 11
    _____XXX,XXX_____,________,________,   // line 12
    _____XXX,_XX_____,________,________,   // line 13
    _____XX_,_XX_____,________,________,   // line 14
    _____X__,__XX____,________,________,   // line 15
    ________,__XX____,________,________,   // line 16
    ________,___XX___,________,________,   // line 17
    ________,XX_XXX_X,XXXXXXXX,________,   // line 18
    ________,X_X_XX__,X_X_X_X_,________,   // line 19
    ________,XX__XX_X,_X_X_X_X,________,   // line 20
    ________,X_X_____,X_______,________,   // line 21
    ________,XX_X_X_X,__XXXXXX,XXX_____,   // line 22
    ________,X_X_X_X_,X_XXXX_X,XXX_____,   // line 23
    ________,________,__XXXX_X,XXX_____,   // line 24
    ________,________,__XXXX_X,XXX_____,   // line 25
    ________,________,__X_____,__X_____,   // line 26
    ________,________,__XXXX_X,XXX_____,   // line 27
    ________,________,__XXXX_X,XXX_____,   // line 28
    ________,________,__XXXX_X,XXX_____,   // line 29
    ________,________,__XXXXXX,XXX_____,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskCopyAbove[] = { 
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 0
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 1
    XXXX__XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 2
    XXXX___X,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 3
    XXXX____,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 4
    XXXX____,_XXXXXXX,XXXXXXXX,XXXXXXXX,   // line 5
    XXXX____,__XXXXXX,XXXXXXXX,XXXXXXXX,   // line 6
    XXXX____,___XXXXX,XXXXXXXX,XXXXXXXX,   // line 7
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 8
    XXXX____,_____XXX,XXXXXXXX,XXXXXXXX,   // line 9
    XXXX____,______XX,XXXXXXXX,XXXXXXXX,   // line 10
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 11
    XXXX____,________,XXXXXXXX,XXXXXXXX,   // line 12
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 13
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 14
    XXXX___X,_____XXX,XXXXXXXX,XXXXXXXX,   // line 15
    XXXX__XX,_____XXX,XXXXXXXX,XXXXXXXX,   // line 16
    XXXX_XX_,________,________,__XXXXXX,   // line 17
    XXXXXXX_,________,________,__XXXXXX,   // line 18
    XXXXXXX_,________,________,__XXXXXX,   // line 19
    XXXXXXX_,________,________,__XXXXXX,   // line 20
    XXXXXXX_,________,________,____XXXX,   // line 21
    XXXXXXX_,________,________,____XXXX,   // line 22
    XXXXXXX_,________,________,____XXXX,   // line 23
    XXXXXXX_,________,________,____XXXX,   // line 24
    XXXXXXX_,________,________,____XXXX,   // line 25
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 26
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 27
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 28
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 29
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 30
    XXXXXXXX,XXXXXXXX,X_______,____XXXX    // line 31
};

// Hotspot for the cursor
static const int HSpotXMoveBelow = 4;	// column in mask
static const int HSpotYMoveBelow = 29;	// line in mask

static const BYTE XORmaskMoveBelow[] = {
    ________,________,________,________,   // line 0
    ________,________,________,________,   // line 1
    ________,________,________,________,   // line 2
    ________,________,________,________,   // line 3
    ________,________,________,________,   // line 4
    ________,________,________,________,   // line 5
    ________,________,________,________,   // line 6
    ________,________,________,________,   // line 7
    ________,X_X_X_X_,X_X__XXX,________,   // line 8
    ________,XX_X_X_X,X_X_X_XX,________,   // line 9
    ________,X_X_____,_____X_X,________,   // line 10
    ________,XX__XX_X,_X_X_X_X,________,   // line 11
    ________,X_X_XX__,X_X_X_X_,________,   // line 12
    ________,XX_XXX_X,XXXXXXXX,________,   // line 13
    ________,___XX___,________,________,   // line 14
    ________,__XX____,________,________,   // line 15
    _____X__,__XX____,________,________,   // line 16
    _____XX_,_XX_____,________,________,   // line 17
    _____XXX,_XX_____,________,________,   // line 18
    _____XXX,XXX_____,________,________,   // line 19
    _____XXX,XXXXXX__,________,________,   // line 20
    _____XXX,XXXXX___,________,________,   // line 21
    _____XXX,XXXX____,________,________,   // line 22
    _____XXX,XXX_____,________,________,   // line 23
    _____XXX,XX______,________,________,   // line 24
    _____XXX,X_______,________,________,   // line 25
    _____XXX,________,________,________,   // line 26
    _____XX_,________,________,________,   // line 27
    _____X__,________,________,________,   // line 28
    ________,________,________,________,   // line 29
    ________,________,________,________,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskMoveBelow[] = { 
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 0
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 1
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 2
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 3
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 4
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 5
    XXXXXXX_,________,________,__XXXXXX,   // line 6
    XXXXXXX_,________,________,__XXXXXX,   // line 7
    XXXXXXX_,________,________,__XXXXXX,   // line 8
    XXXXXXX_,________,________,__XXXXXX,   // line 9
    XXXXXXX_,________,________,__XXXXXX,   // line 10
    XXXXXXX_,________,________,__XXXXXX,   // line 11
    XXXXXXX_,________,________,__XXXXXX,   // line 12
    XXXXXXX_,________,________,__XXXXXX,   // line 13
    XXXX_XX_,________,________,__XXXXXX,   // line 14
    XXXX__XX,_____XXX,XXXXXXXX,XXXXXXXX,   // line 15
    XXXX___X,_____XXX,XXXXXXXX,XXXXXXXX,   // line 16
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 17
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 18
    XXXX____,________,XXXXXXXX,XXXXXXXX,   // line 19
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 20
    XXXX____,______XX,XXXXXXXX,XXXXXXXX,   // line 21
    XXXX____,_____XXX,XXXXXXXX,XXXXXXXX,   // line 22
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 23
    XXXX____,___XXXXX,XXXXXXXX,XXXXXXXX,   // line 24
    XXXX____,__XXXXXX,XXXXXXXX,XXXXXXXX,   // line 25
    XXXX____,_XXXXXXX,XXXXXXXX,XXXXXXXX,   // line 26
    XXXX____,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 27
    XXXX___X,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 28
    XXXX__XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 29
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 30
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX    // line 31
};

static const int HSpotXCopyBelow = 4;	// column in mask
static const int HSpotYCopyBelow = 29;	// line in mask

static const BYTE XORmaskCopyBelow[] = {
    ________,________,________,________,   // line 0
    ________,________,__XXXXXX,XXX_____,   // line 1
    ________,________,__XXXX_X,XXX_____,   // line 2
    ________,________,__XXXX_X,XXX_____,   // line 3
    ________,________,__XXXX_X,XXX_____,   // line 4
    ________,________,__X_____,__X_____,   // line 5
    ________,________,__XXXX_X,XXX_____,   // line 6
    ________,________,__XXXX_X,XXX_____,   // line 7
    ________,X_X_X_X_,X_XXXX_X,XXX_____,   // line 8
    ________,XX_X_X_X,__XXXXXX,XXX_____,   // line 9
    ________,X_X_____,X_______,________,   // line 10
    ________,XX__XX_X,_X_X_X_X,________,   // line 11
    ________,X_X_XX__,X_X_X_X_,________,   // line 12
    ________,XX_XXX_X,XXXXXXXX,________,   // line 13
    ________,___XX___,________,________,   // line 14
    ________,__XX____,________,________,   // line 15
    _____X__,__XX____,________,________,   // line 16
    _____XX_,_XX_____,________,________,   // line 17
    _____XXX,_XX_____,________,________,   // line 18
    _____XXX,XXX_____,________,________,   // line 19
    _____XXX,XXXXXX__,________,________,   // line 20
    _____XXX,XXXXX___,________,________,   // line 21
    _____XXX,XXXX____,________,________,   // line 22
    _____XXX,XXX_____,________,________,   // line 23
    _____XXX,XX______,________,________,   // line 24
    _____XXX,X_______,________,________,   // line 25
    _____XXX,________,________,________,   // line 26
    _____XX_,________,________,________,   // line 27
    _____X__,________,________,________,   // line 28
    ________,________,________,________,   // line 29
    ________,________,________,________,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskCopyBelow[] = { 
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,    // line 0
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 1
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 2
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 3
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 4
    XXXXXXXX,XXXXXXXX,X_______,____XXXX,   // line 5
    XXXXXXX_,________,________,____XXXX,   // line 6
    XXXXXXX_,________,________,____XXXX,   // line 7
    XXXXXXX_,________,________,____XXXX,   // line 8
    XXXXXXX_,________,________,____XXXX,   // line 9
    XXXXXXX_,________,________,____XXXX,   // line 10
    XXXXXXX_,________,________,__XXXXXX,   // line 11
    XXXXXXX_,________,________,__XXXXXX,   // line 12
    XXXXXXX_,________,________,__XXXXXX,   // line 13
    XXXX_XX_,________,________,__XXXXXX,   // line 14
    XXXX__XX,_____XXX,XXXXXXXX,XXXXXXXX,   // line 15
    XXXX___X,_____XXX,XXXXXXXX,XXXXXXXX,   // line 16
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 17
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 18
    XXXX____,________,XXXXXXXX,XXXXXXXX,   // line 19
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 20
    XXXX____,______XX,XXXXXXXX,XXXXXXXX,   // line 21
    XXXX____,_____XXX,XXXXXXXX,XXXXXXXX,   // line 22
    XXXX____,____XXXX,XXXXXXXX,XXXXXXXX,   // line 23
    XXXX____,___XXXXX,XXXXXXXX,XXXXXXXX,   // line 24
    XXXX____,__XXXXXX,XXXXXXXX,XXXXXXXX,   // line 25
    XXXX____,_XXXXXXX,XXXXXXXX,XXXXXXXX,   // line 26
    XXXX____,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 27
    XXXX___X,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 28
    XXXX__XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 29
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 30
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX    // line 31
};

// Hotspot for the cursor
static const int HSpotXMoveChild = 27;	// column in mask
static const int HSpotYMoveChild = 29;	// line in mask

static const BYTE XORmaskMoveChild[] = {
    ________,________,________,________,   // line 0
    ________,________,________,________,   // line 1
    ________,________,________,________,   // line 2
    ________,________,________,________,   // line 3
    ________,________,________,________,   // line 4
    ________,________,________,________,   // line 5
    ________,________,________,________,   // line 6
    ________,________,________,________,   // line 7
    ________,XXXXXXXX,XXXXXXXX,________,   // line 8
    ________,XX_X_X_X,X_X_X_XX,________,   // line 9
    ________,X_X_____,_____X_X,________,   // line 10
    ________,X_X_X_X_,___X__XX,________,   // line 11
    ________,_X_X_X__,XX___X_X,________,   // line 12
    ________,X_X_X_X_,_XX__X_X,________,   // line 13
    ________,________,__XX____,________,   // line 14
    ________,________,___XX___,________,   // line 15
    ________,________,____XX__,___X____,   // line 16
    ________,________,_____XX_,__XX____,   // line 17
    ________,________,______XX,_XXX____,   // line 18
    ________,________,_______X,XXXX____,   // line 19
    ________,________,___XXXXX,XXXX____,   // line 20
    ________,________,____XXXX,XXXX____,   // line 21
    ________,________,_____XXX,XXXX____,   // line 22
    ________,________,______XX,XXXX____,   // line 23
    ________,________,_______X,XXXX____,   // line 24
    ________,________,________,XXXX____,   // line 25
    ________,________,________,_XXX____,   // line 26
    ________,________,________,__XX____,   // line 27
    ________,________,________,___X____,   // line 28
    ________,________,________,________,   // line 29
    ________,________,________,________,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskMoveChild[] = { 
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 0
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 1
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 2
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 3
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 4
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 5
    XXXXXXX_,________,________,__XXXXXX,   // line 6
    XXXXXXX_,________,________,__XXXXXX,   // line 7
    XXXXXXX_,________,________,__XXXXXX,   // line 8
    XXXXXXX_,________,________,__XXXXXX,   // line 9
    XXXXXXX_,________,________,__XXXXXX,   // line 10
    XXXXXXX_,________,________,__XXXXXX,   // line 11
    XXXXXXX_,________,________,__XXXXXX,   // line 12
    XXXXXXX_,________,________,__XXXXXX,   // line 13
    XXXXXXX_,________,________,__XX_XXX,   // line 14
    XXXXXXXX,XXXXXXXX,XX_____X,XXX__XXX,   // line 15
    XXXXXXXX,XXXXXXXX,XXX_____,XX___XXX,   // line 16
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 17
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 18
    XXXXXXXX,XXXXXXXX,X_______,_____XXX,   // line 19
    XXXXXXXX,XXXXXXXX,XX______,_____XXX,   // line 20
    XXXXXXXX,XXXXXXXX,XXX_____,_____XXX,   // line 21
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 22
    XXXXXXXX,XXXXXXXX,XXXXX___,_____XXX,   // line 23
    XXXXXXXX,XXXXXXXX,XXXXXX__,_____XXX,   // line 24
    XXXXXXXX,XXXXXXXX,XXXXXXX_,_____XXX,   // line 25
    XXXXXXXX,XXXXXXXX,XXXXXXXX,_____XXX,   // line 26
    XXXXXXXX,XXXXXXXX,XXXXXXXX,X____XXX,   // line 27
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XX___XXX,   // line 28
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX__XXX,   // line 29
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 30
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX    // line 31
};

// Hotspot for the cursor
static const int HSpotXCopyChild = 27;	// column in mask
static const int HSpotYCopyChild = 29;	// line in mask

static const BYTE XORmaskCopyChild[] = {
    ________,________,________,________,   // line 0
    _____XXX,XXXXXX__,________,________,   // line 1
    _____XXX,X_XXXX__,________,________,   // line 2
    _____XXX,X_XXXX__,________,________,   // line 3
    _____XXX,X_XXXX__,________,________,   // line 4
    _____X__,_____X__,________,________,   // line 5
    _____XXX,X_XXXX__,________,________,   // line 6
    _____XXX,X_XXXX__,________,________,   // line 7
    _____XXX,X_XXXXXX,XXXXXXXX,________,   // line 8
    _____XXX,XXXXXX__,X_X_X_XX,________,   // line 9
    ________,X_X_____,_____X_X,________,   // line 10
    ________,X_X_X_X_,___X__XX,________,   // line 11
    ________,_X_X_X__,XX___X_X,________,   // line 12
    ________,X_X_X_X_,_XX__X_X,________,   // line 13
    ________,________,__XX____,________,   // line 14
    ________,________,___XX___,________,   // line 15
    ________,________,____XX__,___X____,   // line 16
    ________,________,_____XX_,__XX____,   // line 17
    ________,________,______XX,_XXX____,   // line 18
    ________,________,_______X,XXXX____,   // line 19
    ________,________,___XXXXX,XXXX____,   // line 20
    ________,________,____XXXX,XXXX____,   // line 21
    ________,________,_____XXX,XXXX____,   // line 22
    ________,________,______XX,XXXX____,   // line 23
    ________,________,_______X,XXXX____,   // line 24
    ________,________,________,XXXX____,   // line 25
    ________,________,________,_XXX____,   // line 26
    ________,________,________,__XX____,   // line 27
    ________,________,________,___X____,   // line 28
    ________,________,________,________,   // line 29
    ________,________,________,________,   // line 30
    ________,________,________,________    // line 31
};

static const BYTE ANDmaskCopyChild[] = { 
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 0
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 1
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 2
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 3
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 4
    XXXX____,_______X,XXXXXXXX,XXXXXXXX,   // line 5
    XXXX____,________,________,__XXXXXX,   // line 6
    XXXX____,________,________,__XXXXXX,   // line 7
    XXXX____,________,________,__XXXXXX,   // line 8
    XXXX____,________,________,__XXXXXX,   // line 9
    XXXX____,________,________,__XXXXXX,   // line 10
    XXXXXXX_,________,________,__XXXXXX,   // line 11
    XXXXXXX_,________,________,__XXXXXX,   // line 12
    XXXXXXX_,________,________,__XXXXXX,   // line 13
    XXXXXXX_,________,________,__XX_XXX,   // line 14
    XXXXXXXX,XXXXXXXX,XX_____X,XXX__XXX,   // line 15
    XXXXXXXX,XXXXXXXX,XXX_____,XX___XXX,   // line 16
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 17
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 18
    XXXXXXXX,XXXXXXXX,X_______,_____XXX,   // line 19
    XXXXXXXX,XXXXXXXX,XX______,_____XXX,   // line 20
    XXXXXXXX,XXXXXXXX,XXX_____,_____XXX,   // line 21
    XXXXXXXX,XXXXXXXX,XXXX____,_____XXX,   // line 22
    XXXXXXXX,XXXXXXXX,XXXXX___,_____XXX,   // line 23
    XXXXXXXX,XXXXXXXX,XXXXXX__,_____XXX,   // line 24
    XXXXXXXX,XXXXXXXX,XXXXXXX_,_____XXX,   // line 25
    XXXXXXXX,XXXXXXXX,XXXXXXXX,_____XXX,   // line 26
    XXXXXXXX,XXXXXXXX,XXXXXXXX,X____XXX,   // line 27
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XX___XXX,   // line 28
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX__XXX,   // line 29
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,   // line 30
    XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX    // line 31
};

#endif	// _CursorDef_h_
