#include <rose.h>
#include <string>
#include "RtedSymbols.h"
#include "DataStructures.h"
#include "RtedTransformation.h"

using namespace std;
using namespace SageInterface;
using namespace SageBuilder;


/* -----------------------------------------------------------
 * Helper Function
 * -----------------------------------------------------------*/
SgStatement*
RtedTransformation::getStatement(SgExpression* exp) {
  SgStatement* stmt = NULL;
  SgNode* expr = exp;
  while (!isSgStatement(expr) && !isSgProject(expr))
    expr = expr->get_parent();
  if (isSgStatement(expr))
    stmt = isSgStatement(expr);
  return stmt;
}


SgExpression*
RtedTransformation::getExprBelowAssignment( SgExpression* exp ) {
    SgExpression* parent = isSgExpression( exp->get_parent() );
    
    while(  parent
            && !(
                isSgAssignOp( parent )
                || isSgAndAssignOp( parent ) 
                || isSgDivAssignOp( parent ) 
                || isSgIorAssignOp( parent )
                || isSgLshiftAssignOp( parent )
                || isSgMinusAssignOp( parent )
                || isSgModAssignOp( parent )
                || isSgMultAssignOp( parent )
                || isSgPlusAssignOp( parent )
                || isSgPointerAssignOp( parent )
                || isSgRshiftAssignOp( parent )
                || isSgXorAssignOp( parent)
            )) {

        exp = parent;
        parent = isSgExpression( parent->get_parent() );
    }

    return exp;
}


SgExpression*
RtedTransformation::buildString(std::string name) {
  //  SgExpression* exp = buildCastExp(buildStringVal(name),buildPointerType(buildCharType()));
  SgExpression* exp = buildStringVal(name);
  return exp;
}

std::string
RtedTransformation::removeSpecialChar(std::string str) {
  string searchString="\"";
  string replaceString="'";
  string::size_type pos = 0;
  while ( (pos = str.find(searchString, pos)) != string::npos ) {
    str.replace( pos, searchString.size(), replaceString );
    pos++;
  }
  return str;
}


std::string
RtedTransformation::getMangledNameOfExpression(SgExpression* expr) {
  string manglName="";
  // look for varRef in the expr and return its mangled name
  Rose_STL_Container< SgNode * > var_refs = NodeQuery::querySubTree(expr,V_SgVarRefExp);
  if (var_refs.size()==0) {
    // there should be at least one var ref
    cerr << " getMangledNameOfExpression: varRef on left hand side not found : " << expr->unparseToString() << endl;
  } else if (var_refs.size()==1) {
    // correct, found on var ref
    SgVarRefExp* varRef = isSgVarRefExp(*(var_refs.begin()));
    ROSE_ASSERT(varRef && varRef->get_symbol()->get_declaration());
    manglName = varRef->get_symbol()->get_declaration()->get_mangled_name();
    cerr << " getMangledNameOfExpression: found varRef: " << manglName << endl;
  } else if (var_refs.size()>1) {
    // error
    cerr << " getMangledNameOfExpression: Too many varRefs on left hand side : " << var_refs.size() << endl; 
  }
  return manglName;
}

/****************************************
 * This function returns InitializedName
 * for a DotExpr
 ****************************************/
std::pair<SgInitializedName*,SgVarRefExp*>
RtedTransformation::getRightOfDot(SgDotExp* dot, std::string str,
				  SgVarRefExp* varRef) {
  varRef=NULL;
  SgInitializedName* initName = NULL;
  SgExpression* rightDot = dot->get_rhs_operand();
  ROSE_ASSERT(rightDot);
  varRef = isSgVarRefExp(rightDot);
  if (varRef) {
    initName = (varRef)->get_symbol()->get_declaration();
  } else {
    cerr << "RtedTransformation : " << str << " - Unknown : "
	 << rightDot->class_name() << endl;
    ROSE_ASSERT(false);
  }
  return make_pair(initName,varRef);
}

/****************************************
 * This function returns InitializedName
 * for a DotExpr
 ****************************************/
std::pair<SgInitializedName*,SgVarRefExp*>
RtedTransformation::getRightOfPointerDeref(SgPointerDerefExp* dot, std::string str,
					   SgVarRefExp* varRef) {
  varRef=NULL;
  SgInitializedName* initName = NULL;
  SgExpression* rightDot = dot->get_operand();
  ROSE_ASSERT(rightDot);
  varRef = isSgVarRefExp(rightDot);
  if (varRef) {
    initName = (varRef)->get_symbol()->get_declaration();
  } else {
    cerr << "RtedTransformation : " << str << " - Unknown : "
	 << rightDot->class_name() << endl;
    ROSE_ASSERT(false);
  }
  return make_pair(initName,varRef);
}

/****************************************
 * This function returns InitializedName
 * for a ArrowExp
 ****************************************/
std::pair<SgInitializedName*,SgVarRefExp*>
RtedTransformation::getRightOfArrow(SgArrowExp* arrow, std::string str,
				    SgVarRefExp* varRef) {
  varRef=NULL;
  SgInitializedName* initName = NULL;
  SgExpression* rightArrow = arrow->get_rhs_operand();
  ROSE_ASSERT(rightArrow);
  varRef = isSgVarRefExp(rightArrow);
  if (varRef) {
    initName = varRef->get_symbol()->get_declaration();
  } else {
    cerr << "RtedTransformation : " << str << " - Unknown : "
	 << rightArrow->class_name() << endl;
    ROSE_ASSERT(false);
  }
  return make_pair(initName,varRef);
}

/****************************************
 * This function returns InitializedName
 * for a PlusPlusOp
 ****************************************/
std::pair<SgInitializedName*,SgVarRefExp*>
RtedTransformation::getPlusPlusOp(SgPlusPlusOp* plus, std::string str,
				  SgVarRefExp* varRef) {
  varRef=NULL;
  SgInitializedName* initName = NULL;
  SgExpression* expPl = plus->get_operand();
  ROSE_ASSERT(expPl);
  varRef = isSgVarRefExp(expPl);
  if (varRef) {
    initName = varRef->get_symbol()->get_declaration();
  } else {
    cerr << "RtedTransformation : " << str << " - Unknown : "
	 << expPl->class_name() << endl;
    ROSE_ASSERT(false);
  }
  return make_pair(initName,varRef);
}

/****************************************
 * This function returns InitializedName
 * for a MinusMinusOp
 ****************************************/
std::pair<SgInitializedName*,SgVarRefExp*>
RtedTransformation::getMinusMinusOp(SgMinusMinusOp* minus, std::string str
				    , SgVarRefExp* varRef) {
  varRef=NULL;
  SgInitializedName* initName = NULL;
  SgExpression* expPl = minus->get_operand();
  ROSE_ASSERT(expPl);
  varRef = isSgVarRefExp(expPl);
  if (varRef) {
    initName = varRef->get_symbol()->get_declaration();
  } else {
    cerr << "RtedTransformation : " << str << " - Unknown : "
	 << expPl->class_name() << endl;
    ROSE_ASSERT(false);
  }
  return make_pair(initName,varRef);
}

/****************************************
 * This function returns the statement that
 * surrounds a given Node or Expression
 ****************************************/
SgStatement*
RtedTransformation::getSurroundingStatement(SgNode* n) {
  SgNode* stat = n;
  while (!isSgStatement(stat) && !isSgProject(stat)) {
    if (stat->get_parent()==NULL) {
      cerr << " No parent possible for : " << n->unparseToString()
	   <<"  :" << stat->unparseToString() << endl;
    }
    ROSE_ASSERT(stat->get_parent());
    stat = stat->get_parent();
  }
  return isSgStatement(stat);
}

SgExpression* RtedTransformation::getUppermostLvalue( SgExpression* exp ) {
    SgExpression* parent = isSgExpression( exp->get_parent() );
    
    while( parent
            && (isSgDotExp( parent )
                || isSgArrowExp( parent )
                || isSgPointerDerefExp( parent )
                || isSgPntrArrRefExp( parent ))) {
        exp = parent;
        parent = isSgExpression( parent->get_parent() );
    }

    return exp;
}

SgVarRefExp*
RtedTransformation::resolveToVarRefRight(SgExpression* expr) {
  SgVarRefExp* result = NULL;
  SgExpression* newexpr = NULL;
  ROSE_ASSERT(expr);
  if (isSgDotExp(expr)) {
    newexpr= isSgDotExp(expr)->get_rhs_operand();
    result = isSgVarRefExp(newexpr);
    if (!result) {
      cerr <<"  >> resolveToVarRefRight : right : " << newexpr->class_name() << endl;
      exit(1);
    }
  } else {
    cerr <<" >> resolveToVarRefRight : unknown expression " << expr->class_name() <<endl;
    exit(1);
  }

  return result;
}

SgVarRefExp*
RtedTransformation::resolveToVarRefLeft(SgExpression* expr) {
  SgVarRefExp* result = NULL;
  SgExpression* newexpr = NULL;
  ROSE_ASSERT(expr);
  if (isSgDotExp(expr)) {
    newexpr= isSgDotExp(expr)->get_lhs_operand();
    result = isSgVarRefExp(newexpr);
    if (!result) {
      cerr <<"  >> resolveToVarRefRight : right : " << newexpr->class_name() << endl;
      exit(1);
    }
  } else {
    cerr <<" >> resolveToVarRefRight : unknown expression " << expr->class_name() <<endl;
    exit(1);
  }
  
  return result;
}

int RtedTransformation::getDimension(SgInitializedName* initName) {
  ROSE_ASSERT(initName);
  int dimension = 0;
  SgType* type = initName->get_type();
  ROSE_ASSERT(type);
  if (isSgArrayType(type)) {
    dimension++;
  } else {
    while (isSgPointerType(type) && !isSgPointerMemberType(type)) {
      SgPointerType* pointer = isSgPointerType(type);
      ROSE_ASSERT(pointer);
      type = pointer->dereference();
      ROSE_ASSERT(type);
      dimension++;
      ROSE_ASSERT(dimension<10);
      //cerr << "Dimension : " << dimension << "  : " << type->class_name() << endl;
    }
  }
  return dimension;
}

int 
RtedTransformation::getDimension(SgInitializedName* initName, SgVarRefExp* varRef) {
  int dim =-1;
  std::map<SgVarRefExp*, RTedArray*>::const_iterator it = create_array_define_varRef_multiArray.begin();
  for (;it!=create_array_define_varRef_multiArray.end();++it) {
    RTedArray* array = it->second;
    SgInitializedName* init = array->initName;
    if (init==initName) {
      dim=array->getDimension();
      cerr << "Found init : " << init->unparseToString() << " dim : " << dim << "  compare to : " << initName->unparseToString()<<endl;
    }
  }

  std::map<SgInitializedName*, RTedArray*>::const_iterator it2= create_array_define_varRef_multiArray_stack.find(initName);
  for (;it2!=create_array_define_varRef_multiArray_stack.end();++it2) {
    RTedArray* array = it2->second;
    SgInitializedName* init = array->initName;
    if (init==initName) {
      dim=array->getDimension();
    }
  }
  cerr << " -------------------------- resizing dimension to : " << dim << "  for : " << varRef->unparseToString() << endl;
  return dim;
}



bool RtedTransformation::isGlobalExternVariable(SgStatement* stmt) {
  bool externQual =false;
#if 1
  SgDeclarationStatement* declstmt = isSgDeclarationStatement(stmt);
  SgFunctionParameterList* funcparam = isSgFunctionParameterList(stmt);
  if (funcparam) {
    SgFunctionDeclaration* funcdeclstmt = isSgFunctionDeclaration(funcparam->get_parent());
    ROSE_ASSERT(funcdeclstmt);
    externQual = funcdeclstmt->get_declarationModifier().get_storageModifier().isExtern();
    cerr << ">>>>>>>>>>>>>>>> stmt-param : " << funcdeclstmt->unparseToString() << "  " << funcdeclstmt->class_name() <<
      "  " << externQual << endl;
  } else if (declstmt) {
    externQual = declstmt->get_declarationModifier().get_storageModifier().isExtern();
  }
  cerr << ">>>>>>>>>>>>>>>> stmt : " << stmt->unparseToString() << "  " << stmt->class_name() << endl;
#endif
  return externQual;
}


/*************************************************
 * This function adds some very common arguments
 * to instrumented function calls for constructing
 * SourcePosition objects.
 ************************************************/
void RtedTransformation::appendFileInfo( SgNode* node, SgExprListExp* arg_list) {
    ROSE_ASSERT( node );
    appendFileInfo( node->get_file_info(), arg_list );
}
void RtedTransformation::appendFileInfo( Sg_File_Info* file_info, SgExprListExp* arg_list) {

    SgExpression* filename = buildString( file_info->get_filename() );
    SgExpression* linenr = buildString( RoseBin_support::ToString( file_info->get_line() ));
    SgExpression* linenrTransformed = buildString("x%%x");

    appendExpression( arg_list, filename );
    appendExpression( arg_list, linenr );
    appendExpression( arg_list, linenrTransformed );
}


void RtedTransformation::appendTypeInformation( SgInitializedName* initName, SgExprListExp* arg_list ) {
    if( !initName ) return;

    appendTypeInformation( initName, initName -> get_type(), arg_list );
}
void RtedTransformation::appendTypeInformation( SgInitializedName* initName, SgType* type, SgExprListExp* arg_list ) {
    SgType* basetype = NULL;
    ROSE_ASSERT(type);

    SgExpression* basetypeStr = buildString("");
    size_t indirection_level = 0;

    basetype = type;
    while( true )
        if (isSgPointerType( basetype )) {
            basetype = isSgPointerType( basetype )->get_base_type();
            ++indirection_level;
        } else if( isSgArrayType( basetype )) {
            basetype = isSgArrayType( basetype )->get_base_type();
            ++indirection_level;
        } else break;

    if ( indirection_level > 0 )
        basetypeStr = buildString( basetype->class_name() );

    SgExpression* ctypeStr = buildString(type->class_name());
    // arrays in parameters are actually passed as pointers, so we shouldn't
    // treat them as stack arrays
    if(     initName
            && isSgFunctionParameterList( getSurroundingStatement( initName ))
            &&  type->class_name() == "SgArrayType" )
        ctypeStr = buildString( "SgPointerType" );

    appendExpression(arg_list, ctypeStr);
    appendExpression(arg_list, basetypeStr);
    appendExpression(arg_list, buildIntVal( indirection_level ));
}

void RtedTransformation::appendAddressAndSize(SgInitializedName* initName,
					      SgExpression* varRefE,
					      SgExprListExp* arg_list,
					      int appendType
					      ) {

    SgScopeStatement* scope = NULL;
    SgType* basetype = NULL;

    if( initName) {
        SgType* type = initName->get_type();
        ROSE_ASSERT(type);

        if( isSgPointerType( type ) )
            basetype = isSgPointerType(type)->get_base_type();
        scope = initName->get_scope();
    }

    SgExpression* exp = varRefE;
    if (    isSgClassType(basetype) ||
            isSgTypedefType(basetype) ||
            isSgClassDefinition(scope)) {

        // member -> &( var.member )
        exp = getUppermostLvalue( varRefE );
    }

	appendAddress( arg_list, exp );

    // for pointer arithmetic variable access in expressions, we want the
    // equivalent expression that computes the new value.
    //
    // e.g., with
    //  int a[] = { 7, 8 };
    //  int *p = &a[ 0 ];
    //  int q = *(p++);
    //
    // we want the last assignment to check that (p + 1) is a valid address to
    // read
    if(     isSgPlusPlusOp( exp )
            || isSgMinusMinusOp( exp )) {

        exp = isSgUnaryOp( exp ) -> get_operand();
    } else if( isSgPlusAssignOp( exp )
                || isSgMinusAssignOp( exp )) {

        exp = isSgBinaryOp( exp ) -> get_lhs_operand();
    }

    SgTreeCopy copy;
    // consider, e.g.
    //
    //      char* s1;
    //      char s2[20];
    //
    //      s1 = s2 + 3;
    //      
    //  we only want to access s2..s2+sizeof(char), not s2..s2+sizeof(s2)
    //
    //  but we do want to create the array as s2..s2+sizeof(s2)     
    if( appendType & 2 && isSgArrayType( varRefE->get_type() )) {
        appendExpression(
            arg_list,
            buildSizeOfOp(
                isSgType(
                    isSgArrayType( varRefE -> get_type() ) 
                        -> get_base_type() -> copy( copy )))
        );
    } else {
        appendExpression(
            arg_list,
            buildSizeOfOp( isSgExpression( exp -> copy( copy )))
        );
    }
}

/*
 * Appends the address of exp.  If exp is ++, +=, -- or -=, the address of the
 * pointer after the assignment is appended to arg_list.
 */
void RtedTransformation::appendAddress(SgExprListExp* arg_list, SgExpression* exp) {
    SgTreeCopy copy;


    SgIntVal* offset = NULL;
    if( isSgPlusPlusOp( exp )) {
        offset = buildIntVal( 1 );
        exp = isSgUnaryOp( exp ) -> get_operand();
    } else if( isSgMinusMinusOp( exp )) {
        offset = buildIntVal( -1 );
        exp = isSgUnaryOp( exp ) -> get_operand();
    } else if( isSgPlusAssignOp( exp )) {
        offset = isSgIntVal( isSgBinaryOp( exp ) ->get_rhs_operand());
        exp = isSgBinaryOp( exp ) -> get_lhs_operand();
    } else if( isSgMinusAssignOp( exp )) {
        offset = buildIntVal(
            -1 * 
            isSgIntVal( isSgBinaryOp( exp ) ->get_rhs_operand()) -> get_value()
        );
        exp = isSgBinaryOp( exp ) -> get_lhs_operand();
    }

    SgExpression *arg;
    SgCastExp *cast_op = 
        buildCastExp(
            buildAddressOfOp( isSgExpression( exp -> copy( copy ))),
            buildUnsignedLongLongType()
        );

    if( offset != NULL ) {
        ROSE_ASSERT( exp != NULL );
        arg = new SgAddOp(
            exp -> get_file_info(), cast_op,
            new SgMultiplyOp(
                exp -> get_file_info(),
                offset,
                buildSizeOfOp( exp )
            )
        );
    } else
        arg = cast_op;

    appendExpression( arg_list, arg );
}


void RtedTransformation::appendBaseType( SgExprListExp* arg_list, SgType* type ) {
	SgType* base_type = NULL;

	SgArrayType* arr = isSgArrayType( type );
	SgPointerType* ptr = isSgPointerType( type );

	if( arr )
		base_type = arr -> get_base_type();
	else if ( ptr )
		base_type = ptr -> get_base_type();

	if( base_type )
        appendExpression(arg_list, buildString(
			base_type -> class_name()
		));
	else
        appendExpression(arg_list, buildString(""));
}

void RtedTransformation::appendClassName( SgExprListExp* arg_list, SgType* type ) {

    if( isSgClassType( type )) {
        appendExpression(arg_list, buildString(
			isSgClassDeclaration( isSgClassType( type ) -> get_declaration() )
				-> get_mangled_name() )
		);
    } else if( isSgArrayType( type )) {

        appendClassName( arg_list, isSgArrayType( type ) -> get_base_type() );

    } else if( isSgPointerType( type )) {

        appendClassName( arg_list, isSgPointerType( type ) -> get_base_type() );

    } else {

        appendExpression( arg_list, buildString( "" ));
    }
}

