//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------


#include "MTL/PS2KeyDecode.h"


const uint8_t MTL::PS2KeyDecode::scan_table[256] =
{
//   0 8   1 9   2 A   3 B   4 C   5 D   6 E   7 F
    0x00, 0x89, 0x00, 0x85, 0x83, 0x81, 0x82, 0x8C,  // 00
    0x00, 0x8A, 0x88, 0x86, 0x84, '\t', '`' , 0x00,  // 08
    0x00, 0x9F, 0x94, 0x00, 0x96, 'q' , '1' , 0x00,  // 10
    0x00, 0x00, 'z' , 's' , 'a' , 'w' , '2' , 0x00,  // 18
    0x00, 'c' , 'x' , 'd' , 'e' , '4' , '3' , 0x00,  // 20
    0x00, ' ' , 'v' , 'f' , 't' , 'r' , '5' , 0x00,  // 28
    0x00, 'n' , 'b' , 'h' , 'g' , 'y' , '6' , 0x00,  // 30
    0x00, 0x00, 'm' , 'j' , 'u' , '7' , '8' , 0x00,  // 38
    0x00, ',' , 'k' , 'i' , 'o' , '0' , '9' , 0x00,  // 40
    0x00, '.' , '/' , 'l' , ';' , 'p' , '-' , 0x00,  // 48
    0x00, 0x00, '\'', 0x00, '[' , '=' , 0x00, 0x00,  // 50
    0x8E, 0x95, '\n', ']' , 0x00, '#' , 0x00, 0x00,  // 58
    0x00, '\\', 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,  // 60
    0x00, 'j' , 0x00, 0x92, '7' , 0x00, 0x00, 0x00,  // 68
    'm' , '.' , 0x91, 0x92, 0x93, 0x90, 0x1B, 0x8F,  // 70
    0x8B, ';' , 'l' , 'p' , '0' , '9' , 0x9E, 0x00,  // 78

    0x00, 0x89, 0x00, 0x85, 0x83, 0x81, 0x82, 0x8C,  // 00
    0x00, 0x8A, 0x88, 0x86, 0x84, '\t', '`' , 0x00,  // 08
    0x00, 0x9F, 0x94, 0x00, 0x96, 'Q' , '!' , 0x00,  // 10
    0x00, 0x00, 'Z' , 'S' , 'A' , 'W' , '"' , 0x00,  // 18
    0x00, 'C' , 'X' , 'D' , 'E' , '$' , 0x80, 0x00,  // 20
    0x00, ' ' , 'V' , 'F' , 'T' , 'R' , '%' , 0x00,  // 28
    0x00, 'N' , 'B' , 'H' , 'G' , 'Y' , '^' , 0x00,  // 30
    0x00, 0x00, 'M' , 'J' , 'U' , '&' , '*' , 0x00,  // 38
    0x00, '<' , 'K' , 'I' , 'O' , ')' , '(' , 0x00,  // 40
    0x00, '>' , '?' , 'L' , ':' , 'P' , '_' , 0x00,  // 48
    0x00, 0x00, '@', 0x00, '{'  , '+' , 0x00, 0x00,  // 50
    0x8E, 0x95, '\n', '}' , 0x00, '~' , 0x00, 0x00,  // 58
    0x00, '|' , 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,  // 60
    0x00, 'J' , 0x00, 'U' , '&' , 0x00, 0x00, 0x00,  // 68
    'M' , '>' , 'K' , 'I' , 'O' , '*' , 0x00, 0x8F,  // 70
    0x8B, ':' , 'L' , 'P' , ')' , '(' , 0x9E, 0x00   // 78
};
