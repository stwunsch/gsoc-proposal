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

#ifndef INCLUDED_RADAR_SIMULATOR_CC_IMPL_H
#define INCLUDED_RADAR_SIMULATOR_CC_IMPL_H

#include <radar/simulator_cc.h>

namespace gr {
  namespace radar {

    class simulator_cc_impl : public simulator_cc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      simulator_cc_impl(std::vector<float> rng, std::vector<float> vlc, std::vector<float> rcs, int samp_rate, float center_freq);
      ~simulator_cc_impl();
      
      std::vector<float> d_rng, d_vlc, d_rcs;
      float d_center_freq;
      int d_samp_rate;
      
      pmt::pmt_t packet_len_key;
      int num_targets;
      std::vector<gr_complex> phase;
      std::vector<float> freq_shift;
      std::vector<float> scale_ampl;
      
      const static float c_light = 3e8;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIMULATOR_CC_IMPL_H */

