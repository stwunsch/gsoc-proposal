/* -*- c++ -*- */

#define EMULATE_HARDWARE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "emulate_hardware_swig_doc.i"

%{
#include "emulate_hardware/readIQ_c.h"
%}

%include "emulate_hardware/readIQ_c.h"
GR_SWIG_BLOCK_MAGIC2(emulate_hardware, readIQ_c);
