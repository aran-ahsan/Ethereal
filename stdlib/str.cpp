/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <cctype>

#include "../src/VM/Core.hpp"

std::vector< std::string > str_delimit( const std::string & str, const char ch );

var_base_t * add( vm_state_t & vm, func_call_data_t & fcd )
{
	std::string res;
	std::string & a = AS_STR( fcd.args[ 1 ] )->get();
	std::string & b = AS_STR( fcd.args[ 0 ] )->get();
	res = a + b;
	return new var_str_t( res, fcd.args[ 1 ]->parse_ctr() );
}

var_base_t * add_assn( vm_state_t & vm, func_call_data_t & fcd )
{
	std::string & a = AS_STR( fcd.args[ 0 ] )->get();
	std::string & b = AS_STR( fcd.args[ 1 ] )->get();
	a += b;
	return fcd.args[ 0 ];
}

var_base_t * len( vm_state_t & vm, func_call_data_t & fcd )
{
	return new var_int_t( ( int )AS_STR( fcd.args[ 0 ] )->get().size(), fcd.args[ 0 ]->parse_ctr() );
}

var_base_t * empty( vm_state_t & vm, func_call_data_t & fcd )
{
	return TRUE_FALSE( AS_STR( fcd.args[ 0 ] )->get().empty() );
}

var_base_t * clear( vm_state_t & vm, func_call_data_t & fcd )
{
	AS_STR( fcd.args[ 0 ] )->get().clear();
	return nullptr;
}

var_base_t * is_int( vm_state_t & vm, func_call_data_t & fcd )
{
	char * p = 0;
	strtol( AS_STR( fcd.args[ 0 ] )->get().c_str(), & p, 10 );
	return TRUE_FALSE( * p == 0 );
}

var_base_t * to_int( vm_state_t & vm, func_call_data_t & fcd )
{
	return new var_int_t( AS_STR( fcd.args[ 0 ] )->get(), fcd.args[ 0 ]->parse_ctr() );
}

var_base_t * set_at( vm_state_t & vm, func_call_data_t & fcd )
{
	std::string & dat = AS_STR( fcd.args[ 0 ] )->get();
	int idx = fcd.args[ 1 ]->to_int().get_si();
	if( idx < 0 || idx >= ( int )dat.size() ) return fcd.args[ 0 ];
	if( fcd.args[ 2 ]->to_str().size() > 0 ) dat[ idx ] = fcd.args[ 2 ]->to_str()[ 0 ];
	return fcd.args[ 0 ];
}

var_base_t * split( vm_state_t & vm, func_call_data_t & fcd )
{
	const std::string & dat = AS_STR( fcd.args[ 0 ] )->get();
	const char delim = fcd.args.size() > 1 && AS_STR( fcd.args[ 1 ] )->get().size() > 0 ? AS_STR( fcd.args[ 1 ] )->get()[ 0 ] : ' ';
	std::vector< std::string > res = str_delimit( dat, delim );
	std::vector< var_base_t * > res_b;
	for( auto & r : res ) {
		res_b.push_back( new var_str_t( r, fcd.args[ 0 ]->parse_ctr() ) );
	}
	return new var_vec_t( res_b, fcd.args[ 0 ]->parse_ctr() );
}

var_base_t * trim( vm_state_t & vm, func_call_data_t & fcd )
{
	std::string & dat = AS_STR( fcd.args[ 0 ] )->get();
	while( dat.size() > 0 && isspace( dat.front() ) ) {
		dat.erase( dat.begin() );
	}
	while( dat.size() > 0 && isspace( dat.back() ) ) {
		dat.pop_back();
	}
	return fcd.args[ 0 ];
}

#define DECL_FUNC_BOOL__STR( name, oper )				\
	var_base_t * name( vm_state_t & vm, func_call_data_t & fcd )	\
	{								\
		auto & lhs = AS_STR( fcd.args[ 1 ] )->get();		\
		auto & rhs = AS_STR( fcd.args[ 0 ] )->get();		\
		return TRUE_FALSE( lhs oper rhs );			\
	}

DECL_FUNC_BOOL__STR( eq, == )
DECL_FUNC_BOOL__STR( ne, != )
DECL_FUNC_BOOL__STR( lt, < )
DECL_FUNC_BOOL__STR( le, <= )
DECL_FUNC_BOOL__STR( gt, > )
DECL_FUNC_BOOL__STR( ge, >= )

REGISTER_MODULE( str )
{
	vm.funcs.add( { "+", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = add }, true } );
	vm.funcs.add( { "+=", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = add_assn }, false } );

	// comparisons
	vm.funcs.add( { "==", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = eq }, false } );
	vm.funcs.add( { "!=", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = ne }, false } );
	vm.funcs.add( { "<",  2, 2, { "str", "str" }, FnType::MODULE, { .modfn = lt }, false } );
	vm.funcs.add( { "<=", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = le }, false } );
	vm.funcs.add( { ">",  2, 2, { "str", "str" }, FnType::MODULE, { .modfn = gt }, false } );
	vm.funcs.add( { ">=", 2, 2, { "str", "str" }, FnType::MODULE, { .modfn = ge }, false } );

	functions_t & strfns = vm.typefuncs[ "str" ];

	strfns.add( { "len", 0, 0, {}, FnType::MODULE, { .modfn = len }, true } );
	strfns.add( { "empty", 0, 0, {}, FnType::MODULE, { .modfn = empty }, false } );
	strfns.add( { "clear", 0, 0, {}, FnType::MODULE, { .modfn = clear }, false } );
	strfns.add( { "is_int", 0, 0, {}, FnType::MODULE, { .modfn = is_int }, false } );
	strfns.add( { "to_int", 0, 0, {}, FnType::MODULE, { .modfn = to_int }, true } );
	strfns.add( { "set_at", 2, 2, { "int", "str" }, FnType::MODULE, { .modfn = set_at }, false } );
	strfns.add( { "split", 0, 1, { "str" }, FnType::MODULE, { .modfn = split }, true } );
	strfns.add( { "trim", 0, 0, {}, FnType::MODULE, { .modfn = trim }, false } );
}

std::vector< std::string > str_delimit( const std::string & str, const char ch )
{
	std::string temp;
	std::vector< std::string > vec;

	for( auto c : str ) {
		if( c == ch ) {
			if( temp.empty() ) continue;
			vec.push_back( temp );
			temp.clear();
			continue;
		}

		temp += c;
	}

	if( !temp.empty() ) vec.push_back( temp );
	return vec;
}