#ifndef MAZEDRAWER_H
#define MAZEDRAWER_H

#include <iostream>
#include "maze.h"

class mazedrawer {
public:
	mazedrawer(bool useutf8, bool useextascii)
			: uutf(useutf8), uea(useextascii) {}

	void draw(std::ostream &os, const maze &m) const {
		drawhwalls(os,m,0);
		for(std::size_t i=0;i<m.height();i++) {
			drawvwalls(os,m,i);
			drawhwalls(os,m,i+1);
		}
	}

	void draw(std::ostream &os, const maze &m,
				const std::vector<maze::location> &path) {
		drawhwalls(os,m,0);
		for(std::size_t i=0;i<m.height();i++) {
			drawvwalls(os,m,i,path);
			drawhwalls(os,m,i+1);
		}
	}

private:

	bool uutf, uea;
	void drawhwalls(std::ostream &os, const maze &m, std::size_t i) const {
		centerchar(os,m,0,i);
		std::size_t j=0;
		for(auto &&w : m.horizwalls) {
			hwallchar(os,w[i]);
			centerchar(os,m,++j,i);
		}
		os << std::endl;
	}
	void drawvwalls(std::ostream &os, const maze &m, std::size_t i) const {
		bool first = true;
		for(auto &&w : m.vertwalls) { 
			if (!first) blankchar(os);
			else first = false;
			vwallchar(os,w[i]);
		}
		os << std::endl;
	}

	void drawvwalls(std::ostream &os, const maze &m, std::size_t i,
				const std::vector<maze::location> &path) const {
		std::size_t j = 0;
		for(auto &&w : m.vertwalls) { 
			if (j>0) { // below is slow (find), but simplicity is
					// more important than speed right here
				maze::location ll{j-1,i};
				auto l = std::find(path.begin(),path.end(),ll);
				if (l==path.end()) blankchar(os);
				else pathchar(os,(l-path.begin()));
			}
			++j;
			vwallchar(os,w[i]);
		}
		os << std::endl;
	}

	bool isdrawnwall(const maze &m, std::size_t i, std::size_t j,
						maze::direction d) const {
		switch(d) {
			case maze::direction::north: {
			if (j<=0) return false;
			return m.iswall(maze::wall{maze::location{i,j-1},maze::direction::west});
			}
			case maze::direction::south: {
			if (j>=m.height()) return false;
			return m.iswall(maze::wall{maze::location{i,j},maze::direction::west});
			}
			case maze::direction::west: {
			if (i<=0) return false;
			return m.iswall(maze::wall{maze::location{i-1,j},maze::direction::north});
			}
			case maze::direction::east: {
			if (i>=m.width()) return false;
			return m.iswall(maze::wall{maze::location{i,j},maze::direction::north});
			}
		}
		return false;
	}

	static void dumputf(std::ostream &os, int cc) {
		if (cc<0x7F) os << (char)(cc);
		else if (cc<0x07ff) os << (char)((cc>>6) | 0xC0)
						<< (char)((cc & 0x3F) | 0x80);
		else os << (char)((cc>>12) | 0xE0)
				<< (char)(((cc & 0x0FFF)>>6) | 0x80)
				<< (char)((cc & 0x003F) | 0x80);
	}

	void blankchar(std::ostream &os) const {
		os << ' ';
	}

	void vwallchar(std::ostream &os, bool iswall) const {
		if (!iswall) blankchar(os);
		else if (uea) os << (char)(179);
		else if (uutf) dumputf(os, 0x2502);
		else os << '|';
	}

	void hwallchar(std::ostream &os, bool iswall) const {
		if (!iswall) blankchar(os);
		else if (uea) os << (char)(196);
		else if (uutf) dumputf(os, 0x2500);
		else os << '-';
	}
	void centerchar(std::ostream &os, const maze &m,
					std::size_t i, std::size_t j) const {
		if (uea) {
			int bitv = isdrawnwall(m,i,j,maze::direction::north) |
					(isdrawnwall(m,i,j,maze::direction::west)<<1) |
					(isdrawnwall(m,i,j,maze::direction::south)<<2) |
					(isdrawnwall(m,i,j,maze::direction::east)<<3);
			static int extcharcodes[16] = {32,32,32,217,32,179,191,180,
						32,192,196,193,218,195,194,197};
			os << (char)extcharcodes[bitv];
		} else if (uutf) {
			int bitv = isdrawnwall(m,i,j,maze::direction::north) |
					(isdrawnwall(m,i,j,maze::direction::west)<<1) |
					(isdrawnwall(m,i,j,maze::direction::south)<<2) |
					(isdrawnwall(m,i,j,maze::direction::east)<<3);
			static int utfcharcodes[16] = {0x0020, 0x2575, 0x2574,
				0x2518, 0x2577, 0x2502, 0x2510, 0x2524,
				0x2576, 0x2514, 0x2500, 0x2534, 0x250C,
				0x251C, 0x252C, 0x253C};
			dumputf(os,utfcharcodes[bitv]);
		} else os << '+';
	}

	void pathchar(std::ostream &os, int stepnum) const {
		os << (stepnum % 10);
	}
};

#endif
