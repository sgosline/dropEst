#include <Tools/UtilFunctions.h>
#include "PoissonSimpleMergeStrategy.h"

namespace Estimation
{
namespace Merge
{
PoissonSimpleMergeStrategy::PoissonSimpleMergeStrategy(const boost::property_tree::ptree &config)
		: SimpleMergeStrategy(config)
{}

long PoissonSimpleMergeStrategy::get_merge_target(const Estimation::CellsDataContainer &container, size_t base_cell_ind) const
{
	u_u_hash_t cells_with_common_umigs = this->get_cells_with_common_umigs(container, base_cell_ind);

	ul_list_t neighbour_cells;
	neighbour_cells.reserve(cells_with_common_umigs.size());
	for (auto const &cell : cells_with_common_umigs)
	{
		if (Tools::edit_distance(container.cell_barcode(base_cell_ind).c_str(),
								 container.cell_barcode(cell.first).c_str()) > this->_max_merge_edit_distance)
			continue;

		neighbour_cells.push_back(cell.first);
	}

	if (neighbour_cells.size() == 0)
		return base_cell_ind;

	long target = this->_target_estimator.get_best_merge_target(container, base_cell_ind, neighbour_cells);
	if (target != -1)
		return target;

	return base_cell_ind;
}

void PoissonSimpleMergeStrategy::init(const CellsDataContainer &container)
{
	SimpleMergeStrategy::init(container);
	this->_target_estimator.init(container);
}

void PoissonSimpleMergeStrategy::release()
{
	this->_target_estimator.release();
	SimpleMergeStrategy::release();
}

std::string PoissonSimpleMergeStrategy::merge_type() const
{
	return "Poisson Simple";
}
}
}