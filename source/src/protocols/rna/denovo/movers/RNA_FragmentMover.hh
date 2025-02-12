// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file loopRNA_minimizer.hh
/// @brief protocols that are specific to RNA_FragmentMover
/// @author Rhiju Das


#ifndef INCLUDED_protocols_rna_RNA_FragmentMover_HH
#define INCLUDED_protocols_rna_RNA_FragmentMover_HH

// Unit headers
#include <protocols/rna/denovo/movers/RNA_FragmentMover.fwd.hh>

#include <core/fragment/rna/RNA_Fragments.fwd.hh>
#include <core/fragment/rna/RNA_FragmentHomologyExclusion.fwd.hh>

// Package headers
#include <protocols/moves/Mover.hh>

// Project headers
#include <core/types.hh>
#include <core/pose/Pose.fwd.hh>
#include <core/scoring/ScoreFunction.fwd.hh>

#include <core/pose/toolbox/AtomLevelDomainMap.fwd.hh>

// ObjexxFCL Headers

// Utility headers

// C++ headers
#include <string>
#include <map>

namespace protocols {
namespace rna {
namespace denovo {
namespace movers {

/// @brief The RNA de novo structure modeling protocol
class RNA_FragmentMover: public protocols::moves::Mover {

public:

	/// @brief Construct the protocol object given the RNA fragment library to use.
	RNA_FragmentMover( core::fragment::rna::RNA_Fragments const & all_rna_fragments,
		core::pose::toolbox::AtomLevelDomainMapCOP atom_level_domain_map,
		core::Size const symm_hack_arity,
		core::Size const exhaustive = 1,
		core::scoring::ScoreFunctionOP sfxn = nullptr );

	/// @brief Copy constructor
	RNA_FragmentMover(RNA_FragmentMover const & object_to_copy);

	~RNA_FragmentMover() override;

	/// @brief Apply the loop-rebuild protocol to the input pose
	void apply( core::pose::Pose & pose ) override;

	std::string get_name() const override;

	protocols::moves::MoverOP clone() const override;

	// virtual protocols::moves::MoverOP fresh_instance() const;

	core::Size
	random_fragment_insertion( core::pose::Pose & pose, core::Size const frag_size, bool const heating = false );

	// is this defunct now? I think so.
	void
	set_frag_size(
		core::Size const fragment_size
	);

private:

	void
	update_insert_map( core::pose::Pose const & pose );

	core::fragment::rna::RNA_Fragments const & rna_fragments_;
	core::pose::toolbox::AtomLevelDomainMapCOP atom_level_domain_map_;

	std::map < core::Size, core::Size > insert_map_;
	core::Size num_insertable_residues_;
	core::Size insert_map_frag_size_;
	core::Size frag_size_;
	core::Size symm_hack_arity_;
	core::Size exhaustive_ = 1;
	core::scoring::ScoreFunctionOP sfxn_;

	core::fragment::rna::RNA_FragmentHomologyExclusionCOP homology_exclusion_ = nullptr;

}; // class RNA_FragmentMover


} //movers
} //denovo
} //rna
} //protocols

#endif
