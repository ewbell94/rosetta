// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/energy_methods/LoopCloseEnergy.cc
/// @brief  Cost of bringing two chains together.
/// @author Rhiju Das


// Unit headers
#include <core/energy_methods/LoopCloseEnergy.hh>
#include <core/energy_methods/LoopCloseEnergyCreator.hh>

// Project headers
#include <core/id/AtomID.hh>
#include <core/pose/Pose.hh>
#include <core/scoring/EnergyMap.hh>
#include <core/scoring/loop_graph/LoopGraph.hh>
#include <core/pose/full_model_info/FullModelInfo.hh>
#include <core/scoring/loop_graph/evaluator/LoopClosePotentialEvaluator.hh>
#include <core/scoring/methods/EnergyMethodOptions.hh>

// Utility headers
#include <basic/Tracer.hh>

// C++
static basic::Tracer TR( "core.energy_methods.LoopCloseEnergy" );

namespace core {
namespace energy_methods {


/// @details This must return a fresh instance of the LoopCloseEnergy class,
/// never an instance already in use
core::scoring::methods::EnergyMethodOP
LoopCloseEnergyCreator::create_energy_method(
	core::scoring::methods::EnergyMethodOptions const & options
) const {
	return utility::pointer::make_shared< LoopCloseEnergy >( options );
}

core::scoring::ScoreTypes
LoopCloseEnergyCreator::score_types_for_method() const {
	using namespace core::scoring;
	ScoreTypes sts;
	sts.push_back( loop_close );
	return sts;
}


/// c-tor
LoopCloseEnergy::LoopCloseEnergy( core::scoring::methods::EnergyMethodOptions const & options ) :
	parent( utility::pointer::make_shared< LoopCloseEnergyCreator >() ),
	options_( options )
{}

/// copy ctor
LoopCloseEnergy::LoopCloseEnergy( LoopCloseEnergy const & src ):
	WholeStructureEnergy( src ),
	options_( src.options_ )
{}

/// clone
core::scoring::methods::EnergyMethodOP
LoopCloseEnergy::clone() const
{
	return utility::pointer::make_shared< LoopCloseEnergy >( *this );
}

/////////////////////////////////////////////////////////////////////////////
// scoring
/////////////////////////////////////////////////////////////////////////////
void
LoopCloseEnergy::setup_for_scoring( pose::Pose & pose, core::scoring::ScoreFunction const & ) const
{
	if ( !core::pose::full_model_info::full_model_info_defined( pose ) ) return;
	update_loop_atoms_and_lengths( pose );
}

/////////////////////////////////////////////////////////////////////////////
void
LoopCloseEnergy::setup_for_derivatives( pose::Pose & pose, core::scoring::ScoreFunction const & ) const
{
	if ( !core::pose::full_model_info::full_model_info_defined( pose ) ) return;
	update_loop_atoms_and_lengths( pose );
}

/////////////////////////////////////////////////////////////////////////////
void
LoopCloseEnergy::update_loop_atoms_and_lengths( pose::Pose & pose ) const {
	if ( !core::pose::full_model_info::full_model_info_defined( pose ) ) return;
	if ( !loop_graph_ ) loop_graph_ = utility::pointer::make_shared< scoring::loop_graph::LoopGraph >();
	loop_graph_->set_use_6D_potential( options_.loop_close_use_6D_potential() );
	loop_graph_->update( pose );
}

///////////////////////////////////////////////////////////////////////////////
void
LoopCloseEnergy::finalize_total_energy(
	pose::Pose & pose,
	core::scoring::ScoreFunction const &,
	core::scoring::EnergyMap & totals
) const {
	if ( !core::pose::full_model_info::full_model_info_defined( pose ) ) return;

	using namespace core::scoring::loop_graph;
	using namespace core::scoring::constraints;

	totals[ core::scoring::loop_close ] = loop_graph_->total_energy();

	debug_assert( totals.is_finite() );

} // finalize_total_energy

///////////////////////////////////////////////////////////////////////////////
void
LoopCloseEnergy::eval_atom_derivative(
	id::AtomID const & atom_id,
	pose::Pose const & pose,
	kinematics::DomainMap const &,
	core::scoring::ScoreFunction const &,
	core::scoring::EnergyMap const & weights,
	Vector & F1,
	Vector & F2
) const
{
	if ( !core::pose::full_model_info::full_model_info_defined( pose ) ) return;

	using namespace conformation;
	using namespace core::scoring::loop_graph;

	Vector f1, f2;

	for ( Size n = 1; n <= loop_graph_->num_current_pose_loops(); n++ ) {

		evaluator::LoopClosePotentialEvaluatorCOP loop_score_evaluator = loop_graph_->loop_score_evaluator( n );

		if ( atom_id == loop_score_evaluator->current_pose_takeoff_atom() ) {
			loop_score_evaluator->get_f1_f2( f1, f2, true /* takeoff */ );
			F1 += weights[ core::scoring::loop_close ] * f1;
			F2 += weights[ core::scoring::loop_close ] * f2;
		}

		if ( atom_id == loop_score_evaluator->current_pose_landing_atom() ) {
			loop_score_evaluator->get_f1_f2( f1, f2, false /* takeoff */ );
			F1 += weights[ core::scoring::loop_close ] * f1;
			F2 += weights[ core::scoring::loop_close ] * f2;
		}

	}
} // eval atom derivative

core::Size
LoopCloseEnergy::version() const
{
	return 2;
}


} // scoring
} // core
