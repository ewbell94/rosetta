// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/energy_methods/HydroxylTorsionEnergy.hh
/// @brief  Term for proton_chi on Ser/Thr/Tyr residues
/// @author Hahnbeom Park (hahnbeom@gmail.com)

#ifndef INCLUDED_core_energy_methods_HydroxylTorsionEnergy_hh
#define INCLUDED_core_energy_methods_HydroxylTorsionEnergy_hh

// Unit headers
#include <core/energy_methods/HydroxylTorsionEnergy.fwd.hh>
#include <core/scoring/HydroxylTorsionPotential.fwd.hh>

// Package headers
#include <core/scoring/methods/ContextIndependentOneBodyEnergy.hh>
#include <core/scoring/MinimizationData.fwd.hh>

// Project headers
#include <core/pose/Pose.fwd.hh>
#include <core/conformation/Residue.fwd.hh>

#include <utility/vector1.hh>

namespace core {
namespace energy_methods {


class HydroxylTorsionEnergy : public core::scoring::methods::ContextIndependentOneBodyEnergy  {
public:
	typedef core::scoring::methods::ContextIndependentOneBodyEnergy  parent;

public:

	/// ctor
	HydroxylTorsionEnergy();

	/// clone
	core::scoring::methods::EnergyMethodOP
	clone() const override;

	/////////////////////////////////////////////////////////////////////////////
	// methods for ContextIndependentOneBodyEnergies
	/////////////////////////////////////////////////////////////////////////////

	bool
	minimize_in_whole_structure_context( pose::Pose const & ) const override { return false; }

	void
	residue_energy(
		conformation::Residue const & rsd,
		pose::Pose const &,
		core::scoring::EnergyMap & emap
	) const override;

	void
	eval_residue_derivatives(
		conformation::Residue const & rsd,
		core::scoring::ResSingleMinimizationData const & /*res_data_cache*/,
		pose::Pose const & /*pose*/,
		core::scoring::EnergyMap const & /*weights*/,
		utility::vector1< core::scoring::DerivVectorPair > & atom_derivs
	) const override;

	/// @brief P_AA_pp_Energy is context independent; indicates that no
	/// context graphs are required
	void indicate_required_context_graphs( utility::vector1< bool > & ) const override;

	core::Size version() const override;

private:
	core::scoring::HydroxylTorsionPotential const & potential_;

};

} // scoring
} // core

#endif // INCLUDED_core_energy_methods_HydroxylTorsionEnergy_hh

