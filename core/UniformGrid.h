#pragma once

#include "Sequence.h"
#include "AABB.h"
#include "vector.h"
#include "Ray.h"

namespace badCore 
{
	class UniformGrid final
	{
		static constexpr std::size_t CELL_ADDATIVE = 4;
		using Cell = Sequence<int>;

	public:

		UniformGrid(const AABB& bounds, float cellWidth, float cellHeight);

		//empties out all cells, leaves capacity intact
		void clear()noexcept;

		// Inserts a box (AABB) into the uniform grid.
		// Each cell that the box overlaps will contain the `user_index`.
		// If the box is partially outside the grid, only the overlapping cells are filled.
		// If the box is fully outside the grid, no insertion occurs.
		void insert(int user_index, const AABB& box)noexcept;

		// convenience with iterators but does not protect against begin_index_naming being negative or something
		// user should know what index their begin really is. otherwise works with any input iterator, list, vec, custom... as long as dereferenced it is an AABB
		template<std::input_iterator InputIt>
			requires std::same_as<std::remove_cvref_t<std::iter_reference_t<InputIt>>, AABB>
		void insert(InputIt first, InputIt last, int begin_index_naming)noexcept
		{
			for (; first != last; ++first) {
				insert(begin_index_naming++, *first);
			}
		}

		// returns the structure itself
		const Sequence<Cell>& query_grid()const noexcept;

		// queries a region against the grid
		// returns indices to the cells the region intersects with
		void query_region(const AABB& region, Sequence<int>& results)const noexcept;

		// queries a point against the grid
		// returns cell index or -1 if the point is not in bounds of the grid
		int query_point(const float2& point)const noexcept;

		// queries the neighbors of a valid index. in debug does an assert but no protection at runtime
		// returns all neighboring indices, up to a maximum of 8
		void query_neighbors(int cellIndex, Sequence<int>& neighbors)const noexcept;

		// queries a line against the grid
		// IMPORTANT: if the length of [lineEnd - lineOrigin] is 0, it could still be inside the grid but it will NOT be queried
		// returns cell indices, not individual elements
		void query_ray(const Ray& ray, Sequence<int>& cell_indices)const noexcept;

		// sets the capacity of each cell to specified number
		// intended use: on a cleared grid (otherwise would delete elements unknown to user)
		void maintain_uniform_memory(std::size_t cell_capacity_target);

		// returns a specified cell, in debug does an assert, no protection at runtime
		const Cell& get_cell(std::size_t index)const noexcept;

		// returns the bounds of the grid
		const AABB& get_grid_bounds()const noexcept;

		// returns the width of a cell
		float get_cell_width()const noexcept;

		// returns the height of a cell
		float get_cell_height()const noexcept;

	private:
		Sequence<Cell> mCells;
		AABB mBounds;

		int mColumns;
		int mRows;

		float mCellWidth;
		float mCellHeight;

		//invCells are better than mCellWidth for math
		float invCellW;
		float invCellH;
	};
}