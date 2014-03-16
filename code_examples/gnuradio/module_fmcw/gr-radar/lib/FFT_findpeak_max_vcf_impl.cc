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
#include "FFT_findpeak_max_vcf_impl.h"

namespace gr {
  namespace radar {

    FFT_findpeak_max_vcf::sptr
    FFT_findpeak_max_vcf::make(int samp_block, float samp_rate, float freq_min, float freq_max)
    {
      return gnuradio::get_initial_sptr
        (new FFT_findpeak_max_vcf_impl(samp_block, samp_rate, freq_min, freq_max));
    }

    /*
     * The private constructor
     */
    FFT_findpeak_max_vcf_impl::FFT_findpeak_max_vcf_impl(int samp_block, float samp_rate, float freq_min, float freq_max)
      : gr::block("FFT_findpeak_max_vcf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*samp_block),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
		d_samp_block = samp_block;
		d_samp_rate = samp_rate;
		d_freq_min = freq_min;
		d_freq_max = freq_max;
		
		// positive frequencies, no fft-shift!
		d_samp_min_p = (int) ( (freq_min/samp_rate)*(float)samp_block ); // next to 0
		d_samp_max_p = (int) ( (freq_max/samp_rate)*(float)samp_block ); // next to samp_rate/2
		// negative frequencies, no fft-shift!
		d_samp_min_n = samp_block - (int) ( (freq_max/samp_rate)*(float)samp_block ); // next to -samp_rate/2
		d_samp_max_n = samp_block - (int) ( (freq_min/samp_rate)*(float)samp_block ); // next to 0
	}

    /*
     * Our virtual destructor.
     */
    FFT_findpeak_max_vcf_impl::~FFT_findpeak_max_vcf_impl()
    {
    }

    void
    FFT_findpeak_max_vcf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = 1;
    }

    int
    FFT_findpeak_max_vcf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = (float *) output_items[0];
        
        int consumed_items = 0;
        int produced_items = 0;

        // Do <+signal processing+>
        if( (ninput_items[0]>=1) && (noutput_items>=1) ){
			int max_num = 0;
			float max_pk = 0;
			
			// search for max sample
			for(int i=0; i<d_samp_block; i++){
				if((i>d_samp_min_p)&&(i<d_samp_max_p)){
					if(abs(*in)>max_pk){
						max_pk = abs(*in);
						max_num = i;
					}
				}
				if((i>d_samp_min_n)&&(i<d_samp_max_n)){
					if(abs(*in)>max_pk){
						max_pk = abs(*in);
						max_num = i;
					}
				}
				in++;
			}
			
			// calculate frequency to max sample
			if(max_num<d_samp_block/2){
				*out = ((float)max_num)*(d_samp_rate/d_samp_block);
			}
			else{
				*out = -1*((float)( d_samp_block-max_num ))*(d_samp_rate/d_samp_block);
			}
			
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

