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
#include "simulator_cc_impl.h"
#include <math.h>
#include <iostream>

namespace gr {
  namespace radar {

    simulator_cc::sptr
    simulator_cc::make(std::vector<float> rng, std::vector<float> vlc, std::vector<float> rcs, int samp_rate, float center_freq)
    {
      return gnuradio::get_initial_sptr
        (new simulator_cc_impl(rng, vlc, rcs, samp_rate, center_freq));
    }

    /*
     * The private constructor
     */
    simulator_cc_impl::simulator_cc_impl(std::vector<float> rng, std::vector<float> vlc, std::vector<float> rcs, int samp_rate, float center_freq)
      : gr::tagged_stream_block("simulator_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), "packet_len")
    {
		d_rng = rng;
		d_vlc = vlc;
		d_rcs = rcs;
		d_center_freq = center_freq;
		d_samp_rate = samp_rate;
		
		// Set packet length key
		packet_len_key = pmt::string_to_symbol("packet_len");
		
		// Read number targets
        num_targets = d_rng.size();
        
        // Set phases for adding doppler shift (done with init)
        phase.resize(num_targets);
        
        // Set frequency shifts (calc with doppler formula)
        freq_shift.resize(num_targets);
        for(int k=0; k<num_targets; k++) freq_shift[k] = 2*vlc[k]*center_freq/c_light;
        
        // Calc signal amplitudes of reflections with free space path loss and rcs
        scale_ampl.resize(num_targets);
        for(int k=0; k<num_targets; k++){
			scale_ampl[k] = 1/(float)num_targets; // FIXME: need scale
		}
	}

    /*
     * Our virtual destructor.
     */
    simulator_cc_impl::~simulator_cc_impl()
    {
    }

    int
    simulator_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }

    int
    simulator_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        
        // Set noutput_items, defined in tag packet_len, processing one packet every work call
        noutput_items = ninput_items[0];
        
        // Set output to zero
        memset(out, 0, noutput_items*sizeof(gr_complex));
        
        // Add doppler shift
        for(int k=0; k<num_targets; k++){ // go through targets and add doppler shift
			for(int i=0; i<noutput_items; i++){
				out[i] += in[i]*scale_ampl[k]*std::exp(phase[k]); // add doppler shift with rescaled amplitude
				phase[k] = 1j*std::fmod(std::imag(phase[k])+2*M_PI*freq_shift[k]/(float)d_samp_rate,2*M_PI);
			}
		}
		
		/* // FIXME: DC offset breaks simulation?
		float hold = 0;
		for(int k=0; k<noutput_items; k++) hold += pow(abs(out[k]),2);
		std::cout << "checksum: " << hold << ", phase: " << phase[phase.size()-1] << std::endl;
		*/

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

