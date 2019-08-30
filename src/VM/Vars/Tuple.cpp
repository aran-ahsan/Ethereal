/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "Base.hpp"

var_tuple_t::var_tuple_t( std::vector< var_base_t * > & val, const int parse_ctr )
	: var_base_t( VT_TUPLE, true, parse_ctr ), m_val( val ) {}
var_tuple_t::~var_tuple_t()
{
	for( auto & v : m_val ) {
		VAR_DREF( v );
	}
}
std::string var_tuple_t::to_str() const
{
	std::string str = "(";
	for( auto it = m_val.begin(); it != m_val.end(); ++it ) {
		if( it == m_val.end() - 1 ) str += ( * it )->to_str();
		else str += ( * it )->to_str() + ", ";
	}
	str += ")";
	return str;
}
mpz_class var_tuple_t::to_int() const { return mpz_class( m_val.size() ); }
bool var_tuple_t::to_bool() const { return m_val.size() > 0; }

var_base_t * var_tuple_t::copy( const int parse_ctr )
{
	std::vector< var_base_t * > newvec;
	for( auto & v : m_val ) {
		newvec.push_back( v->copy( parse_ctr ) );
	}
	return new var_tuple_t( newvec, parse_ctr );
}

void var_tuple_t::clear()
{
	for( auto & v : m_val ) {
		VAR_DREF( v );
	}
	m_val.clear();
}

std::vector< var_base_t * > & var_tuple_t::get() { return m_val; }

void var_tuple_t::assn( var_base_t * b )
{
	this->clear();
	var_tuple_t * bt = static_cast< var_tuple_t * >( b );
	for( auto & x : bt->m_val ) {
		this->m_val.push_back( x->copy( b->parse_ctr() ) );
	}
}