#include "ff.h"
#include "inst_pre.h"
#include "ServicesDomain.h"
#include "ServicesAction.h"
#include "InterfaceGraph.h"


Operator_pointer ServicesDomain_GetAction(int i){
	return goperators[ServicesDomain_GetAmountActions()-i];
}
int ServicesDomain_GetAmountActions(){
	return gnum_operators;
}

int ServicesDomain_GetAmountPredicates(){
	return gnum_predicates;
}

int ServicesDomain_GetInterfaceSize(int i){
	return ServicesDomain_GetAction(i)->num_vars;
}

int ServicesDomain_GetMaxInterface(){
	int i, max = -1, n = ServicesDomain_GetAmountActions();
	for(i=1;i<=n;i++){
		int v = ServicesDomain_GetInterfaceSize(i);
		max = v > max ?  v : max;
	}
	return max;
}

char *ServicesDomain_GetName(){
	return gdomain_name;
}

char *ServicesDomain_GetTypeName(int type){
	return gtype_names[type];
}
char *ServicesDomain_GetConstantName(int constant){
	return gconstants[constant];
}
Bool ServicesDomain_IsConstantDomain(int constant){
	return gdomain_const[constant];
}

Bool ServicesDomain_IsUniversalType(int type){
	return guniversal_type[type];
}
int ServicesDomain_GetNumTypes(){
	return gnum_types;
}
int ServicesDomain_GetNumConstants(){
	return gnum_constants;
}

int ServicesDomain_GetTypeSize(int type){
	return gtype_size[type];
}

int ServicesDomain_GetTypeConstant(int type, int index){
	return gtype_consts[type][index];
}

void ServicesDomain_ConstantsTable(){
	printf("\nConstants Table:");
	int i;
	for ( i = 0; i < gnum_constants; i++ ) {
	  printf("\n\t%d --> %s", i, gconstants[i]);
	}
}

void ServicesDomain_TypesTable(){
	printf("\nTypes Table:");
	int i;
	for ( i = 0; i < gnum_types; i++ ){
	  printf("\n\t%d --> %s (universal? %d): ", i, gtype_names[i], guniversal_type[i]);
	}
	printf("\n");

}


char *ServicesDomain_GetConstantTypeName(int constant){
	int types_const[MAX_TYPES];
	int i,j,k=0;
	for (i = 1; i < gnum_types; i++){	/*exclude type 0 => artificial-all-objects */
		for (j = 0; j < gtype_size[i]; j++){
			if (gtype_consts[i][j] == constant){
				types_const[k] = i;
				k++;
			}
		}
	}
	int t;
	for(t = 0; t < k; t++){
		int type = types_const[t];
		int r;
		Bool b = TRUE;
		for(r = 0;r < k; r++){
			if (r!=t && !is_subtype(type, types_const[r])){
				b = FALSE;
				break ;
			}
		}
		if (b) return gtype_names[type]; else continue;
	}
	return "nullType";
}



char *ServicesDomain_GetFatherTypeName(int type){
	int ancestor_types[MAX_TYPES];
	int i, k = 0;
	for ( i = 1; i < gnum_types; i++){ /*exclude type 0 => artificial-all-objects */
	  if (type != i && is_subtype(type,i)){
		  ancestor_types[k] = i;
		  k++;
	  }
	}

	int t;
	for(t = 0; t < k; t++){
		int father = ancestor_types[t];
		int r;
		Bool b = TRUE;
		for(r = 0;r < k; r++){
			if (r!=t && !is_subtype(father, ancestor_types[r])){
				b = FALSE;
				break ;
			}
		}
		if (b) return gtype_names[father]; else continue;
	}
	return "nullType";

}


int ServicesDomain_GetActionNumber(Operator *op){
	int i;
	for(i = 1; i <= gnum_operators; i++){
		if (ServicesDomain_GetAction(i)->name == op->name) return i;
	}
	return 0;
}




float ServicesDomain_GetMaxGrounded(){
	int i,n = ServicesDomain_GetAmountActions();
	float max_grounded = -1.0;
	for(i=1;i<=n;i++){
		float grounded = ServicesAction_GetGrounded(ServicesDomain_GetAction(i));
		max_grounded = max_grounded < grounded ? grounded : max_grounded;
	}
	return max_grounded;


}

float ServicesDomain_GetMaxDensity(){
	int i,n = ServicesDomain_GetAmountActions();
	float max_density = -1.0;
	for(i=1;i<=n;i++){
		Operator *op = ServicesDomain_GetAction(i);
		InterfaceGraph g = InterfaceGraph_Create(op);
		float density = InterfaceGraph_Density(g);
		max_density = max_density < density ? density : max_density;
	}
	return max_density;

}

int ServicesDomain_GetMaxCC(){
	int i,n = ServicesDomain_GetAmountActions();
	int max_cc = -1;
	for(i=1;i<=n;i++){
		Operator *op = ServicesDomain_GetAction(i);
		InterfaceGraph g = InterfaceGraph_Create(op);
		int cc = InterfaceGraph_StronglyConnectedComponents(g);
		max_cc = max_cc < cc ? cc : max_cc;
	}
	return max_cc;
}











