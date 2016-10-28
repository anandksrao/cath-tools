/// \file
/// \brief The cath_hit_resolver class definitions

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

#include "cath_hit_resolver.h"

#include "common/file/open_fstream.h"
#include "common/logger.h"
#include "exception/not_implemented_exception.h"
#include "exception/out_of_range_exception.h"
#include "resolve_hits/calc_hit_list.h"
#include "resolve_hits/file/hmmer_hmmsearch_domtblout.h"
#include "resolve_hits/file/hmmer_hmmsearch_out.h"
#include "resolve_hits/options/crh_options.h"
#include "resolve_hits/options/spec/crh_score_spec.h"
#include "resolve_hits/options/spec/crh_spec.h"
#include "resolve_hits/read_and_process_hits/read_and_process_mgr.h"

#include <fstream>

using namespace cath::common;
using namespace cath::opts;
using namespace cath::rslv;
using namespace std::literals::string_literals;

using std::istream;
using std::ifstream;
using std::ostream;
using std::ofstream;

/// \brief Perform resolve-hits according to the specified arguments strings with the specified i/o streams
void cath::rslv::perform_resolve_hits(const str_vec &args,        ///< The arguments strings specifying the resolve-hits action to perform
                                      istream       &arg_istream, ///< The input stream
                                      ostream       &arg_stdout,  ///< The output stream
                                      ostream       &arg_stderr   ///< The error stream
                                      ) {
	perform_resolve_hits(
		make_and_parse_options<crh_options>( args ),
		arg_istream,
		arg_stdout,
		arg_stderr
	);
}

/// \brief Perform resolve-hits according to the specified crh_options with the specified i/o streams
void cath::rslv::perform_resolve_hits(const crh_options &arg_opts,    ///< The crh_options specifying the resolve-hits action to perform
                                      istream           &arg_istream, ///< The input stream
                                      ostream           &arg_stdout,  ///< The output stream
                                      ostream           &arg_stderr   ///< The error stream
                                      ) {
	// If the options are invalid or specify to do_nothing, then just return
	const auto error_or_help_string = arg_opts.get_error_or_help_string();
	if ( error_or_help_string ) {
		arg_stderr << *error_or_help_string << "\n";
		return;
	}

	perform_resolve_hits(
		arg_opts.get_crh_spec(),
		arg_istream,
		arg_stdout
	);
}

/// \brief Perform resolve-hits according to the specified crh_spec with the specified i/o streams
void cath::rslv::perform_resolve_hits(const crh_spec &arg_crh_spec, ///< The crh_input_spec specifying the resolve-hits action to perform
                                      istream        &arg_istream,  ///< The input stream
                                      ostream        &arg_stdout    ///< The output stream
                                      ) {
	// Organise the input stream
	ifstream input_file_stream;
	const auto &input_file_opt = arg_crh_spec.get_input_spec().get_input_file();
	if ( input_file_opt ) {
		if ( ! exists( *input_file_opt ) ) {
			logger::log_and_exit(
				logger::return_code::NO_SUCH_FILE,
				"No such resolve-hits input data file \"" + input_file_opt->string() + "\""
			);
		}
		open_ifstream( input_file_stream, *input_file_opt );
	}
	istream &the_istream_ref = ( arg_crh_spec.get_input_spec().get_read_from_stdin() ? arg_istream : input_file_stream );

	// Organise the output stream
	ofstream output_file_stream;
	const auto &output_file_opt = arg_crh_spec.get_output_spec().get_output_file();
	if ( output_file_opt ) {
		open_ofstream( output_file_stream, *output_file_opt );
	}
	ostream &the_ostream_ref = ( output_file_opt ? output_file_stream : arg_stdout );

	// Prepare a read_and_process_mgr object
	read_and_process_mgr the_read_and_process_mgr = make_read_and_process_mgr(
		the_ostream_ref,
		arg_crh_spec
	);

	try {
		switch( arg_crh_spec.get_input_spec().get_input_format() ) {
			case ( hits_input_format_tag::HMMER_DOMTMBLOUT ) :  {
				parse_domain_hits_table(
					the_read_and_process_mgr,
					the_istream_ref,
					arg_crh_spec.get_score_spec().get_apply_cath_rules()
				);
				break;
			}
			case ( hits_input_format_tag::HMMSEARCH_OUT ) :  {
				parse_hmmsearch_out(
					the_read_and_process_mgr,
					the_istream_ref,
					arg_crh_spec.get_score_spec().get_apply_cath_rules(),
					arg_crh_spec.get_input_spec().get_min_gap_length()
				);
				break;
			}
			case ( hits_input_format_tag::RAW_WITH_SCORES ) :  {
				read_hit_list_from_istream(
					the_read_and_process_mgr,
					the_istream_ref,
					hit_score_type::CRH_SCORE
				);
				break;
			}
			case ( hits_input_format_tag::RAW_WITH_EVALUES ) :  {
				read_hit_list_from_istream(
					the_read_and_process_mgr,
					the_istream_ref,
					hit_score_type::FULL_EVALUE
				);
				break;
			}
			default : {
				BOOST_THROW_EXCEPTION(out_of_range_exception("Value of hits_input_format_tag not recognised"));
			}
		}
	}
	catch (const std::exception &arg_exception) {
		logger::log_and_exit(
			logger::return_code::MALFORMED_RESOLVE_HITS_INFILE,
			"Unable to parse/process resolve-hits input data file \""
				+ ( input_file_opt ? input_file_opt->string() : "<stdin>"s )
				+ "\" of format "
				+ to_string( arg_crh_spec.get_input_spec().get_input_format() )
				+ ". Error was:\n"
				+ arg_exception.what()
		);
	}

	// Close any open file streams
	if ( output_file_opt ) {
		output_file_stream.close();
	}
	if ( input_file_opt ) {
		input_file_stream.close();
	}
}