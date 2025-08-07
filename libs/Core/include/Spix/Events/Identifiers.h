/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

namespace spix {

struct KeyCodes {
    enum : int
    {
        Escape = 0x01000000,
        Tab = 0x01000001,
        Backtab = 0x01000002,
        Backspace = 0x01000003,
        Return = 0x01000004,
        Enter = 0x01000005,
        Insert = 0x01000006,
        Delete = 0x01000007,
        Pause = 0x01000008,
        Print = 0x01000009,
        SysReq = 0x0100000a,
        Clear = 0x0100000b,
        Home = 0x01000010,
        End = 0x01000011,
        Left = 0x01000012,
        Up = 0x01000013,
        Right = 0x01000014,
        Down = 0x01000015,
        PageUp = 0x01000016,
        PageDown = 0x01000017,
        Shift = 0x01000020,
        Control = 0x01000021,
        Meta = 0x01000022,
        Alt = 0x01000023,
        CapsLock = 0x01000024,
        NumLock = 0x01000025,
        ScrollLock = 0x01000026,
        F1 = 0x01000030,
        F2 = 0x01000031,
        F3 = 0x01000032,
        F4 = 0x01000033,
        F5 = 0x01000034,
        F6 = 0x01000035,
        F7 = 0x01000036,
        F8 = 0x01000037,
        F9 = 0x01000038,
        F10 = 0x01000039,
        F11 = 0x0100003a,
        F12 = 0x0100003b,
        F13 = 0x0100003c,
        F14 = 0x0100003d,
        F15 = 0x0100003e,
        F16 = 0x0100003f,
        F17 = 0x01000040,
        F18 = 0x01000041,
        F19 = 0x01000042,
        F20 = 0x01000043,
        F21 = 0x01000044,
        F22 = 0x01000045,
        F23 = 0x01000046,
        F24 = 0x01000047,
        F25 = 0x01000048,
        F26 = 0x01000049,
        F27 = 0x0100004a,
        F28 = 0x0100004b,
        F29 = 0x0100004c,
        F30 = 0x0100004d,
        F31 = 0x0100004e,
        F32 = 0x0100004f,
        F33 = 0x01000050,
        F34 = 0x01000051,
        F35 = 0x01000052,
        Super_L = 0x01000053,
        Super_R = 0x01000054,
        Menu = 0x01000055,
        Hyper_L = 0x01000056,
        Hyper_R = 0x01000057,
        Help = 0x01000058,
        Direction_L = 0x01000059,
        Direction_R = 0x01000060,
        Space = 0x20,
        Char_Exclam = 0x21,
        Char_QuoteDbl = 0x22,
        Char_NumberSign = 0x23,
        Char_Dollar = 0x24,
        Char_Percent = 0x25,
        Char_Ampersand = 0x26,
        Char_Apostrophe = 0x27,
        Char_ParenLeft = 0x28,
        Char_ParenRight = 0x29,
        Char_Asterisk = 0x2a,
        Char_Plus = 0x2b,
        Char_Comma = 0x2c,
        Char_Minus = 0x2d,
        Char_Period = 0x2e,
        Char_Slash = 0x2f,
        Num_0 = 0x30,
        Num_1 = 0x31,
        Num_2 = 0x32,
        Num_3 = 0x33,
        Num_4 = 0x34,
        Num_5 = 0x35,
        Num_6 = 0x36,
        Num_7 = 0x37,
        Num_8 = 0x38,
        Num_9 = 0x39,
        Char_Colon = 0x3a,
        Char_Semicolon = 0x3b,
        Char_Less = 0x3c,
        Char_Equal = 0x3d,
        Char_Greater = 0x3e,
        Char_Question = 0x3f,
        Char_At = 0x40,
        Char_A = 0x41,
        Char_B = 0x42,
        Char_C = 0x43,
        Char_D = 0x44,
        Char_E = 0x45,
        Char_F = 0x46,
        Char_G = 0x47,
        Char_H = 0x48,
        Char_I = 0x49,
        Char_J = 0x4a,
        Char_K = 0x4b,
        Char_L = 0x4c,
        Char_M = 0x4d,
        Char_N = 0x4e,
        Char_O = 0x4f,
        Char_P = 0x50,
        Char_Q = 0x51,
        Char_R = 0x52,
        Char_S = 0x53,
        Char_T = 0x54,
        Char_U = 0x55,
        Char_V = 0x56,
        Char_W = 0x57,
        Char_X = 0x58,
        Char_Y = 0x59,
        Char_Z = 0x5a
    };
};

using KeyModifier = unsigned;
struct KeyModifiers {
    enum : KeyModifier
    {
        None = 0,
        Shift = 1 << 0,
        Control = 1 << 1,
        Alt = 1 << 2,
        Meta = 1 << 3
    };
};

using MouseButton = unsigned;
struct MouseButtons {
    enum : MouseButton
    {
        None = 0,
        Left = 1 << 0,
        Right = 1 << 1,
        Middle = 1 << 2
    };
};

} // namespace spix
