/// \file
/// \brief The overlap_score class definitions

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

#include "overlap_score.hpp"

#include <boost/logic/tribool.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/serialization/export.hpp>

#include "alignment/alignment.hpp"
#include "alignment/common_atom_selection_policy/common_atom_select_ca_policy.hpp"
#include "alignment/common_residue_selection_policy/common_residue_select_all_policy.hpp"
#include "common/clone/make_uptr_clone.hpp"
#include "common/less_than_helper.hpp"
#include "structure/geometry/coord.hpp"
#include "structure/geometry/coord_list.hpp"

#include <iostream> // ***** TEMPORARY *****

using namespace cath::align;
using namespace cath::common;
using namespace cath::score;
using namespace std;
