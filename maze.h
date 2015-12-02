#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>

class mazedrawer;

class maze {
friend class mazedrawer;
public:
	maze(std::size_t width, std::size_t height, bool defaultwall) :
		vertwalls(width+1,std::vector<bool>(height,defaultwall)),
		horizwalls(width,std::vector<bool>(height+1,defaultwall)) { }

	enum class direction {north, south, west, east};

	struct location {
		std::size_t x,y;
	};

	struct wall {
		location l;
		direction d;
	};

	bool iswall(const wall &w) const {
		return *(findwall(w));
	}

	void setwall(const wall &w, bool value) {
		*(findwall(w)) = value;
	}

	// returns whether the location is a valid location inside the maze
	bool inside(const location &l) const {
		return  l.x<width() && l.y<height();
	}

	// find the location adjacent in the given direction
	static location adjloc(const location &l, direction dir) {
		switch(dir) {
			case direction::north: return location{l.x,l.y-1};
			case direction::south: return location{l.x,l.y+1};
			case direction::west: return location{l.x-1,l.y};
			case direction::east: return location{l.x+1,l.y};
		}
	}

	// returns the two locations adjacent to the wall
	static std::pair<location,location> walladj(const wall &w) {
		return std::make_pair(w.l,adjloc(w.l,w.d));
	}

	std::size_t width() const {
		return horizwalls.size();
	}
	std::size_t height() const {
		return vertwalls.empty() ? 0 : vertwalls[0].size();
	}

	// number of wall indices
	std::size_t numwalls() const {
		return width()*height()*2 + height() + width();
	}

	// return the wall associated with index i
	wall getwall(std::size_t i) const {
		if (i<(width()+1)*height())
			return wall{{i/height(),i%height()},direction::west};
		i -= (width()+1)*height();
		return wall{{i%width(),i/width()},direction::north};
	}

	void draw(std::ostream &os,
			bool useutf8=false, bool useextascii=false) const;

	void draw(std::ostream &os, const std::vector<maze::location> &path,
			bool useutf8=false, bool useextascii=false) const;
   
   friend bool operator==(const location &rhs, const location &lhs)
   {
      return (rhs.x == lhs.x) && (rhs.y == lhs.y);
   }

private:

	std::vector<bool>::const_iterator findwall(const wall &w) const {
		switch(w.d) {
			case direction::north: return horizwalls[w.l.x].begin()+(w.l.y);
			case direction::south: return horizwalls[w.l.x].begin()+(w.l.y+1);
			case direction::west: return vertwalls[w.l.x].begin()+(w.l.y);
			case direction::east: return vertwalls[w.l.x+1].begin()+(w.l.y);
		}
	}
	std::vector<bool>::iterator findwall(const wall &w) {
		switch(w.d) {
			case direction::north: return horizwalls[w.l.x].begin()+(w.l.y);
			case direction::south: return horizwalls[w.l.x].begin()+(w.l.y+1);
			case direction::west: return vertwalls[w.l.x].begin()+(w.l.y);
			case direction::east: return vertwalls[w.l.x+1].begin()+(w.l.y);
		}
	}

	std::vector<std::vector<bool>> vertwalls,horizwalls;

};

#include "mazedrawer.h"


inline void maze::draw(std::ostream &os,
			bool useutf8, bool useextascii) const {
		mazedrawer drawer(useutf8,useextascii);
		drawer.draw(os,*this);
	}

inline void maze::draw(std::ostream &os,
			const std::vector<maze::location> &path,
			bool useutf8, bool useextascii) const {
		mazedrawer drawer(useutf8,useextascii);
		drawer.draw(os,*this,path);
	}

#endif
