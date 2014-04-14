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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "signal_generator_cw_c_impl.h"

namespace gr {
  namespace radar {

    signal_generator_cw_c::sptr
    signal_generator_cw_c::make(int packet_len, int samp_rate, float freq, float ampl)
    {
      return gnuradio::get_initial_sptr
        (new signal_generator_cw_c_impl(packet_len, samp_rate, freq, ampl));
    }

    /*
     * The private constructor
     */
    signal_generator_cw_c_impl::signal_generator_cw_c_impl(int packet_len, int samp_rate, float freq, float ampl)
      : gr::sync_block("signal_generator_cw_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		d_packet_len = packet_len;
		d_samp_rate = samp_rate;
		d_freq = freq;
		d_ampl = ampl;
		
		key = pmt::string_to_symbol("packet_len");
		value = pmt::from_long(packet_len);
		srcid = pmt::string_to_symbol("sg_cw");
		phase = (0,0);
	}

    /*
     * Our virtual destructor.
     */
    signal_generator_cw_c_impl::~signal_generator_cw_c_impl()
    {
    }

    int
    signal_generator_cw_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex*) output_items[0];

        // Do <+signal processing+>
        
        // Integrate phase for IQ-signal
        for(int i=0; i<noutput_items; i++){
			// Set tag
			if((nitems_written(0)+i)%d_packet_len==0) add_item_tag(0, nitems_written(0)+i, key, value, srcid);
			
			// Write sample
			*out++ = d_ampl*exp(phase);
			phase = 1j*fmod(imag(phase)+2*M_PI*d_freq/(float)d_samp_rate,2*M_PI);
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

