/// \file
/// \brief The clustmap_options class definitions

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

#include "clustmap_options.hpp"

using namespace cath;
using namespace cath::clust;

using boost::none;
using boost::program_options::positional_options_description;
using std::string;

/// The name of the program that uses this executable_options
const string clustmap_options::PROGRAM_NAME("cath-map-clusters");

/// \brief Get the name of the program that uses this executable_options
string clustmap_options::do_get_program_name() const {
	return PROGRAM_NAME;
}

/// \brief Get the positional options, which in this case is the input block's PO_INPUT_FILE_OR_STDIN option
positional_options_description clustmap_options::get_positional_options() {
	positional_options_description positionals;
	positionals.add( clustmap_input_options_block::PO_WORKING_CLUSTMEMB_FILE.c_str(), 1 );
	return positionals;
}

/// \brief Review all specified options and return a string containing any errors or a help string
///        (possibly using a description of all visible options)
///
/// This is a concrete definition of a virtual method that's pure in executable_options
///
/// This should only be called by executable_options, as the last step of the parse_options()
/// method, after all real parsing has completed.
///
/// \pre The options should have been parsed
///
/// \returns Any error/help string arising from the newly specified options
///          or an empty string if there aren't any
str_opt clustmap_options::do_get_error_or_help_string() const {
	// // If detailed help was requested, then provide it
	// if ( detail_help_ob.has_help_string() ) {
	// 	return detail_help_ob.help_string();
	// }

	if ( get_clustmap_output_spec().get_append_batch_id() && get_clustmap_input_spec().get_read_batches_from_input() ) {
		return "Cannot specify a batch ID for appending (--"
			+ clustmap_output_options_block::PO_APPEND_BATCH_ID
			+ ") when reading batches from input (--"
			+ clustmap_input_options_block::PO_READ_BATCHES_FROM_INPUT
			+ ")";
	}

	// const auto &the_in_spec = the_input_ob.get_clustmap_input_spec();
	// const auto &the_out_spec = the_output_ob.get_crh_output_spec();

	// const boost::filesystem::path & get_working_clustmemb_file() const;
	// const path_opt & get_map_from_clustmemb_file() const;
	// const bool & get_read_batches_from_input() const;

	// // If the user hasn't specified an input file, then return an blank error string
	// // (so the error will just be the basic "See 'cath-map-clusters --help' for usage." message)
	// if ( ! the_in_spec.get_working_clustmemb_file() ) {
	// 	return string{};
	// }

	// const variables_map &vm           = get_variables_map();
	// const auto          &input_format = get_crh_input_spec().get_input_format();
	// if ( specifies_option( arg_vm, crh_score_options_block::PO_APPLY_CATH_RULES ) ) {
	// 	if ( input_format != hits_input_format_tag::HMMER_DOMTBLOUT && input_format != hits_input_format_tag::HMMSEARCH_OUT ) {
	// 		return "The --"
	// 			+ crh_score_options_block::PO_APPLY_CATH_RULES
	// 			+ " option cannot be used with the input format "
	// 			+ to_string( input_format )
	// 			+ "; CATH-Gene3D rules are only applied for "
	// 			+ to_string( hits_input_format_tag::HMMER_DOMTBLOUT )
	// 			+ " and "
	// 			+ to_string( hits_input_format_tag::HMMSEARCH_OUT )
	// 			+ " formats.";
	// 	}
	// }

	// // Check that if the output_hmmsearch_aln option's enabled, the input format is HMMSEARCH_OUT
	// if ( the_out_spec.get_output_hmmsearch_aln() && input_format != hits_input_format_tag::HMMSEARCH_OUT ) {
	// 	return "Cannot use the --"
	// 		+ crh_output_options_block::PO_OUTPUT_HMMSEARCH_ALN
	// 		+ " option if using "
	// 		+ to_string( input_format )
	// 		+ " input format, must be using "
	// 		+ to_string( hits_input_format_tag::HMMSEARCH_OUT );
	// }

	// // Store a map from score type to the equivalent "--worst-permissible-[...]" option name
	// const auto worst_perm_opt_name_of_score = map<hit_score_type, string>{
	// 	{ hit_score_type::FULL_EVALUE, crh_filter_options_block::PO_WORST_PERMISSIBLE_EVALUE   },
	// 	{ hit_score_type::BITSCORE,    crh_filter_options_block::PO_WORST_PERMISSIBLE_BITSCORE },
	// 	{ hit_score_type::CRH_SCORE,   crh_filter_options_block::PO_WORST_PERMISSIBLE_SCORE    },
	// };

	// // Store a map from the score type to the valid formats for which that "--worst-permissible-[...]" option may be specified
	// const auto formats_for_worst_perm_opt_of_score = map< hit_score_type, hits_input_format_tag_vec >{
	// 	{ hit_score_type::FULL_EVALUE, { hits_input_format_tag::RAW_WITH_EVALUES,                                      }, },
	// 	{ hit_score_type::BITSCORE,    { hits_input_format_tag::HMMER_DOMTBLOUT, hits_input_format_tag::HMMSEARCH_OUT, }, },
	// 	{ hit_score_type::CRH_SCORE,   { hits_input_format_tag::RAW_WITH_SCORES                                        }, },
	// };
	// //

	// // For each such score type, check whether the relevant option has been specified and
	// // if so, validate that the input format is suitable or return an error string
	// for (const auto &format_worse_conf : formats_for_worst_perm_opt_of_score) {
	// 	const hit_score_type &score_type    = format_worse_conf.first;
	// 	const auto           &valid_formats = format_worse_conf.second;
	// 	const string         &option_name   = worst_perm_opt_name_of_score.at( score_type );

	// 	if ( specifies_option( vm, option_name ) ) {
	// 		if ( ! contains( valid_formats, input_format ) ) {
	// 			return "Cannot set worst permissible "
	// 				+ to_string( score_type   )
	// 				+ " for input format "
	// 				+ to_string( input_format )
	// 				+ ", for which "
	// 				+ to_string( score_type   )
	// 				+ " isn't the primary score type.";
	// 		}
	// 	}
	// }

	// if ( specifies_options_from_block( vm, crh_html_options_block{} ) && ! has_any_html_output( the_output_ob ) ) {
	// 	return
	// 		"Cannot specify HTML options without outputting any HTML (with --"
	// 		+ crh_output_options_block::PO_HTML_OUTPUT_TO_FILE
	// 		+ ")";
	// }

	// If no error or help string, then return none
	return none;
}

/// \brief Get a string to prepend to the standard help
string clustmap_options::do_get_help_prefix_string() const {
	return "Usage: " + PROGRAM_NAME + R"( [options] <input_file>

)" + get_overview_string() + R"(

When <input_file> is -, the input is read from standard input.)";
}

/// \brief Get a string to append to the standard help
string clustmap_options::do_get_help_suffix_string() const {
	return R"(
The input cluster-membership data should contain lines like:

cluster_name domain_id

...where domain_id is a sequence/protein name,)"
	" optionally suffixed with the domain's segments in notation like '/100-199,350-399'."
	" The suffixes should be present for all of the domain IDs or for none of them."
	R"(One  Domains shouldn't overlap with others in the same cluster-membership data should be unique and non-overlapping.

Input data needn't necessarily be grouped by cluster.

NOTE: When mapping (ie using --)" + clustmap_input_options_block::PO_MAP_FROM_CLUSTMEMB_FILE + R"(, "
	"the algorithm doesn't treat the two cluster membership files identically: "
	"it doesn't care about unmapped domains in the new cluster but "
	"does care about unmapped domains in the old cluster.)";
}

/// \brief Get an overview of the job that these options are for
///
/// This can be used in the --help and --version outputs
string clustmap_options::do_get_overview_string() const {
	return R"(Map names from previous clusters to new clusters based on (the overlaps between)
their members (which may be specified as regions within a parent sequence).
Renumber any clusters with no equivalents.)";
}

/// \brief Ctor, which initialises the detail_help_ob and adds the options_blocks to the parent executable_options
clustmap_options::clustmap_options() {
	super::add_options_block( the_input_ob   );
	super::add_options_block( the_mapping_ob );
	super::add_options_block( the_output_ob  );
}

/// \brief Getter for the spec of the cath-map-clusters input options_block
const clustmap_input_spec & clustmap_options::get_clustmap_input_spec() const {
	return the_input_ob.get_clustmap_input_spec();
}

/// \brief Getter for the spec of the cath-map-clusters mapping options_block
const clust_mapping_spec & clustmap_options::get_clust_mapping_spec() const {
	return the_mapping_ob.get_clust_mapping_spec();
}

/// \brief Getter for the spec of the cath-map-clusters output options_block
const clustmap_output_spec & clustmap_options::get_clustmap_output_spec() const {
	return the_output_ob.get_clustmap_output_spec();
}

// /// \brief Getter for the cath-map-clusters html options_block
// const crh_html_spec & clustmap_options::get_crh_html_spec() const {
// 	return the_html_ob.get_crh_html_spec();
// }

// /// \brief Get the text for the raw format help
// string cath::clust::get_crh_raw_format_help_string() {
// 	return  R"(Format for "raw" input data
// ---------------------------

// One hit per line, using the following space-separated fields:

//  1. query_protein_id : An identifier for the query protein sequence
//  2. match_id         : An identifier for the match
//  3. score            : A (strictly positive) score indicating how good it would be to have that hit in the final result
//  4. starts_stops     : The starts/stops on the query sequence, given in the format: 37-124,239-331

// Example lines:

// qyikaz 1mkfA01/12-210-i5_1,2.9e-20 2983.29780221221 3-103
// qyikaz 1mkfA01/12-210-i5_2,4.9e-19 3510.41568607646 101-224
// qyikaz 1mkfA01/12-210-i5_3,7e-25 3552.10980383852 825-928
// qyikaz 1mkfA01/12-210-i5_4,3.5e-15 2470.04912752062 953-1053
// )";
// }

// /// \brief Get the text for the apply CATH-Gene3d rules help
// string cath::clust::get_crh_cath_rules_help_string() {
// 	return R"(CATH Rules Help [--)"
// 	+ crh_score_options_block::PO_APPLY_CATH_RULES
// 	+ R"(]
// ------------------------------------

// The --)"
// 	+ crh_score_options_block::PO_APPLY_CATH_RULES
// 	+ R"( option applies the following CATH-Gene3D specific rules when parsing from )"
// 	+ to_string( hits_input_format_tag::HMMER_DOMTBLOUT )
// 	+ R"( or )"
// 	+ to_string( hits_input_format_tag::HMMSEARCH_OUT )
// 	+ R"( files.

// If hit's match ID is like "dc_72a964d791dea7a3dd35a8bbf49385b8" (matches /^dc_\w{32}$/):
//  * use the ali_from/ali_to fields rather than env_from/env_to to determine the final start/stop
//  * ignore gaps when parsing an alignment from a )"
// 	+ to_string( hits_input_format_tag::HMMSEARCH_OUT )
// 	+ R"(file (ie keep the hit as one continuous segment)

// If the conditional-evalue is <= 0.001 but the independent-value is > 0.001, then quarter the bitscore when parsing the hit.
// )";
// }