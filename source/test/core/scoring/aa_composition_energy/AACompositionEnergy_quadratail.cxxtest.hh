// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   test/core/scoring/aa_composition_energy/AACompositionEnergy.cxxtest.hh
/// @brief  Test suite for core::scoring::aa_composition_energy::AACompositionEnergy, an energy term for controlling
/// sequence composition during design.
/// @details See also the core::conformation::symmetry::MirrorSymmetricConformation unit tests.  These have
/// another example of AAComposition being set up from code (with constraints attached to the pose).
/// @author Vikram K. Mulligan (vmullig@uw.edu)

// Test headers
#include <cxxtest/TestSuite.h>
#include <core/scoring/aa_composition_energy/AACompositionEnergySetup.fwd.hh>

// Unit headers


// Package Headers
#include <test/core/init_util.hh>

#include <core/pose/annotated_sequence.hh>



#include <basic/Tracer.hh>

//Auto Headers

#include <core/pose/Pose.hh> // AUTO IWYU For Pose
#include <core/scoring/ScoreFunction.hh> // AUTO IWYU For ScoreFunction
#include <core/scoring/annealing/RotamerSets.fwd.hh> // AUTO IWYU For pack, rotamer_set


static basic::Tracer TR("core.scoring.aa_composition_energy.AACompositionEnergy.cxxtest");

// --------------- Test Class --------------- //

// using declarations
using namespace core;
using namespace core::pose;
using namespace core::scoring;
using namespace core::scoring::methods;

using namespace core::pack;
using namespace core::pack::rotamer_set;

class AACompositionEnergyTests_quadratail : public CxxTest::TestSuite {

public:

	// --------------- Fixtures --------------- //

	// Shared initialization goes here.
	void setUp() {
	}

	// Shared finalization goes here.
	void tearDown() {
	}

	/// @brief Test the tail functions with quadratic above and below.
	///
	void test_tailfunctions_quadratic() {
		core_init_with_additional_options("-score:aa_composition_setup_file core/scoring/aa_composition_energy/tailfunction_quadratic.comp -out:levels core.scoring.aa_composition_energy.AACompositionEnergy:500");

		if ( TR.visible() ) {
			TR << "Starting test_tailfunctions_quadratic()." << std::endl;
		}

		// Set up score function
		ScoreFunction scorefxn;
		scorefxn.set_weight( aa_composition, 1 );

		{ //Scope 1: within test range
			Pose pose;
			make_pose_from_sequence( pose, "AAAAAGGGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 0, 1e-6);
		}

		{ //Scope 2: one below ideal, within test range
			Pose pose;
			make_pose_from_sequence( pose, "AAAAGGGGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 10, 1e-6);
		}

		{ //Scope 3: two below ideal, out of test range (quadratic region)
			Pose pose;
			make_pose_from_sequence( pose, "AAAGGGGGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 40, 1e-6);
		}

		{ //Scope 4: three below ideal, out of test range (quadratic region)
			Pose pose;
			make_pose_from_sequence( pose, "AAGGGGGGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 90, 1e-6);
		}

		{ //Scope 5: one above ideal, within test range
			Pose pose;
			make_pose_from_sequence( pose, "AAAAAAGGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 20, 1e-6);
		}

		{ //Scope 6: two above ideal, out of test range (quadratic region)
			Pose pose;
			make_pose_from_sequence( pose, "AAAAAAAGGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 80, 1e-6);
		}

		{ //Scope 6: three above ideal, out of test range (quadratic region)
			Pose pose;
			make_pose_from_sequence( pose, "AAAAAAAAGG", "fa_standard");
			TS_ASSERT_DELTA(scorefxn(pose), 180, 1e-6);
		}

		if ( TR.visible() ) {
			TR << "Test test_tailfunctions_quadratic() complete." << std::endl;
			TR.flush();
		}

		return;
	}

};
