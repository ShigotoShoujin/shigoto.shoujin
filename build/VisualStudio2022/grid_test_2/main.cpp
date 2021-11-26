//https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members

//class initializer initialize base classes before members
// C1{4} runs before z(5)
// 
//struct C1 {
//	int zz;
//	C1(int v) : zz(v) {}
//};
//struct C2 : public C1 {
//	int z;
//	C2() :
//		z(5),
//		C1{4}
//	{}
//};
//

//Design is about choosing compromise, each design has adventages and disadventages

#include "grid.hpp"
#include <iostream>
#include <iomanip>

void GridInit(Grid<int>& grid)
{
	for (int i = 0; auto & it : grid)
		it = ++i;
}

void GridPrintUsingGridIteration(Grid<int>& grid)
{
	for (int i = 0; auto && it : grid)
	{
		std::cout << std::setw(2) << std::setfill('0') << it << ',';

		if (++i == grid.width()) {
			i = 0;
			std::cout << '\n';
		}
	}
	std::cout << '\n';
}
//
//void GridPrintUsingRowsRangeFor(Grid<int>& grid)
//{
//	for (auto&& row : grid.Rows())
//	{
//		for(auto&& cell : row)
//			std::cout << std::setw(2) << std::setfill('0') << *cell << ',';
//		std::cout << '\n';
//	}
//	std::cout << '\n';
//}

void GridPrintUsingRowsBeginEnd(Grid<int>& grid)
{
	GridRows<int> rows = grid.Rows();
	IterableIterator<ForwardIterator<int>> row = rows.begin();
	IterableIterator<ForwardIterator<int>> row_end = rows.end();
	for (; row != row_end; ++row)
	{
		//ForwardIterator<int> cell = row.begin();
		//ForwardIterator<int> cell_end = row.end();
		//for (; cell != cell_end; ++cell)
		//	std::cout << std::setw(2) << std::setfill('0') << *cell << ',';
		std::cout << '\n';
	}
	std::cout << '\n';
}

int main()
{
	Grid<int> grid(8, 5);
	GridInit(grid);
	GridPrintUsingGridIteration(grid);
	//GridPrintUsingRowsRangeFor(grid);
	GridPrintUsingRowsBeginEnd(grid);
}
