#pragma once

#include <vector>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "Stats.h"

#ifdef R_LIBS
#include <RcppArmadillo.h>
#endif

class Stats;

class CountMatrix
{
public:
	typedef std::vector<std::string> s_list_t;
	typedef std::vector<int> i_list_t;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int /* file_version */)
	{
		ar & cell_names & gene_names & counts;
	}

public:
	s_list_t cell_names;
	s_list_t gene_names;
	i_list_t counts;

	CountMatrix()
	{};

	CountMatrix(const s_list_t &cell_names, const s_list_t &gene_names, const i_list_t &counts)
			: cell_names(cell_names)
			, gene_names(gene_names)
			, counts(counts)
	{}
};

class IndropResult
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int /* file_version */)
	{
		ar & this->cm & this->ex_cells_chr_umis_counts & this->nonex_cells_chr_umis_counts &
		this->filtered_cells_chr_umis_counts & this->filtered_cells_chr_umis_counts & this->ex_cell_names &
		this->nonex_cell_names & this->chr_names & this->reads_per_umi & this->umig_covered & this->merge_n;
	}

public:
	CountMatrix cm;

	Stats::int_list_t ex_cells_chr_umis_counts;
	Stats::int_list_t nonex_cells_chr_umis_counts;
	Stats::int_list_t filtered_cells_chr_umis_counts;
	Stats::str_list_t ex_cell_names;
	Stats::str_list_t nonex_cell_names;
	Stats::str_list_t chr_names;

	std::vector<double> reads_per_umi;
	std::vector<int> umig_covered;
	std::vector<int> merge_n;

	IndropResult()
	{};

	IndropResult(const CountMatrix &cm, const Stats &stats, const std::vector<double> &reads_per_umi,
				 const std::vector<int> &umig_covered);

#ifdef R_LIBS
	Rcpp::List get_r_table(const std::string &fname) const;
#endif
};
