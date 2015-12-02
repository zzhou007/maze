//zihang zhou
//ps 8
//861090400
//cs 14

#ifndef SETUNION_H
#define SETUNION_H

// implement all of the method below
class setunion {
public:
   setunion(std::size_t n) : s(n)
   {
      for( size_t i = 0; i <= s.size(); i++)
         s[i] = i;
   }
   
	std::size_t find(std::size_t a) const {
      if (s[a])
		return a;
	  return find(s[a]);
      return a;
	}
	std::size_t find(std::size_t a) {
      if(s[a] != a)
      {
         return s[a] = find(s[a]);
      }
      return a;
	}

	// should perform the union, even if a and
	// b are not the roots
	void unionsets(std::size_t a, std::size_t b) {
      size_t x = find(a);
      size_t y = find(b);
      if(x == y)
         return;
      if(s[x] < s[y])
         s[y] = x;
      else if(s[y] < s[x])
         s[x] = y;
      else
      {
         s[y] = x;
         --s[x];
      }
	}
   

private:
	// store your array here
	// to ease grading, please call it "s"
   std::vector<size_t> s;
};

#endif
