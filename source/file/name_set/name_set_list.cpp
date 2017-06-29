/// \file
/// \brief The name_set_list class definitions

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

#include "name_set_list.hpp"

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/combine.hpp>

#include "common/algorithm/transform_build.hpp"

using namespace cath;
using namespace cath::common;
using namespace cath::file;

using boost::adaptors::transformed;
using boost::algorithm::all_of;
using boost::algorithm::join;
using boost::none;
using boost::range::combine;
using std::move;
using std::ostream;
using std::string;
using std::vector;

/// \brief Build a name_set_list from str_vec and a str_opt_vec
///
/// The resulting name_set_list will have the same size as arg_names_from_acq
///
/// \relates name_set_list
name_set_list cath::file::build_name_set_list(str_vec     arg_names_from_acq, ///< The names obtained from a pdbs_acquirer
                                              str_vec     arg_ids,            ///< Alternative IDs
                                              str_opt_vec arg_domains         ///< Regions for the strucs_context
                                              ) {
	// Convert ids to a new str_opt of the same size as arg_names_from_acq
	str_opt_vec ids;
	ids.reserve( arg_names_from_acq.size() );
	for (string &the_string : arg_ids) {
		ids.push_back( move( the_string ) );
	}
	ids.resize( arg_names_from_acq.size(), none );

	// Adjust arg_domains to the correct size (if necessary)
	arg_domains.resize( arg_names_from_acq.size(), none );

	// Build a name_set_vec from the data
	name_set_vec result;
	result.reserve( arg_names_from_acq.size() );
	for (boost::tuple<string &, str_opt &, str_opt &> &&x : combine( arg_names_from_acq, ids, arg_domains ) ) {
		result.emplace_back(
			std::move( x.get<0>() ),
			std::move( x.get<1>() ),
			std::move( x.get<2>() )
		);
	}

	/// \todo Come C++17, if Herb Sutter has gotten his way (n4029), just use braced list here
	return name_set_list{ result };
}

/// \brief Generate a string describing the specified name_set_list
///
/// \relates name_set_list
string cath::file::to_string(const name_set_list &arg_name_set_list ///< The name_set_list to describe
                             ) {
	return "name_set_list[\n\t"
		+ join(
			arg_name_set_list
				| transformed( [] (const name_set &x) { return to_string( x ); } ),
			",\n\t"
		)
		+ "\n]";
}

/// \brief Insert a description of the specified name_set_list into the specified ostream
///
/// \relates name_set_list
ostream & cath::file::operator<<(ostream             &arg_os,           ///< The ostream into which the description should be inserted
                                 const name_set_list &arg_name_set_list ///< The name_set_list to describe
                                 ) {
	arg_os << to_string( arg_name_set_list );
	return arg_os;
}


/// \brief Return whether all the specified name_sets have specified_ids
///
/// \relates name_set_list
bool cath::file::all_have_specified_id(const name_set_list &arg_name_set_list ///< TODOCUMENT
                                       ) {
	return all_of(
		arg_name_set_list,
		[] (const name_set &x) { return static_cast<bool>( x.get_specified_id() ); }
	);
}

/// \brief Return whether all the specified name_sets have domain_name_from_regions
///
/// \relates name_set_list
bool cath::file::all_have_domain_name_from_regions(const name_set_list &arg_name_set_list ///< TODOCUMENT
                                                   ) {
	return all_of(
		arg_name_set_list,
		[] (const name_set &x) { return static_cast<bool>( x.get_domain_name_from_regions() ); }
	);
}

/// \brief Get a vector of the name_from_acq strings of the specified name_sets
///
/// \relates name_set_list
str_vec cath::file::get_names_from_acq(const name_set_list &arg_name_sets ///< The name_set_list to query
                                       ) {
	return transform_build<str_vec>(
		arg_name_sets,
		[] (const name_set &x) { return x.get_name_from_acq(); }
	);
}

/// \brief Get a vector of the name_from_acq strings of the specified name_sets
///
/// \relates name_set_list
str_vec cath::file::get_domain_or_specified_or_from_acq_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                                              ) {
	return transform_build<str_vec>(
		arg_name_sets,
		[] (const name_set &x) { return get_domain_or_specified_or_name_from_acq( x ); }
	);
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in alignment HTML
///
/// \relates name_set_list
str_vec cath::file::get_alignment_html_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                             ) {
	return get_domain_or_specified_or_from_acq_names( arg_name_sets );
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in generating multi_ssap_alignment file names
///
/// \relates name_set_list
str_vec cath::file::get_multi_ssap_alignment_file_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                                        ) {
	return get_names_from_acq( arg_name_sets );
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in building protein_lists
///
/// \relates name_set_list
str_vec cath::file::get_protein_list_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                           ) {
	return get_domain_or_specified_or_from_acq_names( arg_name_sets );
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in superposition JSON
///
/// \relates name_set_list
str_vec cath::file::get_supn_json_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                        ) {
	return get_names_from_acq( arg_name_sets );
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in generating superposition pdb file names
///
/// \relates name_set_list
str_vec cath::file::get_supn_pdb_file_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                            ) {
	return get_domain_or_specified_or_from_acq_names( arg_name_sets );
}

/// \brief Get a vector of the names from the specified name_sets suitable for use in viewer (eg PyMOL) names
///
/// \relates name_set_list
str_vec cath::file::get_viewer_names(const name_set_list &arg_name_sets ///< The name_set_list to query
                                     ) {
	return get_domain_or_specified_or_from_acq_names( arg_name_sets );
}