/* -*- c++ -*- */

#define CLASSIFY_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "classify_swig_doc.i"

%{
#include "classify/overlap_cvc.h"
#include "classify/median_ff.h"
#include "classify/overlap_fvf.h"
#include "classify/person_vfi.h"
%}

%include "classify/overlap_cvc.h"
GR_SWIG_BLOCK_MAGIC2(classify, overlap_cvc);
%include "classify/median_ff.h"
GR_SWIG_BLOCK_MAGIC2(classify, median_ff);
%include "classify/overlap_fvf.h"
GR_SWIG_BLOCK_MAGIC2(classify, overlap_fvf);

%include "classify/person_vfi.h"
GR_SWIG_BLOCK_MAGIC2(classify, person_vfi);
