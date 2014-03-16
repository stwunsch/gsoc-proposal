/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_EMULATE_HARDWARE_READIQ_C_IMPL_H
#define INCLUDED_EMULATE_HARDWARE_READIQ_C_IMPL_H

#include <emulate_hardware/readIQ_c.h>
#include <cstdio>
#include <string>

namespace gr {
  namespace emulate_hardware {

    class readIQ_c_impl : public readIQ_c
    {
     private:
      // Nothing to declare in this block.

     public:
      readIQ_c_impl(std::string filepath);
      ~readIQ_c_impl();

	  FILE* d_src;
	  double d_buffer[3];
	  int d_items_read;
	  size_t d_result;

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace emulate_hardware
} // namespace gr

#endif /* INCLUDED_EMULATE_HARDWARE_READIQ_C_IMPL_H */

