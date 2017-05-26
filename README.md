# ActionSchemaSplitting

Action Schema Splitting Features:
- supports Strips domain
- supports ADL domains


The algorithm sorts the action schemas into a ranking under notion of splitability.
When the splitabilitity coefficient of an action schema is close to 1 means the 
schema is more suitable to be "spliteable", while if it is close to 0, the schema 
is less likely to be optimized by the technique. Then you can set which of the 
first actions in the ranking you want to divide to.

The algorithm is called in the following way in the command line:

./splitting -o <original-domain> -f <original-task> <split-domain> <split-task> <gamma> <threshold>

where:
<gamma> is a float number in the interval [0,1]
<threshold> is a integer number in the range 1 ... #number of schemas

Remember:

if gamma tends to 0, then the algorithm tries to divide the original action schema into many subactions. 
if gamma tends to 1, then the algorithm tries to divide the original action schema in few subactions. In partcular, 
when gamma = 1, the original action schema is not splittied, i.e., returns the trivial split. 

if threshold = 1, then the algorithm only tries to split to the first action schema in the splitability ranking.
if threshold = 2, then the algorithm only tries to split to the first and the second action schemas in the splitability ranking.
...
if threshold = #number of schemas, then the algorithm tries to split to all action schemas.

These two paramenters (gamma and threshold) handle the two trade-offs of the algorithm.
The PDDL parser used for this algorithm is the parser of FF planner. For any doubt about 
supported syntax by this planner you can visit the url https://fai.cs.uni-saarland.de/hoffmann/ff.html
