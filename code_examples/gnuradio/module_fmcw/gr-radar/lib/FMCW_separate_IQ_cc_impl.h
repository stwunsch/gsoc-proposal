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

#ifndef INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_IMPL_H
#define INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_IMPL_H

#include <radar/FMCW_separate_IQ_cc.h>

namespace gr {
  namespace radar {

    class FMCW_separate_IQ_cc_impl : public FMCW_separate_IQ_cc
    {
     private:
      // Nothing to declare in this block.

     public:
      FMCW_separate_IQ_cc_impl(int samp_block);
      ~FMCW_separate_IQ_cc_impl();
      
      int d_samp_block;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_IMPL_H */

