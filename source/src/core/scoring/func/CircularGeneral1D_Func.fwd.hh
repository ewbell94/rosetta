// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file src/core/scoring/func/CircularGeneral1D_Func.fwd.hh
/// @brief forward declaration for CircularGeneral1D_Func class
/// @author James Thompson


#ifndef INCLUDED_core_scoring_func_CircularGeneral1D_Func_FWD_HH
#define INCLUDED_core_scoring_func_CircularGeneral1D_Func_FWD_HH

#include <utility/pointer/owning_ptr.hh>

// C++ Headers

namespace core {
namespace scoring {
namespace func {

class CircularGeneral1D_Func;
typedef utility::pointer::shared_ptr< CircularGeneral1D_Func > CircularGeneral1D_FuncOP;
typedef utility::pointer::shared_ptr< CircularGeneral1D_Func const > CircularGeneral1D_FuncCOP;

}
}
}

#endif
