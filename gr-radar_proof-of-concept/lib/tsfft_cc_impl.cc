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
#include "tsfft_cc_impl.h"

namespace gr {
  namespace radar {

    tsfft_cc::sptr
    tsfft_cc::make()
    {
      return gnuradio::get_initial_sptr
        (new tsfft_cc_impl());
    }

    /*
     * The private constructor
     */
    tsfft_cc_impl::tsfft_cc_impl()
      : gr::tagged_stream_block("tsfft_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), "packet_len")
    {}

    /*
     * Our virtual destructor.
     */
    tsfft_cc_impl::~tsfft_cc_impl()
    {
		fftwf_destroy_plan(p);
    }

    int
    tsfft_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }

    int
    tsfft_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *in = (gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        
        // Set output to one packet (defined with tagged stream)
        noutput_items = ninput_items[0];
        
        // Execute fft plan
        size_fft = noutput_items;
        p = fftwf_plan_dft_1d(size_fft, reinterpret_cast<fftwf_complex *>(in),
			reinterpret_cast<fftwf_complex *>(out), FFTW_FORWARD, FFTW_ESTIMATE);
		fftwf_execute(p);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

