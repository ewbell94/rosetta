// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @brief
/// @author jk

#include <iostream>
#include <string>

#include <basic/Tracer.hh>
#include <basic/options/util.hh>

#include <core/chemical/AA.hh>
#include <core/conformation/Residue.hh>
#include <core/import_pose/import_pose.hh>
#include <devel/init.hh>
#include <core/pose/Pose.hh>
#include <core/pose/PDBInfo.hh>
#include <core/scoring/ScoreFunction.hh>
#include <core/scoring/ScoreFunctionFactory.hh>
#include <core/scoring/hbonds/HBondSet.hh>
#include <core/scoring/hbonds/hbonds.hh>




#include <basic/options/keys/OptionKeys.hh> // AUTO IWYU For OptionKeys


using namespace core;
using namespace basic::options;
using namespace core::scoring;
using namespace basic::options::OptionKeys;

static basic::Tracer TR( "apps.pilot.hbonds.main" );


int main( int argc, char * argv [] )
{
	try{

		devel::init(argc, argv);

		// create native pose from pdb
		pose::Pose pose_init;
		std::string const input_pdb_name( basic::options::start_file() );
		core::import_pose::pose_from_file( pose_init, input_pdb_name , core::import_pose::PDB_file);

		// fill hbond set
		core::scoring::hbonds::HBondSet hbs1;
		scoring::ScoreFunctionOP scorefxn(get_score_function());
		(*scorefxn)(pose_init);
		core::scoring::hbonds::fill_hbond_set(pose_init, false, hbs1);

		TR << "\n\nHBOND SET:\n";
		for ( Size i=1; i<= Size(hbs1.nhbonds()); ++i ) {
			core::scoring::hbonds::HBond const & hb( hbs1.hbond(i) );
			core::conformation::Residue const & donor = pose_init.residue( hb.don_res() );
			core::conformation::Residue const & accep = pose_init.residue( hb.acc_res() );
			TR << i << ":" <<
				chemical::oneletter_code_from_aa(donor.aa()) <<
				pose_init.pdb_info()->number(donor.seqpos()) << pose_init.pdb_info()->chain(donor.seqpos()) << ' ' <<
				'(' << donor.seqpos() << ')' <<
				donor.atom_name( hb.don_hatm()) << " --- " <<
				chemical::oneletter_code_from_aa(accep.aa()) <<
				pose_init.pdb_info()->number(accep.seqpos()) << pose_init.pdb_info()->chain(accep.seqpos()) << ' ' <<
				'(' << accep.seqpos() << ')' <<
				accep.atom_name( hb.acc_atm()) << "\n";
		}
	} catch (utility::excn::Exception const & e ) {
		e.display();
		return -1;
	}
}

