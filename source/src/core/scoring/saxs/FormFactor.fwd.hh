// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/scoring/saxs/FormFactor.fwd.hh
/// @brief Forward declaration of a form factor data type
/// @author Dominik Gront (dgront@chem.uw.edu.pl)

#ifndef INCLUDED_core_scoring_saxs_FormFactor_fwd_hh
#define INCLUDED_core_scoring_saxs_FormFactor_fwd_hh

// utility headers
#include <utility/pointer/owning_ptr.hh>

namespace core {
namespace scoring {
namespace saxs {

class FormFactor;

typedef utility::pointer::shared_ptr<FormFactor>
	FormFactorOP;
typedef utility::pointer::shared_ptr<FormFactor const>
	FormFactorCOP;

} // saxs
} // scoring
} // core


#endif /* INCLUDED_core_scoring_saxs_FormFactor_FWD_HH */
