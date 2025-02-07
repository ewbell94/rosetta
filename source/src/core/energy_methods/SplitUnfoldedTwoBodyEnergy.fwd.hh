// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/energy_methods/SplitUnfoldedTwoBodyEnergy.fwd.hh
/// @brief  Forward header for energy method for the two body portion of the split unfolded energy.
/// @author Riley Simmons-Edler (rse231@nyu.edu)


#ifndef INCLUDED_core_energy_methods_SplitUnfoldedTwoBodyEnergy_fwd_hh
#define INCLUDED_core_energy_methods_SplitUnfoldedTwoBodyEnergy_fwd_hh

#include <utility/pointer/owning_ptr.hh>

namespace core {
namespace energy_methods {


class SplitUnfoldedTwoBodyEnergy;

typedef utility::pointer::shared_ptr<SplitUnfoldedTwoBodyEnergy> SplitUnfoldedTwoBodyEnergyOP;

} // scoring
} // core


#endif // INCLUDED_core_energy_methods_SplitUnfoldedTwoBodyEnergy_FWD_HH
