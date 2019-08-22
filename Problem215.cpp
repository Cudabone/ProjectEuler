#include <iostream>
#include <vector>
#include <unordered_map>

//Represent Row as a bitfield. Max size of a row is equal to the max width of this type
using Row = uint32_t;

/*
	Appends each possible permutation of a row to outrows
*/
void RowPermutations( std::vector<Row>& outrows, size_t width, Row currentrow, size_t col )
{
	if ( col + 2 == width || col + 3 == width )
	{
		outrows.push_back( currentrow );
		return;
	}

	if ( col + 2 < width )
	{
		RowPermutations( outrows, width, currentrow | (1 << (col + 1)), col + 2 );
	}

	if ( col + 3 < width )
	{
		RowPermutations( outrows, width, currentrow | (1 << (col + 2)), col + 3 );
	}
}

/*
	Returns a vector of all possible row permutations of size width using bricks of size 2 and 3
*/
std::vector<Row> RowPermutations( size_t width )
{
	std::vector<Row> outrows;
	RowPermutations( outrows, width, 0, 0 );
	return outrows;
}

/*
	Returns the number of permutations a wall of size totalrows - currentrow given a previous row
	of rows[prevrowidx]
*/
unsigned long long WallPermutations
( 
	std::vector<std::vector<unsigned long long>>& cache,
	const std::vector<Row>& rows, 
	const std::vector<std::vector<size_t>>& compatible,
	size_t totalrows,
	size_t currentrow,
	size_t prevrowidx 
)
{
	if ( currentrow == totalrows )
	{
		return 1;
	}

	const size_t rowsleftidx = totalrows - (currentrow + 1);
	if ( cache[prevrowidx][rowsleftidx] != 0 )
	{
		return cache[prevrowidx][rowsleftidx];
	}

	unsigned long long total = 0;
	if ( currentrow == 0 )
	{
		for ( size_t i = 0; i < rows.size(); ++i )
		{
			total += WallPermutations( cache, rows, compatible, totalrows, currentrow + 1, i );
		}
	}
	else
	{
		for ( size_t row : compatible[prevrowidx] )
		{
			total += WallPermutations( cache, rows, compatible, totalrows, currentrow + 1, row );
		}
		cache[prevrowidx][rowsleftidx] = total;
	}
	return total;
}

/*
	Returns all possible permutations of rows for a wall of height totalrows.
*/
unsigned long long WallPermutations( const std::vector<Row>& rows, const std::vector<std::vector<size_t>>& compatible, size_t totalrows )
{
	//cache[i][j] == # ways to build a wall of size j+1 when the previous row is rows[i]
	//assuming that 0 is an invalid value => you can complete the wall from any state
	std::vector<std::vector<unsigned long long>> cache( rows.size(), std::vector<unsigned long long>( totalrows, 0 ) );
	return WallPermutations( cache, rows, compatible, totalrows, 0, 0 );
}

/*
	Returns a vector of compatible rows where
	CompatibleRows[i] == vector of compatible row indices of rows when the previous row is rows[i]
*/
std::vector<std::vector<size_t>> CompatibleRows( const std::vector<Row>& rows )
{
	std::vector<std::vector<size_t>> compatiblerows( rows.size(), std::vector<size_t>() );
	for ( size_t i = 0; i < rows.size() - 1; ++i )
	{
		for ( size_t j = i + 1; j < rows.size(); ++j )
		{
			if ( ( rows[i] & rows[j] ) == 0 )
			{
				compatiblerows[i].push_back( j );
				compatiblerows[j].push_back( i );
			}
		}
	}
	return compatiblerows;
}

int main()
{
	const size_t width = 32;
	const size_t height = 10;

	const std::vector<Row> rows = RowPermutations( width );
	const std::vector<std::vector<size_t>> compatiblerows = CompatibleRows( rows );

	std::cout << WallPermutations( rows, compatiblerows, height ) << std::endl;
	return 0;
}
