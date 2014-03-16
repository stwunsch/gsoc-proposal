/* -*- c++ -*- */

#define RADAR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "radar_swig_doc.i"

%{
#include "radar/FMCW_generate_IQ_cc.h"
#include "radar/FMCW_separate_IQ_cc.h"
#include "radar/FFT_findpeak_max_vcf.h"
#include "radar/get_phase_delta_vcf.h"
%}


%include "radar/FMCW_generate_IQ_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, FMCW_generate_IQ_cc);
%include "radar/FMCW_separate_IQ_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, FMCW_separate_IQ_cc);

%include "radar/FFT_findpeak_max_vcf.h"
GR_SWIG_BLOCK_MAGIC2(radar, FFT_findpeak_max_vcf);


%include "radar/get_phase_delta_vcf.h"
GR_SWIG_BLOCK_MAGIC2(radar, get_phase_delta_vcf);
