/* -*- c++ -*- */

#define RADAR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "radar_swig_doc.i"

%{
#include "radar/signal_generator_cw_c.h"
#include "radar/simulator_cc.h"
#include "radar/tsfft_cc.h"
#include "radar/os_cfar_c.h"
#include "radar/est_v_cw.h"
#include "radar/decimator_cc.h"
%}


%include "radar/signal_generator_cw_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, signal_generator_cw_c);
%include "radar/simulator_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, simulator_cc);
%include "radar/tsfft_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, tsfft_cc);
%include "radar/os_cfar_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, os_cfar_c);
%include "radar/est_v_cw.h"
GR_SWIG_BLOCK_MAGIC2(radar, est_v_cw);
%include "radar/decimator_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, decimator_cc);
