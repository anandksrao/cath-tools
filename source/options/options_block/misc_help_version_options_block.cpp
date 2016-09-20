/// \file
/// \brief The misc_help_version_options_block class definitions

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

#include "misc_help_version_options_block.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/optional.hpp>

#include "cath_tools_git_version.h"
#include "common/boost_addenda/string_algorithm/split_build.h"
#include "common/clone/make_uptr_clone.h"
#include "exception/invalid_argument_exception.h"

using namespace boost::algorithm;
using namespace boost::program_options;
using namespace cath;
using namespace cath::common;
using namespace cath::opts;
using namespace std;

using boost::algorithm::is_any_of;
using boost::algorithm::join;
using boost::none;

/// The program's current version
const string misc_help_version_options_block::CATH_TOOLS_VERSION      ( CATH_TOOLS_GIT_VERSION );

/// The program's most recent update date
const string misc_help_version_options_block::CATH_TOOLS_VERSION_DATE ( CATH_TOOLS_GIT_DATE    );

/// \brief The option name for the help option
const string misc_help_version_options_block::PO_HELP   ( "help"    );

/// \brief The option name for the version option
const string misc_help_version_options_block::PO_VERSION( "version" );

/// \brief A standard do_clone method
///
/// This is a concrete definition of a virtual method that's pure in options_block
unique_ptr<options_block> misc_help_version_options_block::do_clone() const {
	return { make_uptr_clone( *this ) };
}

/// \brief Define this block's name (used as a header for the block in the usage)
///
/// This is a concrete definition of a virtual method that's pure in options_block
string misc_help_version_options_block::do_get_block_name() const {
	return "Miscellaneous";
}

/// \brief Add this block's options to the provided options_description
///
/// This is a concrete definition of a virtual method that's pure in options_block
void misc_help_version_options_block::do_add_visible_options_to_description(options_description &arg_desc ///< The options_description to which the options are added
                                                                            ) {
	arg_desc.add_options()
		((PO_HELP    + ",h").c_str(), bool_switch( &help    )->default_value( false ), "Output help message"        )
		((PO_VERSION + ",v").c_str(), bool_switch( &version )->default_value( false ), "Output version information" );
}

/// \brief Identify any conflicts that make the currently stored options invalid
///
/// This is a concrete definition of a virtual method that's pure in options_block
///
/// At present, this always accepts
opt_str misc_help_version_options_block::do_invalid_string() const {
	return none;
}

/// \brief Get the help flag
bool misc_help_version_options_block::get_help() const {
	return help;
}

/// \brief Get the version flag
bool misc_help_version_options_block::get_version() const {
	return version;
}

/// \brief Generate the help string
string misc_help_version_options_block::get_help_string(const options_description &arg_visible_program_options, ///< The full options_description of visible options
                                                        const string              &arg_help_message_prefix,     ///< The prefix to prepend to the output of the options_description
                                                        const string              &arg_help_message_suffix      ///< The suffix to append to the output of the options_description
                                                        ) const {
	ostringstream help_ss;
	help_ss << arg_help_message_prefix << endl;
	help_ss << arg_visible_program_options << endl;
	help_ss << arg_help_message_suffix;
	return help_ss.str();
}

/// \brief Generate the version string
string misc_help_version_options_block::get_version_string(const string &arg_program_name,       ///< The name of the program
                                                           const string &arg_program_description ///< A description of the program
                                                           ) const {
	ostringstream version_ss;
	version_ss << "============\n";
	version_ss << arg_program_name << " " << CATH_TOOLS_VERSION << " [" << CATH_TOOLS_VERSION_DATE << "]\n";
	version_ss << "============\n\n";
	version_ss << arg_program_description << "\n\n\n";
	version_ss << "Build\n";
	version_ss << "-----\n";
	// version_ss << "   " << BOOST_PLATFORM              << "\n";
	version_ss << "   " << __DATE__ << " " << __TIME__ << "\n";
	version_ss << "   " << BOOST_COMPILER              << "\n";
	version_ss << "   " << BOOST_STDLIB                << "\n";
	version_ss << "   Boost " << BOOST_LIB_VERSION           << "\n";
	return version_ss.str();
}
