//zihang zhou
//ps 8
//861090400
//cs 14
#include <random>
#include <iostream>
#include "setunion.h"
#include "maze.h"
#include "mazedrawer.h"
#include <cstdlib>
#include <stdlib.h>

using namespace std;
// generates a random integer between 0 and n-1 (inclusive)
int randomint(int n) {
	static std::random_device rd;
	static std::default_random_engine rand(rd());
	std::uniform_int_distribution<int> uniform(0,n-1);
	return uniform(rand);
}

int main(int argc, char *argv[])
{
	int width = strtoul(argv[1],NULL,0);
	int height = strtoul(argv[2],NULL,0);
	int size = height * width;
	maze m(width, height, true);
	int cell1;
	int cell2;
	//keep looping until we can get from start to end
	if (size  >= 1)
	{
		setunion u (size);
		while (u.find(0) != u.find(size-1))
		{
					
			bool top = false;
			
			pair<int,int> cellp;
			int remwall = randomint (m.numwalls() - (2 * width + 2 * height));
			//only inner walls can be removed 
			//changes random int to number that corresponds with the inner walls
			remwall += height;
			if (remwall > height * width  - 1) 
			{
				top = true;
				remwall = remwall + height + width; 
			}
			//finding the cells connected by a horizontal and vertical wall are different
			if (top)
			{
				cell2 = m.walladj(m.getwall(remwall)).first.x + m.walladj(m.getwall(remwall)).first.y * width;
				cell1 = cell2 - width;
			}
			else
			{
				cell2 = m.walladj(m.getwall(remwall)).first.x + m.walladj(m.getwall(remwall)).first.y * width;
				cell1 = cell2 - 1;
			}
			//test statements
			//cout << remwall << " wall " << endl;
			//cout << cell1 << "," << cell2 << endl;
			
			//check if the two cells are not union
			//union the two cell and remove the wall
			//cout << "before union begin " << u.find(0) << "end " << u.find(size-1) << endl;
			if (u.find(cell1) != u.find(cell2))
			{	
				m.setwall(m.getwall(remwall),0);
				u.unionsets(cell1,cell2);
			}
			//cout << "after union begin " << u.find(0) << "end " << u.find(size-1) << endl;
			
		}
	}
	m.draw(cout);
	return 0;
   
}
// put your main function (and other code you need) here
