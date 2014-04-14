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
#include "decimator_cc_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    decimator_cc::sptr
    decimator_cc::make(int dec_factor)
    {
      return gnuradio::get_initial_sptr
        (new decimator_cc_impl(dec_factor));
    }

    /*
     * The private constructor
     */
    decimator_cc_impl::decimator_cc_impl(int dec_factor)
      : gr::tagged_stream_block("decimator_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), "packet_len")
    {
		d_dec_factor = dec_factor;
	}

    /*
     * Our virtual destructor.
     */
    decimator_cc_impl::~decimator_cc_impl()
    {
    }

    int
    decimator_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0]/d_dec_factor;
      if(ninput_items[0]%d_dec_factor!=0) std::cout << "Bad decimator factor: ninput_items%d_dec_factor=" << ninput_items[0]%d_dec_factor << std::endl;
      return noutput_items ;
    }

    int
    decimator_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        
        // Set new packet_len as noutput_items
        noutput_items = ninput_items[0]/d_dec_factor;
        update_length_tags(ninput_items[0]/d_dec_factor,0);
        
        // Do <+signal processing+>
        for(int k=0; k< ninput_items[0]; k++){
			if(k%d_dec_factor==0) *out++ = in[k];
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

