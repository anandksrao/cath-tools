/// \file
/// \brief The clustmap_input_options_block class header

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

#ifndef _CATH_TOOLS_SOURCE_CLUSTER_OPTIONS_OPTIONS_BLOCK_CLUSTMAP_INPUT_OPTIONS_BLOCK_H
#define _CATH_TOOLS_SOURCE_CLUSTER_OPTIONS_OPTIONS_BLOCK_CLUSTMAP_INPUT_OPTIONS_BLOCK_H

#include "cluster/options/spec/clustmap_input_spec.hpp"
#include "options/options_block/options_block.hpp"

namespace cath {
	namespace clust {

		/// \brief Define an options_block for options specifying how cath-map-clusters should read the input
		class clustmap_input_options_block final : public opts::options_block {
		private:
			using super = opts::options_block;

			/// \brief The spec this options_block configures
			clustmap_input_spec the_spec;

			std::unique_ptr<opts::options_block> do_clone() const final;
			std::string do_get_block_name() const final;
			void do_add_visible_options_to_description(boost::program_options::options_description &,
			                                           const size_t &) final;
			void do_add_hidden_options_to_description(boost::program_options::options_description &,
			                                          const size_t &) final;
			str_opt do_invalid_string(const boost::program_options::variables_map &) const final;
			str_vec do_get_all_options_names() const final;

		public:
			static const std::string PO_WORKING_CLUSTMEMB_FILE;
			static const std::string PO_MAP_FROM_CLUSTMEMB_FILE;
			static const std::string PO_READ_BATCHES_FROM_INPUT;

			const clustmap_input_spec & get_clustmap_input_spec() const;
		};

	} // namespace clust
} // namespace cath

#endif
