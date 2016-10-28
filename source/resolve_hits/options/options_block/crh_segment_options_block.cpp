/// \file
/// \brief The crh_segment_options_block class definitions

/// \copyright
/// CATH Tools - Protein structure comparison tools such as SSAP and SNAP
/// Copyright (C) 2011, Orengo Group, University College London
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "crh_segment_options_block.h"

#include <boost/optional.hpp>

#include "common/clone/make_uptr_clone.h"
#include "common/program_options/prog_opt_num_range.hpp"

#include <limits>

using namespace cath::common;
using namespace cath::opts;
using namespace cath::rslv;
using namespace cath;

using boost::none;
using boost::program_options::bool_switch;
using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using std::numeric_limits;
using std::string;
using std::unique_ptr;

/// \brief The option name for the specification for trimming hits' segments to allow some overlap
const string crh_segment_options_block::PO_OVERLAP_TRIM_SPEC   { "overlap-trim-spec"   };

/// \brief The option name for the minimum segment length
const string crh_segment_options_block::PO_MIN_SEG_LENGTH      { "min-seg-length"      };

/// \brief A standard do_clone method
unique_ptr<options_block> crh_segment_options_block::do_clone() const {
	return { make_uptr_clone( *this ) };
}

/// \brief Define this block's name (used as a header for the block in the usage)
string crh_segment_options_block::do_get_block_name() const {
	return "Segment overlap/removal";
}

/// \brief Add this block's options to the provided options_description
void crh_segment_options_block::do_add_visible_options_to_description(options_description &arg_desc ///< The options_description to which the options are added
                                                                      ) {
	const string length_varname { "<length>" };
	const string trim_varname   { "<trim>"   };

	const auto overlap_trim_spec_notifier = [&] (const trim_spec &x) { the_spec.set_overlap_trim_spec  ( x ); };
	const auto min_seg_length_notifier    = [&] (const residx_t  &x) { the_spec.set_min_seg_length     ( x ); };

	arg_desc.add_options()
		(
			( PO_OVERLAP_TRIM_SPEC ).c_str(),
			value<trim_spec>()
				->value_name   ( trim_varname                             )
				->notifier     ( overlap_trim_spec_notifier               )
				->default_value(
					crh_segment_spec::DEFAULT_OVERLAP_TRIM_SPEC,
					to_options_string(
						crh_segment_spec::DEFAULT_OVERLAP_TRIM_SPEC
					)
				),
			( "Allow different hits' segments to overlap a bit by trimming all segments using spec " + trim_varname
				+ "\nof the form n/m (n is a segment length; m is the *total* length to be trimmed off both ends)"
				+ "\nFor longer segments, total trim stays at m; for shorter, it decreases linearly (to 0 for length 1)."
				+ "\nTo choose: set m to the biggest total trim you'd want for a really long segment;"
				+ "\n           then, set n to length of the shortest segment you'd want to have that total trim").c_str()
		)
		(
			( PO_MIN_SEG_LENGTH ).c_str(),
			value< prog_opt_num_range<residx_t, 0, numeric_limits<residx_t>::max(), int64_t> >()
				->value_name   ( length_varname                           )
				->notifier     ( min_seg_length_notifier                  )
				->default_value( crh_segment_spec::DEFAULT_MIN_SEG_LENGTH ),
			( "Ignore all segments that are fewer than " + length_varname + " residues long" ).c_str()
		);
}

/// \brief Generate a description of any problem that makes the specified crh_segment_options_block invalid
///        or none otherwise
str_opt crh_segment_options_block::do_invalid_string(const variables_map &/*arg_variables_map*/ ///< The variables map, which options_blocks can use to determine which options were specified, defaulted etc
                                                     ) const {
	return none;
}

/// \brief Getter for the crh_segment_spec that the crh_segment_options_block configures
const crh_segment_spec & crh_segment_options_block::get_crh_segment_spec() const {
	return the_spec;
}