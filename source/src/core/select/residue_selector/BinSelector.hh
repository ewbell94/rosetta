// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/select/residue_selector/BinSelector.hh
/// @brief  A ResidueSelector that selects residues based on their torsion bin (e.g. ABEGO bin).
/// @author Vikram K. Mulligan (vmullig@uw.edu)

#ifndef INCLUDED_core_select_residue_selector_BinSelector_HH
#define INCLUDED_core_select_residue_selector_BinSelector_HH

// Unit headers
#include <core/select/residue_selector/BinSelector.fwd.hh>

// Package headers
#include <core/select/residue_selector/ResidueSelector.hh>
#include <core/pose/Pose.fwd.hh>
#include <core/scoring/bin_transitions/BinTransitionCalculator.fwd.hh>

// Utility Headers
#include <utility/tag/Tag.fwd.hh>
#include <utility/tag/XMLSchemaGeneration.fwd.hh>
#include <utility/pointer/deep_copy.hh>

// C++ headers

#ifdef SERIALIZATION
// Cereal headers
#include <cereal/types/polymorphic.fwd.hpp>
#endif // SERIALIZATION

namespace core {
namespace select {
namespace residue_selector {

/// @brief A ResidueSelector that selects residues based on their torsion bin (e.g. ABEGO bin).
///
class BinSelector : public core::select::residue_selector::ResidueSelector {
public:

	/// @brief Constructor.
	///
	BinSelector();

	/// @brief Destructor.
	///
	~BinSelector() override;

	/// @brief Clone operator.
	/// @details Copy the current object (creating the copy on the heap) and return an owning pointer
	/// to the copy.  All ResidueSelectors must implement this.
	core::select::residue_selector::ResidueSelectorOP clone() const override;

	/// @brief "Apply" function.
	/// @details Given the pose, generate a vector of bools with entries for every residue in the pose
	/// indicating whether each residue is selected ("true") or not ("false").
	core::select::residue_selector::ResidueSubset apply( core::pose::Pose const & pose ) const override;

	/// @brief XML parse.
	/// @details Parse RosettaScripts tags and set up this mover.
	void parse_my_tag(
		utility::tag::TagCOP tag,
		basic::datacache::DataMap & datamap
	) override;

	/// @brief Get the mover class name.
	///
	std::string
	get_name() const override;

	/// @brief Get the mover class name.
	///
	static std::string class_name();

	/// @brief Provide XSD information, allowing automatic evaluation of bad XML.
	///
	static void provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd );

	/// @brief Set the name of the bin that the residues that we will select must be in.
	///
	inline void set_bin_name( std::string const &bin_name_in ) { bin_name_ = bin_name_in; }

	/// @brief Get the name of the bin that the residues that we will select must be in.
	///
	inline std::string const & bin_name() const { return bin_name_; }

	/// @brief Set the name of the bin params file that we'll be using.
	///
	inline void set_bin_params_file_name( std::string const &filename_in) { bin_params_file_name_ = filename_in; }

	/// @brief Get the name of the bin params file that we'll be using.
	///
	inline std::string const & bin_params_file_name() const { return bin_params_file_name_; }

	/// @brief Set whether we're selecting alpha-amino acids only.
	///
	inline void set_select_only_alpha_aas( bool const setting ) { select_only_alpha_aas_=setting; }

	/// @brief Get whether we're selecting alpha-amino acids only.
	///
	inline bool select_only_alpha_aas() const { return select_only_alpha_aas_; }

	/// @brief Load the bin params file and check that settings are consistent.
	/// @details Must be called before apply() function.
	void initialize_and_check();

private: //Private functions

	/// @brief Has the initialize_and_check() function been called?
	/// @details Must be called before calling apply().
	inline bool initialized() const { return initialized_; }

private: //Private variables

	/// @brief Has the initialize_and_check() function been called?
	/// @details Must be called before calling apply().
	bool initialized_ = false;

	/// @brief A BinTransitionCalculatorOP pointing at a BinTransitionCalculator object.
	/// @details Object initialized by parse_my_tag() or initialize_bin_transition_calculator() functions.
	utility::pointer::DeepCopyOP< core::scoring::bin_transitions::BinTransitionCalculator const > bin_transition_calculator_;

	/// @brief Should we only select alpha-amino acids?
	/// @details Default true.  If false, then all polymeric residues are potentially selected (if
	/// they're in the correct bin).
	bool select_only_alpha_aas_ = true;

	/// @brief The name of the bin that the residues that we will select will be in.
	///
	std::string bin_name_;

	/// @brief The name of the bin params file that we'll be using.
	///
	std::string bin_params_file_name_;

#ifdef SERIALIZATION
public:
	template< class Archive > void save( Archive & arc ) const;
	template< class Archive > void load( Archive & arc );
#endif // SERIALIZATION

};

} //core
} //select
} //residue_selector

#ifdef    SERIALIZATION
CEREAL_FORCE_DYNAMIC_INIT( core_select_residue_selector_BinSelector )
#endif // SERIALIZATION

#endif //INCLUDEDcore/select/residue_selector_BinSelector_hh
