// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/energy_methods/RNA_DataBackboneEnergy.hh
/// @brief  Statistically derived rotamer pair potential class declaration
/// @author Rhiju Das


#ifndef INCLUDED_core_scoring_rna_RNA_DataBackboneEnergy_hh
#define INCLUDED_core_scoring_rna_RNA_DataBackboneEnergy_hh

// Unit Headers
#include <core/energy_methods/RNA_DataBackboneEnergy.fwd.hh>

// Package headers
#include <core/scoring/methods/ContextIndependentTwoBodyEnergy.hh>

#include <core/scoring/func/Func.fwd.hh>

// Project headers
#include <core/pose/Pose.fwd.hh>

// Utility headers

#include <utility/vector1.hh>


namespace core {
namespace energy_methods {


typedef numeric::xyzVector< core::Real > Vector;

class RNA_DataBackboneEnergy : public core::scoring::methods::ContextIndependentTwoBodyEnergy  {
public:
	typedef core::scoring::methods::ContextIndependentTwoBodyEnergy  parent;
public:


	RNA_DataBackboneEnergy();


	/// clone
	core::scoring::methods::EnergyMethodOP
	clone() const override;

	/////////////////////////////////////////////////////////////////////////////
	// scoring
	/////////////////////////////////////////////////////////////////////////////

	void
	setup_for_scoring( pose::Pose & pose, core::scoring::ScoreFunction const & ) const override;

	void
	setup_for_derivatives( pose::Pose & pose, core::scoring::ScoreFunction const & ) const override;

	void
	setup_for_packing( pose::Pose & pose, utility::vector1< bool > const &, utility::vector1< bool > const & ) const override;

	void
	residue_pair_energy(
		conformation::Residue const & rsd1,
		conformation::Residue const & rsd2,
		pose::Pose const & pose,
		core::scoring::ScoreFunction const &,
		core::scoring::EnergyMap & emap
	) const override;

	void
	eval_intrares_energy(
		conformation::Residue const &,
		pose::Pose const &,
		core::scoring::ScoreFunction const &,
		core::scoring::EnergyMap &
	) const override {}

	void
	eval_atom_derivative(
		id::AtomID const & atom_id,
		pose::Pose const & pose,
		kinematics::DomainMap const & domain_map,
		core::scoring::ScoreFunction const & scorefxn,
		core::scoring::EnergyMap const & weights,
		Vector & F1,
		Vector & F2
	) const override;

	bool
	defines_intrares_energy( core::scoring::EnergyMap const & /*weights*/ ) const override { return false; }

	// virtual
	// void
	// finalize_total_energy(
	//  pose::Pose & pose,
	//  core::scoring::ScoreFunction const &,
	//  core::scoring::EnergyMap &// totals
	// ) const;

	Distance
	atomic_interaction_cutoff() const override;

	void indicate_required_context_graphs( utility::vector1< bool > & ) const override {}

	void
	initialize_atom_numbers_sugar();

	//Vector
	// get_mean_sugar_pos( core::conformation::Residue const & rsd ) const;

	Real
	get_sugar_env_score( core::conformation::Residue const & rsd_buried, core::conformation::Residue const & rsd_other ) const;

	bool
	check_sugar_atom( Size const & n ) const;

	/////////////////////////////////////////////////////////////////////////////
	// data
	/////////////////////////////////////////////////////////////////////////////

private:

	Real const dist_cutoff_;
	Real const dist_fade_;
	Real const well_depth_burial_;
	Real const well_depth_exposed_;
	utility::vector1< Size > atom_numbers_sugar_;
	utility::vector1< Size > atom_numbers_sugar_coarse_;
	core::scoring::func::FuncOP burial_function_;
	core::Size version() const override;

};


} //scoring
} //core

#endif // INCLUDED_core_scoring_ScoreFunction_HH
