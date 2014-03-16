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

#ifndef INCLUDED_CLASSIFY_OVERLAP_FVF_IMPL_H
#define INCLUDED_CLASSIFY_OVERLAP_FVF_IMPL_H

#include <classify/overlap_fvf.h>
#include <boost/circular_buffer.hpp>

namespace gr {
  namespace classify {

    class overlap_fvf_impl : public overlap_fvf
    {
     private:
      // Nothing to declare in this block.

     public:
      overlap_fvf_impl(int samp_block, int samp_gap);
      ~overlap_fvf_impl();
      
      int d_samp_block, d_samp_gap;
      boost::circular_buffer<float> d_cbuffer; // circular buffer (better performance)

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace classify
} // namespace gr

#endif /* INCLUDED_CLASSIFY_OVERLAP_FVF_IMPL_H */

