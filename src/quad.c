#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processing.h"
#include "quad.h"
#include "vec.h"

#define DEFAULT_QUAD_ARRAY_SIZE 1 << 12

int next_quad_index, quad_array_size;
vec_t quad_array;

// number of global temp variables
int global_temp_count = 0;

struct symnode *curr_func_symnode_quad;

// number of global variables, from preprocessing
extern int num_global_vars;
extern struct symtable *flat_id_table;
extern struct symtable *stringconst_table;

int temp_count = 0;

// symnode of the main function (declared in symtab.c)
extern struct symnode *main_func_symnode;

char *quad_op_string[] = {
    "call_func_op",
    "print_int_op",
    "print_float_op",
    "print_string_op",
    "int_to_float_op",
    "assn_int_to_var_op",
    "assn_int_to_arraysub_op",
    "assn_int_from_arraysub_op",
    "assn_float_to_var_op",
    "assn_float_to_arraysub_op",
    "assn_float_from_arraysub_op",
    "add_ints_op", //
    "add_floats_op",
    "sub_ints_op", //
    "sub_floats_op",
    "mult_ints_op", //
    "mult_floats_op",
    "div_ints_op", //
    "div_floats_op",
    "mod_op",
    "lt_ints_op",
    "lt_floats_op",
    "leq_ints_op",
    "leq_floats_op",
    "gt_ints_op",
    "gt_floats_op",
    "geq_ints_op",
    "geq_floats_op",
    "eq_ints_op",
    "eq_floats_op",
    "neq_ints_op",
    "neq_floats_op",
    "int_neg_op",
    "float_neg_op",
    "bang_op",
    "var_inc_op",
    "array_inc_op",
    "var_dec_op",
    "array_dec_op",
    "if_false_op",
    "if_true_op",
    "goto_op",
    "read_int_op",
    "read_double_op",
    "func_decl_op",
    "push_param_op",
    "pop_params_op",
    "alloc_array_op",
    "return_op",
    "assign_int_literal",
    "assign_double_literal",
    "initial_main_call",
    "store_string_op",
};

/* some more prototypes */

// Generates code for the standard binary operation with widening
// : +, -, *, /
struct quadarg *generate_binary_op_with_widening(struct pnode *node,
                                                 enum quadop quad_op_ints,
                                                 enum quadop quad_op_floats);
