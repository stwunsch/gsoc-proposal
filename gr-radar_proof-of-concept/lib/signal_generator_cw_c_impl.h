/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_IMPL_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_IMPL_H

#include <radar/signal_generator_cw_c.h>

namespace gr {
  namespace radar {

    class signal_generator_cw_c_impl : public signal_generator_cw_c
    {
     private:
      // Nothing to declare in this block.

     public:
      signal_generator_cw_c_impl(int packet_len, int samp_rate, float freq, float ampl);
      ~signal_generator_cw_c_impl();
      
      int d_packet_len, d_samp_rate;
      float d_freq, d_ampl;
      
      pmt::pmt_t key, value, srcid;
      std::complex<float> phase;

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_IMPL_H */

