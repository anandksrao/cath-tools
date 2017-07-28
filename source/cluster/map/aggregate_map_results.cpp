/// \map
/// \brief The aggregate_map_results class definitions

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

#include "aggregate_map_results.hpp"

#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "cluster/map/map_results.hpp"
#include "cluster/new_cluster_data.hpp"
#include "cluster/old_cluster_data.hpp"
#include "common/file/open_fstream.hpp"
#include "exception/invalid_argument_exception.hpp"

#include <fstream>

using namespace cath::clust;
using namespace cath::common;

using boost::filesystem::path;
using boost::format;
using boost::numeric_cast;
using std::ofstream;
using std::string;

/// \brief Ctor from a clust_mapping_spec under which the mappings have been / will be performed
aggregate_map_results::aggregate_map_results(const clust_mapping_spec &arg_clust_mapping_spec ///< The clust_mapping_spec under which the mappings have been / will be performed
                                             ) noexcept : the_spec { arg_clust_mapping_spec } {
}

/// \brief Getter for whether this has had results added to it
const bool & aggregate_map_results::get_added_to() const {
	return added_to;
}

/// \brief Getter for the total number of old clusters encountered in the mapping
const size_t & aggregate_map_results::get_num_old_clusters() const {
	return num_old_clusters;
}

/// \brief Getter for the total number of new clusters encountered in the mapping
const size_t & aggregate_map_results::get_num_new_clusters() const {
	return num_new_clusters;
}

/// \brief Getter for the total number of clusters (on either side) that got mapped
const size_t & aggregate_map_results::get_num_mapped_clusters() const {
	return num_mapped_clusters;
}

/// \brief Getter for the total number of old entries encountered in the mapping
const size_t & aggregate_map_results::get_num_old_entries() const {
	return num_old_entries;
}

/// \brief Getter for the total number of new entries encountered in the mapping
const size_t & aggregate_map_results::get_num_new_entries() const {
	return num_new_entries;
}

/// \brief Getter for the total number of entries on either side that got mapped
const size_t & aggregate_map_results::get_num_mapped_entries() const {
	return num_mapped_entries;
}

/// \brief Getter for the specification that was used to perform the mappings
const clust_mapping_spec & aggregate_map_results::get_clust_mapping_spec() const {
	return the_spec;
}

/// \brief Add the results from a new mapping
aggregate_map_results & aggregate_map_results::add_map_results(const map_results      &arg_map_results,  ///< The results to add
                                                               const old_cluster_data &arg_old_clusters, ///< The old clusters that were mapped from
                                                               const new_cluster_data &arg_new_clusters  ///< The new clusters that were mapped to
                                                               ) {
	if ( arg_map_results.the_spec != the_spec ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Cannot add map_results that were performed under a different clust_mapping_spec to an aggregate_map_results"));
	}

	const size_t  curr_num_mapped_clusters       = arg_map_results.chosen_maps.size();
	const size_t  curr_num_old_clusters          = get_num_clusters      ( arg_old_clusters );
	const size_t  curr_num_new_clusters          = get_num_clusters      ( arg_new_clusters );
	const size_t  curr_num_old_entries           = get_num_entries       ( arg_old_clusters );
	const size_t  curr_num_new_entries           = get_num_entries       ( arg_new_clusters );
	const size_t  curr_num_mapped_entries        = ::cath::clust::get_num_mapped_entries( arg_map_results  );

	if ( curr_num_mapped_clusters > curr_num_old_clusters || curr_num_mapped_clusters > curr_num_new_clusters ) {
		BOOST_THROW_EXCEPTION(out_of_range_exception("Cannot have mapped more clusters than there are old/new clusters"));
	}

	num_old_clusters         += curr_num_old_clusters;
	num_new_clusters         += curr_num_new_clusters;
	num_mapped_clusters      += curr_num_mapped_clusters;
	num_old_entries          += curr_num_old_entries;
	num_new_entries          += curr_num_new_entries;
	num_mapped_entries       += curr_num_mapped_entries;
	added_to                  = true;

	return *this;
}

/// \brief Make a aggregate_map_results containing one entry, the mapping between the specified clusters with the specified results
///
/// \relates aggregate_map_results
aggregate_map_results cath::clust::make_aggregate_map_results(const map_results      &arg_map_results,      ///< The results with which the aggregate_map_results should be initialised
                                                              const old_cluster_data &arg_old_cluster_data, ///< The old clusters that were mapped from
                                                              const new_cluster_data &arg_new_cluster_data  ///< The new clusters that were mapped to
                                                              ) {
	aggregate_map_results result{ arg_map_results.the_spec };
	result.add_map_results( arg_map_results, arg_old_cluster_data, arg_new_cluster_data );
	return result;
}

/// \brief Generate a Markdown summary of the specified map_results
///
/// \relates aggregate_map_results
string cath::clust::markdown_summary_string(const aggregate_map_results &arg_aggregate_map_results ///< The map_results to summarise
                                            ) {
	if ( ! arg_aggregate_map_results.get_added_to() ) {
		return "No mapping was performed";
	}
	const size_t &num_old_clusters          = arg_aggregate_map_results.get_num_old_clusters();
	const size_t &num_new_clusters          = arg_aggregate_map_results.get_num_new_clusters();
	const size_t &num_mapped_clusters       = arg_aggregate_map_results.get_num_mapped_clusters();
	const size_t  num_unmapped_old_clusters = num_old_clusters - num_mapped_clusters;
	const size_t  num_unmapped_new_clusters = num_new_clusters - num_mapped_clusters;

	const size_t &num_old_entries           = arg_aggregate_map_results.get_num_old_entries();
	const size_t &num_new_entries           = arg_aggregate_map_results.get_num_new_entries();
	const size_t &num_mapped_entries        = arg_aggregate_map_results.get_num_mapped_entries();
	const size_t  num_unmapped_old_entries  = num_old_entries - num_mapped_entries;
	const size_t  num_unmapped_new_entries  = num_new_entries - num_mapped_entries;

	const double  pc_old_clusters_mapped    = 100.0 * numeric_cast<double>( num_mapped_clusters       ) / numeric_cast<double>( num_old_clusters );
	const double  pc_new_clusters_mapped    = 100.0 * numeric_cast<double>( num_mapped_clusters       ) / numeric_cast<double>( num_new_clusters );
	const double  pc_old_clusters_unmapped  = 100.0 * numeric_cast<double>( num_unmapped_old_clusters ) / numeric_cast<double>( num_old_clusters );
	const double  pc_new_clusters_unmapped  = 100.0 * numeric_cast<double>( num_unmapped_new_clusters ) / numeric_cast<double>( num_new_clusters );

	const double  pc_old_entries_mapped     = 100.0 * numeric_cast<double>( num_mapped_entries        ) / numeric_cast<double>( num_old_entries  );
	const double  pc_new_entries_mapped     = 100.0 * numeric_cast<double>( num_mapped_entries        ) / numeric_cast<double>( num_new_entries  );

	const double  pc_old_entries_unmapped   = 100.0 * numeric_cast<double>( num_unmapped_old_entries  ) / numeric_cast<double>( num_old_entries  );
	const double  pc_new_entries_unmapped   = 100.0 * numeric_cast<double>( num_unmapped_new_entries  ) / numeric_cast<double>( num_new_entries  );

	const double  min_equiv_dom_ol_pc       = 100.0 * arg_aggregate_map_results.get_clust_mapping_spec().get_min_equiv_dom_ol();
	const double  min_equiv_clust_ol_pc     = 100.0 * arg_aggregate_map_results.get_clust_mapping_spec().get_min_equiv_clust_ol();

	const string  dom_pc_str                = ( format( "%.1f" ) % min_equiv_dom_ol_pc ).str();

	return R"(Domain Mapping
==

This section describes how well the map-from domains could be mapped to new domains (and vice versa).
The quality of a mapping between a pair of domains is defined as the percentage overlap over the longer
domain (ie the percentage of the longer domain's residues shared with the other domain).
In this run, the cut-off for defining domain-equivalence was )" + dom_pc_str + R"(%.


Domains from Map-From Clusters
--

| Category                                                         | Number | Percentage |
|------------------------------------------------------------------|--------|------------|
| All                                                                                                |)" + ( format( "%6d" ) % num_old_entries          ).str() + R"( |     100.0% |
| &nbsp; ...of which:                                                                                |        |            |
| &nbsp; &bull; Equivalence-mapped     (ie )" + dom_pc_str + R"( < overlap                         ) |)" + ( format( "%6d" ) % num_mapped_entries       ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_old_entries_mapped   ).str() ) + R"(% |
| &nbsp; &bull; Not equivalence-mapped (ie                         overlap ≤ )" + dom_pc_str + R"( ) |)" + ( format( "%6d" ) % num_unmapped_old_entries ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_old_entries_unmapped ).str() ) + R"(% |

Domains from New Clusters
--

| Category                                                        | Number | Percentage |
|-----------------------------------------------------------------|--------|------------|
| All                                                                             |)" + ( format( "%6d" ) % num_new_entries          ).str() + R"( |     100.0% |
| &nbsp; ...of which:                                                             |        |            |
| &nbsp; &bull; Equivalence-mapped     (ie overlap > )" + dom_pc_str + R"(      ) |)" + ( format( "%6d" ) % num_mapped_entries       ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_new_entries_mapped   ).str() ) + R"(% |
| &nbsp; &bull; Not equivalence-mapped (ie overlap ≤ )" + dom_pc_str + R"(      ) |)" + ( format( "%6d" ) % num_unmapped_new_entries ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_new_entries_unmapped ).str() ) + R"(% |

Cluster Mapping
==

This section describes how well the old clusters could be mapped to the new clusters (and vice versa).
The quality of a mapping between a pair of clusters is defined as the percentage of the domains in
the map-from cluster that have an equivalent domain in the new cluster.
In this run, the cutoff for defining cluster-equivalence was )" + ( format( "%.1f" ) % min_equiv_clust_ol_pc ).str() + R"(%.

Map-From Clusters
--

| Category                             | Number | Percentage |
|--------------------------------------|--------|------------|
| All                                  |)" + ( format( "%6d" ) % num_old_clusters          ).str() + R"( |     100.0% |
| &nbsp; ...of which:                  |        |            |
| &nbsp; &bull; Equivalence-mapped     |)" + ( format( "%6d" ) % num_mapped_clusters       ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_old_clusters_mapped   ).str() ) + R"(% |
| &nbsp; &bull; Not equivalence-mapped |)" + ( format( "%6d" ) % num_unmapped_old_clusters ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_old_clusters_unmapped ).str() ) + R"(% |

New Clusters
--

| Category                             | Number | Percentage |
|--------------------------------------|--------|------------|
| All                                  |)" + ( format( "%6d" ) % num_new_clusters          ).str() + R"( |     100.0% |
| &nbsp; ...of which:                  |        |            |
| &nbsp; &bull; Equivalence-mapped     |)" + ( format( "%6d" ) % num_mapped_clusters       ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_new_clusters_mapped   ).str() ) + R"(% |
| &nbsp; &bull; Not equivalence-mapped |)" + ( format( "%6d" ) % num_unmapped_new_clusters ).str() + R"( | )" + ( ( format( "%5.1f" ) % pc_new_clusters_unmapped ).str() ) + R"(% |
)";
}

/// TODO: Consider splitting "Domains from Map-From Clusters" numbers entry:
///  * Not equivalence-mapped (ie     overlap ≤ X )
/// into:
///  * Insufficiently-mapped  (ie 0 < overlap ≤ X )
///  * Completely-unmapped    (ie     overlap = 0 )

/// TODO: Consider adding the below text
///
/// This should be done by storing the best mapping percentage achieved by mapped-from domain
/// and then aggregating (if necessary) across multiple mappings. To aggregate,
/// it might be best to do something like store counts for scores *rounded to (say) three decimal places*
/// in an unordered_map<double, size_t>
/*
Distribution of Domain Mapping Percentages for Domains from Map-From Clusters
--

Excluding completely-unmapped domains:

| Percentile through distribution of mapping percentages | Mapping Percentage |
|--------------------------------------------------------|--------------------|
|                                                     25 |                30% |
|                                                     50 |                40% |
|                                                     75 |                50% |
|                                                     90 |                60% |
|                                                     95 |                70% |
|                                                     98 |                80% |
|                                                     99 |                90% |
|                                                    100 |                90% |

Including completely unmapped domains:

| Percentile through distribution of mapping percentages | Mapping Percentage |
|--------------------------------------------------------|--------------------|
|                                                     25 |                30% |
|                                                     50 |                40% |
|                                                     75 |                50% |
|                                                     90 |                60% |
|                                                     95 |                70% |
|                                                     98 |                80% |
|                                                     99 |                90% |
|                                                    100 |                90% |
*/

/// TODO: Consider adding:
/*
Distribution of Cluster-mapping Percentages for Map-From Clusters
--

Excluding completely-unmapped clusters:

**this needs tweaking**

**in particular, it should be clear how multiple candidate old-froms to the same new-to are resolved**

| Percentile through distribution of mapping percentages | Mapping Percentage |
|--------------------------------------------------------|--------------------|
|                                                     25 |                30% |
|                                                     50 |                40% |
|                                                     75 |                50% |
|                                                     90 |                60% |
|                                                     95 |                70% |
|                                                     98 |                80% |
|                                                     99 |                90% |
|                                                    100 |                90% |

Including completely unmapped clusters:

| Percentile through distribution of mapping percentages | Mapping Percentage |
|--------------------------------------------------------|--------------------|
|                                                     25 |                30% |
|                                                     50 |                40% |
|                                                     75 |                50% |
|                                                     90 |                60% |
|                                                     95 |                70% |
|                                                     98 |                80% |
|                                                     99 |                90% |
|                                                    100 |                90% |
*/

/// \brief Write a Markdown summary of the specified aggregate_map_results to the specified file
///
/// \relates aggregate_map_results
void cath::clust::write_markdown_summary_string_to_file(const path                  &arg_output_file,          ///< The file to which the Markdown summary should be written
                                                        const aggregate_map_results &arg_aggregate_map_results ///< The aggregate_map_results to summarise
                                                        ) {
	ofstream md_ostream;
	open_ofstream( md_ostream, arg_output_file );
	md_ostream << markdown_summary_string( arg_aggregate_map_results );
	md_ostream.close();
}