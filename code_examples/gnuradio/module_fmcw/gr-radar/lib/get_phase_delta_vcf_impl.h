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

#ifndef INCLUDED_RADAR_GET_PHASE_DELTA_VCF_IMPL_H
#define INCLUDED_RADAR_GET_PHASE_DELTA_VCF_IMPL_H

#include <radar/get_phase_delta_vcf.h>

namespace gr {
  namespace radar {

    class get_phase_delta_vcf_impl : public get_phase_delta_vcf
    {
     private:
      // Nothing to declare in this block.

     public:
      get_phase_delta_vcf_impl(int len_vec);
      ~get_phase_delta_vcf_impl();
      
      int d_len_vec;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_GET_PHASE_DELTA_VCF_IMPL_H */

