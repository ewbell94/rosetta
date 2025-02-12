// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/pack/interaction_graph/NPDHBSimpleInteractionGraph.fwd.hh
/// @brief  Data structure for representing changing energies for a Pose undergoing rotamer
///         substitutions with the non-pairwise decomposable hydrogen bond potential
/// @author Andrew Leaver-Fay (aleaverfay@gmail.com)

#ifndef INCLUDED_core_pack_interaction_graph_NPDHBSimpleInteractionGraph_FWD_HH
#define INCLUDED_core_pack_interaction_graph_NPDHBSimpleInteractionGraph_FWD_HH

// Utility headers
#include <utility/pointer/owning_ptr.hh>

namespace core {
namespace pack {
namespace interaction_graph {

class NPDHBSimpleNode;
class NPDHBSimpleEdge;
class NPDHBSimpleInteractionGraph;

typedef utility::pointer::shared_ptr< NPDHBSimpleInteractionGraph > NPDHBSimpleInteractionGraphOP;
typedef utility::pointer::shared_ptr< NPDHBSimpleInteractionGraph const > NPDHBSimpleInteractionGraphCOP;

}
}
}

#endif

