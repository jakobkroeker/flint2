/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

int
main(void)
{
    int iter;
    flint_rand_t state;
    flint_randinit(state);

    flint_printf("inflate....");
    fflush(stdout);

    for (iter = 0; iter < 100 * flint_test_multiplier(); iter++)
    {
        TEMPLATE(T, poly_t) poly1, poly2, poly3, xp;
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, t) one;
        ulong inflation;

        TEMPLATE(T, ctx_randtest)(ctx, state);

        TEMPLATE(T, poly_init)(poly1, ctx);
        TEMPLATE(T, poly_init)(poly2, ctx);
        TEMPLATE(T, poly_init)(poly3, ctx);
        TEMPLATE(T, poly_init)(xp, ctx);

        TEMPLATE(T, poly_randtest)(poly1, state, n_randint(state, 20), ctx);
        inflation = n_randint(state, 10);

        TEMPLATE(T, poly_inflate)(poly2, poly1, inflation, ctx);

        TEMPLATE(T, init)(one, ctx);
        TEMPLATE(T, one)(one, ctx);
        TEMPLATE(T, poly_set_coeff)(xp, inflation, one, ctx);
        TEMPLATE(T, poly_compose)(poly3, poly1, xp, ctx);
        TEMPLATE(T, clear)(one, ctx);

        if (!TEMPLATE(T, poly_equal)(poly2, poly3, ctx))
        {
            flint_printf("FAIL: not equal to compose (inflation = %wu)\n", inflation);
            flint_printf("poly1:\n"); TEMPLATE(T, poly_print)(poly1, ctx); flint_printf("\n\n");
            flint_printf("poly2:\n"); TEMPLATE(T, poly_print)(poly2, ctx); flint_printf("\n\n");
            flint_printf("poly3:\n"); TEMPLATE(T, poly_print)(poly3, ctx); flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_inflate)(poly1, poly1, inflation, ctx);
        if (!TEMPLATE(T, poly_equal)(poly1, poly2, ctx))
        {
            flint_printf("FAIL: aliasing (inflation = %wu)\n", inflation);
            flint_printf("poly1:\n"); TEMPLATE(T, poly_print)(poly1, ctx); flint_printf("\n\n");
            flint_printf("poly2:\n"); TEMPLATE(T, poly_print)(poly2, ctx); flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear)(poly1, ctx);
        TEMPLATE(T, poly_clear)(poly2, ctx);
        TEMPLATE(T, poly_clear)(poly3, ctx);
        TEMPLATE(T, poly_clear)(xp, ctx);
        TEMPLATE(T, ctx_clear)(ctx);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}


#endif