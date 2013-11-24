#include <iostream>
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IRBuilder.h"
#include "llvm/ValueSymbolTable.h"
#include "llvm/Support/raw_ostream.h"

llvm::Function *generate_function_main(llvm::IRBuilder<>* builder, llvm::Module *mod) {
	std::vector<llvm::Type*> param_types(0, 
		llvm::Type::getVoidTy(llvm::getGlobalContext()));
	llvm::Type* result_type = llvm::Type::getInt32Ty(llvm::getGlobalContext());
	std::string function_name = "main";
	llvm::FunctionType* function_type 
		= llvm::FunctionType::get(result_type, param_types, false);
	llvm::Function* function = llvm::Function::Create(
		function_type,
		llvm::Function::ExternalLinkage,
		function_name,
		mod);

	llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(
		llvm::getGlobalContext(), 
		"entry", 
		function);

	builder->SetInsertPoint(basic_block);
	// TODO: generate function body
	int var_int_value = 34;
	std::string var_name = "i";
	llvm::Value* var;
	llvm::AllocaInst* alloca = builder->CreateAlloca(
		llvm::Type::getInt32Ty(llvm::getGlobalContext()),
		0,
		var_name);
	var = alloca;
	llvm::ValueSymbolTable& function_value_symbol_table 
		= function->getValueSymbolTable();

	llvm::Value* lhs_v;
	llvm::Value* rhs_v;

	// 'lhs_v' is equal to 'var'
        lhs_v = function_value_symbol_table.lookup(var_name);
	rhs_v = llvm::ConstantInt::get(
			llvm::Type::getInt32Ty(llvm::getGlobalContext()), var_int_value);
	builder->CreateStore(rhs_v, lhs_v);

	// return i
	builder->CreateRet(builder->CreateLoad(lhs_v, "var_tmp"));
	return function;
}

void print_module(llvm::Module* mod, std::string file_name) {
	llvm::PassManager pass_manager;
	std::string error;
	llvm::raw_fd_ostream raw_stream(file_name.c_str(), error);

	pass_manager.add(llvm::createPromoteMemoryToRegisterPass());
	pass_manager.add(llvm::createPrintModulePass(&raw_stream));
	pass_manager.run(*mod);
	raw_stream.close();
}

/*
 * int main() {
 *     int i = 0;
 *     return i;
 * }
 */
int main(int argc, char *argv[]) {
	std::string file_name = "main.ll";
	llvm::IRBuilder<> *builder 
		= new llvm::IRBuilder<>(llvm::getGlobalContext());
	llvm::Module *mod = new llvm::Module("null", llvm::getGlobalContext());
	generate_function_main(builder, mod);
	print_module(mod, file_name);

	return 0;
}
