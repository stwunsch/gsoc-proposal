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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "person_vfi_impl.h"

namespace gr {
  namespace classify {

    person_vfi::sptr
    person_vfi::make(int samp_block, float lsquare_max, float step_freq_max, int samp_gap_fft, int samp_rate_fft)
    {
      return gnuradio::get_initial_sptr
        (new person_vfi_impl(samp_block, lsquare_max, step_freq_max, samp_gap_fft, samp_rate_fft));
    }

    /*
     * The private constructor
     */
    person_vfi_impl::person_vfi_impl(int samp_block, float lsquare_max, float step_freq_max, int samp_gap_fft, int samp_rate_fft)
      : gr::block("person_vfi",
              gr::io_signature::make(1, 1, sizeof(float)*samp_block),
              gr::io_signature::make(1, 1, sizeof(int)))
    {
		d_samp_block = samp_block;
		d_lsquare_max = lsquare_max;
		d_step_freq_max = step_freq_max;
		d_samp_gap_fft = samp_gap_fft;
		d_samp_rate_fft = samp_rate_fft;
	}

    /*
     * Our virtual destructor.
     */
    person_vfi_impl::~person_vfi_impl()
    {
    }

    void
    person_vfi_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = 1;
    }

    int
    person_vfi_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        int *out = (int *) output_items[0];

        // Do <+signal processing+>
        if(noutput_items>0){
			// find peaks
			
			// evaluate offset
			
			// evaluate amplitude
			
			// evaluate phaseshift
			
			// evaluate frequency
			
			// do least-square test
			
			// check lsquare_max and step_freq_max
			
			// return 1: person detected, 0: no person detected
		}
        
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (1);

        // Tell runtime system how many output items we produced.
        return 1;
    }

  } /* namespace classify */
} /* namespace gr */

