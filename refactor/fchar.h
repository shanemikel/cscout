/* 
 * (C) Copyright 2001 Diomidis Spinellis.
 *
 * A character coming from a file.
 * This class also handles trigraphs and newline splicing.
 *
 * Include synopsis:
 * #include <fstream>
 * #include <stack>
 * #include <deque>
 * #include <map>
 *
 * #include "fileid.h"
 * #include "cpp.h"
 * #include "tokid.h"
 * #include "fchar.h"
 *
 * $Id: fchar.h,v 1.8 2002/09/17 07:55:39 dds Exp $
 */

#ifndef FCHAR_
#define FCHAR_

using namespace std;

struct fchar_context {
	int line_number;
	Tokid ti;
};

typedef stack <fchar_context> stackFchar_context;

class Fchar;

// For pushback; two token stack needed to parse the "..."
typedef stack <Fchar> stackFchar;

class Fchar {
private:
	void Fchar::simple_getnext();	// Trigraphs and slicing
	static ifstream in;		// Stream we are reading from
	static Fileid fi;		// and its Fileid
	static int line_number;		// Current line number
	static stackFchar_context cs;	// Pushed contexts (from push_input())
	static stackFchar ps;		// Putback stack
	static int stack_lock_size;	// So many elements can not be removed
					// from the push_input stack

	int val;
	Tokid ti;			// (pos_type from tellg(), fi)

public:
	// Will read characters from file named s
	static void set_input(const string& s);
	// From now on will read from s; on EOF resume with previous file
	static void push_input(const string& s);
	// Next constructor will return c
	static void putback(Fchar c);
	/*
	 * Lock current context into the file stack, so it can not
	 * be popped by getnext until unlock is called.
	 * Call it after a push_input operation
	 */
	static void lock_stack() { stack_lock_size = cs.size(); }
	static void unlock_stack() { stack_lock_size = 0; }

	// Construct an unititialized one
	Fchar() {};
	// Construct it with a given value and tokid
	Fchar(int v, Tokid t) : val(v), ti(t) {};
	Fchar(int v) : val(v) {};
	// Read next from stream
	void getnext();
	// Return the character value (or EOF)
	inline int get_char() const { return (val); };
	// Return the character's Tokid
	inline Tokid get_tokid() const { return (ti); };
	// Return the current line number
	static int get_line_num() { return line_number; }
	static string  get_path() { return fi.get_path(); }
	// Return the fileid of the file we are processing
	static Fileid get_fileid() { return fi; }
};

#endif /* FCHAR_ */
