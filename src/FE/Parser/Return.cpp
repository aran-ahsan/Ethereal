/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "Internal.hpp"
#include "../Ethereal.hpp"

stmt_return_t * parse_return( src_t & src, parse_helper_t * ph )
{
	int tok_ctr = ph->tok_ctr();
	ph->next();

	expr_res_t expr = { 0, nullptr };
	int err, cols;
	err = find_next_of( ph, cols, { TOK_COLS } );
	if( err < 0 ) {
		if( err == -1 ) {
			PARSE_FAIL( "could not find the semicolon for the loop's init statement" );
		}
		goto fail;
	}

	expr = parse_expr( src, ph, cols );
	ph->set_tok_ctr( cols );
	if( expr.res != 0 ) goto fail;
	return new stmt_return_t( expr.expr, tok_ctr );
fail:
	if( expr.expr ) delete expr.expr;
	return nullptr;
}

bool stmt_return_t::bytecode( bytecode_t & bcode )
{
	return true;
}