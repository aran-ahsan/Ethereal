/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "../src/VM/Core.hpp"

#include "Core/Int.hpp"
#include "Core/Flt.hpp"
#include "Core/Bool.hpp"

static std::unordered_map< std::string, const char * > COL = {
	{ "0", "\033[0m" },

	{ "r", "\033[0;31m" },
	{ "g", "\033[0;32m" },
	{ "y", "\033[0;33m" },
	{ "b", "\033[0;34m" },
	{ "m", "\033[0;35m" },
	{ "c", "\033[0;36m" },
	{ "w", "\033[0;37m" },

	{ "br", "\033[1;31m" },
	{ "bg", "\033[1;32m" },
	{ "by", "\033[1;33m" },
	{ "bb", "\033[1;34m" },
	{ "bm", "\033[1;35m" },
	{ "bc", "\033[1;36m" },
	{ "bw", "\033[1;37m" },
};

void apply_colors( std::string & str )
{
	for( auto it = str.begin(); it != str.end(); ) {
		if( * it == '{' && ( it == str.begin() || ( * ( it - 1 ) != '$' && * ( it - 1 ) != '%' && * ( it - 1 ) != '#' && * ( it - 1 ) != '\\' ) ) ) {
			it = str.erase( it );
			if( it != str.end() && * it == '{' ) {
				++it;
				continue;
			}

			std::string var;

			while( it != str.end() && * it != '}' ) {
				var += * it;
				it = str.erase( it );
			}

			// Remove the ending brace
			if( it != str.end() ) it = str.erase( it );

			if( var.empty() ) continue;

			std::string val = COL[ var ];

			it = str.insert( it, val.begin(), val.end() );
			it += val.size();
		}
		else {
			++it;
		}
	}
}

var_base_t * colorize( vm_state_t & vm, func_call_data_t & fcd )
{
	std::string data = fcd.args[ 0 ]->to_str();
	apply_colors( data );
	return new var_str_t( data, fcd.args[ 0 ]->parse_ctr() );
}

var_base_t * col_print( vm_state_t & vm, func_call_data_t & fcd )
{
	for( auto & v : fcd.args ) {
		std::string data = v->to_str();
		apply_colors( data );
		fprintf( stdout, "%s", data.c_str() );
	}
	return nullptr;
}

var_base_t * col_println( vm_state_t & vm, func_call_data_t & fcd )
{
	for( auto & v : fcd.args ) {
		std::string data = v->to_str();
		apply_colors( data );
		fprintf( stdout, "%s", data.c_str() );
	}
	fprintf( stdout, "\n" );
	return nullptr;
}

REGISTER_MODULE( term )
{
	vm.funcs.add( { "colorize", 1,  1, { "str" }, FnType::MODULE, { .modfn = colorize }, true } );
	vm.funcs.add( { "cprint",   1, -1, { "_whatever_" }, FnType::MODULE, { .modfn = col_print }, false } );
	vm.funcs.add( { "cprintln", 0, -1, { "_whatever_" }, FnType::MODULE, { .modfn = col_println }, false } );
}