/* 
 * (C) Copyright 2001 Diomidis Spinellis.
 *
 * Debugpoints can be set for selectively enabling debugging output
 * The debugpoint() macro should be used to efficiently disable debugging output
 *
 * $Id: debug.h,v 1.1 2001/09/03 07:43:11 dds Exp $
 */

#ifndef DEBUG_
#define DEBUG_

class Debug {
private:
	string fname;
	int line;

	static set<Debug> dp;		// Enabled debug points
	static bool enabled;		// Global enable variable
public:
	Debug(const string f, int l) : line(l), fname(f) {};

	static inline bool is_db_set(const string fname, int line);
	static void db_set(string fname, int line);
	static inline bool is_enabled() { return enabled; };
	inline friend bool operator <(const class Debug a, const class Debug b);
};

#define DP() (Debug::is_enabled() && Debug::is_db_set((const string)__FILE__, __LINE__))

inline bool
operator <(const class Debug a, const class Debug b)
{
	if (a.fname == b.fname)
		return (a.line < b.line);
	else
		return (a.fname < b.fname);
}

inline bool
Debug::is_db_set(const string fname, int line)
{
	return dp.find(Debug(fname, line)) != dp.end();
}

#endif /* DEBUG_ */