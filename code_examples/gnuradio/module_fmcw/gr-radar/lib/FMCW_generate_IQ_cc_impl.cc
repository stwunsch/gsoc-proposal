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
#include "FMCW_generate_IQ_cc_impl.h"

namespace gr {
  namespace radar {

    FMCW_generate_IQ_cc::sptr
    FMCW_generate_IQ_cc::make(int samp_block)
    {
      return gnuradio::get_initial_sptr
        (new FMCW_generate_IQ_cc_impl(samp_block));
    }

    /*
     * The private constructor
     */
    FMCW_generate_IQ_cc_impl::FMCW_generate_IQ_cc_impl(int samp_block)
      : gr::block("FMCW_generate_IQ_cc",
              gr::io_signature::make(3, 3, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		d_samp_block = samp_block;
	}

    /*
     * Our virtual destructor.
     */
    FMCW_generate_IQ_cc_impl::~FMCW_generate_IQ_cc_impl()
    {
    }

    void
    FMCW_generate_IQ_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = d_samp_block;
        ninput_items_required[1] = d_samp_block;
        ninput_items_required[2] = d_samp_block;
    }

    int
    FMCW_generate_IQ_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_cw = (const gr_complex *) input_items[0];
        const gr_complex *in_up = (const gr_complex *) input_items[1];
        const gr_complex *in_down = (const gr_complex *) input_items[2];
        gr_complex *out = (gr_complex *) output_items[0];
        
        int consumed_items = 0;
        int produced_items = 0;

        // Do <+signal processing+>
        if( (noutput_items>=3*d_samp_block) && (ninput_items[0]>=d_samp_block) && (ninput_items[1]>=d_samp_block) && (ninput_items[2]>=d_samp_block) ){
			for(int i=0; i<d_samp_block; i++) *out++ = *in_cw++;
			for(int i=0; i<d_samp_block; i++) *out++ = *in_up++;
			for(int i=0; i<d_samp_block; i++) *out++ = *in_down++;
			
			consumed_items = d_samp_block;
			produced_items = 3*d_samp_block;
		}
		else{
			1; // do nothing
		}
		
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed_items);

        // Tell runtime system how many output items we produced.
        return produced_items;
    }

  } /* namespace radar */
} /* namespace gr */

