
#ifndef __SPLITDOMAINOUTPUT_H
#define __SPLITDOMAINOUTPUT_H

#include "BlackBox.h"
#include "SubPartition.h"
#include "SplitAction.h"
#include "SplitDomain.h"
#include "SplitToken.h"

void SplitOutput_DomainName(FILE *f);
void SplitOutput_Requirements(FILE *f);
void SplitOutput_Types(FILE *f);
void SplitOutput_Constants(FILE *f);
void SplitOutput_Predicates(FILE *f, SplitDomain splitDomain);


void SplitOutput_WffNode(FILE *f, WffNode *formula, Operator *op);
void SplitOutput_Atom(FILE *f, Fact *fact, Operator *op);
void SplitOutput_Not(FILE *f, WffNode *formula, Operator *op);
void SplitOutput_And(FILE *f, WffNode *formula, Operator *op);
void SplitOutput_Or(FILE *f, WffNode *formula, Operator *op);
void SplitOutput_All(FILE *f, WffNode *formula, Operator *op);
void SplitOutput_Ex(FILE *f, WffNode *formula, Operator *op);




void SplitOutput_Effect(FILE *f, Effect *eff_formula, Operator *op);
void SplitOutput_AtomicEffs(FILE *f, Effect *eff_formula, Operator *op);
void SplitOutput_OneEffFormula(FILE *f, Effect *eff_formula, Operator *op);

void SplitOutput_SplitToken(FILE *f, SplitToken token, Operator *op);

void SplitOutput_SubPartition(FILE *f, SubPartition sp, Operator *op, int order);
void SplitOutput_SplitAction(FILE *f, SplitAction splitAction);
void SplitOutput_SplitDomain(char *file, SplitDomain splitDomain);




void SplitOutput_ProblemName(FILE *f);
void SplitOutput_Objects(FILE *f);
void SplitOutput_Init(FILE *f);
void SplitOutput_Goal(FILE *f);
void SplitOutput_SplitProblem(char *file);


void SplitOutput_SplitLog(char *file, SplitDomain splitDomain);
void SplitOutput_SplittedOperators(char *splitted_operator_path, SplitDomain splitDomain);
void SplitOutput_SplitabilityGraph(char *splitability_graph_path);










#endif
