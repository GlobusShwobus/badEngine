#pragma once

#include "Sequence.h"
#include "Rectangle.h"
#include "Ray.h"


//TODO asserts for constructor
//TODO:: check if checking validity is better than clamps in query_region

namespace badEngine {


	static constexpr std::size_t CELL_ADDATIVE = 3;
	class UniformGrid {

		using Cell = Sequence<int>;

	public:

		UniformGrid(const AABB& bounds, float cellWidth, float cellHeight)
			:mBounds(bounds), mCellWidth(cellWidth), mCellHeight(cellHeight)
		{
			mColumns = bounds.w / cellWidth;
			mRows = bounds.h / cellHeight;

			invCellW = 1.0f / mCellWidth;
			invCellH = 1.0f / mCellHeight;

			mCells.resize(mColumns * mRows);

			for (auto& cell : mCells) {
				//cell.set_capacity(10);
				cell.set_additive(CELL_ADDATIVE);//actually pretty good in this scenario, massive help for early inserts while not doing potentially wasteful allocations
			}
		}

		//empties out all cells, leaves capacity intact
		void clear()noexcept {
			for (auto& cell : mCells) {
				cell.clear();
			}
		}

		//insert does not check if box top left is within the range of the grid
		//if x or y aren't in range they will be inserted to the clamped index at x, y or both x and y axis
		void insert(int user_index, const AABB& box)noexcept
		{
			const auto g4 = grid_range(box);
			for (int y = g4.miny; y < g4.maxy; ++y) {
				const int offset = y * mColumns;//cache multiplication, minor thing...
				for (int x = g4.minx; x < g4.maxx; ++x) {
					mCells[static_cast<std::size_t>(offset + x)].emplace_back(user_index);
				}
			}
		}
		template<std::input_iterator InputIt>
			requires std::same_as<std::remove_cvref_t<std::iter_reference_t<InputIt>>, AABB>
		void insert(InputIt first, InputIt last, int begin_index_naming) {
			for (; first != last; ++first) {
				insert(begin_index_naming++, *first);
			}
		}
		//returns all potential collision candidates, includes duplicates
		//doing basic intersecion tests with duplicates should be always be better than cache misses, otherwise sorting is left to the user
		void query_pairs(Sequence<std::pair<int, int>>& pairs)noexcept {
			//for every cell
			for (const auto& cell : mCells) {
				//collect all potential collisions
				for (std::size_t a = 0; a < cell.size(); ++a) {
					for (std::size_t b = a + 1; b < cell.size(); ++b) {
						pairs.emplace_back(cell[a], cell[b]);
					}
				}
			}
		}
		//collects all elements within the region
		//the rectangle can be partially intersecting the bounds of the grid but returns no results if the region is fully outside
		void query_region(const AABB& region, Sequence<int>& results)noexcept {
			const auto g4 = grid_range(region);

			for (int y = g4.miny; y < g4.maxy; ++y) {
				const int offset = y * mColumns;
				for (int x = g4.minx; x < g4.maxx; ++x) {
					for (int id : mCells[static_cast<std::size_t>(offset + x)]) {
						results.emplace_back(id);
					}
				}
			}
		}
		//converts a point to a cell index, returns -1 if point is outside of the grid
		inline int query_point(const float2& point)const noexcept {
			int x = static_cast<int>((point.x - mBounds.x) * invCellW);
			int y = static_cast<int>((point.y - mBounds.y) * invCellH);

			if (x < 0 || x >= mColumns || y < 0 || y >= mRows) {
				return -1;
			}

			return y * mColumns + x;
		}
		void query_ray(const Ray& ray, Sequence<int>& cell_indices)const noexcept
		{
			//1) check if the ray intersects with the grid at all
			Hit hit;
			sweep(ray, mBounds, hit);
			if (hit.t < 0)
				return; //no hit

			//2) ray starting cell (with epsilon because a ray can be exactly on the edge and move on an infite plane, which is bad because it would hit a number of cells but never register)
			float2 entry = ray.origin + ray.dir * (hit.t + 0.0001f);

			//3) Calculate starting cell
			int cellX = static_cast<int>((entry.x - mBounds.x) * invCellW);
			int cellY = static_cast<int>((entry.y - mBounds.y) * invCellH);

			cellX = bad_clamp(cellX, 0, mColumns - 1);
			cellY = bad_clamp(cellY, 0, mRows - 1);
			//4) get the sign of a step
			const float2 step = sign_vector(ray.dir);
			//5) compute the distance to cross one cell in x and y
			const float2 delta = float2(
				(step.x == 0.0f) ? INFINITY : std::fabs(mCellWidth / ray.dir.x),
				(step.y == 0.0f) ? INFINITY : std::fabs(mCellHeight / ray.dir.y)
			);
			//6) compute the inital tMax, here if maxX or maxY is infinity, we can just always move 1 axis and never the other
			float nextBoundaryX = mBounds.x + (step.x > 0 ? (cellX + 1) * mCellWidth : cellX * mCellWidth);
			float nextBoundaryY = mBounds.y + (step.y > 0 ? (cellY + 1) * mCellHeight : cellY * mCellHeight);

			float tMaxX = (step.x == 0.0f) ? INFINITY : (nextBoundaryX - ray.origin.x) / ray.dir.x;
			float tMaxY = (step.y == 0.0f) ? INFINITY : (nextBoundaryY - ray.origin.y) / ray.dir.y;

			//7) traverse the grid
			int currX = cellX;
			int currY = cellY;
			while (currX >= 0 && currX < mColumns && currY >= 0 && currY < mRows) {
				//get the cell index
				cell_indices.emplace_back(currY * mColumns + currX);

				//next step
				if (tMaxX < tMaxY) {
					currX += step.x;
					tMaxX += delta.x;
				}
				else {
					currY += step.y;
					tMaxY += delta.y;
				}
			}
		}
		void query_ray2(const float2& lineOrigin, const float2& lineEnd,  Sequence<int>& cell_indices)const noexcept
		{
			//1) compute ray direction and segment length, if segment legth is 0 then there is no ray, could still mean a point intersection though...
			const float2 direction = lineEnd - lineOrigin;
			const float segmentLength = length_vector(direction);
			
			if (segmentLength == 0.0f)
				return;
			//2) check if the ray intersects with the grid at all, if it doesn't then there is nothing to do
			Ray ray(lineOrigin, direction / segmentLength);
			Hit hit;
			sweep(ray, mBounds, hit);
			if (hit.t < 0)
				return;
			//3) early exit if intersection point is beyond segment length
			if (hit.t > segmentLength)
				return;

			//4) ray starting cell (with epsilon because a ray can be exactly on the edge and move on an infite plane, which is bad because it would hit a number of cells but never register)
			float2 entry = ray.origin + ray.dir * (hit.t + 0.0001f);

			//5) Calculate starting cell and clamp it to corner (float gets rounded down to int)
			int cellX = static_cast<int>((entry.x - mBounds.x) * invCellW);
			int cellY = static_cast<int>((entry.y - mBounds.y) * invCellH);
			cellX = bad_clamp(cellX, 0, mColumns - 1);
			cellY = bad_clamp(cellY, 0, mRows - 1);
			//6) get the sign of a step
			const float2 step = sign_vector(ray.dir);
			//7) compute the distance to cross one cell in x and y
			const float2 delta = float2(
				(step.x == 0.0f) ? INFINITY : std::fabs(mCellWidth / ray.dir.x),
				(step.y == 0.0f) ? INFINITY : std::fabs(mCellHeight / ray.dir.y)
			);
			//8) compute the inital tMax, here if maxX or maxY is infinity, we can just always move 1 axis and never the other
			float nextBoundaryX = mBounds.x + (step.x > 0 ? (cellX + 1) * mCellWidth : cellX * mCellWidth);
			float nextBoundaryY = mBounds.y + (step.y > 0 ? (cellY + 1) * mCellHeight : cellY * mCellHeight);

			float tMaxX = (step.x == 0.0f) ? INFINITY : (nextBoundaryX - ray.origin.x) / ray.dir.x;
			float tMaxY = (step.y == 0.0f) ? INFINITY : (nextBoundaryY - ray.origin.y) / ray.dir.y;

			//9) current distance traveled along the ray, start at hit.t (entry point) + epsilon
			float currentT = hit.t + 0.0001f;

			//10) traverse the grid with segment length limit
			int currX = cellX;
			int currY = cellY;
			while (
				currX >= 0 && currX < mColumns && 
				currY >= 0 && currY < mRows &&
				currentT <= segmentLength
				) {
				//get the cell index
				cell_indices.emplace_back(currY * mColumns + currX);

				//next step
				if (tMaxX < tMaxY) {
					currentT = tMaxX;//update the next intersection point
					currX += static_cast<int>(step.x);
					tMaxX += delta.x;
				}
				else {
					currentT = tMaxY;//update the next intersection point
					currY += static_cast<int>(step.y);
					tMaxY += delta.y;
				}
			}
		}
		void query_ray3(const float2& lineOrigin, const float2& lineEnd, Sequence<int>& cell_indices)const noexcept
		{
			constexpr float EPS = 1e-4f;
			//1) compute ray direction and segment length, if segment legth is 0 then there is no ray, could still mean a point intersection though...
			const float2 dir = lineEnd - lineOrigin;
			const float segmentLength = length_vector(dir);

			if (segmentLength == 0.0f)
				return;
			//2) check if the origin is inside grid bounds
			Ray ray(lineOrigin, dir / segmentLength);
			const AABB& bounds = mBounds;
			bool originInside =
				ray.origin.x >= bounds.x &&
				ray.origin.x < bounds.x + bounds.w &&
				ray.origin.y >= bounds.y &&
				ray.origin.y < bounds.y + bounds.h;
			//3) compute entry and exit T
			float entryT = 0.0f;
			float exitT = segmentLength;
			if (!originInside) {
				Hit hit;
				sweep(ray, bounds, hit);
				//no intersection at all
				if (hit.t == INFINITY)
					return;

				//entry beyond segment
				if (hit.t > segmentLength)
					return;
				entryT = hit.t;
			}
			//4) entry point (nudged inside)
			float currentT = entryT + EPS;
			if (currentT > segmentLength)
				return;
			float2 entryPos = ray.origin + ray.dir * currentT;
			//5) Calculate starting cell and clamp it to corner (float gets rounded down to int)
			int cellX = static_cast<int>((entryPos.x - bounds.x) * invCellW);
			int cellY = static_cast<int>((entryPos.y - bounds.y) * invCellH);
			cellX = bad_clamp(cellX, 0, mColumns - 1);
			cellY = bad_clamp(cellY, 0, mRows - 1);
			//6) get the sign of a step
			float2 step = sign_vector(ray.dir);
			//7) compute the distance to cross one cell in x and y
			const float2 delta = float2(
				(step.x == 0.0f) ? INFINITY : std::fabs(mCellWidth / ray.dir.x),
				(step.y == 0.0f) ? INFINITY : std::fabs(mCellHeight / ray.dir.y)
			);
			//8) compute the inital tMax, here if maxX or maxY is infinity, we can just always move 1 axis and never the other
			float nextBoundaryX = mBounds.x + (step.x > 0 ? (cellX + 1) * mCellWidth : cellX * mCellWidth);
			float nextBoundaryY = mBounds.y + (step.y > 0 ? (cellY + 1) * mCellHeight : cellY * mCellHeight);

			float tMaxX = (step.x == 0.0f) ? INFINITY : (nextBoundaryX - ray.origin.x) / ray.dir.x;
			float tMaxY = (step.y == 0.0f) ? INFINITY : (nextBoundaryY - ray.origin.y) / ray.dir.y;

			//9) Traverse grid
			while (
				cellX >= 0 && cellX < mColumns &&
				cellY >= 0 && cellY < mRows &&
				currentT <= segmentLength
				)
			{
				cell_indices.emplace_back(cellY * mColumns + cellX);

				// Step to next cell
				if (tMaxX < tMaxY)
				{
					currentT = tMaxX;
					tMaxX += delta.x;
					cellX += step.x;
				}
				else if (tMaxY < tMaxX)
				{
					currentT = tMaxY;
					tMaxY += delta.y;
					cellY += step.y;
				}
				else // Corner crossing
				{
					currentT = tMaxX;
					tMaxX += delta.x;
					tMaxY += delta.y;
					cellX += step.x;
					cellY += step.y;
				}
			}
		}
		//if this function is called on a populated grid, it will remove elemnts
		//intended usage: call it periodically IF there are a lot of moving objects on a cleared out grid
		void maintain_uniform_memory(std::size_t cell_capacity_target) {

		}

		//returns the structure itself
		const Sequence<Cell>& get_grid()const noexcept {
			return mCells;
		}
		const Cell& get_cell(std::size_t index)const noexcept {
			assert(mCells.size() > index);
			return mCells[index];
		}
		const AABB& get_grid_bounds()noexcept {
			return mBounds;
		}
		float get_cell_width()const noexcept {
			return mCellWidth;
		}
		float get_cell_height()const noexcept {
			return mCellHeight;
		}




		//get how much elements are actually stored
		//keep in mind an object can overlap multiple cells
		std::size_t debug_elements_count()const {
			std::size_t counter = 0;
			for (auto& cell : mCells) {
				counter += cell.size();
			}
			return counter;
		}
	private:

		struct GridInt4 {
			int minx;
			int miny;
			int maxx;
			int maxy;
		};

		inline GridInt4 grid_range(const AABB& box)const noexcept
		{
			GridInt4 g4;
			//NOTE: the reason for std::ceil is because we must include partially overlapping cells
			g4.minx = static_cast<int>((box.x - mBounds.x) * invCellW);					     //left edge (round down)
			g4.miny = static_cast<int>((box.y - mBounds.y) * invCellH);					     //top edge (round down)
			g4.maxx = static_cast<int>(std::ceil((box.x + box.w - mBounds.x) * invCellW));	 //right edge (round up)
			g4.maxy = static_cast<int>(std::ceil((box.y + box.h - mBounds.y) * invCellH));	 //bottom edge (round up)

			//NOTE: the reason for clamp high difference is because we do 0 based indexing AND the loop is exclusionary (using < instead of <=)
			g4.minx = bad_clamp(g4.minx, 0, mColumns - 1);	    //clamp x to left edge if negative, to right if wider than width, or keep
			g4.miny = bad_clamp(g4.miny, 0, mRows - 1);		    //clamp y to top edge if negative, to bottom if deeper than height, or keep
			g4.maxx = bad_clamp(g4.maxx, 0, mColumns);			//clamp x to left edge if negative, to right if wider than width, or keep
			g4.maxy = bad_clamp(g4.maxy, 0, mRows);				//clamp y to top edge if negative, to bottom if deeper than height, or keep
			return g4;
		}

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