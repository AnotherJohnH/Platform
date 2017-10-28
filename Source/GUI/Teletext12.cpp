//------------------------------------------------------------------------------
// The font data in this file is a derivative of font data reverse engineered
// from bitmap images published in the data sheet for the Mullard SAA5050
// "Teletext Character Generator" (dated July 1982). That publicly available
// document does not contain a copyright notice or any mention of permissions
// or restrictions that apply to the use of the data. However, according to law
// that does not necessarily mean that the data is free from restrictions on its
// use. At the time of publication, the copyright holder for the font data may
// have been Mullard or may have been the designers of the original teletext
// system, the British Broadcasting Corporation. If the actual copyright holder
// becomes apparent and wishes to retrospectively declare reasonable and legal
// restrictions on the data, then either those restrictions must be obeyed or
// this data file shouled be deleted.
//
// The statements above shall be included in all copies or substantial portions
// of the data.
//
// THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE DATA OR THE USE OR OTHER DEALINGS IN THE
// DATA.
//------------------------------------------------------------------------------

// Based on the font published in the SAA5050 datasheet.

#include "GUI/Font/Teletext.h"

namespace GUI {

static const uint8_t font_teletext12_data[] =
{
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10,
   0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x48, 0x48, 0xFC, 0x48, 0x48, 0xFC, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00,
   0x7C, 0x92, 0x90, 0x90, 0x7C, 0x12, 0x12, 0x92, 0x7C, 0x00, 0x00, 0x00, 0xC0, 0xC4, 0x08, 0x10,
   0x20, 0x40, 0x8C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x60, 0x90, 0x90, 0x50, 0xA0, 0x90, 0x94, 0x88,
   0x74, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00,
   0x10, 0x28, 0x44, 0x82, 0x82, 0x82, 0x44, 0x28, 0x10, 0x00, 0x00, 0x00, 0x10, 0x30, 0x10, 0x10,
   0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x02, 0x04, 0x38, 0x40, 0x80,
   0xFE, 0x00, 0x00, 0x00, 0xFE, 0x02, 0x04, 0x08, 0x1C, 0x02, 0x02, 0x84, 0x78, 0x00, 0x00, 0x00,
   0x18, 0x28, 0x48, 0x48, 0x88, 0x88, 0xFE, 0x08, 0x08, 0x00, 0x00, 0x00, 0xFC, 0x80, 0x80, 0xF8,
   0x04, 0x04, 0x04, 0x84, 0x78, 0x00, 0x00, 0x00, 0x38, 0x40, 0x80, 0x80, 0xF8, 0x84, 0x84, 0x84,
   0x78, 0x00, 0x00, 0x00, 0xFC, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00,
   0x78, 0x84, 0x84, 0x84, 0x78, 0x84, 0x84, 0x84, 0x78, 0x00, 0x00, 0x00, 0x78, 0x84, 0x84, 0x84,
   0x7C, 0x04, 0x08, 0x10, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00,
   0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E,
   0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10,
   0x20, 0x00, 0x00, 0x00, 0x78, 0x84, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00,
   0x78, 0x84, 0x84, 0xBC, 0xA4, 0xA4, 0xBC, 0x80, 0x78, 0x00, 0x00, 0x00, 0x10, 0x28, 0x44, 0x82,
   0x82, 0xFE, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0xF8, 0x84, 0x84, 0x84, 0xF8, 0x84, 0x84, 0x84,
   0xF8, 0x00, 0x00, 0x00, 0x78, 0x84, 0x80, 0x80, 0x80, 0x80, 0x80, 0x84, 0x78, 0x00, 0x00, 0x00,
   0xF8, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0xF8, 0x00, 0x00, 0x00, 0xFC, 0x80, 0x80, 0x80,
   0xF0, 0x80, 0x80, 0x80, 0xFC, 0x00, 0x00, 0x00, 0xFC, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80,
   0x80, 0x00, 0x00, 0x00, 0x78, 0x84, 0x80, 0x80, 0x80, 0x9C, 0x84, 0x84, 0x7C, 0x00, 0x00, 0x00,
   0x84, 0x84, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x84, 0x00, 0x00, 0x00, 0x38, 0x10, 0x10, 0x10,
   0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x84,
   0x78, 0x00, 0x00, 0x00, 0x84, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88, 0x84, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x00, 0x00, 0x00, 0xC6, 0xAA, 0x92, 0x82,
   0x82, 0x82, 0x82, 0x82, 0x82, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x62, 0x52, 0x4A, 0x46, 0x42,
   0x42, 0x00, 0x00, 0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00, 0x00, 0x00,
   0xF8, 0x84, 0x84, 0x84, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x78, 0x84, 0x84, 0x84,
   0x84, 0xA4, 0x94, 0x88, 0x74, 0x00, 0x00, 0x00, 0xF8, 0x84, 0x84, 0x84, 0xF8, 0xA0, 0x90, 0x88,
   0x84, 0x00, 0x00, 0x00, 0x78, 0x84, 0x80, 0x80, 0x78, 0x04, 0x04, 0x84, 0x78, 0x00, 0x00, 0x00,
   0xFC, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x84, 0x84, 0x84, 0x84,
   0x84, 0x84, 0x84, 0x84, 0x78, 0x00, 0x00, 0x00, 0x84, 0x84, 0x84, 0x84, 0x48, 0x48, 0x30, 0x30,
   0x30, 0x00, 0x00, 0x00, 0x82, 0x82, 0x82, 0x82, 0x82, 0x92, 0x92, 0x92, 0x6C, 0x00, 0x00, 0x00,
   0x84, 0x84, 0x84, 0x48, 0x30, 0x48, 0x84, 0x84, 0x84, 0x00, 0x00, 0x00, 0x82, 0x82, 0x82, 0x44,
   0x38, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0xFC, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80,
   0xFC, 0x00, 0x00, 0x00, 0x70, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10,
   0x10, 0x10, 0x10, 0x10, 0x70, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x84, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00,
   0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
   0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44,
   0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x40, 0x40, 0x40, 0x40, 0x3C, 0x00, 0x00, 0x00,
   0x04, 0x04, 0x04, 0x3C, 0x44, 0x44, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
   0x44, 0x44, 0x7C, 0x40, 0x38, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 0x10, 0x38, 0x10, 0x10, 0x10,
   0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x38, 0x00,
   0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x30,
   0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10,
   0x10, 0x20, 0x00, 0x00, 0x40, 0x40, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, 0x00, 0x00,
   0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68,
   0x54, 0x54, 0x54, 0x54, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44,
   0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x78, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C,
   0x44, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x30, 0x20, 0x20, 0x20,
   0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x40, 0x40, 0x3C, 0x02, 0x7C, 0x00, 0x00, 0x00,
   0x00, 0x10, 0x10, 0x38, 0x10, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44,
   0x44, 0x44, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x10,
   0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x54, 0x54, 0x28, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44,
   0x44, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x08, 0x10, 0x20, 0x40,
   0xFC, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 0x10, 0x30, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00,
   0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x10,
   0x18, 0x10, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x92, 0x0C, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
   0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
   0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
   0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
   0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
   0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
   0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0,
   0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0,
   0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x70, 0x70, 0x70,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x07, 0x07, 0x07, 0x00, 0x77, 0x77, 0x77,
   0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77
};

const Font font_teletext12 = {{8, 12}, 0x20, 0xFF, 1, font_teletext12_data};

} // namespace GUI
