// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   protocols/jd2/NoOutputJobOutputterCreator.cc
/// @brief  NoOutputJobOutputCreator class, part of November 2011 XRW2.
/// @author Steven Lewis smlewi@gmail.com, Brian Weitzner brian.weitzner@gmail.com

// Unit headers
#include <protocols/jd2/NoOutputJobOutputterCreator.hh>
#include <protocols/jd2/NoOutputJobOutputter.hh>


namespace protocols {
namespace jd2 {

//CREATOR SECTION
std::string
NoOutputJobOutputterCreator::keyname() const
{
	return "NoOutputJobOutputter";
}

protocols::jd2::JobOutputterOP
NoOutputJobOutputterCreator::create_JobOutputter() const {
	return utility::pointer::make_shared< NoOutputJobOutputter >();
}

} // namespace jd2
} // namespace protocols

