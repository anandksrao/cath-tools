/// \file
/// \brief The alignment_scaffold header

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

#ifndef ALIGNMENT_SCAFFOLD_H_INCLUDED
#define ALIGNMENT_SCAFFOLD_H_INCLUDED

#include "alignment/align_type_aliases.h"
#include "common/type_aliases.h"

#include <iosfwd>

namespace cath { namespace align { class alignment; } }

namespace cath {
	namespace align {
		namespace detail {

			opt_aln_posn_vec alignment_entry_of_scaffold_string(const std::string &);
			std::string scaffold_line_of_alignment_entry(const alignment &,
			                                             const size_t &);

		}

		alignment alignment_of_scaffold_lines(const str_vec &);
		alignment alignment_of_scaffold(const std::string &);
		str_vec scaffold_lines_of_alignment(const alignment &);
		std::string scaffold_of_alignment(const alignment &);
	}
}

#endif