/// \file
/// \brief The superfamily_of_domain class header

#ifndef SUPERFAMILY_OF_DOMAIN_H_INCLUDED
#define SUPERFAMILY_OF_DOMAIN_H_INCLUDED

#include <boost/filesystem/path.hpp>

#include "common/type_aliases.h"

#include <string>
#include <unordered_map>

namespace cath {
	namespace homcheck {

		namespace detail {
			bool is_valid_superfamily_id(const std::string &);
			std::string fold_of_superfamily_id(const std::string &);
		}

		/// \brief A lookup from domain_id to the superfamily in which that domain is currently classified
		///        (or will be classified after actions suggested by this code)
		class superfamily_of_domain final {
		private:
			/// \brief Hash from domain_id to superfamily ID
			///
			/// Pre-existing superfamily_ids satisfy meet is_valid_superfamily_id()
			/// New superfamily IDs are built from pre-existing ones and look like: 2.60.40.new_sf_in_fold_of_1cukA01
			std::unordered_map<std::string, std::string> sf_of_dom;

			static const std::string NEW_SF_CORE_STRING;

		public:
			superfamily_of_domain() = default;
			superfamily_of_domain(const str_str_pair_vec &);

			size_t size() const;

			bool is_in_new_superfamily(const std::string &) const;
			bool has_superfamily_of_domain(const std::string &) const;
			const std::string & get_superfamily_of_domain(const std::string &) const;

			void add_domain_in_new_sf_in_fold_of_domain(const std::string &,
			                                            const std::string &);

			// See lines ~600-700 in /usr/local/svn/source/cathcgi/trunk/update/HomCheck.pl
//			$oNewHClassification = $oDB->addChildNodeToClassificationNoCommit($oNewTClassification, $strNewHName, $strNewHComment);
//			$aDomainMessages     = $oDB->assignDomainNoCommit($oDomain, $oRelatedDomain,  $domainHistoryAssignmentEventId,  'DOMAIN_ASSIGNED', $strComment, $oNewHClassification, undef, $xmlDataStructure, my $preserveSolid = undef);
//			void add_(const std::string &);
		};

		superfamily_of_domain parse_superfamily_of_domain(std::istream &);
		superfamily_of_domain parse_superfamily_of_domain(const boost::filesystem::path &);
		superfamily_of_domain parse_superfamily_of_domain(const std::string &);

	}
}

#endif