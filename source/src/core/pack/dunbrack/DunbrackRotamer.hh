// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   core/scoring/dunbrack/DunbrackRotamer.hh
/// @brief  Fixed-sized dunbrack rotamer object
/// @author Andrew Leaver-Fay


#ifndef INCLUDED_core_pack_dunbrack_DunbrackRotamer_hh
#define INCLUDED_core_pack_dunbrack_DunbrackRotamer_hh

// Unit headers
#include <core/pack/dunbrack/DunbrackRotamer.fwd.hh>

// Package headers
#include <core/pack/dunbrack/RotamerLibraryScratchSpace.fwd.hh>
#include <core/types.hh>

// Project headers

// Numeric headers
#include <numeric/random/random.fwd.hh>
#include <numeric/angle.functions.hh>


// Utility headers
#include <utility/fixedsizearray1.hh>
#include <utility/VirtualBase.hh>


#include <utility/vector1.hh> // AUTO IWYU For vector1


namespace core {
namespace pack {
namespace dunbrack {

Size positive_pow( Size mantissa, Size exponent );

template< Size N >
inline Size product( utility::fixedsizearray1< Size, N > factors ) {
	Size x = factors[ 1 ];
	for ( Size i = 2; i <= N; ++i ) x *= factors[ i ];
	return x;
}

inline bool
bit_is_set(
	Size num,
	Size num_len,
	Size pos
) {
	return ( num - 1 ) & ( 1 << ( num_len - pos ) );
}

// AMW: WHY IS n_bb SEPARATE???
template< Size N >
inline Size make_index(
	utility::fixedsizearray1< Size, N > const & num_bins,
	utility::fixedsizearray1< Size, N > const & bb_bin
) {
	Size index = 1;
	for ( Size bbi = 1; bbi <= N; ++bbi ) {
		Size addend = ( bb_bin[ bbi ] - 1 );
		for ( Size bbj = N; bbj > bbi; --bbj ) {
			//Size jp1kp1lp1 = jj * N_BB_BINS[3] * N_BB_BINS[2] + kk * N_BB_BINS[3] + ll+1;
			////Size jp1kp1lp1 = (bb_bin[1]-1) * N_BB_BINS[3] * N_BB_BINS[2] + kk * N_BB_BINS[3] + ll+1;

			addend *= num_bins[ bbj ];
		}
		index += addend;
		//( bb_bin[ bbi ] - 1 ) * positive_pow( num_bins[ bbi ], n_bb - bbi );
	}
	return index;
}

// AMW: this makes ZERO SENSE.
template< Size N >
inline Size make_index(
	utility::fixedsizearray1< Size, N > const & num_bins,
	utility::fixedsizearray1< Size, N+1 > const & bb_bin
) {
	Size index = 1;
	for ( Size bbi = 1; bbi <= N; ++bbi ) {
		index += ( bb_bin[ bbi ] - 1 ) * positive_pow( num_bins[ bbi ], N - bbi );
	}
	return index;
}

/// @brief Variant of above make_index function for bb_bin as a vector.
template< Size N >
inline Size make_index(
	utility::fixedsizearray1< Size, N > const & num_bins,
	utility::vector1< Size > const & bb_bin
) {
	debug_assert( bb_bin.size() == N );
	Size index = 1;
	for ( Size bbi = 1; bbi <= N; ++bbi ) {
		Size addend = ( bb_bin[ bbi ] - 1 );
		for ( Size bbj = N; bbj > bbi; --bbj ) {
			addend *= num_bins[ bbj ];
		}
		index += addend;
	}
	return index;
}

template < Size N >
inline Size make_conditional_index(
	utility::fixedsizearray1< Size, N > num_bins,
	Size cond_i,
	utility::fixedsizearray1< Size, N > bin_true,
	utility::fixedsizearray1< Size, N > bin_false
) {
	Size index = 1;
	for ( Size bbi = 1; bbi <= N; ++bbi ) {
		Size addend;
		if ( ( cond_i - 1 ) & ( 1 << ( N - bbi ) ) ) {
			addend = ( bin_true[ bbi ] - 1 );
			//index += ( bin_true[ bbi ]  - 1 ) * positive_pow( num_bins[ bbi ], n_bb - bbi );
		} else {
			addend = ( bin_false[ bbi ] - 1 );
			//index += ( bin_false[ bbi ] - 1 ) * positive_pow( num_bins[ bbi ], n_bb - bbi );
		}
		for ( Size bbj = N; bbj > bbi; --bbj ) {
			addend *= num_bins[ bbj ];
		}
		index += addend;
	}
	return index;
}

///AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
template < Size N >
inline Size make_conditional_index(
	utility::fixedsizearray1< Size, N > num_bins,
	Size cond_i,
	utility::fixedsizearray1< Size, N+1 > bin_true,
	utility::fixedsizearray1< Size, N+1 > bin_false
) {
	Size index = 1;
	for ( Size bbi = 1; bbi <= N; ++bbi ) {
		if ( ( cond_i - 1 ) & ( 1 << ( N - bbi ) ) ) {
			index += ( bin_true[ bbi ]  - 1 ) * positive_pow( num_bins[ bbi ], N - bbi );
		} else {
			index += ( bin_false[ bbi ] - 1 ) * positive_pow( num_bins[ bbi ], N - bbi );
		}
	}
	return index;
}

/// @brief A class whose size is known at compile time.  A vector of these objects
/// will occupy a perfectly contiguous region of memory, with no extra space
/// allocated for pointers.  These objects may be rapidly allocated and deallocated
/// on the stack.
///
/// @details S for Size, P for Precision.  Now incorporating the bicubic spline
/// code from numerical recipies that was ported to Rosetta from the Meiler lab
/// code base (biotools?) by Steven Combs. Also Size N for number of backbones.
template < Size S, Size N, class P >
class DunbrackRotamerMeanSD {
public:

	DunbrackRotamerMeanSD(
		typename utility::vector1< P > const & chimean_in,
		typename utility::vector1< P > const & chisd_in,
		P const prob_in
	) :
		rotamer_probability_( prob_in )
	{
		for ( Size deriv_i = 1; deriv_i <= ( 1 << N ); ++deriv_i ) {
			n_derivs_[ deriv_i ] = 0.0;
		}

		for ( Size ii = 1; ii <= S; ++ii ) {
			chi_mean_[ ii ] = chimean_in[ ii ];
			chi_sd_  [ ii ] = chisd_in  [ ii ];
		}
	}

	DunbrackRotamerMeanSD():
		rotamer_probability_( P( 0.0 ) )
	{
		for ( Size deriv_i = 1; deriv_i <= ( 1 << N ); ++deriv_i ) {
			n_derivs_[ deriv_i ] = 0.0;
		}
		for ( Size ii = 1; ii <= S; ++ii ) {
			chi_mean_[ ii ] = P( 0.0 );
			chi_sd_  [ ii ] = P( 0.0 );
		}
	}

	P chi_mean( Size which_chi ) const {
		return chi_mean_[ which_chi ];
	}

	P chi_sd( Size which_chi ) const {
		return chi_sd_[ which_chi ];
	}

	P
	rotamer_probability() const {
		return rotamer_probability_;
	}

	utility::fixedsizearray1< core::Real, (1<<N) >
	n_derivs() const {
		return n_derivs_;
	}

	P &
	chi_mean( Size which_chi ) {
		return chi_mean_[ which_chi ];
	}

	P &
	chi_sd( Size which_chi ) {
		return chi_sd_[ which_chi ];
	}

	P &
	rotamer_probability() {
		return rotamer_probability_;
	}

	utility::fixedsizearray1< core::Real, (1<<N) > & n_derivs() {
		return n_derivs_;
	}

	void chi_mean ( Size which_chi, P chi_mean_in ) {
		chi_mean_[ which_chi ] = chi_mean_in;
	}

	void chi_sd ( Size which_chi, P chi_sd_in ) {
		chi_sd_[ which_chi ] = chi_sd_in;
	}

	void rotamer_probability( P rotprob_in ) {
		rotamer_probability_ = rotprob_in;
	}

private:
	utility::fixedsizearray1< P, S > chi_mean_;
	utility::fixedsizearray1< P, S > chi_sd_;
	P rotamer_probability_;
	utility::fixedsizearray1< core::Real, (1<<N) > n_derivs_; // We increase the precision of the derivatives for better numeric stability in integration tests.
};

template < Size S, Size N, class P >
class DunbrackRotamer : public DunbrackRotamerMeanSD< S, N, P > {
public:
	typedef DunbrackRotamerMeanSD< S, N, P > parent;

public:

	DunbrackRotamer() :
		parent()
	{
		for ( Size ii = 1; ii <= S; ++ii ) {
			rotwell_[ ii ] = 0;
		}
	}

	DunbrackRotamer(
		typename utility::vector1< P > const & chimean_in,
		typename utility::vector1< P > const & chisd_in,
		P const prob_in,
		typename utility::vector1< Size > const & rotwell_in
	) :
		parent( chimean_in, chisd_in, prob_in )
	{
		for ( Size ii = 1; ii <= S; ++ii ) rotwell_[ ii ] = rotwell_in[ ii ];
	}

	Size rotwell( Size which_chi ) const {
		return rotwell_[ which_chi ];
	}


	Size &
	rotwell( Size which_chi ) {
		return rotwell_[ which_chi ];
	}


	void rotwell( Size which_chi, Size rotwell_in ) {
		rotwell_[ which_chi ] = rotwell_in;
	}

private:
	utility::fixedsizearray1< Size, S > rotwell_;
};


template < Size S, Size N, class P >
class PackedDunbrackRotamer : public DunbrackRotamerMeanSD< S, N, P > {
public:
	typedef DunbrackRotamerMeanSD< S, N, P > parent;

public:

	PackedDunbrackRotamer(
		typename utility::vector1< P > const & chimean_in,
		typename utility::vector1< P > const & chisd_in,
		P const prob_in,
		Size const packed_rotno_in
	) :
		parent( chimean_in, chisd_in, prob_in ),
		packed_rotno_( packed_rotno_in )
	{
		//std::cout << "In the ctor body that is used in read_from_file " <<std::endl;
	}

	PackedDunbrackRotamer(
		DunbrackRotamer< S, N, P > const & sibling,
		Size const packed_rotno_in
	) :
		parent( sibling ),
		packed_rotno_( packed_rotno_in )
	{}

	PackedDunbrackRotamer(
		DunbrackRotamer< S, N, P > const & sibling
	) :
		parent( sibling ),
		packed_rotno_( 0 )
	{}


	PackedDunbrackRotamer() :
		parent(),
		packed_rotno_( 0 )
	{}

	Size &
	packed_rotno() {
		return packed_rotno_;
	}

	void
	packed_rotno( Size packed_rotno_in ) {
		packed_rotno_ = packed_rotno_in;
	}

	Size
	packed_rotno() const {
		return packed_rotno_;
	}


private:

	Size packed_rotno_;

};

class DunbrackRotamerSampleData
{
public:
	DunbrackRotamerSampleData();
	DunbrackRotamerSampleData( bool is_nrchi_sample );
	~DunbrackRotamerSampleData();

public:
	/// Setters

	void set_nrchi_sample( bool setting );
	void set_nchi( Size nchi );
	void set_rotwell(  Size chi_index, Size rotwell );
	void set_rotwell( utility::vector1< Size > const & );
	void set_chi_mean( Size chi_index, Real mean    );
	void set_chi_sd(   Size chi_index, Real sd      );
	void set_prob( Real probability );

	void set_nrchi_lower_boundary( Real low );
	void set_nrchi_upper_boundary( Real high );
	void set_nrchi_probability( Real nrchi_prob );

	bool  nrchi_sample() const { return nrchi_sample_; }
	bool  chi_is_nonrotameric( Size chi ) const { return nrchi_sample_ && chi == nchi_; }
	Size  nchi() const { return nchi_; }
	Size4 const & rot_well() const { return rot_well_; }
	Real4 const & chi_mean() const { return chi_mean_; }
	Real4 const & chi_sd() const { return chi_sd_; }
	Real  probability() const { return probability_; }

	Real  nrchi_lower_boundary() const {debug_assert( nrchi_sample_ ); return nrchi_lower_boundary_; }
	Real  nrchi_upper_boundary() const {debug_assert( nrchi_sample_ ); return nrchi_upper_boundary_; }
	Real  nrchi_probability() const {debug_assert( nrchi_sample_ ); return nrchi_probability_; }

	void  assign_random_chi( utility::vector1< Real > & chi_angles, numeric::random::RandomGenerator & RG, core::Real factor=1.0) const;
	Real  chi_probability( utility::vector1< Real > const & chi_angles, core::Real factor=1.0 ) const;

private:
	bool  nrchi_sample_; /// Does this sample describe a semi-rotameric residue?
	Size  nchi_;         /// The number of chi values the library describes; Between 1 & 4.
	Size4 rot_well_;     /// The integer description of the rotamer conformation
	Real4 chi_mean_;     /// The chi angle at the center of the distribution
	Real4 chi_sd_;       /// Standard deviation from the center for each chi.
	Real  probability_;  /// The probability of finding the rotamer in this rot_well.

	Real  nrchi_lower_boundary_; /// Requires nrchi_sample_.  The lower boundary of the non-rotameric chi well, in degrees.
	Real  nrchi_upper_boundary_; /// Requires nrchi_sample_.  The upper boundary of the non-rotameric chi well, in degrees.
	Real  nrchi_probability_;    /// Requires nrchi_sample_.  The probability that the non-rotameric chi ends up in this well given its rotameric-chi assignment.
};


/// @brief a simple class for passing data around in virtual function
/// calls of the rotamer creating process.  Derived classes will be simple
/// containers for interpolated rotameric data that 1) has to be available
/// to the derived class when building rotamers and 2) cannot be stored as
/// member data in the derived class in a thread-safe manner.  Derived classes
/// of the RotamerBuildingData can be declared on the stack, passed into
/// the RotamericSingleResidueDunbrackLibrary::build_chi_sets function,
/// and then in the (virtual) chisamples_for_rotamer function, the derived classes
/// may be downcast.
class RotamerBuildingData : public utility::VirtualBase
{
public:
	~RotamerBuildingData() override = 0;
};

/// @brief Perform cubic interpolation over each of N axes, using the
/// 2^N derivatives at 2^N gridpoints
/// @details
/// The way encoding gridpoints and derivatives into a linear structure like this
/// is actually pretty simple. Imagine the "right or left" part of a cube, or the
/// "derivative taken or not" on a particular variable, as zero or one. Then "just
/// the actual function value" maps to 000, the z derivative (for example) maps
/// to 001, d2/dydz maps to 011, etc.
/// @param[in] n_derivs is a 2^N x 2^N matrix: 2^N derivatives at 2^N gridpoints
/// @param[in] dbbp is how far along the bin our target point is, in each direction
/// @param[in] binwbb is the bin width in each direction
/// @param[out] val is the interpolated value
/// @param[out] dvaldbb are the interpolated derivatives
template < Size N >
void
polycubic_interpolation(
	utility::fixedsizearray1< utility::fixedsizearray1< Real, ( 1 << N ) >, ( 1 << N ) > n_derivs,
	utility::fixedsizearray1< Real, N > dbbp,
	utility::fixedsizearray1< Real, N > binwbb,
	Real & val,
	utility::fixedsizearray1< Real, N > & dvaldbb
) {
	utility::fixedsizearray1< Real, N > invbinwbb;
	utility::fixedsizearray1< Real, N > binwbb_over_6;
	utility::fixedsizearray1< Real, N > dbbm;
	utility::fixedsizearray1< Real, N > dbb3p;
	utility::fixedsizearray1< Real, N > dbb3m;
	for ( Size ii = 1; ii <= N; ++ii ) {
		invbinwbb[ ii ] = 1/binwbb[ ii ];
		binwbb_over_6[ ii ] = binwbb[ ii ] / 6 ;
		dbbm[ ii ] = 1 - dbbp[ ii ];
		dbb3p[ ii ] = ( dbbp[ ii ] * dbbp[ ii ] * dbbp[ ii ] - dbbp[ ii ] ) * binwbb[ ii ] * binwbb_over_6[ ii ];
		dbb3m[ ii ] = ( dbbm[ ii ] * dbbm[ ii ] * dbbm[ ii ] - dbbm[ ii ] ) * binwbb[ ii ] * binwbb_over_6[ ii ];
	}

	// there are 2^N deriv terms, i.e. the value, the N first derivatives,
	// the N^2 second derivatives... up to the single Nth derivative

	// The value has its own functional form.
	val = 0;
	for ( Size iid = 1; iid <= (1 << N); ++iid ) {
		for ( Size iiv = 1; iiv <= (1 << N); ++iiv ) {
			Real valterm = n_derivs[ iid ][ iiv ];

			for ( Size jj = 1; jj <= N; ++jj ) { // each bb
				Size two_to_the_jj_compl = 1 << ( N - jj );
				if ( ( iiv - 1 ) & two_to_the_jj_compl ) {
					valterm *= ( ( iid - 1 ) & two_to_the_jj_compl ) ? dbb3p[ jj ] : dbbp[ jj ];
				} else {
					valterm *= ( ( iid - 1 ) & two_to_the_jj_compl ) ? dbb3m[ jj ] : dbbm[ jj ];
				}
			}

			val += valterm;
		}
	}

	//Each of the N first derivatives.
	for ( Size bbn = 1; bbn <= N; ++bbn ) {
		dvaldbb[ bbn ] = 0;

		for ( Size iid = 1; iid <= (1 << N); ++iid ) {
			for ( Size iiv = 1; iiv <= (1 << N); ++iiv ) {
				Real valterm = n_derivs[ iid ][ iiv ]; // v000

				for ( Size jj = 1; jj <= N; ++jj ) {
					Size two_to_the_jj_compl = 1 << ( N - jj );

					// Half of the values from iiv = 1 to 2^N come from
					// "bb_bin_next" and half from "bb_bin."
					if ( ( iiv - 1 ) & two_to_the_jj_compl ) {

						// Does the iid-th derivative have a jj-backbone deriv?
						if ( ( iid - 1 ) & two_to_the_jj_compl ) {
							valterm *= ( bbn == jj ) ?      ( 3 * dbbp[ jj ] * dbbp[ jj ] - 1 ) * binwbb_over_6[ jj ] : dbb3p[ jj ];
						} else {
							valterm *= ( bbn == jj ) ?      invbinwbb[ jj ]                                           :  dbbp[ jj ];
						}
					} else { // bb_bin
						if ( ( iid - 1 ) & two_to_the_jj_compl ) { // is derived
							valterm *= ( bbn == jj ) ? -1 * ( 3 * dbbm[ jj ] * dbbm[ jj ] - 1 ) * binwbb_over_6[ jj ] : dbb3m[ jj ];
						} else {
							// subtract all terms where bbn was taken from bb_bin
							valterm *= ( jj == bbn ) ? -1 * invbinwbb[ jj ]                                           :  dbbm[ jj ];
						}
					}
				}

				dvaldbb[ bbn ] += valterm;
			}
		}
	}
}

/// @brief Interpolate polylinearly in N dimensions for values that AREN'T angles.
/// @details This doesn't handle wraparound.
/// @author Original author unknown.
/// @author Pulled into a new function by Vikram K. Mulligan (vmulligan@flatironinstitute.org), 12 Sept. 2018.
/// @param[in] vals The values at the corners of the gridpoints.  This vector should have N^2 points.
/// @param[in] bbd Coordinate within the grid square/cube/hypercube, with each dimension ranging from 0 to 1.
/// @param[in] binrange Size of each bin.  For example, 10 degrees by 10 degrees by 10 degrees...
/// @param[out] val The interpolated value.
/// @param[out] dval_dbb The derivatives of the interpolated value, with respect to the degrees of freedom over which the grid is defined.
template <Size N>
void
interpolate_polylinear_by_value_noangles(
	utility::fixedsizearray1< core::Real, ( 1 << N ) > const & vals,
	utility::fixedsizearray1< core::Real, N > const & bbd,
	utility::fixedsizearray1< core::Real, N > const & binrange,
	core::Real & val,
	utility::fixedsizearray1< core::Real, N > & dval_dbb
) {
	val = 0;

	for ( Size ii = 1; ii <= vals.size(); ++ii ) {
		core::Real valterm = vals[ ii ];
		for ( Size jj = 1; jj <= N; ++jj ) {
			valterm *= bit_is_set( ii, N, jj ) ? bbd[ jj ] : (1.0f - bbd[ jj ]);
		}
		val += valterm;
	}

	for ( Size ii = 1; ii <= N; ++ii ) {
		dval_dbb[ ii ] = 0.0f;
		for ( Size jj = 1; jj <= vals.size(); ++jj ) {
			core::Real valterm = 1;

			for ( Size kk = 1; kk <= N; ++kk ) {
				if ( kk == ii ) {
					valterm *= vals[ jj ];
					valterm *= bit_is_set( jj, N, kk ) ?      1.0f : -1.0f;
				} else {
					valterm *= bit_is_set( jj, N, kk ) ? bbd[ kk ] :  (1.0f - bbd[ kk ]);
				}
			}
			dval_dbb[ ii ] += valterm;
		}
		dval_dbb[ ii ] /= binrange[ii];
	}
}

/// @brief Interpolate polylinearly in N dimensions, for outputs that are angles (and have to handle wraparound properly).
/// @author Original author unknown.
/// @author Angle interpolation rewritten by Vikram K. Mulligan (vmulligan@flatironinstitute.org) and Andy Watkins, 12 Sept. 2018.
/// @note We assume that angles are in DEGREES.
/// @param[in] vals The values at the corners of the gridpoints.  This vector should have N^2 points.
/// @param[in] bbd Coordinate within the grid square/cube/hypercube, with each dimension ranging from 0 to 1.
/// @param[in] binrange Size of each bin.  For example, 10 degrees by 10 degrees by 10 degrees...
/// @param[out] val The interpolated value.
/// @param[out] dval_dbb The derivatives of the interpolated value, with respect to the degrees of freedom over which the grid is defined.
template < core::Size N > inline
void
interpolate_polylinear_by_value_angles_degrees (
	utility::fixedsizearray1< core::Real, ( 1 << N ) > const & vals,
	utility::fixedsizearray1< core::Real, N > const & bbd,
	utility::fixedsizearray1< core::Real, N > const & binrange,
	core::Real & val,
	utility::fixedsizearray1< core::Real, N > &dval_dbb
) {
	utility::fixedsizearray1< core::Real, (1 << (N-1) ) > subvals, curderivvals;
	utility::fixedsizearray1< core::Real, (N-1) > sub_bbd, sub_binrange;

	// First, interpolate the points along the current dimension:
	for ( core::Size i(1), imax( 1 << (N-1) ); i<=imax; ++i ) {
		utility::fixedsizearray1< core::Real, 2 > const curvals{ vals[i], vals[i+imax] };
		utility::fixedsizearray1< core::Real, 1 > const curbbds{ bbd[1] }, curbinrange{ binrange[1] };
		utility::fixedsizearray1< core::Real, 1 > curderiv;
		interpolate_polylinear_by_value_angles_degrees( curvals, curbbds, curbinrange, subvals[i], curderiv); //Calls the N=1 specialization.
		curderivvals[i] = curderiv[1];
	}

	// Set up subarrays for the next lower dimension:
	for ( core::Size i(2); i<=N; ++i ) {
		sub_bbd[i-1] = bbd[i];
		sub_binrange[i-1] = binrange[i];
	}

	// Interpolate (non-angles) the (N-1)^2 derivative points to get derivative in current dimension:
	utility::fixedsizearray1< core::Real, (N-1) > dummy; //Not used
	interpolate_polylinear_by_value_noangles( curderivvals, sub_bbd, sub_binrange, dval_dbb[1], dummy );

	// Call this function recursively for the next lower dimension:
	utility::fixedsizearray1< core::Real, (N-1) > sub_derivs;
	interpolate_polylinear_by_value_angles_degrees( subvals, sub_bbd, sub_binrange, val, sub_derivs );

	// Update derivatives for lower dimensions:
	for ( core::Size i(2); i<=N; ++i ) {
		dval_dbb[i] = sub_derivs[i-1];
	}
}

/// @brief Interpolate polylinearly in N dimensions, for outputs that are angles (and have to handle wraparound properly).
/// @details This version is for the special N=1 case.  It is ultimately the end of the recusive call chain for the version below.
/// @author Original author unknown.
/// @author Angle interpolation rewritten by Vikram K. Mulligan (vmulligan@flatironinstitute.org) and Andy Watkins, 12 Sept. 2018.
/// @note We assume that angles are in DEGREES.
/// @param[in] vals The values at the corners of the gridpoints.  This vector should have N^2 points.
/// @param[in] bbd Coordinate within the grid square/cube/hypercube, with each dimension ranging from 0 to 1.
/// @param[in] binrange Size of each bin.  For example, 10 degrees by 10 degrees by 10 degrees...
/// @param[out] val The interpolated value.
/// @param[out] dval_dbb The derivatives of the interpolated value, with respect to the degrees of freedom over which the grid is defined.
template <> inline
void
interpolate_polylinear_by_value_angles_degrees <1> (
	utility::fixedsizearray1< core::Real, 2 > const & vals,
	utility::fixedsizearray1< core::Real, 1 > const & bbd,
	utility::fixedsizearray1< core::Real, 1 > const & binrange,
	core::Real & val,
	utility::fixedsizearray1< core::Real, 1 > & dval_dbb
) {
	debug_assert(vals.size() == 2);
	debug_assert(bbd.size() == 1);
	debug_assert(binrange.size() == 1);
	utility::fixedsizearray1< core::Real, 2 > rangedvals_0, rangedvals_180;

	for ( core::Size i(1); i<=2; ++i ) {
		rangedvals_180[i] = numeric::principal_angle_degrees(vals[i]);
		rangedvals_0[i] = numeric::nonnegative_principal_angle_degrees(vals[i]);
	}
	if ( std::abs( rangedvals_0[1] - rangedvals_0[2] ) < std::abs( rangedvals_180[1] - rangedvals_180[2] ) ) {
		//Discontinuity is at 0
		val = numeric::principal_angle_degrees( bbd[1] * rangedvals_0[2] + (1.0-bbd[1]) * rangedvals_0[1] );
		dval_dbb[1] = (rangedvals_0[2] - rangedvals_0[1]) / binrange[1];
	} else {
		//Discontinuity is at 180
		val = numeric::principal_angle_degrees( bbd[1] * rangedvals_180[2] + (1.0-bbd[1]) * rangedvals_180[1] );
		dval_dbb[1] = (rangedvals_180[2] - rangedvals_180[1]) / binrange[1];
	}
}

/// @brief Interpolate polylinearly in N dimensions.
/// @author Original author unknown.
/// @author Angle interpolation rewritten by Vikram K. Mulligan (vmulligan@flatironinstitute.org) and Andy Watkins, 12 Sept. 2018.
/// @param[in] vals The values at the corners of the gridpoints.  This vector should have N^2 points.
/// @param[in] bbd Coordinate within the grid square/cube/hypercube, with each dimension ranging from 0 to 1.
/// @param[in] binrange Size of each bin.  For example, 10 degrees by 10 degrees by 10 degrees...
/// @param[in] angles Are we interpolating angles, or just some value?  Note that, if angles, we assume angles are in DEGREES.
/// @param[out] val The interpolated value.
/// @param[out] dval_dbb The derivatives of the interpolated value, with respect to the degrees of freedom over which the grid is defined.
template < Size N >
void
interpolate_polylinear_by_value(
	utility::fixedsizearray1< core::Real, ( 1 << N ) > const & vals,
	utility::fixedsizearray1< core::Real, N > const & bbd,
	utility::fixedsizearray1< core::Real, N > const & binrange,
	bool angles,
	core::Real & val,
	utility::fixedsizearray1< core::Real, N > & dval_dbb
) {
	debug_assert( N != 0 );

#ifndef NDEBUG
	for ( core::Size i(1); i<=N; ++i ) {
		debug_assert( bbd[i] >= -1e-5 && bbd[i] <= 1.0 + 1e-5 );
		debug_assert( binrange[i] > 1e-12 );
	}
#endif

	if ( angles ) {
		interpolate_polylinear_by_value_angles_degrees( vals, bbd, binrange, val, dval_dbb );
	} else {
		interpolate_polylinear_by_value_noangles( vals, bbd, binrange, val, dval_dbb );
	}
}

template < Size S, Size N/*, class P*/ >
DunbrackRotamer< S, N, Real >
increase_rotamer_precision(
	DunbrackRotamer< S, N, DunbrackReal > const & original_rotamer
)
{
	DunbrackRotamer< S, N, Real > new_rotamer;
	for ( Size ii = 1; ii <= S; ++ii ) {
		new_rotamer.chi_mean( ii ) = static_cast< Real > ( original_rotamer.chi_mean( ii ) );
		new_rotamer.chi_sd( ii )   = static_cast< Real > ( original_rotamer.chi_sd( ii ) );
		new_rotamer.rotwell( ii )  = original_rotamer.rotwell( ii );
	}
	new_rotamer.rotamer_probability() = static_cast< Real > ( original_rotamer.rotamer_probability() );
	return new_rotamer;
}

/// DOUG DOUG DOUG
template < Size T, Size N >
class RotamericData : public RotamerBuildingData
{
public:
	RotamericData( DunbrackRotamer< T, N, Real > const & rotamer_in ) :
		rotamer_( rotamer_in )
	{
		rotamer_.rotamer_probability() = rotamer_in.rotamer_probability();
	}

	~RotamericData() override {}

	DunbrackRotamer< T, N, Real > const &
	rotamer() const {
		return rotamer_;
	}

private:
	DunbrackRotamer< T, N, Real > rotamer_;
};


} //dunbrack
} //scoring
} // core

#endif
