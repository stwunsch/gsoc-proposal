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
#include "median_ff_impl.h"

namespace gr {
  namespace classify {

    median_ff::sptr
    median_ff::make(int samp_range)
    {
      return gnuradio::get_initial_sptr
        (new median_ff_impl(samp_range));
    }

    /*
     * The private constructor
     */
    median_ff_impl::median_ff_impl(int samp_range)
      : gr::sync_block("median_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
		d_samp_range = samp_range;
		// resize buffer
		d_buffer.resize(samp_range);
		d_sort.resize(samp_range);
		// fill buffer with zeros
		for(int i=0; i<samp_range; i++) d_buffer.push_back(0);
	}

    /*
     * Our virtual destructor.
     */
    median_ff_impl::~median_ff_impl()
    {
    }

    int
    median_ff_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
        for(int i=0; i<noutput_items; i++){
			// push sample to the front in buffer
			d_buffer.push_back(*in++);
			// copy to sort buffer and do sort
			for(int i=0; i<d_samp_range; i++) d_sort[i] = d_buffer[i];
			std::sort(d_sort.begin(),d_sort.end());
			// find the median value and push to output-stream, median value is best for odd samp_range
			*out++ = d_sort[d_samp_range/2];
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace classify */
} /* namespace gr */

