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

#ifndef INCLUDED_CLASSIFY_PERSON_VFI_IMPL_H
#define INCLUDED_CLASSIFY_PERSON_VFI_IMPL_H

#include <classify/person_vfi.h>

namespace gr {
  namespace classify {

    class person_vfi_impl : public person_vfi
    {
     private:
      // Nothing to declare in this block.

     public:
      person_vfi_impl(int samp_block, float lsquare_max, float step_freq_max, int samp_gap_fft, int samp_rate_fft);
      ~person_vfi_impl();
      
      int d_samp_block, d_samp_gap_fft, d_samp_rate_fft;
      float d_lsquare_max, d_step_freq_max;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace classify
} // namespace gr

#endif /* INCLUDED_CLASSIFY_PERSON_VFI_IMPL_H */

