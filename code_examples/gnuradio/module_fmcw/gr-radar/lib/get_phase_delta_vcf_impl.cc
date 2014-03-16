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
#include "get_phase_delta_vcf_impl.h"
#include <algorithm>
#include <iostream>

namespace gr {
  namespace radar {

    get_phase_delta_vcf::sptr
    get_phase_delta_vcf::make(int len_vec)
    {
      return gnuradio::get_initial_sptr
        (new get_phase_delta_vcf_impl(len_vec));
    }

    /*
     * The private constructor
     */
    get_phase_delta_vcf_impl::get_phase_delta_vcf_impl(int len_vec)
      : gr::block("get_phase_delta_vcf",
              gr::io_signature::make(2, 2, sizeof(float)*len_vec),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
		d_len_vec = len_vec;
	}

    /*
     * Our virtual destructor.
     */
    get_phase_delta_vcf_impl::~get_phase_delta_vcf_impl()
    {
    }

    void
    get_phase_delta_vcf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = 1;
        ninput_items_required[1] = 1;
    }

    int
    get_phase_delta_vcf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in1 = (const float*) input_items[0];
        const float *in2 = (const float*) input_items[1];
        float *out = (float *) output_items[0];
        
        int consumed_items = 0;
        int produced_items = 0;

        // Do <+signal processing+>
        if((ninput_items[0]>=1)&&(ninput_items[1]>=1)&&(noutput_items>=1)){
			// get amplitude signal 1
			float max1 = *in1;
			float min1 = *in1;
			for(int i=0; i<d_len_vec; i++){
				if(max1<*in1) max1 = *in1;
				if(min1>*in1) min1 = *in1;
				in1++;
			}
			float amp1 = (max1-min1)/2.0;
			
			// get amplitude signal 2
			float max2 = *in2;
			float min2 = *in2;
			for(int i=0; i<d_len_vec; i++){
				if(max2<*in2) max2 = *in2;
				if(min2>*in2) min2 = *in2;
				in2++;
			}
			float amp2 = (max2-min2)/2.0;
			
			// get offset signal1*signal2
			in1 = (const float*) input_items[0];
			in2 = (const float*) input_items[1];
			float max12 = (*in1)*(*in2);
			float min12 = (*in1)*(*in2);
			for(int i=0; i<d_len_vec; i++){
				if(max12<(*in1)*(*in2)) max12 = (*in1)*(*in2);
				if(min12>(*in1)*(*in2)) min12 = (*in1)*(*in2);
				in1++; in2++;
			}
			float offset = (max12+min12)/2.0;
			
			// calc delta phase
			*out = acos( (2.0*offset)/(amp1*amp2) );
			
			// tell runtime system how many items consumed/produced
			consumed_items = 1;
			produced_items = 1;
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

