// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   protocols/frag_picker/scores/TalosSSSimilarity.hh
/// @brief  scores a fragment by secondary structure similarity
/// @author rvernon@u.washington.edu

#ifndef INCLUDED_protocols_frag_picker_scores_TalosSSSimilarity_hh
#define INCLUDED_protocols_frag_picker_scores_TalosSSSimilarity_hh

// type headers
#include <core/types.hh>

// package headers
#include <protocols/frag_picker/FragmentPicker.hh>
#include <protocols/frag_picker/scores/FragmentScoreMap.fwd.hh>
#include <protocols/frag_picker/scores/CachingScoringMethod.hh>

#include <core/fragment/SecondaryStructure.fwd.hh>

#include <utility/vector1.hh>

#include <protocols/frag_picker/VallProvider.hh> // AUTO IWYU For VallProvider


namespace protocols {
namespace frag_picker {
namespace scores {

typedef utility::vector1<utility::vector1<core::Real> > Matrix;

/// @brief  scores a fragment by secondary structure similarity
/// @detail The score for each position is P(H), P(L) or P(E) if
/// a vall residue is within Helix, Loop or Extended secondary stucture element, respectively.
/// P(H), P(L) and P(E) denotes the probability that a given residue in a query
/// is within Helix, Loop or Extended secondary stucture element.
/// The total score of a fragment is a simple sum of all positions; for N-mer fragment is a sum of N terms\n
/// If P(H), P(L) and P(E) probabilities takes only 1.0 and 0.0 values, result of this scoring function
/// should be the same as SecondaryIdentity, although the later one is faster.
class TalosSSSimilarity: public CachingScoringMethod {
public:

	TalosSSSimilarity(core::Size priority, core::Real lowest_acceptable_value, bool use_lowest,
		core::fragment::SecondaryStructureOP query_prediction, std::string const & prediction_name,
		core::Size sequence_length, utility::vector1<core::Size> & frag_sizes, core::Size longest_vall_chunk);

	~TalosSSSimilarity() override {}

	void do_caching(VallChunkOP) override;
	void do_caching_simple(VallChunkOP);
	bool cached_score(FragmentCandidateOP f, FragmentScoreMapOP empty_map) override;
	void clean_up() override {}

	/// @brief Computes the score
	bool score(FragmentCandidateOP, FragmentScoreMapOP) override;

	/// @brief returns the secondary structure porediction object that is used by this score
	inline core::fragment::SecondaryStructureOP get_secondary_prediction() { return query_ss_; }

	inline std::string& get_prediction_name() { return prediction_name_; };
protected:
	Matrix scores_;
	utility::vector1< Matrix > cache_;
private:
	std::string prediction_name_;
	core::fragment::SecondaryStructureOP query_ss_;
	utility::vector1< utility::vector1< core::Real > > raw_probs_;
	core::Size query_len_;

	core::Real H_mult_, L_mult_, E_mult_;

	std::string cached_scores_id_;
};

/// @brief  Maker class that produces a new TalosSSSimilarity object
class MakeTalosSSSimilarity: public MakeFragmentScoringMethod {
public:

	MakeTalosSSSimilarity() :
		MakeFragmentScoringMethod("TalosSSSimilarity") {
	}

	FragmentScoringMethodOP make(core::Size priority, core::Real lowest_acceptable_value, bool use_lowest,
		FragmentPickerOP picker, std::string const & prediction_id) override {

		core::Size sequence_length = picker->get_query_seq()->length();
		core::Size vall_max_len = picker->get_vall()->get_largest_chunk_size();

		core::fragment::SecondaryStructureOP query_prediction( picker->get_query_ss(prediction_id) );
		if ( ! query_prediction ) {
			utility_exit_with_message( "Unable to find secondary structure prediction for " + prediction_id );
		}
		return (FragmentScoringMethodOP) utility::pointer::make_shared< TalosSSSimilarity >(priority,
			lowest_acceptable_value, use_lowest,
			query_prediction,prediction_id,
			sequence_length,picker->frag_sizes_,vall_max_len);
	}
};

} // scores
} // frag_picker
} // protocols


#endif /* INCLUDED_protocols_frag_picker_scores_TalosSSSimilarity_HH */
