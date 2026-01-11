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
			//NOTE: the reason for std::ceil is because we must include partially overlapping cells
			int minx = static_cast<int>((box.x - mBounds.x) * invCellW);					     //left edge (round down)
			int miny = static_cast<int>((box.y - mBounds.y) * invCellH);					     //top edge (round down)
			int maxx = static_cast<int>(std::ceil((box.x + box.w - mBounds.x) * invCellW));	     //right edge (round up)
			int maxy = static_cast<int>(std::ceil((box.y + box.h - mBounds.y) * invCellH));	     //bottom edge (round up)

			//NOTE: the reason for clamp high difference is because we do 0 based indexing AND the loop is exclusionary (using < instead of <=)
			minx = bad_clamp(minx, 0, mColumns - 1);	    //clamp x to left edge if negative, to right if wider than width, or keep
			miny = bad_clamp(miny, 0, mRows - 1);		    //clamp y to top edge if negative, to bottom if deeper than height, or keep
			maxx = bad_clamp(maxx, 0, mColumns);			//clamp x to left edge if negative, to right if wider than width, or keep
			maxy = bad_clamp(maxy, 0, mRows);				//clamp y to top edge if negative, to bottom if deeper than height, or keep

			for (int y = miny; y < maxy; ++y) {
				const int offset = y * mColumns;//cache multiplication, minor thing...
				for (int x = minx; x < maxx; ++x) {
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
		
		//returns the structure itself
		const Sequence<Cell>& query_grid()const noexcept {
			return mCells;
		}

		//collects all elements within the region
		//the rectangle can be partially intersecting the bounds of the grid but returns no results if the region is fully outside
		void query_region(const AABB& region, Sequence<int>& results)noexcept {
			//NOTE: the reason for std::ceil is because we must include partially overlapping cells
			int minx = static_cast<int>((region.x - mBounds.x) * invCellW);					             //left edge (round down)
			int miny = static_cast<int>((region.y - mBounds.y) * invCellH);					             //top edge (round down)
			int maxx = static_cast<int>(std::ceil((region.x + region.w - mBounds.x) * invCellW));	     //right edge (round up)
			int maxy = static_cast<int>(std::ceil((region.y + region.h - mBounds.y) * invCellH));	     //bottom edge (round up)

			//NOTE: the reason for clamp high difference is because we do 0 based indexing AND the loop is exclusionary (using < instead of <=)
			minx = bad_clamp(minx, 0, mColumns - 1);	    //clamp x to left edge if negative, to right if wider than width, or keep
			miny = bad_clamp(miny, 0, mRows - 1);		    //clamp y to top edge if negative, to bottom if deeper than height, or keep
			maxx = bad_clamp(maxx, 0, mColumns);			//clamp x to left edge if negative, to right if wider than width, or keep
			maxy = bad_clamp(maxy, 0, mRows);				//clamp y to top edge if negative, to bottom if deeper than height, or keep

			for (int y = miny; y < maxy; ++y) {
				const int offset = y * mColumns;
				for (int x = minx; x < maxx; ++x) {
					for (int id : mCells[static_cast<std::size_t>(offset + x)]) {
						results.emplace_back(id);
					}
				}
			}
		}
		//queries a point against the grid
		//returns cell index or -1 if the point is not in bounds of the grid
		inline int query_point(const float2& point)const noexcept {
			int x = static_cast<int>((point.x - mBounds.x) * invCellW);
			int y = static_cast<int>((point.y - mBounds.y) * invCellH);

			if (x < 0 || x >= mColumns || y < 0 || y >= mRows) {
				return -1;
			}

			return y * mColumns + x;
		}
		//queries a line against the grid
		//IMPORTANT: if the length of [lineEnd - lineOrigin] is 0, it could still be inside the grid but it will NOT be queried
		//returns cell indices, not individual elements
		void query_ray(const float2& lineOrigin, const float2& lineEnd, Sequence<int>& cell_indices)const noexcept
		{
			static constexpr float EPS = 0.0001;
		
			//1) compute ray direction and segment length, if segment legth is 0 then there is no ray, could still mean a point intersection though...
			const float2 dir = lineEnd - lineOrigin;
			const float segmentLength = length_vector(dir);
			if (segmentLength == 0.0f) return;

			//2) create a ray and check if the origin is inside grid bounds
			const Ray ray(lineOrigin, dir / segmentLength);

			const bool originInside =
				ray.origin.x >= mBounds.x &&
				ray.origin.x < mBounds.x + mBounds.w &&
				ray.origin.y >= mBounds.y &&
				ray.origin.y < mBounds.y + mBounds.h;

			//3) the point origin can be in the grid or out. if it is inside, by default entry MUST be 0. Doing a sweep blindly will result in not quering inner cells. 
			//otherwise do a sweep test if the origin is outside the grid and find entry that way
			float entryT = 0.0f;
			if (!originInside) {
				Hit hit;
				sweep(ray, mBounds, hit);
				//no intersection at all
				if (hit.t == INFINITY)
					return;
				//entry does happen but only not in the length of the segment
				if (hit.t > segmentLength)
					return;
				entryT = hit.t;
			}

			//4) apply a tiny epsilon to entryT to avoid edges cases like traversing EXACTLY on the edges.
			//also check again if entry is within the maximum segment length
			float currentT = entryT + EPS;
			if (currentT > segmentLength)
				return;

			//5) unlike sweep, query_ray is about where to start traversing the grid thus sweeps hit.pos does not apply. logical traversal must be respected
			//calculate the starting points of traversal in cell indices format clamping twice. once implicitly from float to int but secondly making sure it stays in the grid
			//the second clamp forces traversal to begin in the grid, not some value outside. not a crash but fails to query
			float2 entryPos = ray.origin + ray.dir * currentT;
			int cellX = static_cast<int>((entryPos.x - mBounds.x) * invCellW);
			int cellY = static_cast<int>((entryPos.y - mBounds.y) * invCellH);
			cellX = bad_clamp(cellX, 0, mColumns - 1);
			cellY = bad_clamp(cellY, 0, mRows - 1);

			//6) get the sign of a step, which way to traverse
			const int2 step = sign_vector(ray.dir);

			//7) get the distance difference per step. per 1 x axis step how much y length changes; per 1 y axis step how much x length changes (basic graph stuff)
			//if step.x or step.y is 0 it means it's axis aligned and should never choose the other step, thus give it infinity value
			const float2 delta = float2(
				(step.x == 0.0f) ? INFINITY : std::fabs(mCellWidth / ray.dir.x),
				(step.y == 0.0f) ? INFINITY : std::fabs(mCellHeight / ray.dir.y)
			);

			//8) initalize the time along the ray when each axis crosses its next cell boundary
			const float nextBoundaryX = mBounds.x + (step.x > 0 ?
				(cellX + 1) * mCellWidth : cellX * mCellWidth);
			const float nextBoundaryY = mBounds.y + (step.y > 0 ?
				(cellY + 1) * mCellHeight : cellY * mCellHeight);

			//9) since the origin can be anywhere in the grid, set up initially as the length that is "already traversed"
			float traversedX = (step.x == 0.0f) ? INFINITY :
				(nextBoundaryX - entryPos.x) / ray.dir.x + currentT;
			
			float traversedY = (step.y == 0.0f) ? INFINITY :
				(nextBoundaryY - entryPos.y) / ray.dir.y + currentT;

			//10) Traverse grid as long as cell indecies are within range and currentT is within segment length
			while (
				cellX >= 0 && cellX < mColumns &&
				cellY >= 0 && cellY < mRows &&
				currentT <= segmentLength
				)
			{
				cell_indices.emplace_back(cellY * mColumns + cellX);

				//step to next cell if either is dominant
				if (traversedX < traversedY)
				{
					currentT = traversedX;
					traversedX += delta.x;
					cellX += step.x;
				}
				else if (traversedY < traversedX)
				{
					currentT = traversedY;
					traversedY += delta.y;
					cellY += step.y;
				}
				else //45 degree angles
				{
					currentT = traversedX;
					traversedX += delta.x;
					traversedY += delta.y;
					cellX += step.x;
					cellY += step.y;
				}
			}
		}
		
		
		//if this function is called on a populated grid, it will remove elemnts
		//intended usage: call it periodically IF there are a lot of moving objects on a cleared out grid
		void maintain_uniform_memory(std::size_t cell_capacity_target) {
			for (auto& cell : mCells) {
				cell.set_capacity(cell_capacity_target);
			}
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