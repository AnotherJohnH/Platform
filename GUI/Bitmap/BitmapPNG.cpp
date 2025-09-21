//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// See RFC-2083

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <string>
#include <vector>

#include "STB/Zlib.h"
#include "STB/Colour.h"
#include "STB/Endian.h"

#include "GUI/Bitmap.h"


class PNG
{
public:
   PNG() = default;

   ~PNG() = default;

   bool load(PLT::Bitmap* bitmap, const std::string& filename)
   {
      FILE* fp = fopen(filename.c_str(), "r");
      if (fp == nullptr)
      {
         error("Failed to open file");
         return false;
      }

      char header[8];
      if ((fread(header, sizeof(header), 1, fp) != 1) ||
          (::strncmp(header, "\x89PNG\r\n\x1A\n", sizeof(header)) != 0))
      {
         error("Signature error");
         fclose(fp);
         return false;
      }

      Chunk chunk{fp};

      bool ok = true;

      while(ok)
      {
         if (!chunk.readHeader()) break;

              if (chunk.isType("IHDR")) { ok = processChunkIHDR(chunk); }
         else if (chunk.isType("PLTE")) { ok = processChunkPLTE(chunk); }
         else if (chunk.isType("IDAT")) { ok = processChunkIDAT(chunk); }
         else if (chunk.isType("IEND")) { break; }
         else                           { ok = chunk.skip(); }
      }

      fclose(fp);

      if (ok)
      {
         buildImage(bitmap);
      }

      return ok;
   }

private:
   //! PNG file chunck
   class Chunk
   {
   public:
      Chunk(FILE* fp_)
         : fp(fp_)
      {}

      //! Read chunk header
      bool readHeader()
      {
         if (remaining_data_bytes != 0) return false;
         if (fread(&header, sizeof(Header), 1, fp) != 1) return false;

         remaining_data_bytes = header.length;
         return true;
      }

      //! Check type of this chunk
      bool isType(const char* type_) const
      {
         return ::strncmp(header.type, type_, 4) == 0;
      }

      //! Get chunck type
      std::string getType() const
      {
         std::string type;
         for(size_t i=0; i<sizeof(header.type); i++)
            type += header.type[i];
         return type;
      }

      //! Return number of data bytes still to be read from the current chunk
      size_t getRemaining() const
      {
         return remaining_data_bytes;
      }

      //! Read some chunk data
      bool read(void* data, size_t length)
      {
         if (length > remaining_data_bytes) return false;
         if (fread(data, length, 1, fp) != 1) return false;

         // TODO accumulate CRC
         remaining_data_bytes -= length;
         return true;
      }

      //! Read some chunk data
      template <typename TYPE>
      bool read(TYPE& data)
      {
         return read(&data, sizeof(TYPE));
      }

      //! Skip all remaining chunk data (and the CRC)
      bool skip()
      {
         size_t offset_to_next_chunk = remaining_data_bytes + sizeof(uint32_t);
         remaining_data_bytes = 0;
         return fseek(fp, offset_to_next_chunk, SEEK_CUR) == 0;
      }

      //! Read and validate the CRC
      bool checkCRC()
      {
         if (remaining_data_bytes != 0) return false;
         if (fread(&crc, sizeof(crc), 1, fp) != 1) return false;

         // TODO compare CRC
         return true;
      }

   private:
      struct Header
      {
         STB::Big32 length{0};
         char       type[4];
      };

      FILE*      fp{nullptr};
      Header     header;
      size_t     remaining_data_bytes{0};
      STB::Big32 crc{0};
   };

   class FileStream : public STB::ZLib::Io
   {
   public:
      FileStream(Chunk& chunk_, std::vector<uint8_t>& out_)
         : chunk(chunk_)
         , out(out_)
      {
      }

      //! Get next byte from input stream
      static uint8_t get_byte(void *that)
      {
         FileStream* file = (FileStream*) that;
         return file->getByte();
      }

   private:
      static const size_t BUFFER_SIZE = 512;

      //! Get next byte from input stream
      virtual uint8_t getByte() override
      {
         if (index == limit)
         {
            // Buffer exhausted => refill

            if (chunk.getRemaining() == 0)
            {
               // Chunck exhausted => load next chunk
               if (!chunk.checkCRC())
               {
                  error("PNG CRC error");
               }

               // Read next chunck
               if (!chunk.readHeader())
               {
                  error("PNG header read failure");
               }
               else if (chunk.getType() != "IDAT")
               {
                  error("PNG unexpected chunk type");
               }
            }

            limit = std::min(chunk.getRemaining(), sizeof(buffer));

            if (!chunk.read(buffer, limit))
            {
               error("PNG data read error");
            }

            index = 0;
         }

         return buffer[index++];
      }

      //! Recieve next image byte
      virtual void putByte(uint8_t byte) override
      {
          out.push_back(byte);
      }

      virtual void error(const std::string& message) override
      {
         fprintf(stderr, "ERR: %s\n", message.c_str());
      }

      Chunk&  chunk;
      size_t  index{0};
      size_t  limit{0};
      uint8_t buffer[BUFFER_SIZE];

      std::vector<uint8_t>& out;
   };

   bool error(const std::string& message)
   {
      fprintf(stderr, "ERR: PNG %s\n", message.c_str());
      return false;
   }

   static const uint8_t PNG_COLOUR_GREY       = 0;
   static const uint8_t PNG_COLOUR_RGB        = 2;
   static const uint8_t PNG_COLOUR_INDEXED    = 3;
   static const uint8_t PNG_COLOUR_GREY_ALPHA = 4;
   static const uint8_t PNG_COLOUR_RGB_ALPHA  = 6;

   static const uint8_t PNG_INTERLACE_NONE  = 0;
   static const uint8_t PNG_INTERLACE_ADAM7 = 1;

   static const uint8_t PNG_FILTER_METHOD_ZERO = 0;

   static const uint8_t PNG_COMPRESSION_ZLIB = 0;

   //! Read and process the IHDR chunk
   bool processChunkIHDR(Chunk& chunk)
   {
      struct ChunkIHDR
      {
         STB::Big32 width;
         STB::Big32 height;
         uint8_t    bit_depth{8};
         uint8_t    colour_type{PNG_COLOUR_RGB};
         uint8_t    compression_method{PNG_COMPRESSION_ZLIB};
         uint8_t    filter_method{PNG_FILTER_METHOD_ZERO};
         uint8_t    interlace_method{PNG_INTERLACE_NONE};
      };

      ChunkIHDR ihdr;

      if (!chunk.read(ihdr))
      {
         return error("failed to read IHDR");
      }

      if ((ihdr.interlace_method != PNG_INTERLACE_NONE) ||
          (ihdr.compression_method != PNG_COMPRESSION_ZLIB))
      {
         return error("unsupported format in IHDR");
      }

      colour_type = ihdr.colour_type;
      width       = ihdr.width;
      height      = ihdr.height;
      bit_depth   = ihdr.bit_depth;

      switch(colour_type)
      {
      case PNG_COLOUR_GREY:
         bytes_per_pixel = (bit_depth + 7) / 8;
         pitch           = (bit_depth * width) / 8 + 1;
         break;

      case PNG_COLOUR_RGB:
         bytes_per_pixel = (bit_depth / 8) * 3;
         pitch           = bytes_per_pixel * width + 1;
         break;

      case PNG_COLOUR_INDEXED:
         bytes_per_pixel = 1;
         pitch           = (bit_depth * width) / 8 + 1;
         break;

      case PNG_COLOUR_GREY_ALPHA:
         bytes_per_pixel = (bit_depth / 8) * 2;
         pitch           = bytes_per_pixel * width + 1;
         break;

      case PNG_COLOUR_RGB_ALPHA:
         bytes_per_pixel = (bit_depth / 8) * 4;
         pitch           = bytes_per_pixel * width + 1;
         break;

      default:
         return error("unsupported colour type");
      }

      image.reserve(height * pitch);

      return checkCRC(chunk);
   }

   //! Read and process the PLTE chunk
   bool processChunkPLTE(Chunk& chunk)
   {
      unsigned size = chunk.getRemaining() / 3;

      palette.resize(1<<bit_depth);

      if (size != palette.size())
      {
         return error("PLTE size error");
      }

      for(unsigned index = 0; index < size; index++)
      {
         struct RGB
         {
            uint8_t red, grn, blu;
         };

         RGB entry;

         if (!chunk.read(entry))
         {
            return error("PLTE read error");
         }

         palette[index] = STB::RGBA(entry.red, entry.grn, entry.blu, 0xFF);
      }

      return checkCRC(chunk);
   }

   //! Read and process the IDAT chunk
   bool processChunkIDAT(Chunk& chunk)
   {
      FileStream file{chunk, image};
      STB::ZLib  zlib{&file};

      (void) zlib.inflate();

      return checkCRC(chunk);
   }

   bool checkCRC(Chunk& chunk)
   {
      if (!chunk.checkCRC())
      {
         std::string message = "CRC error in ";
         message += chunk.getType();
         return error(message);
      }
      return true;
   }

   //! Reverse average filter to re-create a scan-line
   void avgFilter(uint8_t* line, bool first)
   {
      if (first)
      {
         for(unsigned i = bytes_per_pixel; i < (pitch - 1); i++)
         {
            uint8_t& x = line[i];
            uint8_t  a = line[i - bytes_per_pixel];
            x = x + a/2;
         }
      }
      else
      {
         unsigned i;

         for(i = 0; i < bytes_per_pixel; i++)
         {
            uint8_t& x = line[i];
            uint8_t  b = line[signed(i) - signed(pitch)];
            x = x + b/2;
         }

         for(; i < (pitch - 1); i++)
         {
            uint8_t& x = line[i];
            uint8_t  a = line[i - bytes_per_pixel];
            uint8_t  b = line[signed(i) - signed(pitch)];
            x = x + (a + b)/2;
         }
      }
   }

   //! Reverse PAETH filter to re-create a scan-line
   void paethFilter(uint8_t* line, bool first)
   {
      if (first)
      {
         for(unsigned i = bytes_per_pixel; i < (pitch - 1); i++)
         {
            uint8_t& x = line[i];
            uint8_t  a = line[i - bytes_per_pixel];
            x = x + a;
         }
      }
      else
      {
         unsigned i;

         for(i = 0; i < bytes_per_pixel; i++)
         {
            uint8_t& x = line[i];
            uint8_t  b = line[signed(i) - signed(pitch)];
            x = x + b;
         }

         for(; i < (pitch - 1); i++)
         {
            uint8_t& x = line[i];
            uint8_t  a = line[i - bytes_per_pixel];
            uint8_t  b = line[signed(i) - signed(pitch)];
            uint8_t  c = line[signed(i) - signed(pitch) - bytes_per_pixel];

            signed   p  = a + b - c;
            unsigned pa = abs(p - a);
            unsigned pb = abs(p - b);
            unsigned pc = abs(p - c);

            if ((pa <= pb) && (pa <= pc))
            {
               x = x + a;
            }
            else if (pb <= pc)
            {
               x = x + b;
            }
            else
            {
               x = x + c;
            }
         }
      }
   }

   //! Reverse a scan-line filter
   uint8_t* reverseFilter(unsigned y)
   {
      const uint8_t FILTER_NONE    = 0;
      const uint8_t FILTER_SUB     = 1;
      const uint8_t FILTER_UP      = 2;
      const uint8_t FILTER_AVERAGE = 3;
      const uint8_t FILTER_PAETH   = 4;

      bool     first  = (y == 0);
      uint8_t* line   = &image[pitch * y];
      uint8_t  filter = *line++;

      switch(filter)
      {
      case FILTER_NONE:
         break;

      case FILTER_SUB:
         for(unsigned i = bytes_per_pixel; i < (pitch - 1); i++)
         {
            uint8_t& x = line[i];
            uint8_t  a = line[i - bytes_per_pixel];
            x = x + a;
         }
         break;

      case FILTER_UP:
         if (!first)
         {
            for(unsigned i = 0; i < (pitch - 1); i++)
            {
               uint8_t& x = line[i];
               uint8_t  b = line[signed(i) - signed(pitch)];
               x = x + b;
            }
         }
         break;

      case FILTER_AVERAGE:
         avgFilter(line, first);
         break;

      case FILTER_PAETH:
         paethFilter(line, first);
         break;

      default:
         error("unexpected filter type");
      }

      return line;
   }

   //! Render a line of grey pixels
   void buildLineGrey(const uint8_t* line, unsigned y, PLT::Bitmap* bitmap)
   {
      for(unsigned x = 0; x < width;)
      {
         uint8_t byte = *line++;
         uint8_t level;

         switch(bit_depth)
         {
         case 1:
            for(int bit = 7; bit >= 0; bit--)
            {
               static uint8_t pal_1bit[2] = {0x00, 0xFF};

               level = pal_1bit[(byte >> bit) & 0b1];
               bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);
            }
            break;

         case 2:
            for(int lsb = 6; lsb >= 0; lsb -= 2)
            {
               static uint8_t pal_2bit[4] = {0x00, 0x55, 0xAA, 0xFF};

               level = pal_2bit[(byte >> lsb) & 0b11];
               bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);
            }
            break;

         case 4:
            level = byte & 0b1111;
            level = (level << 4) | level;
            bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);

            level = byte >> 4;
            level = (level << 4) | level;
            bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);
            break;

         case 8:
            level = byte;
            bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);
            break;

         case 16:
            level = byte;
            bitmap->point(STB::RGBA(level, level, level, 0xFF), x++, y);
            // XXX LS 8 bits ignored
            line++;
            break;

         default:
            assert(!"unexpected depth");;
         }
      }
   }

   //! Render a line of RGB pixels
   void buildLineRGB(const uint8_t* line, unsigned y, PLT::Bitmap* bitmap)
   {
      for(unsigned x = 0; x < width; x++)
      {
         switch(bit_depth)
         {
         case 8:
            bitmap->point(STB::RGBA(line[0], line[1], line[2], 0xFF), x, y);
            line += 3;
            break;

         case 16:
            // XXX LS 8 bits of each channel ignored
            bitmap->point(STB::RGBA(line[0], line[2], line[4], 0xFF), x, y);
            line += 6;
            break;

         default:
            assert(!"unexpected depth");;
         }
      }
   }

   //! Render a line of palette indexed pixels
   void buildLineIndexed(const uint8_t* line, unsigned y, PLT::Bitmap* bitmap)
   {
      //assert(palette->size() >= (1<<bit_depth)); TODO

      for(unsigned x = 0; x < width;)
      {
         uint8_t byte = *line++;
         uint8_t index;

         switch(bit_depth)
         {
         case 1:
            for(int bit = 7; bit >= 0; bit--)
            {
               index = (byte >> bit) & 0b1;
               bitmap->point(palette[index], x++, y);
            }
            break;

         case 2:
            for(int lsb = 6; lsb >= 0; lsb -= 2)
            {
               index = (byte >> lsb) & 0b11;
               bitmap->point(palette[index], x++, y);
            }
            break;

         case 4:
            index = byte & 0b1111;
            bitmap->point(palette[index], x++, y);

            index = byte >> 4;
            bitmap->point(palette[index], x++, y);
            break;

         case 8:
            index = byte;
            bitmap->point(palette[index], x++, y);
            break;

         default:
            error("unexpected depth");
         }
      }
   }

   //! Render a line of RGB with alpha pixels
   void buildLineRGBA(const uint8_t* line, unsigned y, PLT::Bitmap* bitmap)
   {
      for(unsigned x = 0; x < width; x++)
      {
         switch(bit_depth)
         {
         case 8:
            bitmap->point(STB::RGBA(line[0], line[1], line[2], line[3]), x, y);
            line += 4;
            break;

         case 16:
            // XXX LS 8 bits of each channel ignored
            bitmap->point(STB::RGBA(line[0], line[2], line[4], line[6]), x, y);
            line += 8;
            break;

         default:
            error("unexpected depth");
         }
      }
   }

   //! Render a line of grey with alpha pixels
   void buildLineGreyAlpha(const uint8_t* line, unsigned y, PLT::Bitmap* bitmap)
   {
      for(unsigned x = 0; x < width; x++)
      {
         switch(bit_depth)
         {
         case 8:
            bitmap->point(STB::RGBA(line[0], line[0], line[0], line[1]), x, y);
            line += 2;
            break;

         case 16:
            // XXX LS 8 bits ignored
            bitmap->point(STB::RGBA(line[0], line[0], line[0], line[2]), x, y);
            line += 4;
            break;

         default:
            error("unexpected depth");
         }
      }
   }

   //! Translate decompressed PNG image data into pixels
   void buildImage(PLT::Bitmap* bitmap)
   {
      bitmap->resize(width, height);

      for(unsigned y=0; y < height; y++)
      {
         uint8_t* line = reverseFilter(y);

         switch(colour_type)
         {
         case PNG_COLOUR_GREY:       buildLineGrey(     line, y, bitmap); break;
         case PNG_COLOUR_RGB:        buildLineRGB(      line, y, bitmap); break;
         case PNG_COLOUR_INDEXED:    buildLineIndexed(  line, y, bitmap); break;
         case PNG_COLOUR_RGB_ALPHA:  buildLineRGBA(     line, y, bitmap); break;
         case PNG_COLOUR_GREY_ALPHA: buildLineGreyAlpha(line, y, bitmap); break;
         }
      }
   }

   unsigned                  width{0};
   unsigned                  height{0};
   unsigned                  pitch{0};
   uint8_t                   colour_type{0};
   uint8_t                   bit_depth{0};
   uint8_t                   bytes_per_pixel{8};
   std::vector<STB::Colour>  palette{};
   std::vector<uint8_t>      image{};
};


namespace GUI {

//! Read bitmap from .png file
bool Bitmap::readPNG(const std::string& filename)
{
   return PNG().load(&bitmap, filename);
}

}
