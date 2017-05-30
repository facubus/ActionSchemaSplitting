
#ifndef __SERVICESDOMAIN_H
#define __SERVICESDOMAIN_H


Operator_pointer ServicesDomain_GetAction(int i); /*1<=i<=n*/
int ServicesDomain_GetAmountActions();
int ServicesDomain_GetAmountPredicates();
int ServicesDomain_GetInterfaceSize(int i); /*1<=i<=n*/
int ServicesDomain_GetMaxInterface();
char *ServicesDomain_GetName();


char *ServicesDomain_GetTypeName(int type);
char *ServicesDomain_GetConstantName(int constant);
Bool ServicesDomain_IsConstantDomain(int constant);
Bool ServicesDomain_IsUniversalType(int type);
int ServicesDomain_GetNumTypes();
int ServicesDomain_GetNumConstants();
int ServicesDomain_GetTypeSize(int type);
int ServicesDomain_GetTypeConstant(int type, int index); /*0 <= index < size*/


char *ServicesDomain_GetConstantTypeName(int constant);
char *ServicesDomain_GetFatherTypeName(int type);




void ServicesDomain_ConstantsTable();
void ServicesDomain_TypesTable();


int ServicesDomain_GetActionNumber(Operator *op);


/*usefull for global normalization*/
float ServicesDomain_GetMaxGrounded();
float ServicesDomain_GetMaxDensity();
int ServicesDomain_GetMaxCC();


#endif
