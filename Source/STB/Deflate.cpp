//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
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

// See RFC-1951
//
// NOTE some comments and code snippets have been derived from the RFC

#include <cassert>
#include <cstdint>

#include <array>
#include <vector>

#include "STB/Deflate.h"


class DeflateImpl
{
public:
   DeflateImpl(STB::Deflate::Io* io_, size_t log2_window_size_)
      : io(io_)
   {
       size_t window_size = 1 << log2_window_size_;

       window_mask = window_size - 1;

       window.resize(window_size);
   }

   //! Inflate a DEFLATE stream \return number of bytes in the output stream
   size_t inflate()
   {
      bytes_out = 0;

      bool quit = false;

      while(!quit)
      {
         // Read block header
         quit           = getBits(1);
         unsigned btype = getBits(2);

         if (btype == 0b00)
         {
            skipToNextByte();

            uint16_t len  = uint16_t(getBits(16));
            uint16_t nlen = uint16_t(getBits(16));

            if (nlen != uint16_t(~len))
            {
               io->error("DEFLATE LEN != ~NLEN");
               return 0;
            }

            copyBlock(len);
         }
         else
         {
            switch(btype)
            {
            case 0b01: buildFixedHuffmanTrees();   break;
            case 0b10: buildDynamicHuffmanTrees(); break;
            default:   io->error("DEFLATE bad block type"); return 0;
            }

            inflateBlock();
         }
      }

      skipToNextByte();

      return bytes_out;
   }

private:
   static const unsigned MAX_BITS         =  15;
   static const unsigned DISTANCE_CODES   =  32;
   static const unsigned LIT_LEN_CODES    = 288;
   static const unsigned CODE_LEN_CODES   =  19;
   static const unsigned END_OF_BLOCK_LIT = 256;

   static const size_t   MAX_HUFF_NODES = DISTANCE_CODES * 2 - 1 +
                                          LIT_LEN_CODES  * 2 - 1 +
                                          CODE_LEN_CODES * 2 - 1;

   struct HuffCode
   {
      unsigned len;
      uint16_t value;
   };

   struct HuffNode
   {
      HuffNode* branch[2] = {};
      int       symbol{0};
   };

   struct ExtraBits
   {
      unsigned bits;
      unsigned base;
   };

   void resetNodeAlloc()
   {
      node_list.resize(MAX_HUFF_NODES);
      next_free_node = 0;
   }

   HuffNode* allocNode()
   {
      assert(next_free_node < node_list.size());

      HuffNode* node = &node_list[next_free_node++];

      node->branch[0] = nullptr;
      node->branch[1] = nullptr;
      node->symbol    = 0;

      return node;
   }

   //! Compute ascending sequence of Huffman codes from bit length data
   static void huffBitLengthToCodes(HuffCode* table, unsigned num_codes)
   {
      // Count the number of codes for each bit length
      std::array<unsigned,MAX_BITS+1> bl_count = {};
      for(unsigned i = 0; i < num_codes; i++ )
      {
         bl_count[ table[i].len ]++;
      }

      // Find the numerical value of the smallest code for each bit length
      std::array<unsigned,MAX_BITS+1> next_code;
      unsigned code = 0;
      for(unsigned bits = 1; bits <= MAX_BITS; bits++)
      {
         code = (code + bl_count[ bits - 1 ]) << 1;
         next_code[bits] = code;
      }

      // Assign numerical values to all codes
      for(unsigned i = 0; i < num_codes; i++)
      {
         unsigned len = table[i].len;
         if (len != 0)
         {
            table[i].value = next_code[len]++;
         }
      }
   }

   //! Build a binary tree to decode a huffman encoded stream
   HuffNode* buildHuffmanTree(HuffCode* table, unsigned num_codes)
   {
      huffBitLengthToCodes(table, num_codes);

      HuffNode* root = allocNode();

      for(unsigned i = 0; i < num_codes; i++)
      {
         if (table[i].len != 0)
         {
            uint16_t value = table[i].value;
            HuffNode* node = root;

            for(signed bit = table[i].len - 1; bit >= 0; bit--)
            {
               unsigned digit = (value >> bit) & 1;

               if (node->branch[digit] == nullptr)
               {
                  node->branch[0] = allocNode();
                  node->branch[1] = allocNode();
                  node->symbol    = -1;
               }

               node = node->branch[digit];
            }

            node->symbol = i;
         }
      }

      return root;
   }

   //! Build Huffman trees with a static configuration
   void buildFixedHuffmanTrees()
   {
      resetNodeAlloc();

      HuffCode code[LIT_LEN_CODES];

      for(unsigned i =   0; i <= 143; i++) { code[i].len = 8; }
      for(unsigned i = 144; i <= 255; i++) { code[i].len = 9; }
      for(unsigned i = 256; i <= 279; i++) { code[i].len = 7; }
      for(unsigned i = 280; i <= 287; i++) { code[i].len = 8; }

      lit_len_tree = buildHuffmanTree(code, LIT_LEN_CODES);

      for(unsigned i = 0; i < DISTANCE_CODES; i++ ) { code[i].len = 5; }

      dist_tree = buildHuffmanTree(code, DISTANCE_CODES);
   }

   //! Build Huffman trees with a dynamic configuration from the stream
   void buildDynamicHuffmanTrees()
   {
      resetNodeAlloc();

      static unsigned code_length_order[CODE_LEN_CODES] =
      {
         16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
      };

      // Read size of code length tables
      unsigned hlit  = getBits(5) + END_OF_BLOCK_LIT + 1;
      unsigned hdist = getBits(5) + 1;
      unsigned hclen = getBits(4) + 4;

      HuffCode code[LIT_LEN_CODES];
      unsigned i;

      // Read code length table
      for(i = 0; i < hclen; i++)
      {
         code[ code_length_order[i] ].len = getBits(3);
      }
      for(; i < CODE_LEN_CODES; i++)
      {
         code[code_length_order[i]].len = 0;
      }

      HuffNode* code_len_tree = buildHuffmanTree(code, CODE_LEN_CODES);

      unsigned repeat;
      unsigned prev_code_len = 0;
      unsigned code_len;

      // Read literal/length code length table
      for(i = 0; i < hlit;)
      {
         code_len = decodeCodeLen(code_len_tree, prev_code_len, repeat);
         while(repeat-- > 0)
         {
            code[i++].len = code_len;
         }
      }
      for(; i < LIT_LEN_CODES; i++)
      {
         code[i].len = 0;
      }

      lit_len_tree = buildHuffmanTree(code, LIT_LEN_CODES);

      // Read distance code length table
      for(i = 0; i < hdist;)
      {
         code_len = decodeCodeLen(code_len_tree, prev_code_len, repeat);
         while(repeat-- > 0)
         {
            code[i++].len = code_len;
         }
      }
      for(; i < DISTANCE_CODES; i++)
      {
         code[i].len = 0;
      }

      dist_tree = buildHuffmanTree(code, DISTANCE_CODES);
   }

   //! Return the next N bits from the input stream
   uint32_t getBits(unsigned num_bits)
   {
      uint32_t value = 0;

      for(unsigned bit = 0; bit < num_bits; bit++)
      {
         if (buffer_bits == 0)
         {
            buffer      = io->getByte();
            buffer_bits = 8;
         }

         value |= (buffer & 1) << bit;

         buffer >>= 1;
         buffer_bits--;
      }

      return value;
   }

   //! Ignore remaining bits in last byte read from the input stream
   void skipToNextByte()
   {
      if (buffer_bits != 8)
      {
         buffer      = 0;
         buffer_bits = 0;
      }
   }

   void putByte(uint8_t byte)
   {
      window[bytes_out++ & window_mask] = byte;
      io->putByte(byte);
   }

   int getSymbol(HuffNode* tree)
   {
      HuffNode* node = tree;

      while(node->symbol == -1)
      {
         node = node->branch[getBits(1)];
      }

      return node->symbol;
   }

   //! Decode distance value from DEFLATE stream
   unsigned decodeDist()
   {
      static ExtraBits extra[30] =
      {
         { 0,    1}, { 0,    2}, { 0,     3}, { 0,     4}, { 1,     5},
         { 1,    7}, { 2,    9}, { 2,    13}, { 3,    17}, { 3,    25},
         { 4,   33}, { 4,   49}, { 5,    65}, { 5,    97}, { 6,   129},
         { 6,  193}, { 7,  257}, { 7,   385}, { 8,   513}, { 8,   769},
         { 9, 1025}, { 9, 1537}, {10,  2049}, {10,  3073}, {11,  4097},
         {11, 6145}, {12, 8193}, {12, 12289}, {13, 16385}, {13, 24577}
      };

      unsigned index      = getSymbol(dist_tree);
      unsigned extra_bits = extra[index].bits;
      unsigned dist       = extra[index].base;

      if (extra_bits != 0)
      {
         dist += getBits(extra_bits);
      }

      return dist;
   }

   //! Decode literal or length value from DEFLATE stream
   int decodeLitLen()
   {
      static ExtraBits extra[29] =
      {
         {0,   3}, {0,   4}, {0,   5}, {0,   6}, {0,   7},
         {0,   8}, {0,   9}, {0,  10}, {1,  11}, {1,  13},
         {1,  15}, {1,  17}, {2,  19}, {2,  23}, {2,  27},
         {2,  31}, {3,  35}, {3,  43}, {3,  51}, {3,  59},
         {4,  67}, {4,  83}, {4,  99}, {4, 115}, {5, 131},
         {5, 163}, {5, 195}, {5, 227}, {0, 258}
      };

      int lit_len = getSymbol(lit_len_tree);

      if (lit_len > signed(END_OF_BLOCK_LIT))
      {
         lit_len -= END_OF_BLOCK_LIT + 1;

         unsigned extra_bits = extra[lit_len].bits;
         lit_len             = extra[lit_len].base;

         if (extra_bits > 0)
         {
            lit_len += getBits(extra_bits);
         }

         lit_len = -lit_len;
      }

      return lit_len;
   }

   //! Decode code bit length value from DEFLATE stream
   unsigned decodeCodeLen(HuffNode* tree, unsigned& prev_code_len, unsigned& repeat)
   {
      unsigned code_len = getSymbol(tree);

      switch(code_len)
      {
      case 16:
         repeat   = getBits(2) + 3;
         code_len = prev_code_len;
         break;

      case 17:
         repeat   = getBits(3) + 3;
         code_len = 0;
         break;

      case 18:
         repeat   = getBits(7) + 11;
         code_len = 0;
         break;

      default:
         repeat = 1;
         break;
      }

      prev_code_len = code_len;

      return code_len;
   }

   //! Copy a block of data
   void copyBlock(unsigned length)
   {
      for(unsigned i=0; i<length; i++)
      {
         putByte(uint8_t(getBits(8)));
      }
   }

   //! Inflate a DEFLATE block of data via LZ77 decompressor
   void inflateBlock()
   {
      while(true)
      {
         signed lit_len = decodeLitLen();
         if (lit_len < 0)
         {
            unsigned dist = decodeDist();

            for(signed len = lit_len; len < 0; len++)
            {
               putByte(window[(bytes_out - dist) & window_mask]);
            }
         }
         else if (lit_len == signed(END_OF_BLOCK_LIT))
         {
            break;
         }
         else
         {
            putByte(uint8_t(lit_len));
         }
      }
   }

   // I/O state
   STB::Deflate::Io*     io{nullptr};
   uint8_t               buffer{0};
   unsigned              buffer_bits{0};
   size_t                bytes_out{0};

   // Compression window
   std::vector<uint8_t>  window;
   uint32_t              window_mask{0};

   // Huffman tree storage
   std::vector<HuffNode> node_list;
   size_t                next_free_node{0};

   // Huffman trees
   HuffNode*             lit_len_tree{nullptr};
   HuffNode*             dist_tree{nullptr};
};


namespace STB {

size_t Deflate::inflate(size_t log2_window_size)
{
   DeflateImpl impl{io, log2_window_size};

   return impl.inflate();
}

}
