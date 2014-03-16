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
#include <iostream>

namespace gr {
  namespace classify {

    person_vfi::sptr
    person_vfi::make(int samp_block, int samp_gap, int samp_gap_fft, int samp_rate_fft, float step_freq_max, float lsquare_max)
    {
      return gnuradio::get_initial_sptr
        (new person_vfi_impl(samp_block, samp_gap, samp_gap_fft, samp_rate_fft, step_freq_max, lsquare_max));
    }

    /*
     * The private constructor
     */
    person_vfi_impl::person_vfi_impl(int samp_block, int samp_gap, int samp_gap_fft, int samp_rate_fft, float step_freq_max, float lsquare_max)
      : gr::block("person_vfi",
              gr::io_signature::make(1, 1, sizeof(float)*samp_block),
              gr::io_signature::make(1, 1, sizeof(int)))
    {
		d_samp_block = samp_block;
		d_samp_gap = samp_gap;
		d_lsquare_max = lsquare_max;
		d_step_freq_max = step_freq_max;
		d_samp_gap_fft = samp_gap_fft;
		d_samp_rate_fft = samp_rate_fft;
		// resize circular buffer
		d_cbuffer.resize(samp_block);
		// fill circular buffer with zeros
		for(int i=0; i<samp_block; i++) d_cbuffer.push_front(0);
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
        
        std::vector<float> max_pks;
		std::vector<int> max_num;
		std::vector<float> min_pks;
		std::vector<int> min_num;
		int slope; // 1: up, -1: down
		int num_pks; // min(size(max),size(min)) to calc parameters
		float offset, freq, phase, amp, lsquare, step_freq;
		float pi = 3.14159265359;

        // Do <+signal processing+>
        if(noutput_items>0){
			// fill circuit buffer
			for(int i=0; i<d_samp_gap; i++) d_cbuffer.push_back(*in++);
			
			// find peaks (max and min)
			if(*(in+1)>*in) slope = 1; // set beginning slope
			else slope = -1;

			for(int i=1; i<d_samp_block; i++){ // search input for min and max
				if( ( slope==1 )&&( *(in+i)<*(in+i-1) ) ){ // if slope is positive and next sample ist falling
					max_pks.push_back(*(in+i-1));
					max_num.push_back(i-1);
					//std::cout << "max" << "\t" << i << "\t" << *(in+i) << std::endl;
					slope = -1;
				}
				else if( ( slope==-1 )&&( *(in+i)>*(in+i-1) ) ){ // if slope is negative and next sample is rising
					min_pks.push_back(*(in+i-1));
					min_num.push_back(i-1);
					//std::cout << "min" << "\t" << i << "\t" << *(in+i) << std::endl;
					slope = 1;
				}
			}
			
			if(!(max_num.size()==0)&&!(min_num.size()==0)){ // if there is not a single max or min prevent crashing the block
				
			/*
			 * // Debug Max und Min
			std::cout << "Max" << std::endl;
			for(int i=0; i<max_pks.size(); i++) std::cout << "(" << max_num[i] << "," << max_pks[i] << ")" << std::endl;
			std::cout << "Min" << std::endl;
			for(int i=0; i<min_pks.size(); i++) std::cout << "(" << min_num[i] << "," << min_pks[i] << ")" << std::endl;
			*/
			
			// calc min(size(max),size(min))
			num_pks = std::min(max_num.size(),min_num.size());
			
			// evaluate offset
			for(int i=0; i<num_pks; i++){
				offset += (max_pks[i] + min_pks[i])/2.0;
			}
			offset = offset/float(num_pks);
			
			// evaluate amplitude
			for(int i=0; i<num_pks; i++){
				amp += (max_pks[i] - min_pks[i])/2.0;
			}
			amp = amp/float(num_pks);
			
			// evaluate phaseshift
			phase = -max_num[0];
			
			// evaluate frequency
			freq = (max_num[num_pks]-max_num[0] + min_num[num_pks]-min_num[0])/2.0; // calc denominator of fraction
			if(freq!=0) freq = (num_pks-1)/freq;
			
			// do least-square test
			lsquare = 0;
			for(int i=0; i<d_samp_block; i++){
				lsquare+=pow(*(in+i)-(offset+amp*cos(2*pi*freq*i+phase)),2);
			}
			lsquare = lsquare/float(d_samp_block);
			
			// check lsquare_max and step_freq_max
			// return 1: person detected, 0: no person detected
			//std::cout << lsquare << std::endl;
			step_freq = freq/float(d_samp_gap_fft/float(d_samp_rate_fft));
			if( ( step_freq<d_step_freq_max )&&( lsquare<d_lsquare_max ) ){
				*out = 1;
			}
			else{
				*out = 0;
			}
			
		} // end if there is not a singel max or min
		else *out = 0;
			
		}
		else{
			*out = 0;
		}
        
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (1);

        // Tell runtime system how many output items we produced.
        return 1;
    }

  } /* namespace classify */
} /* namespace gr */

