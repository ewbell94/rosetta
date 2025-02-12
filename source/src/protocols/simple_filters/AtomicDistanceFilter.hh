// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/simple_filters/AtomicDistanceFilter.hh
/// @brief Filter for looking at specific atom distances
/// @author Rocco Moretti (rmoretti@uw.edu)

#ifndef INCLUDED_protocols_simple_filters_AtomicDistanceFilter_hh
#define INCLUDED_protocols_simple_filters_AtomicDistanceFilter_hh


// Project Headers
#include <protocols/filters/Filter.hh>
#include <core/pose/Pose.fwd.hh>
#include <core/pose/ResidueIndexDescription.fwd.hh>
#include <core/types.hh>
#include <utility/tag/Tag.fwd.hh>
#include <basic/datacache/DataMap.fwd.hh>

#include <core/select/residue_selector/ResidueSelector.fwd.hh>


namespace protocols {
namespace simple_filters {

/// @brief detects atomic contacts between two atoms of two residues
class AtomicDistanceFilter : public protocols::filters::Filter
{
private:
	typedef protocols::filters::Filter parent;
public:
	/// @brief default ctor
	AtomicDistanceFilter();
	AtomicDistanceFilter( core::Size const res1, core::Size const res2, std::string const & atom_desig1="CB", std::string const & atom_desig2="CB", bool as_type1=false, bool as_type2=false, core::Real const distance=4.0);
	bool apply( core::pose::Pose const & pose ) const override;
	core::Real compute( core::pose::Pose const & pose ) const;
	void report( std::ostream & out, core::pose::Pose const & pose ) const override;
	core::Real report_sm( core::pose::Pose const & pose ) const override;
	filters::FilterOP clone() const override {
		return utility::pointer::make_shared< AtomicDistanceFilter >( *this );
	}
	filters::FilterOP fresh_instance() const override{
		return utility::pointer::make_shared< AtomicDistanceFilter >();
	}

	~AtomicDistanceFilter() override= default;
	void parse_my_tag( utility::tag::TagCOP tag,
		basic::datacache::DataMap &
	) override;

	std::string
	name() const override;

	static
	std::string
	class_name();

	static
	void
	provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd );

	void
	set_selector_for_res1( core::select::residue_selector::ResidueSelectorCOP selector1 ){
		selector1_ = selector1;
	}

	void
	set_selector_for_res2( core::select::residue_selector::ResidueSelectorCOP selector2 ){
		selector2_ = selector2;
	}

	void
	set_atomname1( std::string && atomname1 ){
		atomdesg1_ = atomname1;
		astype1_ = false;
	}

	void
	set_atomname2( std::string && atomname2 ){
		atomdesg2_ = atomname2;
		astype2_ = false;
	}

private:
	core::select::residue_selector::ResidueSelectorCOP selector1_ = nullptr;
	core::select::residue_selector::ResidueSelectorCOP selector2_ = nullptr;

	core::pose::ResidueIndexDescriptionCOP residue1_, residue2_;
	std::string atomdesg1_, atomdesg2_;
	bool astype1_, astype2_; // if desg should be interpreted as an atomtype
	core::Real distance_;
};

} // filters
} // protocols

#endif
