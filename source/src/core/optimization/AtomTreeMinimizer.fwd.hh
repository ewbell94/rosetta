// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/optimization/AtomTreeMinimizer.fwd.hh
/// @brief  core::optimization::AtomTreeMinimizer forward declarations
/// @author Stuart G. Mentzer (Stuart_Mentzer@objexx.com)

#ifndef INCLUDED_core_optimization_AtomTreeMinimizer_fwd_hh
#define INCLUDED_core_optimization_AtomTreeMinimizer_fwd_hh

#include <utility/pointer/owning_ptr.hh>

namespace core {
namespace optimization {


// Forward
class AtomTreeMinimizer;
typedef utility::pointer::shared_ptr< AtomTreeMinimizer       > AtomTreeMinimizerOP;
typedef utility::pointer::shared_ptr< AtomTreeMinimizer const > AtomTreeMinimizerCOP;


} // namespace optimization
} // namespace core


#endif // INCLUDED_core_optimization_AtomTreeMinimizer_FWD_HH
