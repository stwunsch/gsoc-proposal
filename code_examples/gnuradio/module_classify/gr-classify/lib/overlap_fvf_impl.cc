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
#include "overlap_fvf_impl.h"

namespace gr {
  namespace classify {

    overlap_fvf::sptr
    overlap_fvf::make(int samp_block, int samp_gap)
    {
      return gnuradio::get_initial_sptr
        (new overlap_fvf_impl(samp_block, samp_gap));
    }

    /*
     * The private constructor
     */
    overlap_fvf_impl::overlap_fvf_impl(int samp_block, int samp_gap)
      : gr::block("overlap_fvf",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)*samp_block))
    {
		d_samp_block = samp_block;
		d_samp_gap = samp_gap;
		// resize circular buffer
		d_cbuffer.resize(samp_block);
		// fill circular buffer with zeros
		for(int i=0; i<d_samp_block; i++) d_cbuffer.push_front(0);
	}

    /*
     * Our virtual destructor.
     */
    overlap_fvf_impl::~overlap_fvf_impl()
    {
    }

    void
    overlap_fvf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = d_samp_gap;
    }

    int
    overlap_fvf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
        // fill circular buffer with one gap (samp_gap samples)
        for(int i=0; i<d_samp_gap; i++) d_cbuffer.push_front(*(in+d_samp_gap-1-i));
        // push circular buffer to output-stream
        for(int i=0; i<d_samp_block; i++) *out++ = d_cbuffer[i];
        
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (d_samp_gap);

        // Tell runtime system how many output items we produced.
        return 1;
    }

  } /* namespace classify */
} /* namespace gr */

