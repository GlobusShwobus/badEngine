#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Rect.h"
#include "Ray.h"

namespace badCore 
{
	/**
	* \brief
	* Represents a spatial grid with uniform cell sizes.
	* 
	* Architecturally is literally just a array of arrays.
	* 
	* Stores integers for maximum efficienty. The integers represent the indices of actual objects where ever they may be stored.
	* 
	* IMPORTANT:
	* 
	* The grid is intended to be reused. The grid should be filled before any geometric logic is done. The potential colliders should
	* be collected after which the structure becomes invalid. It may be cleared.
	* 
	* After a 'warmup', once each cell alocates enough memory, the rebuilding of the grid becomes cheap. Cheaper than any readjustment
	* logic would buy in terms of performance. It is a very cache friendly structure and easy to use supporting just insertion and query methods.
	* 
	* NOTE: Query region gives back the indices of all elements of cells, while other query methods just give back the indices of cells.
	*/
	class UniformGrid final
	{
		static constexpr std::size_t CELL_ADDATIVE = 4;
		using Cell = Sequence<int>;

	public:

		/**
		* Builds the uniform grid with given bounds and sizes for cells
		* 
		* Each cells gets an initial boost to their allocation power but does not reserve memory.
		* 
		* \param bounds the world bounds to be divided with cell dimension
		* \param cellwidth width of each cell in the grid
		* \param cellheight height of each cell in the grid
		* 
		* \throw in DEBUG asserts logical values for some fields.
		*/
		UniformGrid(const Rect& bounds, float cellWidth, float cellHeight);

		/// <summary> empties out all cells, leaves capacity intact </summary>
		void clear()noexcept;

		/**
		* Inserts an element representation as int into the grid using a rect as a measurement of its size.
		* 
		* Depending on the size of the box, the element may be inserted into multiple cells.
		* 
		* \param user_index representation of the element.
		* \param box the size of the represntative element.
		*/
		void insert(int user_index, const Rect& box);

		/**
		* Range based insert. Internally just calls insert on each element so it might not be 
		* worth it making pairs of ints and rects and just manually looping your structure instead and calling insert.
		* 
		* \param first forward iterator to the first element 
		* \param last forward iterator to the last element (excluded)
		*/
		template<std::forward_iterator FwdIt>
			requires std::same_as<std::remove_cvref_t<std::iter_reference_t<FwdIt>>, std::pair<int, Rect>>
		void insert(FwdIt first, FwdIt last)
		{
			for (; first != last; ++first) {
				const auto& [index, rect] = *first;
				insert(index, rect);
			}
		}

		/// <returns> Exposes the grid structure itself as const for N^2 iteration or any other read reason... </returns>
		
		/**
		* Exposes the grid structure itself as const for N^2 iteration or any other read reason...
		* \returns an array of cells.
		*/
		inline const Sequence<Cell>& query_grid()const noexcept { return mCells; }

		/**
		* Queries a region against the grid.
		* 
		* Fills the given sequence with ALL indices of a cell that intersects with the region. This means even though the cell intersected
		* the obejct the index represents may not. It also could include duplicates but tests showed duplication is cheaper than a complex grid.
		* (cache friendliness!)
		* 
		* \param region area to be queries
		* \param results reference array that is filled with indices of elements per intersected cell
		*/
		void query_region(const Rect& region, Sequence<int>& results)const;

		/**
		* Queries a point against the grid giving back the index of a cell that contains the point.
		* 
		* \param point the point to look for
		* \returns index of the cell that contained the point.
		*/
		int query_point(const Point& point)const noexcept;

		/**
		* Queries an index of the cell and returns back all of the (valid) neighboring indices of cells.
		* 
		* \param cellIndex the index of the cell to address.
		* \param neighbors reference array that is filled with cell indices
		* \throws in DEBUG asserts that the cellIndex is valid otherwise noexcept
		*/
		void query_neighbors(int cellIndex, Sequence<int>& neighbors)const;

		/**
		* Queries a ray, or a line segment specifically, against the grid returning all cell indices intersecting with the ray.
		* 
		* \param ray the line segment to query against
		* \param cell_indices reference array that is filled with indices of cells
		*/
		void query_ray(const Ray& ray, Sequence<int>& cell_indices)const;

		// sets the capacity of each cell to specified number
		// intended use: on a cleared grid (otherwise would delete elements unknown to user)

		/**
		* Calls Sequence::reserve on each cell in the grid.
		* 
		* If a cell has more memory than capacity reserve will do nothing.
		* 
		* \param capacity to reserve per cell
		* 
		* \note misleading function name as a relic of older logic
		*/
		void maintain_uniform_memory(std::size_t capacity);

		// returns a specified cell, in debug does an assert, no protection at runtime

		/**
		* Exposes a cell for read access.
		* 
		* If the user provides a bad out of range index, it is UB.
		* 
		* \param index the index of the cell
		* \returns cell
		* 
		* \throw in DEBUG asserts valid size; otherwise noexcept or UB.
		*/
		const Cell& get_cell(std::size_t index)const noexcept
		{
			assert(mCells.size() > index);
			return mCells[index];
		}

		/**
		* \returns returns the bounds of the grid
		*/
		const Rect& get_grid_bounds()const noexcept { return mBounds; }

		/**
		* \returns returns the width of each cell
		*/
		float get_cell_width()const noexcept { return mCellWidth; }

		/**
		* \returns returns the height of each cell
		*/
		float get_cell_height()const noexcept { return mCellHeight; }

	private:
		class Sequence<Cell> mCells;
		class Rect mBounds;

		int mColumns;
		int mRows;

		float mCellWidth;
		float mCellHeight;

		//invCells are better than mCellWidth for math
		float invCellW;
		float invCellH;
	};
}