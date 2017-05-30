

/*
 * THIS SOURCE CODE IS SUPPLIED  ``AS IS'' WITHOUT WARRANTY OF ANY KIND, 
 * AND ITS AUTHOR AND THE JOURNAL OF ARTIFICIAL INTELLIGENCE RESEARCH 
 * (JAIR) AND JAIR'S PUBLISHERS AND DISTRIBUTORS, DISCLAIM ANY AND ALL 
 * WARRANTIES, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, AND
 * ANY WARRANTIES OR NON INFRINGEMENT.  THE USER ASSUMES ALL LIABILITY AND
 * RESPONSIBILITY FOR USE OF THIS SOURCE CODE, AND NEITHER THE AUTHOR NOR
 * JAIR, NOR JAIR'S PUBLISHERS AND DISTRIBUTORS, WILL BE LIABLE FOR 
 * DAMAGES OF ANY KIND RESULTING FROM ITS USE.  Without limiting the 
 * generality of the foregoing, neither the author, nor JAIR, nor JAIR's
 * publishers and distributors, warrant that the Source Code will be 
 * error-free, will operate without interruption, or will meet the needs 
 * of the user.
 */








#include "ff.h"
#include "inst_pre.h"
#include "pddl-output.h"










/*************************
 * TYPE STRIPPED OUTPUT  *
 *************************/
















void output_type_stripped_files( void )

{

  FILE *fts, *ops;
  int i, j;
  Effect *eff;
  Literal *lit;
  WffNode *in;
  Bool hit;

  /* just to get the requirements right... !
   */
  collect_needed_constructs();

  printf("\n\nwriting fact file ``facts.pddl''."); fflush(stdout);

  if ( (fts = fopen( "facts.pddl", "w")) == NULL ) {
    printf("\n\nCannot open file facts.pddl.\n\n");
    exit( 1 );
  }
  fprintf(fts, "(define (problem typestripped-%s)\n", gproblem_name);
  fprintf(fts, "(:domain typestripped-%s)\n", gdomain_name);

  for ( i = 0; i < gnum_constants; i++ ) {
    if ( !gdomain_const[i] ) break;
  }
  if ( i < gnum_constants ) {
    fprintf(fts, "(:objects\n");
    for ( i = 0; i < gnum_constants; i++ ) {
      if ( gdomain_const[i] ) continue;
      fprintf(fts, "%s\n", gconstants[i]);
    }
    fprintf(fts, ")\n");
  }

  if ( gnum_full_initial > 0 || gnum_constants > 0 ) {
    fprintf(fts, "(:init\n");
    /* give type static preds; skip universal ones.
     */
    for ( i = 0; i < gnum_types; i++ ) {
      if ( guniversal_type[i] ) {
	continue;
      }
      for ( j = 0; j < gtype_size[i]; j++ ) {
	fprintf(fts, "(TYPE-%s %s)\n", 
		gtype_names[i], gconstants[gtype_consts[i][j]]);
      }
    }
    for ( i = 0; i < gnum_full_initial; i++ ) {
      if ( gfull_initial[i].predicate == 0 ) {
	continue;
      }
      fprint_Fact(fts, &(gfull_initial[i]) );
      fprintf(fts, "\n");
    }
    fprintf(fts, ")\n");
  }

  fprintf(fts, "(:goal\n");
  /* print a Wff into output file; vars are called ?x<varnum>; in a cond eff,
   * the cond params are #opparams + <condparamnr.>
   */
  fprint_typestripped_Wff( fts, ggoal );
  fprintf(fts, ")\n");
  fprintf(fts, ")\n");
  fclose( fts );



  /* domain header, predicates
   */
  printf("\nwriting domain file ``domain.pddl''."); fflush(stdout);

  if ( (ops = fopen( "domain.pddl", "w")) == NULL ) {
    printf("\n\nCannot open file domain.pddl.\n\n");
    exit( 1 );
  }
  fprintf(ops, "(define (domain typestripped-%s)\n", gdomain_name);
  fprintf(ops, "(:requirements\n");
  fprintf(ops, ":strips\n");
  if ( gnegative_preconditions ) {
    fprintf(ops, ":negative-preconditions\n");
  }
  if ( gdisjunctive_preconditions ) {
    fprintf(ops, ":disjunctive-preconditions\n");
  }
  if ( gequality ) {
    fprintf(ops, ":equality\n");
  }
  if ( gquantified_preconditions ) {
    fprintf(ops, ":quantified-preconditions\n");
  } else {
    if ( guniversal_preconditions ) {
      fprintf(ops, ":guniversal-preconditions\n");
    }
    if ( gexistential_preconditions ) {
      fprintf(ops, ":gexistential-preconditions\n");
    }
  }
  if ( gconditional_effects ) {
    fprintf(ops, ":conditional-effects\n");
  }
  if ( gdps ) {
    fprintf(ops, ":derived-predicates\n");
  }
  fprintf(ops, ")\n");

  for ( i = 0; i < gnum_constants; i++ ) {
    if ( gdomain_const[i] ) break;
  }
  if ( i < gnum_constants ) {
    fprintf(fts, "(:constants\n");
    for ( i = 0; i < gnum_constants; i++ ) {
      if ( !gdomain_const[i] ) continue;
      fprintf(fts, "%s\n", gconstants[i]);
    }
    fprintf(fts, ")\n");
  }

  fprintf(ops, "(:predicates\n");
  for ( i = 0; i < gnum_types; i++ ) {
    if ( guniversal_type[i] ) {
      continue;
    }
    fprintf(ops, "(TYPE-%s ?x0)\n", gtype_names[i]);
  }
  /* skip artificial "=" predicate
   */
  for ( i = 1; i < gnum_predicates; i++ ) {
    fprintf(ops, "(%s", gpredicates[i]);
    for ( j = 0; j < garity[i]; j++ ) {
	fprintf(ops, " ?x%d", j);
    }
    fprintf(ops, ")\n");
  }
  fprintf(ops, ")\n");

  for ( i = 0; i < gnum_operators; i++ ) {
    if ( goperators[i]->preconds &&
	 goperators[i]->preconds->connective == FAL ) continue;
    /* first, the derivation rules
     */
    if ( !goperators[i]->axiom ) {
      continue;
    }
    if ( !goperators[i]->preconds ) {
      printf("\n\nderiv. rule formula empty??\n\n");
      exit( 1 );
    }
    if ( goperators[i]->preconds->connective == TRU ) {
      printf("\n\nderiv. rule formula TRUE??\n\n");
      exit( 1 );
    }
    fprintf(ops, "\n(:derived ");
    if ( !goperators[i]->effects ||
	 !goperators[i]->effects->effects ) {
      printf("\n\neffect of deriv. rule not as expected\n\n");
      exit( 1 );
    }
    fprint_Fact( ops, &(goperators[i]->effects->effects->fact) );
    fprintf(ops, "\n");
    /* now the rule formula, ie the op preconds, possibly
     * in conjunction with the arg (param) types.
     */
    hit = FALSE;
    for ( j = 0; j < goperators[i]->num_vars; j++ ) {
      if ( !guniversal_type[goperators[i]->var_types[j]] ) {
	hit = TRUE;
      }
    }
    if ( hit ) {
      fprintf(ops, "(and\n");
    }
    if ( hit && goperators[i]->preconds->connective == AND ) {
      for ( in = goperators[i]->preconds->sons; in; in = in->next ) {
	fprint_typestripped_Wff( ops, in );
      }
    } else {
      fprint_typestripped_Wff( ops, goperators[i]->preconds );
    }
    if ( hit ) {
      for ( j = 0; j < goperators[i]->num_vars; j++ ) {
	if ( !guniversal_type[goperators[i]->var_types[j]] ) {
	  fprintf(ops, "(TYPE-%s ?x%d)\n", 
		  gtype_names[goperators[i]->var_types[j]], j);	  
	}
      }
      fprintf(ops, ")\n");
    }
    fprintf(ops, ")\n");
  }

  for ( i = 0; i < gnum_operators; i++ ) {
    if ( goperators[i]->preconds &&
	 goperators[i]->preconds->connective == FAL ) continue;
    /* second, the real ops
     */
    if ( goperators[i]->axiom ) {
      continue;
    }
    fprintf(ops, "\n(:action %s\n", goperators[i]->name);
    fprintf(ops, ":parameters (");
    hit = FALSE;
    for ( j = 0; j < goperators[i]->num_vars; j++ ) {
      fprintf(ops, "?x%d", j);
      if ( !guniversal_type[goperators[i]->var_types[j]] ) {
	hit = TRUE;
      }
      if ( j < goperators[i]->num_vars - 1 ) {
	fprintf(ops, " ");
      }
    }
    fprintf(ops,")\n");

    /* precond, with param types if necessary
     */
    if ( hit || 
	 (goperators[i]->preconds &&
	  goperators[i]->preconds->connective != TRU) ) {
      fprintf(ops, ":precondition\n");
    }
    if ( hit ) {
      fprintf(ops, "(and\n");
    }
    if ( goperators[i]->preconds &&
	 goperators[i]->preconds->connective != TRU ) {
      if ( hit && goperators[i]->preconds->connective == AND ) {
	for ( in = goperators[i]->preconds->sons; in; in = in->next ) {
	  fprint_typestripped_Wff( ops, in );
	}
      } else {
	fprint_typestripped_Wff( ops, goperators[i]->preconds );
      }
    } 
    if ( hit ) {
      for ( j = 0; j < goperators[i]->num_vars; j++ ) {
	if ( !guniversal_type[goperators[i]->var_types[j]] ) {
	  fprintf(ops, "(TYPE-%s ?x%d)\n", 
		  gtype_names[goperators[i]->var_types[j]], j);	  
	}
      }
      fprintf(ops, ")\n");
    }

    /* effect
     */
    fprintf(ops, ":effect\n");
    fprintf(ops, "(and\n");
    for ( eff = goperators[i]->effects; eff; eff = eff->next ) {
      if ( eff->conditions && eff->conditions->connective == FAL ) {
	continue;
      }
      hit = FALSE;
      for ( j = 0; j < eff->num_vars; j++ ) {
	fprintf(ops, "(forall (?x%d)\n", goperators[i]->num_vars + j);
	if ( !guniversal_type[eff->var_types[j]] ) {
	  hit = TRUE;
	}
      }
      if ( hit ||
	   (eff->conditions && eff->conditions->connective != TRU) ) {
	fprintf(ops, "(when\n");
      }
      if ( hit ) {
	fprintf(ops, "(and\n");
      }
      if ( eff->conditions && eff->conditions->connective != TRU ) {
	if ( hit && eff->conditions->connective == AND ) {
	  for ( in = eff->conditions->sons; in; in = in->next ) {
	    fprint_typestripped_Wff( ops, in );
	  }
	} else {
	  fprint_typestripped_Wff( ops, eff->conditions );      
	}
      }
      if ( hit ) {
	for ( j = 0; j < eff->num_vars; j++ ) {
	  if ( !guniversal_type[eff->var_types[j]] ) {
	    fprintf(ops, "(TYPE-%s ?x%d)\n", 
		    gtype_names[eff->var_types[j]], 
		    goperators[i]->num_vars + j);
	  }
	}
 	fprintf(ops, ")\n");
      }
      if ( hit ||
	   (eff->conditions && eff->conditions->connective != TRU) ) {
	fprintf(ops, "(and\n");
      }
      /* eff literals as simple list here
       */
      for ( lit = eff->effects; lit; lit = lit->next ) {
	if ( lit->negated ) {
	  fprintf(ops, "(not ");
	}
	fprint_Fact(ops, &(lit->fact) );
	if ( lit->negated ) {
	  fprintf(ops, ")");
	}
	fprintf(ops, "\n");
      }
      if ( hit ||
	   (eff->conditions && eff->conditions->connective != TRU) ) {
	fprintf(ops, ")\n");
	fprintf(ops, ")\n");
      }
      /* close the forall quantifiers implementing the eff params
       */
      for ( j = 0; j < eff->num_vars; j++ ) {
	fprintf(ops, ")\n");
      }
    }
    fprintf(ops, ")\n");
    fprintf(ops, ")\n");
  }

  fprintf(ops, ")\n");
  fclose( ops );

}



void fprint_Fact( FILE *fi, Fact *f )

{

  int j;

  if ( f->predicate == -1 ) {
    printf("\nequality = in print??\n\n");
    exit( 1 );
  }

  fprintf(fi, "(%s", gpredicates[f->predicate]);
  for ( j=0; j<garity[f->predicate]; j++ ) {
    if ( f->args[j] >= 0 ) {
      fprintf(fi, " %s", gconstants[(f->args)[j]]);
    } else {
      fprintf(fi, " ?x%d", DECODE_VAR( f->args[j] ));
    }
  }
  fprintf(fi, ")");

}



void fprint_typestripped_Wff( FILE *fi, WffNode *n )

{

  WffNode *i;

  if ( !n ) {
    printf("\nprinting empty Wff??\n\n");
    exit( 1 );
  }
  
  switch ( n->connective ) {
  case ALL: 
    fprintf(fi, "(forall (?x%d)\n", n->var);
    if ( !guniversal_type[n->var_type] ) {
      fprintf(fi, "(or\n");
    }
    if ( !guniversal_type[n->var_type] &&
	 n->son->connective == OR ) {
      for ( i = n->son->sons; i; i = i->next ) {
	fprint_typestripped_Wff( fi, i );
      }
    } else { 
      fprint_typestripped_Wff( fi, n->son );
    }
    if ( !guniversal_type[n->var_type] ) {
      fprintf(fi, "(not (TYPE-%s ?x%d)))\n", gtype_names[n->var_type], n->var);
    }
    fprintf(fi, ")\n");
    break;
  case EX:
    fprintf(fi, "(exists (?x%d)\n", n->var);
    if ( !guniversal_type[n->var_type] ) {
      fprintf(fi, "(and\n");
    }
    if ( !guniversal_type[n->var_type] &&
	 n->son->connective == AND ) {
      for ( i = n->son->sons; i; i = i->next ) {
	fprint_typestripped_Wff( fi, i );
      }
    } else { 
      fprint_typestripped_Wff( fi, n->son );
    }
    if ( !guniversal_type[n->var_type] ) {
      fprintf(fi, "(TYPE-%s ?x%d))\n", gtype_names[n->var_type], n->var);
    }
    fprintf(fi, ")\n");
    break;
  case AND: 
    fprintf(fi, "(and\n");
    for ( i = n->sons; i; i = i->next ) {
      fprint_typestripped_Wff( fi, i );
    }
    fprintf(fi, ")\n");
    break;
  case OR:  
    fprintf(fi, "(or\n");
    for ( i = n->sons; i; i = i->next ) {
      fprint_typestripped_Wff( fi, i );
    }
    fprintf(fi, ")\n");
    break;
  case NOT:
    if ( n->son->connective == ATOM ) {
      fprintf(fi, "(not ");
      fprint_Fact( fi, n->son->fact );
      fprintf(fi, ")\n");
    } else {
      fprintf(fi, "(not\n");
      fprint_typestripped_Wff( fi, n->son );
      fprintf(fi, ")\n");
    }
    break;
  case ATOM:
    fprint_Fact( fi, n->fact );
    fprintf(fi, "\n");
    break;
  case TRU:
  case FAL:
  default:
    printf("\nWff output: %d is wrong Node specifier\n", n->connective);
    exit(1);
  }

}










/* silly stuff for the :requirements field
 */











void collect_needed_constructs( void )

{

  int i, j;
  Bool hit;
  Effect *eff;

  for ( i = 0; i < gnum_operators; i++ ) {
    for ( eff = goperators[i]->effects; eff; eff = eff->next ) {
      if ( eff->conditions && eff->conditions->connective == FAL ) {
	continue;
      }
      hit = FALSE;
      for ( j = 0; j < eff->num_vars; j++ ) {
	if ( !guniversal_type[eff->var_types[j]] ) {
	  hit = TRUE;
	}
      }
      if ( hit &&
	   (!eff->conditions || eff->conditions->connective == TRU) ) {
	gconditional_effects = TRUE;
      }
    }
  }

  collect_needed( ggoal );
  for ( i = 0; i < gnum_operators; i++ ) {
    collect_needed( goperators[i]->preconds );
    for ( eff = goperators[i]->effects; eff; eff = eff->next ) {
      collect_needed( eff->conditions );
    }
  }

}



void collect_needed( WffNode *n )

{

  if ( !n ) {
    return;
  }
  
  switch ( n->connective ) {
  case ALL: 
    if ( !guniversal_type[n->var_type] ) {
      gdisjunctive_preconditions = TRUE;
      gnegative_preconditions = TRUE;
    }
    break;
  case EX:
  case AND: 
  case OR:  
  case NOT:
  case ATOM:
  case TRU:
  case FAL:
    break;
  default:
    printf("\nWff output: %d is wrong Node specifier\n", n->connective);
    exit(1);
  }

}



