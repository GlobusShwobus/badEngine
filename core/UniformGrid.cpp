#include "pch.h"
#include "UniformGrid.h"
#include "Utility.h"


namespace badCore 
{
	UniformGrid::UniformGrid(const Rect& bounds, float cellWidth, float cellHeight)
		:mBounds(bounds), mCellWidth(cellWidth), mCellHeight(cellHeight)
	{
		const auto bWidth = bounds.get_width();
		const auto bHeight = bounds.get_height();

		//assign column and row counts
		mColumns = static_cast<int>(bWidth / cellWidth);
		mRows    = static_cast<int>(bHeight / cellHeight);

		//assert bare minimum valid grid, if mcolumns are 0 or negative it should cover all bad cases
		assert(mColumns >= 1);
		assert(mRows >= 1);
		assert(bWidth * cellWidth == static_cast<float>(mColumns));
		assert(bHeight * cellHeight == static_cast<float>(mRows));

		//cache division to instead do * operator later
		invCellW = 1.0f / mCellWidth;
		invCellH = 1.0f / mCellHeight;
		//set enough storage and default initalize cells
		mCells.resize(static_cast<std::size_t>(mColumns * mRows));
		//an optimization to boost inital cell growth rate, it stops being relevant for mid to large storage cells
		//setting capacity instead doesn't seem to matter too much as it just pushes allocation requirement here, not insert and even then doesn't same much
		for (auto& cell : mCells) {
			//cell.set_capacity(10);
			cell.set_additive(CELL_ADDATIVE);//actually pretty good in this scenario, massive help for early inserts while not doing potentially wasteful allocations
		}
	}
	
	void UniformGrid::clear()noexcept
	{
		for (auto& cell : mCells)
			cell.clear();
	}

	void UniformGrid::insert(int user_index, const Rect& box)noexcept
	{
		//NOTE: the reason for std::ceil is because we must include partially overlapping cells
		int minx = static_cast<int>((box.min.x - mBounds.min.x) * invCellW);					     //left edge (round down)
		int miny = static_cast<int>((box.min.y - mBounds.min.y) * invCellH);					     //top edge (round down)
		int maxx = static_cast<int>(std::ceil((box.max.x - mBounds.min.x) * invCellW));	     //right edge (round up)
		int maxy = static_cast<int>(std::ceil((box.max.y - mBounds.min.x) * invCellH));	     //bottom edge (round up)

		//NOTE: the reason for clamp high difference is because we do 0 based indexing AND the loop is exclusionary (using < instead of <=)
		minx = core_clamp(minx, 0, mColumns - 1);	    //clamp x to left edge if negative, to right if wider than width, or keep
		miny = core_clamp(miny, 0, mRows - 1);		    //clamp y to top edge if negative, to bottom if deeper than height, or keep
		maxx = core_clamp(maxx, 0, mColumns);			//clamp x to left edge if negative, to right if wider than width, or keep
		maxy = core_clamp(maxy, 0, mRows);				//clamp y to top edge if negative, to bottom if deeper than height, or keep

		for (int y = miny; y < maxy; ++y) {
			const int offset = y * mColumns;//cache multiplication, minor thing...
			for (int x = minx; x < maxx; ++x) {
				mCells[static_cast<std::size_t>(offset + x)].emplace_back(user_index);
			}
		}
	}

	const Sequence<UniformGrid::Cell>& UniformGrid::query_grid()const noexcept {
		return mCells;
	}

	void UniformGrid::query_region(const Rect& region, Sequence<int>& results)const noexcept
	{
		//NOTE: the reason for std::ceil is because we must include partially overlapping cells
		int minx = static_cast<int>((region.min.x - mBounds.min.x) * invCellW);					             //left edge (round down)
		int miny = static_cast<int>((region.min.y - mBounds.min.y) * invCellH);					             //top edge (round down)
		int maxx = static_cast<int>(std::ceil((region.max.x - mBounds.min.x) * invCellW));	     //right edge (round up)
		int maxy = static_cast<int>(std::ceil((region.max.y - mBounds.min.y) * invCellH));	     //bottom edge (round up)

		//NOTE: the reason for clamp high difference is because we do 0 based indexing AND the loop is exclusionary (using < instead of <=)
		minx = core_clamp(minx, 0, mColumns - 1);	    //clamp x to left edge if negative, to right if wider than width, or keep
		miny = core_clamp(miny, 0, mRows - 1);		    //clamp y to top edge if negative, to bottom if deeper than height, or keep
		maxx = core_clamp(maxx, 0, mColumns);			//clamp x to left edge if negative, to right if wider than width, or keep
		maxy = core_clamp(maxy, 0, mRows);				//clamp y to top edge if negative, to bottom if deeper than height, or keep

		for (int y = miny; y < maxy; ++y) {
			const int offset = y * mColumns;
			for (int x = minx; x < maxx; ++x) {
				results.emplace_back(static_cast<std::size_t>(offset + x));
			}
		}
	}

	int UniformGrid::query_point(const float2& point)const noexcept
	{
		int x = static_cast<int>((point.x - mBounds.min.x) * invCellW);
		int y = static_cast<int>((point.y - mBounds.min.y) * invCellH);

		if (x < 0 || x >= mColumns || y < 0 || y >= mRows) {
			return -1;
		}

		return y * mColumns + x;
	}

	void UniformGrid::query_neighbors(int cellIndex, Sequence<int>& neighbors)const noexcept
	{
		assert(mCells.size() > cellIndex && cellIndex >= 0);
		//1) work back the formula [ y * mColums + x ] to get x and y origin to do arithmetic from -1 to 1
		const int x = cellIndex % mColumns;//column index
		const int y = cellIndex / mColumns;//row index

		//MENTAL MODEL
		//    _________________________________    
		//    |  (-1,-1) |  (0, -1) | (1, -1) | 
		//    |__________|__________|_________|  	 
		//    |  (-1, 0) |  x and y | (1,  0) |  	 
		//    |__________|__________|_________|  	 
		//    |  (-1, 1) |  (0,  1) | (1,  1) |  	 
		//    |__________|__________|_________|  	 		   	

		//2) traverse the grid
		for (int dy = -1; dy <= 1; ++dy) {
			const int ny = y + dy;
			//if neightbor index is less than 0 or more than rows, it is off the edge thus doesn't actually exist so skip it
			if (ny < 0 || ny >= mRows)continue;

			for (int dx = -1; dx <= 1; ++dx) {
				int nx = x + dx;
				//if neighbor is less than 0 or more than columns, it is off the edge thus skip. also if both are 0's then it is the cellIndex itself, skip that too
				if (nx < 0 || nx >= mColumns)continue;
				if (dx == 0 && dy == 0)continue;
				//put together the neighbor [y * mColums + x]
				neighbors.emplace_back(ny * mColumns + nx);
			}
		}
	}

	void UniformGrid::query_ray(const Ray& ray, Sequence<int>& cell_indices)const noexcept
	{
		//1) if segmentLength legth is 0 then there is no ray, could still mean a point intersection though...
		const float& segmentLength = ray.rayLen;
		if (segmentLength == 0.0f)
			return;

		//2) create a ray and check if the origin is inside grid bounds
		const bool originInside =
			ray.rayOrigin.x >= mBounds.min.x &&
			ray.rayOrigin.x <  mBounds.max.x &&
			ray.rayOrigin.y >= mBounds.min.y &&
			ray.rayOrigin.y <  mBounds.max.y;

		//3) the point origin can be in the grid or out. if it is inside, by default entry MUST be 0. Doing a sweep blindly will result in not quering inner cells. 
		//otherwise do a sweep test if the origin is outside the grid and find entry that way
		float entryT = 0.0f;
		if (!originInside) {
			
			auto sweep = ray.sweep_test(mBounds);
			if (sweep.is_hit)
				entryT = sweep.time;
		}

		//4) apply a tiny epsilon to entryT to avoid edges cases like traversing EXACTLY on the edges.
		//also check again if entry is within the maximum segment length
		float currentT = entryT + EPSILON_core;
		if (currentT > segmentLength)
			return;

		//5) unlike sweep, query_ray is about where to start traversing the grid thus sweeps hit.pos does not apply. logical traversal must be respected
		//calculate the starting points of traversal in cell indices format clamping twice. once implicitly from float to int but secondly making sure it stays in the grid
		//the second clamp forces traversal to begin in the grid, not some value outside. not a crash but fails to query
		float2 entryPos = ray.rayOrigin + ray.rayDir * currentT;
		int cellX = static_cast<int>((entryPos.x - mBounds.min.x) * invCellW);
		int cellY = static_cast<int>((entryPos.y - mBounds.min.y) * invCellH);
		cellX = core_clamp(cellX, 0, mColumns - 1);
		cellY = core_clamp(cellY, 0, mRows - 1);

		//6) get the sign of a step, which way to traverse
		const auto rayDirSign = ray.rayDir.sign();
		const int step_x = static_cast<int>(rayDirSign.x);
		const int step_y = static_cast<int>(rayDirSign.y);

		//7) get the distance difference per step. per 1 x axis step how much y length changes; per 1 y axis step how much x length changes (basic graph stuff)
		//if step.x or step.y is 0 it means it's axis aligned and should never choose the other step, thus give it infinity value
		const float2 delta = float2(
			(step_x == 0) ? INFINITY : std::fabs(mCellWidth / ray.rayDir.x),
			(step_y == 0) ? INFINITY : std::fabs(mCellHeight / ray.rayDir.y)
		);

		//8) initalize the time along the ray when each axis crosses its next cell boundary
		const float nextBoundaryX = mBounds.min.x + (step_x > 0 ?
			(cellX + 1) * mCellWidth : cellX * mCellWidth);
		const float nextBoundaryY = mBounds.min.y + (step_y > 0 ?
			(cellY + 1) * mCellHeight : cellY * mCellHeight);

		//9) since the origin can be anywhere in the grid, set up initially as the length that is "already traversed"
		float traversedX = (step_x == 0.0f) ? INFINITY :
			(nextBoundaryX - entryPos.x) / ray.rayDir.x + currentT;

		float traversedY = (step_y == 0.0f) ? INFINITY :
			(nextBoundaryY - entryPos.y) / ray.rayDir.y + currentT;

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
				cellX += step_x;
			}
			else if (traversedY < traversedX)
			{
				currentT = traversedY;
				traversedY += delta.y;
				cellY += step_y;
			}
			else //45 degree angles
			{
				currentT = traversedX;
				traversedX += delta.x;
				traversedY += delta.y;
				cellX += step_x;
				cellY += step_y;
			}
		}
	}

	void UniformGrid::maintain_uniform_memory(std::size_t cell_capacity_target)
	{
		for (auto& cell : mCells) {
			cell.set_capacity(cell_capacity_target);
		}
	}

	const UniformGrid::Cell& UniformGrid::get_cell(std::size_t index)const noexcept
	{
		assert(mCells.size() > index);
		return mCells[index];
	}		

	const Rect& UniformGrid::get_grid_bounds()const noexcept
	{
		return mBounds;
	}

	float UniformGrid::get_cell_width()const noexcept
	{
		return mCellWidth;
	}

	float UniformGrid::get_cell_height()const noexcept
	{
		return mCellHeight;
	}
}