// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/protein_interface_design/movers/DockAndRetrieveSidechains.hh
/// @author Sarel Fleishman (sarelf@u.washington.edu), Jacob Corn (jecorn@u.washington.edu)

#ifndef INCLUDED_protocols_protein_interface_design_movers_DockAndRetrieveSidechains_hh
#define INCLUDED_protocols_protein_interface_design_movers_DockAndRetrieveSidechains_hh

#include <core/pose/Pose.fwd.hh>
#include <utility/tag/Tag.fwd.hh>
#include <protocols/moves/Mover.hh>
#include <basic/datacache/DataMap.fwd.hh>

#include <protocols/docking/DockingProtocol.fwd.hh>
#include <protocols/symmetric_docking/SymDockProtocol.fwd.hh>


namespace protocols {
namespace protein_interface_design {
namespace movers {

class DockAndRetrieveSidechains : public protocols::moves::Mover
{
public:
	DockAndRetrieveSidechains();
	protocols::moves::MoverOP clone() const override;
	protocols::moves::MoverOP fresh_instance() const override { return utility::pointer::make_shared< DockAndRetrieveSidechains >(); }
	void apply( core::pose::Pose & pose ) override;
	void parse_my_tag( utility::tag::TagCOP tag, basic::datacache::DataMap & ) override;
	~DockAndRetrieveSidechains() override;

	std::string
	get_name() const override;

	static
	std::string
	mover_name();

	static
	void
	provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd );

private:
	protocols::docking::DockingProtocolOP docking_mover_;
	protocols::symmetric_docking::SymDockProtocolOP sym_docking_mover_;
	bool low_res_protocol_only_, conserve_foldtree_, symmetry_;
};

} // movers
} // protein_interface_design
} // protocols


#endif /*INCLUDED_protocols_protein_interface_design_movers_DockAndRetrieveSidechains_HH*/
