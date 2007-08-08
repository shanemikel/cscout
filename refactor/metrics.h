/*
 * (C) Copyright 2002 Diomidis Spinellis.
 *
 * A metrics and a metrics summary container.
 *
 * One metrics object is created and updated for every file.
 * Most metrics are collected during the postprocessing phase to
 * keep the main engine fast and clean.
 * This design also ensures that the character-based metrics processing
 * overhead will be incured exactly once for each file.
 *
 * During postprocessing call:
 * process_char() or process_id() while going through each file
 * msum.add_unique_id once() for every EC
 * msum.add_id() for each identifier having an EC
 * summarize_files() at the end of processing
 *
 * $Id: metrics.h,v 1.18 2007/08/08 14:45:34 dds Exp $
 */

#ifndef METRICS_
#define METRICS_

// Matrics we collect
// Keep in sync with metric_names[]
enum e_metric {
// During post-processing
	em_nchar,		// Number of characters
	em_nccomment,		// Comment characters
	em_nspace,		// Space characters
	em_nlcomment,		// Number of line comments
	em_nbcomment,		// Number of block comments
	em_nline,		// Number of lines
	em_maxlinelen,		// Maximum number of characters in a line
	em_nstring,		// Number of character strings
	em_uline,		// Number of unprocessed lines
	em_ncopies,		// Number of copies of the file

// During processing (once based on processed)
	em_npfunction,		// Defined project-scope functions
	em_nffunction,		// Defined file-scope (static) functions
	em_npvar,		// Defined project-scope variables
	em_nfvar,		// Defined file-scope (static) variables
	em_naggregate,		// Number of complete structure / union declarations
	em_namember,		// Number of declared aggregate members
	em_nenum,		// Number of complete enumeration declarations
	em_nemember,		// Number of declared enumeration elements
	em_nppdirective,	// Number of cpp directives
	em_nppcond,		// Number of processed cpp conditionals (ifdef, if, elif)
	em_nppfmacro,		// Number of defined cpp function-like macros
	em_nppomacro,		// Number of defined cpp object-like macros
	em_nincfile,		// Number of directly included files
	em_nstatement,		// Number of statements
	metric_max
};

// States while processing characters
enum e_cfile_state {
	s_normal,
	s_saw_slash,		// After a / character
	s_saw_str_backslash,	// After a \ character in a string
	s_saw_chr_backslash,	// After a \ character in a character
	s_cpp_comment,		// Inside C++ comment
	s_block_comment,	// Inside C block comment
	s_block_star,		// Found a * in a block comment
	s_string,		// Inside a string
	s_char,			// Inside a character
};

class Metrics {
private:
	vector <int> count;	// File metric counts

// Helper variables
	bool processed;		// True after file has been processed
	int currlinelen;
	enum e_cfile_state cstate;
	static string metric_names[];

public:
	Metrics() :
		count(metric_max, 0),
		processed(false),
		currlinelen(0),
		cstate(s_normal)
	{}

	// Return the name given the enumeration member
	static const string &name(int n) { return metric_names[n]; }

	// Called for all file characters appart from identifiers
	void process_char(char c);
	// Called for every identifier
	void process_id(const string &s);
	// Called when encountering unprocessed lines
	void add_unprocessed() { count[em_uline]++; }
	// Called to set the number of other identical files
	void set_ncopies(int n) { count[em_ncopies] = n; }

	// Manipulate the processing-based metrics
	void add_ppdirective() { if (!processed) count[em_nppdirective]++; }
	void add_ppcond() { if (!processed) count[em_nppcond]++; }
	void add_ppfmacro() { if (!processed) count[em_nppfmacro]++; }
	void add_ppomacro() { if (!processed) count[em_nppomacro]++; }
	void add_incfile() { if (!processed) count[em_nincfile]++; }

	void add_statement() { if (!processed) count[em_nstatement]++; }
	// Increment the number of functions for the file being processed
	void add_function(bool is_file_scoped) {
		if (processed)
			return;
		if (is_file_scoped)
			count[em_nffunction]++;
		else
			count[em_npfunction]++;
	}
	void add_aggregate() { if (!processed) count[em_naggregate]++; }
	void add_amember() { if (!processed) count[em_namember]++; }
	void add_enum() { if (!processed) count[em_nenum]++; }
	void add_emember() { if (!processed) count[em_nemember]++; }
	void add_pvar() { if (!processed) count[em_npvar]++; }
	void add_fvar() { if (!processed) count[em_nfvar]++; }
	void done_processing() { processed = true; }

	// Get methods
	enum e_cfile_state get_state() { return cstate; }
	// Generic
	int get_metric(int n) const { return count[n]; }

	// Number of characters
	int get_nchar() const { return count[em_nchar]; }
	// Number of line comments
	int get_nlcomment() const { return count[em_nlcomment]; }
	// Number of block comments
	int get_nbcomment() const { return count[em_nbcomment]; }
	// Number of lines
	int get_nline() const { return count[em_nline]; }
	// Maximum number of characters in a line
	int get_maxlinelen() const { return count[em_maxlinelen]; }
	// Comment characters
	int get_nccomment() const { return count[em_nccomment]; }
	// Space characters
	int get_nspace() const { return count[em_nspace]; }
	// Defined project-scope functions
	int get_npfunction() const { return count[em_npfunction]; }
	// Defined file-scope (static) functions
	int get_nffunction() const { return count[em_nffunction]; }
	// Defined project-scope variables
	int get_npvar() const { return count[em_npvar]; }
	// Defined file-scope (static) variables
	int get_nfvar() const { return count[em_nfvar]; }
	// Number of complete aggregate (struct/union) declarations
	int get_naggregate() const { return count[em_naggregate]; }
	// Number of declared aggregate (struct/union) members
	int get_namember() const { return count[em_namember]; }
	// Number of complete enumeration declarations
	int get_nenum() const { return count[em_nenum]; }
	// Number of declared enumeration elements
	int get_nemember() const { return count[em_nemember]; }
	// Number of cpp directives
	int get_nppdirective() const { return count[em_nppdirective]; }
	// Number of processed cpp conditionals (ifdef, if, elif)
	int get_nppcond() const { return count[em_nppcond]; }
	// Number of defined cpp function-like macros
	int get_nppfmacro() const { return count[em_nppfmacro]; }
	// Number of defined cpp object-like macros
	int get_nppomacro() const { return count[em_nppomacro]; }
	// Number of directly included files
	int get_nincfile() const { return count[em_nincfile]; }
	// Number of statements
	int get_nstatement() const { return count[em_nstatement]; }
	// Number of character strings
	int get_nstring() const { return count[em_nstring]; }
	// Number of unprocessed lined
	int get_uline() const { return count[em_uline]; }
};

class Eclass;
class FileMetricsSet;
class IdMetricsSet;

// A class for keeping taly of various identifier type counts
class IdCount {
private:
	int total;
	vector <int> count;		// Counts per identifier attribute
public:
	IdCount() :
		total(0),
		count(attr_end, 0)
	{}
	int get_count(int i) { return count[i]; }
	// Adjust class members according to the attributes of EC
	// using function object f
	template <class UnaryFunction>
	void add(Eclass *ec, UnaryFunction f);
	friend ostream& operator<<(ostream& o, const IdMetricsSet &m);
};

class Fileid;
class IdMetricsSet;

// Counting file details
class FileCount {
private:
	// Totals for all files
	int nfile;		// Number of unique files
	vector <int> count;	// File metric counts
public:
	FileCount(int v = 0) :
		nfile(0),
		count(metric_max, v)
	{}
	int get_metric(int i) { return count[i]; }
	// Add the details of file fi
	template <class BinaryFunction>
	void add(Fileid &fi, BinaryFunction f);
	friend ostream& operator<<(ostream& o, const FileMetricsSet &m);
};

// One such set is kept for readable and writable identifiers
class IdMetricsSet {
	friend class IdMetricsSummary;
	IdCount once;	// Each identifier EC is counted once
	IdCount len;	// Use the len of each EC
	IdCount maxlen;	// Maximum length for each type
	IdCount minlen;	// Minimum length for each type
	IdCount all;	// Each identifier counted for every occurance in a file
public:
	friend ostream& operator<<(ostream& o, const IdMetricsSet &m);
};

// One such set is kept for readable and writable files
class FileMetricsSet {
	friend class FileMetricsSummary;
	FileCount total;// File details, total
	FileCount min;	// File details, min across files
	FileCount max;	// File details, max across files
public:
	FileMetricsSet();
	friend ostream& operator<<(ostream& o, const FileMetricsSet &m);
	int get_total(int i) { return total.get_metric(i); }
};

// This can be kept per project and globally
class IdMetricsSummary {
	IdMetricsSet rw[2];			// For writable (0) and read-only (1) cases
public:
	// Called for every identifier occurence
	void add_id(Eclass *ec);
	// Called for every unique identifier occurence (EC)
	void add_unique_id(Eclass *ec);
	friend ostream& operator<<(ostream& o,const IdMetricsSummary &ms);
};

// This can be kept per project and globally
class FileMetricsSummary {
	FileMetricsSet rw[2];			// For read-only and writable cases
public:
	// Create file-based summary
	void summarize_files();
	friend ostream& operator<<(ostream& o,const FileMetricsSummary &ms);
	int get_total(int i) { return rw[0].get_total(i) + rw[1].get_total(i); }
	int get_readonly(int i) { return rw[1].get_total(i); }
	int get_writable(int i) { return rw[0].get_total(i); }
};

// Global metrics
extern IdMetricsSummary id_msum;
extern FileMetricsSummary file_msum;

#endif /* METRICS_ */
